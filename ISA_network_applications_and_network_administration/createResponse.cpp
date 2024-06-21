//Author: Monika Rosinská, xrosin00
//Date: 15. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#include "createResponse.h"

void createResponse(unsigned char buffer[], int messLen, int errorNumber, unsigned char* answer){
    //set ID
    answer[0] = buffer[0];
    answer[1] = buffer[1];

    char answerBit = (1<<7)+1;
    char recursionAndError = (1<<7) + errorNumber;
    //+1 or +4 or +5 - if condition
    answer[2] = answerBit;
    answer[3] = recursionAndError;

    for(int i = 4; i < messLen; i++){
        answer[i] = buffer[i];
    }
}