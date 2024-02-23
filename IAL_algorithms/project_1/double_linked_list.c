
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    tDLElemPtr curr = L->Last;
    while(curr != NULL){
        L->Last = L->Last->lptr;
        free(curr);
        curr = L->Last;
    }
    //this is not compulsory, but it's for sure and it shows the List seems like when it was initialized
    L->First = NULL;
    L->Last = NULL;
    L->Act = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr temp = (tDLElemPtr) malloc(sizeof(struct tDLElem)); //allocating memory for new element
    if(temp == NULL){
        DLError();
        return;
    }
    //setting elements of temp on wanted values
    temp->data = val;
    temp->lptr = NULL;  //it will be set as first element -> left element will be NULL
    temp->rptr = L->First; //it will be set as first element -> right element will be the one which is first NOW

    //if list is empty, first element = last element = now created (temp)
    if(L->First == NULL){
        L->First = temp;
        L->Last = temp;
    }
    else{
        L->First->lptr = temp;  //the one element, which is first NOW, will have on left now created element
        L->First = temp;    //setting new element as first element  
    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr temp = (tDLElemPtr) malloc(sizeof(struct tDLElem));; //alocating memory for new element
    if(temp == NULL){
        DLError();
        return;
    }
    //setting elements of temp on wanted values
    temp->data = val;
    temp->rptr = NULL; //it will be set as last element -> right element will be NULL
    temp->lptr = L->Last;//it will be set as last element -> left element will be the one which is first NOW
   
    //if list is empty, first element = last element = now created (temp)
    if(L->Last == NULL){
        L->First = temp;
        L->Last = temp;
    }
    else{
        L->Last->rptr = temp; //the one element, which is last NOW, will have on right now created element
        L->Last = temp; //setting new element as last element  
    }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;
    return;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
    return;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if(L->First == NULL){
        DLError();
        return;
    }
    *val = L->First->data;
    return;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if(L->First == NULL){
       DLError();
       return;
    }
    *val = L->Last->data;
    return;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if(L->First == NULL){
        return;
    }
    tDLElemPtr temp = L->First;
    //if first element is active, activity will be NULL
    if(L->Act == temp){
        L->Act = NULL;
    }
    //if first element is last element too, set it as NULL;
    if(temp == L->Last){
        L->Last = NULL;
    }
    //else next elemet (on right) will be set as first element, so it will not have any element on left
    else{
        temp->rptr->lptr = NULL;
    }
    L->First = L->First->rptr; //seting new first element
    free(temp); //freeing element we want to delete
    return;
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
    if(L->First == NULL){
        return;
    }
	tDLElemPtr temp = L->Last;
    //if last element is active, activity will be NULL
    if(L->Act == temp){
        L->Act = NULL;
    }
    //if first element is last element too, set it as NULL;
    if(L->First == temp){
        L->First = NULL;
    }
    //else previous elemet (on left) will be set as last element, so it will not have any element on right
    else{
        temp->lptr->rptr = NULL;
    }
    L->Last = L->Last->lptr;    //seting new last element
    free(temp); //freeing element we want to delete
    return;

}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    //if there's no active element or if the active element is the last, do nothing
	if(L->Act == NULL || L->Act == L->Last){
        return;
    }
    tDLElemPtr delete = L->Act->rptr; //saving adress of element we want to delete
    L->Act->rptr = L->Act->rptr->rptr;  //seting new element on right side of active element
    //if the element we want to delete is not the last one
    if(delete->rptr != NULL){            //                                                                                                 ↓-------left prtr------|
        delete->rptr->lptr = L->Act;    //setting left neighbor of element on the right side of deleting element as active element ( ACTIVE E.  elemet to delete  next element)
    }
    //if the element we want to delete is the last one
    else{
        L->Last = L->Act;   //new last element will be the active one
        L->Act->rptr = NULL; //next element on the right side of active element will be none (NULL)
    }
    free(delete); //freeing element we want to delete
    return;
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    //if there's no active element or if the active element is the first, do nothing
	if(L->Act == NULL || L->Act == L->First){
        return;
    }
    tDLElemPtr delete = L->Act->lptr; //saving adress of element we want to delete
    L->Act->lptr = delete->lptr;    //seting new element on the left side of active element
    //if the element we want to delete is the first one
    if(delete == L->First){
        L->First = L->Act;  //new first element will be the active one
    }
    //if the element we want to delete is not the first one
    else{                             //                                                                                                      |-------right prtr------↓
        delete->lptr->rptr = L->Act; //setting right neighbor of element on the left side of deleting element as active element (previous element  elemet to delete  ACTIVE E. )
    }
    free(delete); //freeing element we want to delete
    return;	
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL){
        return;
    }
    tDLElemPtr temp = (tDLElemPtr) malloc(sizeof(struct tDLElem)); //alocating memory for new element
    if(temp == NULL){
        DLError();
        return;
    }
    //setting elements of temp on wanted values
    temp->data = val;
    temp->lptr = L->Act;    //it will be set after active element -> left element will be the active one
    temp->rptr = L->Act->rptr;  //next element (on the right side) will be the one which is after active element NOW
    //the active element is not the last one
    if(L->Act != L->Last){          //                                                                                                                     ↓-left prtr-|         
        L->Act->rptr->lptr = temp; //setting left element of the element which is NOW next active element as now created element (active element    new element    next element)
    }
    //if the active element is the last one
    else{
        L->Last = temp; //set new element as the last one
    }
    L->Act->rptr = temp; //next element of active element is now the new one
    return;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL){
        return;
    }
    tDLElemPtr temp = malloc(sizeof(struct tDLElem)); //alocating memory for new element
    if(temp == NULL){
        DLError();
        return;
    }
    //setting elements of temp on wanted values
    temp->data = val;
    temp->rptr = L->Act;    //it will be set before active element -> right element will be the active one
    temp->lptr = L->Act->lptr;  //previous element (on the left side) will be the one which is nefore active element NOW
    //if the active elemnt is the first one
    if(L->Act == L->First){
        L->First = temp;    //first element will be the new one
    }
    //if the active element is not the first one
    else{                            //                                                                                                         |-right prtr-↓
        L->Act->lptr->rptr = temp;  //setting rigt element of the element which is NOW before active element as now created element (previous element     new element   active element)
    }
    L->Act->lptr = temp; //previous element of active element is now the new one
    return;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if(L->Act == NULL){
        DLError();
        return;
    }
    *val = L->Act->data;
    return;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if(L->Act == NULL){
        return;
    }
    L->Act->data = val;
    return;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if(L->Act == NULL){
        return;
    }
    L->Act = L->Act->rptr;
	return;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act == NULL){
        return;
    }
    L->Act = L->Act->lptr;
    return;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return L->Act != NULL;
}

/* Konec c206.c*/