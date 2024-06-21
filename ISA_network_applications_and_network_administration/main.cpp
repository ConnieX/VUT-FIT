//Author: Monika Rosinská, xrosin00
//Date: 13. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)


#include <iostream>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <list>

#include "createConnection.h"
#include "processRequest.h"
#include "filterDomains.h"
#include "processArgs.h"
#include "createResponse.h"

using namespace std;

#define BUFFER_SIZE 2048

//#pragma ide diagnostic ignored "EndlessLoop"
//#pragma clang diagnostic push

int main(int argc, char* argv[]){
    int portNumber;
    string dnsServer;
    string fileName;
    processArgs(argc, argv, &dnsServer, &portNumber, &fileName);

    //temp values
   // fileName = "./file.txt";
    //dnsServer = "dns.google";

    char serverIP[dnsServer.length()+1];
    strcpy(serverIP, dnsServer.c_str());

    list<string> bannedDomains = loadBannedDomains(fileName);

    int serverSocket = createSocket();
    setTimer(serverSocket);
    struct sockaddr_in serverAddress = setServerAddress(serverIP);
    connectToServer(serverSocket, serverAddress);

    int portSocket = createSocket();
    struct sockaddr_in socketAddress = setSocketAddress(portNumber);
    bindSocket(portSocket, socketAddress);
    socklen_t socklen = sizeof(socketAddress);

    unsigned char buffer[BUFFER_SIZE]; //array for received message
    int messLength;
    int bufferPos = 0;

    char response[BUFFER_SIZE];
    int resLength;

    string domainName;

    while(true) {
        domainName = "";
        memset(buffer, 0, BUFFER_SIZE);
        //receive message from port, store in in buffer of length 2048, no flags, use this socket address of length socklen
        messLength = recvfrom(portSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &socketAddress, &socklen);

        if (messLength > 0) {
          //  buffer[messLength] = 0; //store zero at the end of message
            if(processMessage(buffer, &bufferPos, &domainName) == false){
                unsigned char answer[BUFFER_SIZE];
                createResponse(buffer, messLength, 1, answer);
                sendto(portSocket, answer, messLength, 0, (struct sockaddr*)&socketAddress, (socklen_t)(sizeof(socketAddress)));
            }
            else{
                if(domainName.empty() || reqIsOfTypeA(buffer, bufferPos)){
                    if(checkDomain(domainName, bannedDomains)){
                        send(serverSocket, buffer, messLength, 0);
                        resLength = read(serverSocket, response, BUFFER_SIZE);
                        if(resLength == -1){
                            unsigned char answer[BUFFER_SIZE];
                            createResponse(buffer, messLength, 2, answer);
                            sendto(portSocket, answer, messLength, 0, (struct sockaddr*)&socketAddress, (socklen_t)(sizeof(socketAddress)));
                        }
                        sendto(portSocket, response, resLength, 0, (struct sockaddr*)&socketAddress, (socklen_t)(sizeof(socketAddress)));
                    }
                    else{
                        memset(&response, 0, sizeof(response));
                        unsigned char answer[BUFFER_SIZE];
                        createResponse(buffer, messLength, 5, answer);
                        sendto(portSocket, answer, messLength, 0, (struct sockaddr*)&socketAddress, (socklen_t)(sizeof(socketAddress)));
                    }
                }
                else{
                    unsigned char answer[BUFFER_SIZE];
                    createResponse(buffer, messLength, 4, answer);
                    sendto(portSocket, answer, messLength, 0, (struct sockaddr*)&socketAddress, (socklen_t)(sizeof(socketAddress)));
                }
            }
        }
    }
}

//#pragma clang diagnostic pop