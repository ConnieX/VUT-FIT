//Author: Monika Rosinská, xrosin00
//Date: 15. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#include "createConnection.h"

#define TIMEOUT 3

//function for creating socket
//if creating fails, executes program, else returns socket
int createSocket(){
    int newSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (newSocket == -1) {
        perror("Can't create socket.\n");
        exit(EXIT_FAILURE);
    }
    else{
        return newSocket;
    }
}

//function for binding socket
void bindSocket(int soc, sockaddr_in sa){
    if (bind(soc,(struct sockaddr *)&sa, sizeof sa) == -1) {
        perror("Bind failed.\n Please make sure you are not using reserved ports (0 to 1023).\n Note that"
               " default port is 53.\n If you want to run program on reserved port, use correct command"
               " (for example on Ubuntu use sudo).\n");
        close(soc);
        exit(EXIT_FAILURE);
    }
}

//function for connecting to server
void connectToServer(int soc, sockaddr_in sa){
    if (connect(soc, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        perror("Connection failed.\n");
        exit(EXIT_FAILURE);
    }
}

//function for creating and filling endpoint of communication
//communication with client
sockaddr_in setSocketAddress(unsigned int port){
    struct sockaddr_in sa{};
    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    return sa;
}

//function for creating and filling endpoint of communication
//communication with server
sockaddr_in setServerAddress(const char* ip){
    struct sockaddr_in sa{};
    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(53);
    if(inet_pton(AF_INET, ip, &(sa.sin_addr.s_addr)) <= 0)
    {
        perror("Invalid address. Address is not supported. \n");
        exit(EXIT_FAILURE);
    }
    return sa;
}

//function that set timer for socket you are connected to server
struct timeval setTimer(int socketID){
    struct timeval timer;
    timer.tv_sec = TIMEOUT;
    timer.tv_usec = 0;
    setsockopt(socketID, SOL_SOCKET, SO_RCVTIMEO, &timer, sizeof(timer));
    return timer;
}
