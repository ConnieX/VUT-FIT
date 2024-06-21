//Author: Monika Rosinská, xrosin00
//Date: 14. 11. 2020
//Project: Vysoké učení technické v Brně
//         Fakulta informačních technologií
//         Síťové aplikace a správa sítí (ISA)

#ifndef ISA_FILTERDOMAINS_H
#define ISA_FILTERDOMAINS_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <list>
#include <fstream>
using namespace std;

//load banned domain from given file
//
//if file cannot be opened, executes program
//if file is empty (or doesn't contain any domain name, it will warn user about it)
//
//string fileName - string containing path to file
//returns list of banned domains
list<string> loadBannedDomains(string fileName);

//function that check, if domain is not banned and if it's not subdomain of banned domain
//
//string domainName - domain name you want to check
//list<string> bannedDomains - list with banned domains
//
//returns true if domainName is allowed, false if domain is banned
bool checkDomain(string DomainName, list<string> bannedDomains);

#endif
