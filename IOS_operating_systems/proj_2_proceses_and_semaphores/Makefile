#Makefile
#Reseni IOS - projekt 2, 25. 4. 2019
#Autor: Monika Rosinska, xrosin00, VUT FIT

PARAMS = -std=gnu99 -Wall -Wextra -Werror -pedantic


all: proj2

#sestaveni programu
proj2: proj2.c process_generator.c shared.c
	gcc ${PARAMS} proj2.c process_generator.c shared.c -o proj2 -lpthread -g
	
clean: 
	rm -f *.o proj2 proj2.out			