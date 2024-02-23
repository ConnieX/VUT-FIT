#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <queue>
#include <mutex>
#include <regex>
#include <vector>
#include <iostream>
#include <string.h>


#define extra_mutex 2 //count of extra mutex (one for global variables, one for main proccess)
std::vector<std::mutex *> zamky; // array of mutexs

char *line; //global variable for given text
int score; //global variable for score
int running_thread; //global variable for count of running proccesses

//function for reading line from stdin
char *read_line(int *res) {
	std::string line;
	char *str;
	if (std::getline(std::cin, line)) {
		str=(char *) malloc(sizeof(char)*(line.length()+1));
		strcpy(str,line.c_str());
		*res=1;
		return str;
	} else {
		*res=0;
		return NULL;
	}

}

//function for each thread - regex match and change score if needed
//ID - ID of thread
//reg - regex
//my_score - score we should add to global score if regex match
void f(int ID, char *reg, int my_score) {
	bool match; //bool for remember if we have to change score or not
	do{
		match = false; //set as false (can be true after previous line)
		(*(zamky[ID+extra_mutex])).lock(); //wait for main process (printing line and loading new line)
		//if not EOL
		if(line != NULL){
			if(std::regex_match (line, std::regex(reg))){
				match = true; //if line match regex, we have to add my_score to score
			}
		}
		//wait for lock and lock global variables
		while(!(*(zamky[1])).try_lock());
		if(match == true){
			score += my_score;
		}
		running_thread--;
		//if this thread is the last one, unlock main proccess
		if(running_thread == 0){
			(*(zamky[0])).unlock();
		}
		(*(zamky[1])).unlock(); //unlock globa wariables
	} while(line != NULL); //while not EOL
}

int main(int argc, char *argv[]) {
	//controling count of arguments
	if(argc < 3){
		printf("USAGE: pgrep MIN_SCORE RE1 SC1 [ RE2 SC2 ] [ RE3 SC3 ] ...\n");
		return 0;
	}
	if((argc%2 == 1)){
		printf("USAGE: pgrep MIN_SCORE RE1 SC1 [ RE2 SC2 ] [ RE3 SC3 ] ...\n");
		return 0;
	}

	//controling >integer< parameters (each odd parameter)
	bool not_int = false;
	for(int j = 1; j < argc; j+=2){
		char *input = argv[j];
		int len = strlen(input);
		for(int i = 0; i < len; i++){
			//if char is not digit (except '-'), change argv (all parts) to 0 score
			if(!isdigit(input[i]) && (input[0] != '-' && i == 0)){
				for(int k = 0; k < len; k++){
					argv[j][k] = '0';
				}
				break;
			}
		}
	}

	int minimum; //minimum score
	//converting from char to int
	char *chsum = argv[1];
	sscanf(chsum, "%d", &minimum);

	int num = (argc - 1)/2; //count of threads
	int num_zamky = num+extra_mutex; //mutex for each thread, one for main proccess, one for global variables
	std::vector <std::thread *> threads; //array of threads

	// creating mutex
	zamky.resize(num_zamky);
	for(int i=0;i<num_zamky;i++){	
		std::mutex *new_zamek = new std::mutex();
		zamky[i]=new_zamek;
		(*(zamky[i])).lock();
	}
	(*(zamky[1])).unlock(); //unlock mutex for global variables

	// creating threads
	threads.resize(num);
	int j = 2; //first score is at index 2
	//variables for corventing char to int
	char *chscore = argv[1];
	int my_score;
	for(int i=0; i < num; i++){
		chscore = argv[j+1];
		sscanf(chscore, "%d", &my_score);
		std::thread *new_thread = new std::thread (f,i,argv[j],my_score);
		threads[i]=new_thread;
		j += 2;
	}

	int in;	
	do{
		while(!(*(zamky[1])).try_lock()); //lock global variables
		line = read_line(&in);
		running_thread = num;	
		score = 0; //actualize score
		(*(zamky[1])).unlock();

		//unlock all threads
		for(int j = extra_mutex; j < num_zamky; j++){
			(*(zamky[j])).unlock();
		}
		(*(zamky[0])).lock(); //wait for all threads
		if((minimum <= score) && (line != NULL)){
			printf("%s\n",line);
		}
		free(line);
	} while(line != NULL);

	// free thread memory
	for(int i=0;i<num;i++){
		(*(threads[i])).join();
		delete threads[i];
	}
	// free mutex memory
	for(int i=0;i<num_zamky;i++){
		delete zamky[i];
	}	
}
