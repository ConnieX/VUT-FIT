//Author: Monika Rosinská, xrosin00
//Date: 14. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#include "processRequest.h"

//function for processing message and getting domain name
bool processMessage(unsigned char buffer[], int* bufferPos, string* domainName){
    int len; //length of domain name part

    len = buffer[12];
    *bufferPos = DOM_NAME_BYTE;
    if(len>63 || len <= 0){
        return false;
    }
    while(len != 0){
        len += (*bufferPos); //get position of last char
        for(; (*bufferPos) < len; (*bufferPos)++) {
            (*domainName) += buffer[*bufferPos];
        }

        len = buffer[(*bufferPos)++];  //get length of next part

        //if there's another part, add dot to string
        if(len != 0){
            (*domainName) += '.';
        }
    }
    if((*domainName).length() > MAX_DOMAIN_NAME_LEN){
        return false;
    }
    return true;
}

//fuction that check, if request is of type A (address)
bool reqIsOfTypeA(unsigned char buffer[], int bufferPos){
    int reqType = buffer[bufferPos] + buffer[bufferPos+1];
    if (reqType == TYPE_A){
        return true;
    }
    else{
        return false;
    }
}