//shared.c
//Reseni IOS - projekt 2, 25. 4. 2019
//Autor: Monika Rosinska, xrosin00, VUT FIT
//functions for creating and destroy semaphores

#include "shared.h"
#include "process_generator.h"

//function: creating semaphores
void create_sems(){
    sem_unlink("/sem_boat");
    sem_boat = sem_open("/sem_boat", O_CREAT | O_EXCL, 0666, 0);
    sem_unlink("/sem_board_hacker");
    sem_board_hacker = sem_open("/sem_board_hacker", O_CREAT | O_EXCL, 0666, 0);
    sem_unlink("/sem_board_serfer");
    sem_board_serfer = sem_open("/sem_board_serfer", O_CREAT | O_EXCL, 0666, 0);
    sem_unlink("/sem_exit");
    sem_exit = sem_open("/sem_exit", O_CREAT | O_EXCL, 0666, 0);
    sem_unlink("/sem_critical");
    sem_critical = sem_open("/sem_critical", O_CREAT | O_EXCL, 0666, 1);
    sem_unlink("/sem_call");
    sem_call = sem_open("/sem_call", O_CREAT | O_EXCL, 0666, 1);
}

//function: destroying semaphores
void des_sems(){
    sem_unlink("/sem_boat");
    sem_close(sem_boat);
    sem_unlink("/sem_board_hacker");
    sem_close(sem_board_hacker);
    sem_unlink("/sem_board_serfer");
    sem_close(sem_board_serfer);
    sem_unlink("/sem_exit");
    sem_close(sem_exit);
    sem_unlink("/sem_critical");
    sem_close(sem_critical);
    sem_unlink("/sem_call");
    sem_close(sem_call);
}
