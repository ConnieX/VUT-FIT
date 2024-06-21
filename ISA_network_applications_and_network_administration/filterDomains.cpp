//Author: Monika Rosinská, xrosin00
//Date: 14. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#include "filterDomains.h"
#define WHITESPACE " \n\r\t\f\v"

string removeSpaces(string line){
    size_t start = line.find_first_not_of(WHITESPACE);
    if(start == string::npos){
        return "";
    }
    else{
        return line.substr(start);
    }
}

//load banned domain from given file
//executes program if file cannot be opened
list<string> loadBannedDomains(string fileName){
    ifstream banFile(fileName); //open file
    if (banFile.peek() == ifstream::traits_type::eof()) //check for file open failure
    {
        perror("Error during opening file.\n");
        exit(EXIT_FAILURE);
    }

    list<string> bannedDomains;
    string line;
    while(getline(banFile, line)){
        removeSpaces(line);
        //remove blank lines and comments
        if(line != "" && line.find_first_of("#") != 0){
            bannedDomains.push_back(line);
        }
    }

    banFile.close();
    if (bannedDomains.empty()){
        cout << "Note that file is empty. No domains will be filtered.\n Execute program if you want to use another file.\n";
    }
    return bannedDomains;
}

//function that check, if domain is not banned and if it's not subdomain of banned domain
//returns true if domainName is allowed, false if domain is banned
bool checkDomain(string domainName, list<string> bannedDomains){
    size_t pos; //position of dot (to remove subdomain)
    do{
        //go through list with banned domains
        for(string bannedDomain : bannedDomains){
            if (domainName == bannedDomain){
                return false;
            }
        }
        pos = domainName.find_first_of('.'); //get next dot in domain name
        pos++; //move to next char
        domainName = domainName.substr(pos); //split domain at first dot and save second part
    } while(pos != 0); //in pos will be 0 if no dot is found

    return true;
}