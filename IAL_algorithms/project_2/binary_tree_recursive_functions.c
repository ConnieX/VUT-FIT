/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/

    *RootPtr = NULL;
}

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/

    //if RootPtr is NULL, there's no Node, so we can't find any -> FALSE
    if(RootPtr == NULL){
        return FALSE;
    }
    //if we found Node with key K
    if(RootPtr->Key == K){
        *Content = RootPtr->BSTNodeCont;
        return TRUE;
    }
    //if key K is smaller than key of current node, search in the right subtree
    if(K > RootPtr->Key){
        return BSTSearch(RootPtr->RPtr, K, Content);
    }
    //else key K is greater than key of current node, search in the left subtree
    else{
        return BSTSearch(RootPtr->LPtr, K, Content);
    }
}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/

    //if there's no Node
    if(*RootPtr == NULL){
        (*RootPtr) = (tBSTNodePtr) malloc(sizeof(struct tBSTNode)); //allocate room for new node and check it
        if(*RootPtr == NULL){
            return;
        }
        //setting values of new node
        (*RootPtr)->BSTNodeCont = Content;
        (*RootPtr)->Key = K;
        (*RootPtr)->RPtr = NULL;
        (*RootPtr)->LPtr = NULL;
        return;
    }
    //if key K is smaller than key of current node, search right place in the right subtree
    if(K > (*RootPtr)->Key){
        BSTInsert(&((*RootPtr)->RPtr), K, Content);
    }
    //else if key K is smaller than key of current node, search right place in the left subtree
    else if(K < (*RootPtr)->Key)
    {
        BSTInsert(&((*RootPtr)->LPtr), K, Content);
    }
    //else we it's the same value, we can ignore it or set it, for me it's better to show the content is here and it's already saved
    else{
        (*RootPtr)->BSTNodeCont = Content;
    }
    return;
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně.
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete().
**/

    //if right node of root pointer is NULL, there's no subtree -> we can move and free the node
    if((*RootPtr)->RPtr == NULL){
        tBSTNodePtr delete = *RootPtr; //save pointer to the adresse of the node we want to move
        //setting values of node we want to replace by the most rignt one
        PtrReplaced->Key = delete->Key;
        PtrReplaced->BSTNodeCont = delete->BSTNodeCont;
        PtrReplaced->LPtr = delete->LPtr;
        free(delete); //freeing space for node we moved to place of "deleted" one
        return;
    }
    //else if there's only one node after right node of root pointer
    else if((*RootPtr)->RPtr->RPtr == NULL){
        tBSTNodePtr delete = (*RootPtr)->RPtr; //save pointer to the adresse of the node we want to move
        //setting values of node we want to replace by the most rignt one
        PtrReplaced->Key = delete->Key;
        PtrReplaced->BSTNodeCont = delete->BSTNodeCont;
        (*RootPtr)->RPtr = delete->LPtr;
        free(delete); //freeing space for node we moved to place of "deleted" one
        return;
    }
    //else there's more nodes in right subtree -> recursion
    else{
        ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr));
    }
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/
    //if root pointer is NULL, there's no node we could delete
    if(*RootPtr == NULL){
        return;
    }
    tBSTNodePtr deletePtr = *RootPtr; //save pointer to the adresse of the node we want to delete
    //if key K is smaller then key of root pointer, do:
    if(K < (*RootPtr)->Key){
        //if there's no node in left subtree, there can't be node with key K, return
        if((*RootPtr)->LPtr == NULL){
            return;
        }
        //if key of next node on the left is same as we search
        if(K == (*RootPtr)->LPtr->Key){
            deletePtr = (*RootPtr)->LPtr; //save pointer to the adresse of the node we want to delete
            //if this node doesn't have any subtrees, we can just free it
            if(((*RootPtr)->LPtr->LPtr == NULL) && ((*RootPtr)->LPtr->RPtr == NULL)){
                (*RootPtr)->LPtr = NULL;
                free(deletePtr);
            }
            //if this node  have only right subtree, set this subtree as left subtree of root pointer and delete the given node
            else if(((*RootPtr)->LPtr->LPtr == NULL)){
                (*RootPtr)->LPtr = deletePtr->RPtr;
                free(deletePtr);
            }
            //if this node  have only left subtree, set this subtree as left subtree of root pointer and delete the given node
            else if(((*RootPtr)->LPtr->RPtr == NULL)){
                (*RootPtr)->LPtr = deletePtr->LPtr;
                free(deletePtr);
            }
            //else we have to replace the node we want to delete by the most rignt node of left subtree
            else{
                ReplaceByRightmost((*RootPtr)->LPtr, &((*RootPtr)->LPtr->LPtr));
            }
            return;
        }
        //if key of next node on the left is not the same as we search, we have to search in next left subtree
        BSTDelete(&(*RootPtr)->LPtr, K);
    }
    //if key K is greater than key of root pointer, do:
    else if(K > (*RootPtr)->Key){
        //if there's no node in right subtree, there can't be node with key K, return
        if((*RootPtr)->RPtr == NULL){
            return;
        }
        //if key of next node on the right is same as we search
        if (K == (*RootPtr)->RPtr->Key){
            deletePtr = (*RootPtr)->RPtr; //save pointer to the adresse of the node we want to delete
            //if this node doesn't have any subtrees, we can just free it
            if(((*RootPtr)->RPtr->LPtr == NULL) && ((*RootPtr)->RPtr->RPtr == NULL)){
                (*RootPtr)->RPtr = NULL;
                free(deletePtr);
            }
            //if this node  have only right subtree, set this subtree as left subtree of root pointer and delete the given node
            else if(((*RootPtr)->RPtr->LPtr == NULL)){
                (*RootPtr)->RPtr = deletePtr->RPtr;
                free(deletePtr);
            }
            //if this node  have only left subtree, set this subtree as left subtree of root pointer and delete the given node
            else if(((*RootPtr)->RPtr->RPtr == NULL)){
                (*RootPtr)->RPtr = deletePtr->LPtr;
                free(deletePtr);
            }
            else{
                //else we have to replace the node we want to delete by the most rignt node of left subtree
                ReplaceByRightmost((*RootPtr)->RPtr, &((*RootPtr)->RPtr->LPtr));
            }
            return;
        }
        //if key of next node on the left is not the same as we search, we have to search in next right subtree
        BSTDelete(&(*RootPtr)->RPtr, K);
    }
    //else we found node with the key K
	else{
        //if this node doesn't have any subtrees, we can just free it
		if(((*RootPtr)->LPtr == NULL) && ((*RootPtr)->RPtr == NULL)){
			free(*RootPtr);
			*RootPtr = NULL;
			return;
		}
        //if this node  have only right subtree, set this subtree as left subtree of root pointer and delete the given node
		else if(((*RootPtr)->LPtr == NULL)){
			*RootPtr = (*RootPtr)->RPtr;
			free(deletePtr);
			return;
		}
        //if this node  have only left subtree, set this subtree as left subtree of root pointer and delete the given node
		else if(((*RootPtr)->RPtr == NULL)){
			*RootPtr = (*RootPtr)->LPtr;
			free(deletePtr);
			return;
		}
		else{
            //else we have to replace the node we want to delete by the most rignt node of left subtree
			ReplaceByRightmost((*RootPtr), &(*RootPtr)->LPtr);
		}
	}
    return;
}

void BSTDispose (tBSTNodePtr *RootPtr) {
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/

    //if root pointer is NULL, there's nothing to free
    if(*RootPtr == NULL){
        return;
    }
    //free left and right subtree
    BSTDispose(&((*RootPtr)->LPtr));
    BSTDispose(&((*RootPtr)->RPtr));
    //free root pointer and set it on NULL (free just free memory for another use, but pointer still point to the adress)
    free(*RootPtr);
    *RootPtr = NULL;
    return;
}

/* konec c401.c */