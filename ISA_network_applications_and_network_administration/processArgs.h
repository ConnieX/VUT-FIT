//Author: Monika Rosinská, xrosin00
//Date: 15. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#ifndef ISA_PROCESSARGS_H
#define ISA_PROCESSARGS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

//print message and exit program with EXIT_FAILURE
//string message - message you want to print to stdout
void exit(string message);

//process given arguments
//
//int argc - number of arguments
//string* dnsServer - variable to store dns server name/IP
//char* argv[] - array with arguments
//int* portNumber - variable to store port number
//string* file - variable to store name of file with banned domains
//
//executes program if unknown argument is given, if there are more arguments of same type
//  or if server or file with banned domains is not given
//else fills dnsServer, portNumber and file
void processArgs(int argc, char* argv[], string* dnsServer, int* portNumber, string* file);

#endif
