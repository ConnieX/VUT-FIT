//process_generator.h
//Reseni IOS - projekt 2, 25. 4. 2019
//Autor: Monika Rosinska, xrosin00, VUT FIT
//header file for process_generator.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#ifndef _PROCESS_GEN_H_
#define _PROCESS_GEN_H_

void wait_random_time(int max);

//function: boat
//check count of hackers and serf on pier
//if teher's pissible combination of passengers, boards them
void boat();

//function: hacker processing
//id - id of hacker
//ret_time - time hackers waits until going back to pier
//voyage_time - time of voyage (captain "sleeps")
void add_hacker(int id, unsigned int ret_time, unsigned int voyage_time);

//function: serfer processing
//id - id of serfer
//ret_time - time serfers waits until going back to pier
//voyage_time - time of voyage (captain "sleeps")
void add_serf(int id, unsigned int ret_time, unsigned int voyage_time);

//function: generator - generate hacker and serfer process
//person_count - how many hackers (serfers) processes generate
//gen_time - waiting time between generating next process
//voyage_time - time of voyage
//ret_time - time before hacker(serfer) try to return on pier
//action - process to generate
pid_t generator(unsigned int person_count, unsigned int gen_time, unsigned int voyage_time, unsigned int ret_time, void (*action)());

#endif