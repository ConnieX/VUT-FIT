//Author: Monika Rosinská, xrosin00
//Date: 15. 11. 2020
//Project: Vysoké Učení Technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#ifndef ISA_CREATECONNECTION_H
#define ISA_CREATECONNECTION_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


//function for creating socket
//if creating fails, executes program, else returns socket
int createSocket();

//function for binding socket
//
//int soc - socket number to bind
//sockaddr_in sa - address of socket soc
//
//if binding fails, executes program
void bindSocket(int soc, sockaddr_in sa);

//function for connecting to server
//
//int soc - socket number you want to connect to
//sockaddr_in sa - address of socket soc
//
//if connecting fails, executes program
void connectToServer(int soc, sockaddr_in sa);

//function for creating and filling endpoint of communication
//communication with client
//
//unsigned int port - port number program should expect requests
//returns filled sockaddr_in
sockaddr_in setSocketAddress(unsigned int port);

//function for creating and filling endpoint of communication
//communication with server
//
//const char* ip - IP address of DNS server
//if IP is invalid, executes program, else returns filled sockaddr_in
sockaddr_in setServerAddress(const char* ip);

//function that set timer for socket you are connected to server
//  so if server will not respond, program will not wait to infinitys
//int socketID - socket ID you are connected to the server
struct timeval setTimer(int socketID);

#endif