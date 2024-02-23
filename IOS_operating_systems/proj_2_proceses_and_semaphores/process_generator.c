//process_generator.c
//Reseni IOS - projekt 2, 25. 4. 2019
//Autor: Monika Rosinska, xrosin00, VUT FIT
//generating processes, functions for processes

#include "process_generator.h"
#include "shared.h"

//function: sleep process for random time of <0;max>
//if max == 0, wait 0 sec
void wait_random_time(int max)
{
	if (max > 0) {
		usleep((random() % max * 1000));
	}
}

//function: boat
//check count of hackers and serf on pier
//if teher's pissible combination of passengers, boards them
void boat(){
	unsigned int hackers; //counter of hackers on pier
	unsigned int serfers; //counter of serfs on pier
		//if someone is on boat, nothing happens
		if(counter->on_boat != 0){
			return;
		}
		//if there are 4 or more hackers, "boards" 4 hackers
		if(counter->waiting_hackers > 3){
			hackers = 4;
			serfers = 0;
		}
		//if there are 4 or more serfs, "boards" 4 serfs
		else if(counter->waiting_serfs > 3){
			hackers = 0;
			serfers = 4;
		}
		//if there are 2 or more hackers and serfs, "boards" 2 hackers and 2 serfs
		else if((counter->waiting_hackers > 1) && (counter->waiting_serfs > 1)){
			hackers = 2;
			serfers = 2;
		}
		//else boards nobody
		else{
			hackers = 0;
			serfers = 0;
		}

		//if boat "boarded" 4 people
		if(hackers+serfers == 4){
			sem_wait(sem_critical); //critical section
			counter->on_boat = 0; //on boat is nobody (for security)
			//boards all hackers (2 or 4)
			for (unsigned int i = 0; i < hackers; i++){
				sem_post(sem_board_hacker);
				counter->waiting_hackers--;
				//sem_post(sem_critical);
			}
			//boards all serfs (2 or 4)
			for (unsigned int i = 0; i < serfers; i++){
				//sem_wait(sem_critical);
				sem_post(sem_board_serfer);
				counter->waiting_serfs--;
			}
			sem_post(sem_critical); //end of critical section
		}
	return;
}

//function: hacker processing
//id - id of hacker
//ret_time - time hackers waits until going back to pier
//voyage_time - time of voyage (captain "sleeps")
void add_hacker(int id, unsigned int ret_time, unsigned int voyage_time){
	sem_wait(sem_critical); //critical section
	counter->running++;
    fprintf(output, "%d	: HACK %d	: starts\n", counter->action_id, id);
	fflush(output);
	counter->action_id++;
	sem_post(sem_critical); //end of critical section
	//cycle, until he is on pier
	while(1){
		//if there's free place on pier
		sem_wait(sem_critical); //critical section
		if((counter->waiting_hackers + counter->waiting_serfs) < counter->max_waiting){
			counter->waiting_hackers++;
			counter->waiting++;
			fprintf(output, "%d	: HACK %d	: waits		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
			fflush(output);
			counter->action_id++;
			sem_post(sem_critical); //end of critical section
			//sem_post(sem_boat); ODDELAT
			break;
		}
		//if there's not free place
		else{
			fprintf(output, "%d	: HACK %d	: leaves queue		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
			fflush(output);
			counter->action_id++;
			sem_post(sem_critical); //end of critical section
			wait_random_time(ret_time); //wait before try to come again
			sem_wait(sem_critical); //critical section
			fprintf(output, "%d	: HACK %d	: is back\n", counter->action_id, id);
			fflush(output);
			counter->action_id++;
			sem_post(sem_critical); //end of critical section
		}
	}
		
	sem_wait(sem_call); //critical section
	boat(); //call boat (check if 'I' can board)
	sem_post(sem_call); //end of critical section
		
	sem_wait(sem_board_hacker); //wait for signal to board
	sem_wait(sem_critical); //critical section
	counter->on_boat++;
	//if there's 4 people on boat, become a captain (the last peron on boat becomes a captain);
	if(counter->on_boat == 4){
		counter->waiting = counter->waiting-4; //decrement waiting people
		fprintf(output, "%d	: HACK %d	: boards		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
		fflush(output);
		counter->action_id++;
		sem_post(sem_critical); //end of critical section
		wait_random_time(voyage_time); //wait - representing voyage
		//sending singnal to other passengers to leave boat
		for(int i = 0; i < 3; i++){
			sem_post(sem_exit);
		}
		//wait until there's only him on boat
		while(counter->on_boat > 1){
			sem_wait(sem_boat);
		}
		sem_wait(sem_critical); //critical section
		fprintf(output, "%d	: HACK %d	: captain exits		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
		fflush(output);
		counter->action_id++;
		counter->on_boat--;
		sem_post(sem_critical); //end of critical section
		sem_wait(sem_call);//critical section
		boat(); //call boat (if there are waiting people on pier, they can't call boat again)
		sem_post(sem_call); //end of critical section
	}
	//if he's not last person on boat
	else{
		sem_post(sem_critical); //end of critical section
		sem_wait(sem_exit); //wait for signal to leave boat
		sem_wait(sem_critical); //critical section
		fprintf(output, "%d	: HACK %d	: member exits		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
		fflush(output);
		counter->action_id++;
		counter->on_boat--;
		sem_post(sem_critical); //end of critical section
		sem_post(sem_boat); //send signal "I'm leaving boat"
	}
	sem_wait(sem_critical);
	counter->running--;
	sem_post(sem_critical);
	fclose(output);
	des_sems();
	munmap(&counter, sizeof(counters_t)); //free memory (free shared variables)
	exit(EXIT_SUCCESS);
}

//function: serfer processing
//id - id of serfer
//ret_time - time serfers waits until going back to pier
//voyage_time - time of voyage (captain "sleeps")
void add_serf(int id, unsigned int ret_time, unsigned int voyage_time){
	sem_wait(sem_critical); //critical section
	counter->running++;
    fprintf(output, "%d	: SERF %d	: starts\n", counter->action_id, id);
	fflush(output);
	counter->action_id++;
	sem_post(sem_critical); //end of critical section
	//cycle, until he is on pier
	while(1){
		//if there's free place on pier
		sem_wait(sem_critical); //critical section
		if((counter->waiting_hackers + counter->waiting_serfs) < counter->max_waiting){
			counter->waiting_serfs++;
			counter->waiting++;
			fprintf(output, "%d	: SERF %d	: waits		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
			fflush(output);
			counter->action_id++;
			sem_post(sem_critical); //end of critical section
			//sem_post(sem_boat); ODDELAT
			break;
		}
		//if there's not free place
		else{
			fprintf(output, "%d	: SERF %d	: leaves queue		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
			fflush(output);
			counter->action_id++;
			sem_post(sem_critical); //end of critical section
			wait_random_time(ret_time); //wait before try to come again
			sem_wait(sem_critical); //critical section
			fprintf(output, "%d	: SERF %d	: is back\n", counter->action_id, id);
			fflush(output);
			counter->action_id++;
			sem_post(sem_critical); //end of critical section
		}
	}
		
	sem_wait(sem_call); //critical section
	boat(); //call boat (check if 'I' can board)
	sem_post(sem_call); //end of critical section
		
	sem_wait(sem_board_serfer); //wait for signal to board
	sem_wait(sem_critical); //critical section
	counter->on_boat++;
	//if there's 4 people on boat, become a captain (the last peron on boat becomes a captain);
	if(counter->on_boat == 4){
		counter->waiting = counter->waiting-4; //decrement waiting people
		fprintf(output, "%d	: SERF %d	: boards		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
		fflush(output);
		counter->action_id++;
		sem_post(sem_critical); //end of critical section
		wait_random_time(voyage_time); //wait - representing voyage
		//sending singnal to other passengers to leave boat
		for(int i = 0; i < 3; i++){
			sem_post(sem_exit);
		}
		//wait until there's only him on boat
		while(counter->on_boat > 1){
			sem_wait(sem_boat);
		}
		sem_wait(sem_critical); //critical section
		fprintf(output, "%d	: SERF %d	: captain exits		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
		fflush(output);
		counter->action_id++;
		counter->on_boat--;
		sem_post(sem_critical); //end of critical section
		sem_wait(sem_call);//critical section
		boat(); //call boat (if there are waiting people on pier, they can't call boat again)
		sem_post(sem_call); //end of critical section
	}
	//if he's not last person on boat
	else{
		sem_post(sem_critical); //end of critical section
		sem_wait(sem_exit); //wait for signal to leave boat
		sem_wait(sem_critical); //critical section
		fprintf(output, "%d	: SERF %d	: member exits		:%d		:%d\n", counter->action_id, id, counter->waiting_hackers, counter->waiting_serfs);
		fflush(output);
		counter->action_id++;
		counter->on_boat--;
		sem_post(sem_critical); //end of critical section
		sem_post(sem_boat); //send signal "I'm leaving boat"
	}
	sem_wait(sem_critical);
	counter->running--;
	sem_post(sem_critical);
	fclose(output);
	des_sems();
	munmap(&counter, sizeof(counters_t)); //free memory (free shared variables)
	exit(EXIT_SUCCESS);
}

//function: generator - generate hacker and serfer process
//person_count - how many hackers (serfers) processes generate
//gen_time - waiting time between generating next process
//voyage_time - time of voyage
//ret_time - time before hacker(serfer) try to return on pier
//action - process to generate
pid_t generator(unsigned int person_count, unsigned int gen_time, unsigned int voyage_time, unsigned int ret_time, void (*action)()){
	pid_t pid = fork(); // create new process for generating processes
	if (pid != 0) {
		return pid;
	}

	//create >person count< processes
	for (unsigned int i = 1; i <= person_count; i++) {
		// wait before generating new process
		wait_random_time(gen_time);
		// create new process
		pid_t child_pid = fork();
		if (child_pid == 0) {
			action(i,ret_time, voyage_time); // call process
		} 
		else if (child_pid < 0) {
			// fork error
			return child_pid;
		}
		// process continue;
		wait_random_time(gen_time);
	}

	//wait for all person processes
	while (wait(NULL)) {
		if (counter->running < 1) {
		 // no processes
			break;
		}
	}
	fclose(output);
	des_sems();
	munmap(&counter, sizeof(counters_t)); //free memory (free shared variables)
	// exit processes generator
	exit(EXIT_SUCCESS);
}