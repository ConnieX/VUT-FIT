//Author: Monika Rosinská, xrosin00
//Date: 15. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#include "processArgs.h"

#define DEFAULT_PORT 53

void exit(const char* message){
    perror(message);
    exit(EXIT_FAILURE);
}

//process given arguments
//executes program if unknown argument is given, if there are more arguments of same type
//  or if server or file with banned domains is not given
void processArgs(int argc, char* argv[], string* dnsServer, int* portNumber, string* file){
    bool serverFilled = false;
    bool filterFileFilled = false;
    bool portFilled = false;

    for(int i = 1; i < argc; i+=2){
        string param = argv[i];
        //parameter that indicates that next argument is DNS server name/IP
        if(param == "-s"){
            if(serverFilled)
                exit("Please insert only one DNS server.\n");
            auto address = gethostbyname(argv[i+1]);
            if(address == nullptr){
                exit("DNS server is invalid.\n");
                break;
            }
            *dnsServer = string(inet_ntoa(**(in_addr**)address->h_addr_list));
            serverFilled = true;
        }
            //parameter that indicates that next argument is port number
        else if(param == "-p"){
            if(portFilled)
                exit("Please insert only one port number.\n");
            portFilled = true;
            const char* value = argv[i+1];
            sscanf(value, "%d", portNumber);

            //sscanf returns EOF if cannot convert str to int; port < 0 is invalid
            if(*portNumber <= 0){
                exit("Port number is invalid.\n");
            }
        }
        //parameter that indicates that next argument is file with banned domains
        else if(param == "-f"){
            if(filterFileFilled)
                exit("Please insert only one file with banned domains.\n");
            filterFileFilled  = true;
            *file = argv[i+1];
        }
        else{
            cout << "Unknown parameter." + param + "\n";
            exit("Unknown parameter.\n");
        }
    }
    //if port is not filled, set default value
    if(!portFilled)
        *portNumber = DEFAULT_PORT;
    //server name and file with banned domains must be given
    if(!filterFileFilled)
        exit("Please insert file with banned domains.\n");
    if(!serverFilled)
        exit("Please insert DNS server.\n");
    return;
}
