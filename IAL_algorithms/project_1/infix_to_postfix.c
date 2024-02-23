
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
  if (stackEmpty(s)){
    return;
  }
  unsigned int lb = 40; //ASCII value of left bracket
  char c; //variable for char from stack
  for(int i = s->top; i > -1; i--){ //do until stack is empty
    stackTop(s, &c); //store char from top af the stack to c
    stackPop(s);
    if(c == lb){ //if the char is left bracket, return
      return;
    }
    postExpr[*postLen]= c; //store c to the end of result string
    (*postLen)++; //increase count of characters in result string 
  }
  return;
}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
  static unsigned int zero = 48, nine = 57, a = 97, z = 122, A = 65, Z = 90;         //ASCII value of numbers 0, 9 and chars a, A, z, Z
  static unsigned int plus = 43, minus = 45, mul = 42, div = 47, lb = 40, rb = 41;  //ASCII value of plus, minus, multiple, division, left bracket and right bracket
  static char on_stack = '0'; //variable for char at the top of stack (reduce number of accesses to stack); now stack is empty 
  //if the char is number or char a-z or A-Z, put it in the result string
  if((c >= zero && c <= nine) || (c >= a && c <= z) || (c >= A && c <= Z)){
    postExpr[*postLen] = c;
    (*postLen)++;
  }
  else if(c == plus || c == minus){
   // while there's plus/minus/mul or div on stack or while stack is not empty move char from stack to result string
    while(on_stack == plus || on_stack == minus || on_stack == mul || on_stack == div){
      if(stackEmpty(s)){
        break;
      }
      postExpr[*postLen] = on_stack; //move char from stack to result string
      (*postLen)++;                  //increase count of characters in result string
      stackPop(s);                   //decrease top of the stack
      on_stack = s->arr[s->top];     //save char on top of stack to variable
    }
    stackPush(s, c); //push char to stack
   }
   else if(c == mul || c == div){
     // while there's mul or div on stack or while stack is not empty move char from stack to result string
     while(on_stack == mul || on_stack == div){
       if(stackEmpty(s)){
         break;
       }
       postExpr[*postLen] = on_stack; //move char from stack to result string
       (*postLen)++;                  //increase count of characters in result string
       stackPop(s);                   //decrease top of the stack
       on_stack = s->arr[s->top];     //save char on top of stack to variable
    }
     stackPush(s, c); //push char to stack
    }
    else if(c == lb){
      stackPush(s, c); //push char (left bracket) to stack
    }
    else if(c == rb){
     untilLeftPar(s, postExpr, postLen); // move char from stack to result string until there's left bracket on the top of the stack
    }
    on_stack = s->arr[s->top]; //save char on top of stack to variable
  }

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {
  char *posExpr = malloc(MAX_LEN); //result string
  if (posExpr == NULL){
    return posExpr;
  }

  tStack s;   //stack for chars that cannot be put in result string now
  s.top = -1;

  unsigned int n = 0; //number of chars from infix string we already read
  unsigned int m = 0; //number of chars in result string
  char c = infExpr[n];  //variable for chars in infix expresion

  unsigned int eq = 61; //ASCII value of equality
  
  //while the char isn't equality
  while(c != eq){
    doOperation(&s, c, posExpr, &m);  //call function for decide what to do with char
    n++;  //increase counter of already read chars from infix expression
    c = infExpr[n]; //load new char from infix expression
  }
  while(!stackEmpty(&s)){
    posExpr[m] = s.arr[s.top]; //put char at top of stack to result string
    m++;  //increase count of characters in result string
    stackPop(&s);
  }
  posExpr[m] = c; //put last char (it always will be equality) at the end of result string
  m++;
  posExpr[m] = '\0'; //put end '\0' at the end of result string (it marks end of string)
  return posExpr;
}

/* Konec c204.c */
