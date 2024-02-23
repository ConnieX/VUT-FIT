//proj2.c
//Reseni IOS - projekt 2, 25. 4. 2019
//Autor: Monika Rosinska, xrosin00, VUT FIT
//main function, loading arguments, callig generating process

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "process_generator.h"
#include "shared.h"

//function: try convert argument into int
//if argument is number, return 0
//if argument contains another character than numbers, return 1
int convert_int(char *argument){
    char c;
    int index = 0;
    int zero = 48; //ASCII value of 0
    int nine = 57; //ASCII value of 9
    while((c = argument[index])){
        if(c < zero || c > nine){
            //if it's another char than number
            return 1;
        }
        index++;
    }
    return 0;
}

//function: control arguments
int control_argument(char *argument, int index){
    if(index == 1){
        // 'P' must be integer, on interval <2;inf>, must be even
        if(convert_int(argument) == 1 || strtol(argument,NULL,10) < 2 || strtol(argument, NULL, 10)%2 != 0){
            fprintf(stderr, "Wrong argument number %d - %s: count must be integer, must be greater than 2, must be even.\n", index, argument);
            return 1;
        }
    }
    else if(index == 2 || index == 3 || index == 4){
        // 'H', 'S' and 'R' must be integer, on interval <0;2000>
        if(convert_int(argument) == 1 || strtol(argument,NULL,10) < 0 || (strtol(argument,NULL,10) > 2000)){
            fprintf(stderr, "Wrong argument number %d - %s: count must be integer, must be greater than 0, must be less than 2 000.\n", index, argument);
            return 1;
        }
    }
    else if(index == 5){
        // 'W' must be integer, on interval <20,;2000>
        if(convert_int(argument) == 1 || strtol(argument,NULL,10) < 20 || (strtol(argument,NULL,10) > 2000)){
            fprintf(stderr, "Wrong argument number %d - %s: count must be integer, must be greater than 20, must be less than 2 000.\n", index, argument);
            return 1;
        }
    }
    else{
        // 'C' must be integer, on interval <5;inf>
        if( convert_int(argument) == 1 || strtol(argument,NULL,10) < 5){
            fprintf(stderr, "Wrong argument number %d - %s: count must be integer, must be greater than 20, must be less than 2 000.\n", index, argument);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]){
    //if rignt count of arguments is not given
    if(argc != 7){
        fprintf(stderr, "Wrong count of arguments. Please give us: \n   P - count of persons in each category \n   H - maximal time to generate >hackers< process \n   S - maximal time to generate >serfers< process \n   R - maximal time of voyage \n   W - maximal time of waiting before returning to the pier \n   C - capacity of the pier\n");
        return 1;
    }
    //control all arguments
    for(int i = 1; i < 7; i++){
        if(control_argument(argv[i], i) == 1){
            return 1;
        }
    }

    //load all arguments to variables (we know they're integers)
    unsigned int P = strtol(argv[1], NULL, 10);
    unsigned int H = strtol(argv[2], NULL, 10);
    unsigned int S = strtol(argv[3], NULL, 10);
    unsigned int R = strtol(argv[4], NULL, 10);
    unsigned int W = strtol(argv[5], NULL, 10);
    unsigned int C = strtol(argv[6], NULL, 10);

    //allocating memory for shared variables
    counter = mmap(NULL, sizeof(counters_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON , 0, 0);
    counter->action_id = 1; //id starts at 1
    counter->waiting_hackers = 0; //no waiting hackers
    counter->waiting_serfs = 0; //no waiting serfs
    counter->waiting = 0; //no waiting persons
    counter->running = 0; //no running processes
    counter->max_waiting = C; //capacity of pier

    create_sems(); //creating semaphores
    output = fopen("proj2.out", "w");

    pid_t p_hacker, p_serf; //processes for hackers and serfs

    //generate processes hacker and serf, control if generating was succesfull
    if ( (p_hacker = generator(P, H, R, W, &add_hacker)) < 0 || (p_serf = generator(P, S, R, W, &add_serf) < 0 )){
		des_sems(); //destroy semaphores
        munmap(&counter, sizeof(counters_t)); //free memory (free shared variables)
        fclose(output);
        return 1; //exit failure;
	}

	waitpid(p_hacker, NULL, 0); //wait for ending hacker processes
	waitpid(p_serf, NULL, 0); //wait for ending serf processes
    des_sems(); //destroy semaphores
    munmap(&counter, sizeof(counters_t)); //free memory (free shared variables)
    fclose(output);
    return 0;
}