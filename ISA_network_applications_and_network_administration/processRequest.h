//Author: Monika Rosinská, xrosin00
//Date: 14. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#ifndef ISA_PROCESSREQUEST_H
#define ISA_PROCESSREQUEST_H

#include <iostream>
#include <cstdio>
#include <stdio.h>

using namespace std;

#define TYPE_A 1
#define MAX_DOMAIN_NAME_LEN 63
#define DOM_NAME_BYTE 13


//function for processing message and getting domain name
//
//unsigned char buffer[] - array with message to process
//int* bufferPos - variable with actual position in buffer
//int mesLength - length of received message
//string* domainName - variable to store domain name from request
//
//return true if domain name is ok, false if it's not
bool processMessage(unsigned char buffer[], int* bufferPos, string* domainName);

//function that check, if request is of type A (address)
//
//unsigned char buffer[] - array with request
//int bufferPos - variable with actual position in buffer
//              - assumes that this is the position where type of request is stored (depends on length of domain name)
//int mesLength - length of received message
//
//returns true if request if of type A, false if it's not
//if it's not of type A, send response to client with err code 5
bool reqIsOfTypeA(unsigned char buffer[], int bufferPos);

#endif
