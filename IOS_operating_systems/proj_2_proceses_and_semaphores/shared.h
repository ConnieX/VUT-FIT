//shared.h
//Reseni IOS - projekt 2, 25. 4. 2019
//Autor: Monika Rosinska, xrosin00, VUT FIT
//header file for shared.c, declare semaphores, shared variables and output file

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <semaphore.h>


#ifndef _SHARED_H_
#define _SHARED_H_


//typedef struct counters;
typedef struct counters{
    unsigned int action_id;
    unsigned int waiting_hackers;
    unsigned int waiting_serfs;
    unsigned int waiting;
    unsigned int running; //running processes
    unsigned int max_waiting; //capacity of pier
    unsigned int on_boat; //count of persons on boat
}counters_t;


counters_t *counter; 

sem_t *sem_boat; //semaphore for signalize leaving a boat
sem_t *sem_board_hacker; //semaphore for boarding hackers
sem_t *sem_board_serfer; //semaphore for boarding serfes
sem_t *sem_exit; //semaphore for waiting for a leaving boat
sem_t *sem_critical; //semaphore for critical sections
sem_t *sem_call; //semaphore for calling boat (can't use sem_critical - doesn't work)

FILE *output;

//function: initializate semaphores
void create_sems();
//function: destroys semaphores
void des_sems();

#endif
