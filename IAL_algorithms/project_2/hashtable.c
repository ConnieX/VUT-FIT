/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	//each pointer in table set on NULL
	for(int i = 0; i < HTSIZE; i++){
		(*ptrht)[i] = NULL;
	}
	return;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	tHTItem* keyItem = (*ptrht)[hashCode(key)]; //process given key and go in corresponding index in table
	//go throught each item in corresponding "row" in table, if key of current item is same as the key, return it
	while(keyItem != NULL){
		if(strcmp(key, keyItem->key) == 0){
			return keyItem;
		}
		keyItem = keyItem->ptrnext;
	}
	return NULL;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	tHTItem* found = htSearch(ptrht, key); //search if there's item with this key
	//if item with this key exists, so actualize its data
	if(found != NULL){
		found->data = data;
	}
	//if not
	if(found == NULL){
		tHTItem * newItem = (tHTItem *) malloc(sizeof(tHTItem));
		//allocating failed
		if(newItem == NULL){
			return;
		}
		//setting values of item
		newItem->data = data;
		newItem->key = key;
		newItem->ptrnext = NULL;
		//getting hash value
		int hashKey = hashCode(key);
		//if there's some item in this hash value, set is as next item of new item
		if((*ptrht)[hashKey] != NULL){
			newItem->ptrnext = (*ptrht)[hashKey];
		}
		(*ptrht)[hashKey] = newItem; //set new item as first item of this hash value
		
	}
	return;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	tHTItem* item = htSearch(ptrht, key); //search for item
	if(item == NULL){
		return NULL;
	}
	return (&item->data);
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	tHTItem* keyItem = (*ptrht)[hashCode(key)]; //process given key and go in corresponding index in table
	tHTItem* prevItem = keyItem; //variable for saving previous item
	//go throught each item in corresponding "row" in table, if key of current item is same as the key, break
	while(keyItem != NULL){
		if(strcmp(key, keyItem->key) == 0){
			break;
		}
		prevItem = keyItem; //save current item
		keyItem = keyItem->ptrnext; //move to next item
	}
	//if keyItem is NULL, we didn't find corresponding item -> do nothing
	if(keyItem == NULL){
		return;
	}
	//if keyItem is first item in "row", set first item of "row" as next item of keyItem
	if(keyItem == (*ptrht)[hashCode(key)]){
		(*ptrht)[hashCode(key)] = keyItem->ptrnext;
	}
	//else set next item of previous item as next item of found item
	else{
		prevItem->ptrnext = keyItem->ptrnext;
	}
	free(keyItem); //free item we want to delete
	return;
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	tHTItem* deleteItem; //variable for item we want to free
	tHTItem* currItem; //variable for moving in row of hasht table
	//for each hash value of table
	for(int i = 0; i < HTSIZE; i++){
		currItem = (*ptrht)[i]; //save first item of "row" in table
		//while we are not at the end of "row"
		while(currItem != NULL){
			deleteItem = currItem; //save last visited item
			currItem = currItem->ptrnext;	//move in "row" of table
			free(deleteItem); //free previous item
		}
		(*ptrht)[i] = NULL; //set pointer on NULL
	}
	return;
}
