/**
 * Monika Rosinska, xrosin00
 * 25. 10. 2019
 * IPS, du1
 */

#include "mmal.h"
#include <sys/mman.h> // mmap
#include <stdbool.h> // bool
#include <assert.h> // assert
#include <string.h> // memcpy
#include <stdio.h>

#ifdef NDEBUG
/**
 * The structure header encapsulates data of a single memory block.
 *   ---+------+----------------------------+---
 *      |Header|DDD not_free DDDDD...free...|
 *   ---+------+-----------------+----------+---
 *             |-- Header.asize -|
 *             |-- Header.size -------------|
 */
typedef struct header Header;
struct header {

    /**
     * Pointer to the next header. Cyclic list. If there is no other block,
     * points to itself.
     */
    Header *next;

    /// size of the block
    size_t size;

    /**
     * Size of block in bytes allocated for program. asize=0 means the block 
     * is not used by a program.
     */
    size_t asize;
};

/**
 * The arena structure.
 *   /--- arena metadata
 *   |     /---- header of the first block
 *   v     v
 *   +-----+------+-----------------------------+
 *   |Arena|Header|.............................|
 *   +-----+------+-----------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
typedef struct arena Arena;
struct arena {

    /**
     * Pointer to the next arena. Single-linked list.
     */
    Arena *next;

    /// Arena size.
    size_t size;
};

#define PAGE_SIZE (128*1024)

#endif // NDEBUG

Arena *first_arena = NULL;

//arena_alloc needs acces to this function
static
void hdr_ctor();

/**
 * Return size alligned to PAGE_SIZE
 */
static
size_t allign_page(size_t size)
{
    size = size+sizeof(Arena)+sizeof(Header); //we need space for Arena header anad min one Header
    size_t temp = size/(PAGE_SIZE);  //how many times there's PAGE_SIZE in size
    //rounding up
    if (size%(PAGE_SIZE) != 0){
        temp++;
    }
    temp *= (PAGE_SIZE);
    return temp;
}

/**
 * Allocate a new arena using mmap.
 * @param req_size requested size in bytes. Should be alligned to PAGE_SIZE.
 * @return pointer to a new arena, if successfull. NULL if error.
 * @pre req_size > sizeof(Arena) + sizeof(Header)
 */

/**
 *   +-----+------------------------------------+
 *   |Arena|....................................|
 *   +-----+------------------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
static
Arena *arena_alloc(size_t req_size)
{
    Arena *new_arena = NULL;
    size_t size = allign_page(req_size); //allign req size to PAGE_SIZE
    new_arena = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,0); //memory mapping of new arena
    //if arena pointer is null there was problem with mmap; return NULL
    if (new_arena == NULL){
        return NULL;
    }
    new_arena->size = size; //setting size of new arena
    new_arena->next = NULL; //setting pointer to next arena

     //constructing first header
    Header *h1 = (Header*)(&new_arena[1]);
    hdr_ctor(h1, req_size);
    h1->asize = req_size;

    //testing if I should construct next header and block
    //if rest of the size of arena is less or equal to sizeof(Header) - if yes, I cannot construct second header and block
    //rest = size of arena - data in first block - size of header - sizeof header of arena
    size_t rest = new_arena->size - h1->size - sizeof(Header) - sizeof(Arena);
    if(rest <= sizeof(Header)){
        h1->next = h1; //first header will be pointing to herself
        h1->size = size;  //size is equal to what we allocated
    }
    else{
        rest -= sizeof(Header); //i have to deduct size of Header (the new header)
        Header *h2 = (Header*)(h1->next);
        hdr_ctor(h2, rest);
        h2->next = h1;  //second header will be pointing to the first header
    }
    return new_arena;
}

/**
 * Appends a new arena to the end of the arena list.
 * @param a     already allocated arena
 */
static
void arena_append(Arena *a)
{
    //if there's no arena, set new arena as first arena
    if (first_arena == NULL){
        first_arena = a; 
    }
    //else add new arena at the end; prev arena will be pointing to the new one
    else{
        Arena *nextA = first_arena;
        while(nextA->next != NULL){
            nextA = nextA->next;
        }
        nextA->next = a;
    }
    return;
}

/**
 * Header structure constructor (alone, not used block).
 * @param hdr       pointer to block metadata.
 * @param size      size of free block
 * @pre size > 0
 */
/**
 *   +-----+------+------------------------+----+
 *   | ... |Header|........................| ...|
 *   +-----+------+------------------------+----+
 *
 *                |-- Header.size ---------|
 */
static
void hdr_ctor(Header *hdr, size_t size)
{
    hdr->size = size;
    hdr->asize = 0;
    char *ptr = (char *)hdr;
    hdr->next = (Header *) (ptr + sizeof(Header) + size);
    return;
}

/**
 * Checks if the given free block should be split in two separate blocks.
 * @param hdr       header of the free block
 * @param size      requested size of data
 * @return true if the block should be split
 * @pre hdr->asize == 0
 * @pre size > 0
 */
static
bool hdr_should_split(Header *hdr, size_t size)
{
    return hdr->size > size + sizeof(Header);
}

/**
 * Splits one block in two.
 * @param hdr       pointer to header of the big block
 * @param req_size  requested size of data in the (left) block.
 * @return pointer to the new (right) block header.
 * @pre   (hdr->size >= req_size + 2*sizeof(Header))
 */
/**
 * Before:        |---- hdr->size ---------|
 *
 *    -----+------+------------------------+----
 *         |Header|........................|
 *    -----+------+------------------------+----
 *            \----hdr->next---------------^
 */
/**
 * After:         |- req_size -|
 *
 *    -----+------+------------+------+----+----
 *     ... |Header|............|Header|....|
 *    -----+------+------------+------+----+----
 *             \---next--------^  \--next--^
 */
static
Header *hdr_split(Header *hdr, size_t req_size)
{
    char *ptr = (char *)(hdr + 1); //pointer at the beginning of metadata
    Header *new_h = (Header *)(ptr + req_size); //move of the reqested data size and construct new header here
    hdr_ctor(new_h, hdr->size - req_size - sizeof(Header));
    //next header of new header is next header of hdr
    new_h->next = hdr->next;
    //set given header
    hdr->size = req_size;
    hdr->next = new_h;
    hdr->asize = req_size;
    return new_h;
}

/**
 * Detect if two adjacent blocks could be merged.
 * @param left      left block
 * @param right     right block
 * @return true if two block are free and adjacent in the same arena.
 * @pre left->next == right
 * @pre left != right
 */
static
bool hdr_can_merge(Header *left, Header *right)
{   
    //two headers and blocks can merge, if they left is pointing to the rigt, asize of both is 0 and header cannot point to herself
    return ((left->next == right) && (left->asize == 0) && (right->asize == 0) && (left->next != left));
}

/**
 * Merge two adjacent free blocks.
 * @param left      left block
 * @param right     right block
 * @pre left->next == right
 * @pre left != right
 */
static
void hdr_merge(Header *left, Header *right)
{
    left->next = right->next;
    left->size = left->size + sizeof(Header) + right->size;
}

/**
 * Finds the free block that fits best to the requested size.
 * @param size      requested size
 * @return pointer to the header of the block or NULL if no block is available.
 * @pre size > 0
 */
static
Header *best_fit(size_t size)
{
    Arena *a = first_arena;
    Header *curr_h = NULL; //variable for current header
    Header *first_h = NULL; //variable for frist header of arena (for checking we are at the beginning of arena again)
    Header *best = NULL; //variable for saving best found fit
    //while we are not behind last arena
    while(a != NULL){
        first_h = (Header*)(&a[1]); //set first header of arena
        curr_h = first_h;   //start at first header of arena
        do{
            //if header and block is unused
            if(curr_h->asize == 0){
                //if there's enough of space
                if(curr_h->size >= size){
                    //if best is NULL (so best fit wasn't find yet) or current size is smaller than saved size, save new header
                    if(best == NULL || curr_h->size < best->size){
                        best = curr_h;
                    }
                }
            }
            curr_h = curr_h->next; //move to the next header
        }while(curr_h != first_h);
        a = a->next; //moveto to the next arena
    }
    return best;
}

/**
 * Search the header which is the predecessor to the hdr. Note that if 
 * @param hdr       successor of the search header
 * @return pointer to predecessor, hdr if there is just one header.
 * @pre first_arena != NULL
 * @post predecessor->next == hdr
 */
static
Header *hdr_get_prev(Header *hdr)
{
    Header *curr_h = hdr;
    //while next header of current header is same as given header, move to the next header
    while((curr_h->next) != hdr){
        curr_h = curr_h->next;
    }
    return curr_h;
}

/**
 * Allocate memory. Use best-fit search of available block.
 * @param size      requested size for program
 * @return pointer to allocated data or NULL if error or size = 0.
 */
void *mmalloc(size_t size)
{
    if(size == 0){
        return NULL;
    }
    void *new_area = best_fit(size); //save pointer to free data
    //if NULL was returned
    if(new_area == NULL){
        Arena *new_a = arena_alloc(size); //allocate new arena
        arena_append(new_a); //append new arena
        Header *arena_header = (Header *)&new_a[1]; //save first header of arena (where our data are saved)
        new_area = (void *)(arena_header + 1); //move to the data and store it in variable
    }
    else{
       Header *new_h = (Header*)new_area; //save pointer to the Header 
       if(hdr_should_split(new_h, size)){
           hdr_split(new_h, size);
       }     
       new_h->asize = size;
       new_area = new_h + 1; //store pointer to data in variable
    }
    return new_area;
}

/**
 * Search if given header is first header
 * @param hdr       successor of the search header
 * @return true if h is first header of some arena, false if not
 */
bool is_first(Header *h){
    Arena *a = first_arena; //start at first arena
    Header *first_h = NULL;
    while(a != NULL){
        first_h = (Header*)(&a[1]); //save first header of current arena
        if(first_h == h){
            return true;
        }
        a = a->next; //moving at next arena
    }
    return false;
}

/**
 * Search if given header is last header
 * @param hdr       successor of the search header
 * @return true if h is last header of some arena, false if not
 */
bool is_last(Header *h){
    Arena *a = first_arena; //start at first arena
    Header *last_h = NULL;
    while(a != NULL){
        last_h = (Header*)(&a[1]); //save first header of current arena
        last_h = hdr_get_prev(last_h);
        if(last_h == h){
            return true;
        }
         a = a->next; //moving at next arena
    }
    return false;
}

/**
 * Free memory block.
 * @param ptr       pointer to previously allocated data
 * @pre ptr != NULL
 */
void mfree(void *ptr)
{
    Header *h = (Header *)ptr; //type conversion of void pointer to Header pointer
    h = h-1;  //move to the header of given data  
    h->asize = 0; //set asize as 0 = unused

    //if header is not last header of some arena, try to merge
    if((h != h->next) && (!is_last(h))){
        if(hdr_can_merge(h, h->next)){
            hdr_merge(h, h->next);
        }  
    }
    //if header is not first header of some arena, try to merge
    if((h != h->next) && (!is_first(h))){
        Header *prev = hdr_get_prev(h);
        if(hdr_can_merge(prev, h)){
            hdr_merge(prev, h);
        }
    }
    return;
}

/**
 * Reallocate previously allocated block.
 * @param ptr       pointer to previously allocated data
 * @param size      a new requested size. Size can be greater, equal, or less
 * then size of previously allocated block.
 * @return pointer to reallocated space.
 */
void *mrealloc(void *ptr, size_t size)
{
    Header *h = (Header *)ptr; //type conversion of void pointer to Header pointer
    h = h-1; //move to the header of given data 
    //if wanted size is smaller or equal than current size, try to split block; if block can split, split; else set new asize
    if(size <= h->size){
        if(hdr_should_split(h, size)){
            hdr_split(h, size);
        }
        else{
            h->asize = size;
        }
        return ptr;
    }
    //if there's free block after this block, and if there's enought space for wanted size, merge them and change asize to wanted size
    if((!is_last(h)) && (h->next->asize == 0) && ((h->size + sizeof(Header) + h->next->size) >= size)){
        hdr_merge(h, h->next);
        h->asize = size;
        return ptr;
    }
    //if there's not enought space after this block, realloc - call mmalloc, copy data and set current header as unused (asize = 0)
    void *new = mmalloc(size);
    memcpy(new, ptr, h->asize);
    h->asize = 0;
    return new;
}
