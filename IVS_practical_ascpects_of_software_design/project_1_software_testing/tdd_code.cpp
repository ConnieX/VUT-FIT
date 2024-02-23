//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Monika Rosinska <xrosin00@stud.fit.vutbr.cz>
// $Date:       $2019-04-03
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Monika Rosinska
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
    head = NULL;
}

PriorityQueue::~PriorityQueue()
{
    Element_t *deleteElement;
    deleteElement = GetHead();
    while (deleteElement != NULL)
    {
        Element_t *currentElement = deleteElement;
        deleteElement = deleteElement->pNext;
        delete currentElement;
    }
}

void PriorityQueue::Insert(int value)
{
    Element_t *newElement = new Element_t();
    newElement->value = value;
    Element_t *currentElement = GetHead();
    Element_t *nextElement;

    //empty queue
    if (currentElement == NULL)
    {
        head = newElement;
        newElement->pPrev = NULL;
        newElement->pNext = NULL;
        return;
    }
    //insert at the begining of the queue
    if (currentElement->value >= value)
    {
        head = newElement;
        newElement->pPrev = NULL;
        newElement->pNext = currentElement;
        currentElement->pPrev = newElement;
        return;
    }

    else
    {
        while (currentElement->pNext != NULL && currentElement->pNext->value < value)
        {
            currentElement = currentElement->pNext;
        }
        if (currentElement->pNext == NULL)
        {
            currentElement->pNext = newElement;
            newElement->pPrev = currentElement;
            newElement->pNext = NULL;
            return;
        }
        nextElement = currentElement->pNext;
        currentElement->pNext = newElement;
        nextElement->pPrev = newElement;
        newElement->pPrev = currentElement;
        newElement->pNext = nextElement;
    }
}

bool PriorityQueue::Remove(int value)
{
    Element_t *searchedElement = Find(value);
    //if element was't found
    if (searchedElement == NULL)
    {
        return false;
    }
    //if it's first element
    if (searchedElement->pPrev == NULL)
    {
        //if there's more than one element
        if (searchedElement->pNext != NULL)
        {
            searchedElement->pNext->pPrev = NULL;
            head = searchedElement->pNext;
        }
        //if there's only one element
        else
        {
            head = NULL;
        }
    }
    //if it's last element
    else if (searchedElement->pNext == NULL)
    {
        searchedElement->pPrev->pNext = NULL;
    }
    else
    {
        Element_t *previousElement = searchedElement->pPrev;
        Element_t *nextElement = searchedElement->pNext;
        previousElement->pNext = nextElement;
        nextElement->pPrev = previousElement;
    }
    delete searchedElement;
    return true;
}
PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t *currentElement = GetHead();
    while (currentElement != NULL)
    {
        printf("%d\n", currentElement->value);
        if (currentElement->value == value)
            return currentElement;
        currentElement = currentElement->pNext;
    }
    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return head;
}

/*** Konec souboru tdd_code.cpp ***/
