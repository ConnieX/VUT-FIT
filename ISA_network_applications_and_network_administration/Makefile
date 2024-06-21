OBJS	= main.o createConnection.o createResponse.o filterDomains.o processArgs.o processRequest.o
SOURCE	= main.cpp createConnection.cpp createResponse.cpp filterDomains.cpp processArgs.cpp processRequest.cpp
HEADER	= createConnection.h createResponse.h filterDomains.h processArgs.h processRequest.h
OUT	= dns
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

createConnection.o: createConnection.cpp
	$(CC) $(FLAGS) createConnection.cpp

createResponse.o: createResponse.cpp
	$(CC) $(FLAGS) createResponse.cpp

filterDomains.o: filterDomains.cpp
	$(CC) $(FLAGS) filterDomains.cpp

processArgs.o: processArgs.cpp
	$(CC) $(FLAGS) processArgs.cpp

processRequest.o: processRequest.cpp
	$(CC) $(FLAGS) processRequest.cpp


clean:
	rm -f $(OBJS) $(OUT)

test:
	python3 test.py