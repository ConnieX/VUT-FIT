//PRL projekt: ODD-EVEN MERGE SORT
//Autor: Monika Rosinská, xrosin00
//Datum: 8. 4. 2022

#include <fstream>
#include <iterator>
#include <vector>
#include <cstdio>
#include <mpi.h>

using namespace std;

// work for processes (except main process)
// waits for input, sort it and send it to next processes
// input_1 - id of process, which sends me input 1
// input_2 - id of process, which sends me input 2
// output_1 - id of process which I send lower value
// output_2 - id of process which I  send higher value
void run_process(int input_1, int input_2, int output_1, int output_2){
    int input_1_value;
    int input_2_value;
    MPI_Recv(&input_1_value,1,MPI_INT,input_1,MPI_ANY_TAG ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&input_2_value,1,MPI_INT,input_2,MPI_ANY_TAG ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // sort input to right order
    if (input_1_value > input_2_value){
        int tmp = input_2_value;
        input_2_value = input_1_value;
        input_1_value = tmp;
    }

    MPI_Send(&input_1_value, 1, MPI_INT,output_1,1,MPI_COMM_WORLD);
    MPI_Send(&input_2_value, 1, MPI_INT,output_2,2,MPI_COMM_WORLD);
}

// loads input from file numbers
vector<int> load_input() {
    ifstream data("numbers", ios::binary);
    // load it as unsigned char, because it's stored by 8 byte binary code
    vector<unsigned char> input(
            (istreambuf_iterator<char>(data)),
            (istreambuf_iterator<char>()));

    // save it as numbers
    vector<int> numbers;
    for (unsigned char i : input) {
        numbers.push_back(i);
    }

    // print input
    for (int i : numbers){
        printf("%i ", i);
    }
    printf("\n");
    return numbers;
}

// send numbers to other processes
// numbers - vector of numbers to send (first two are sorted)
void sendInputs(vector<int> numbers){
    // MPI library function for sending messages
    // buffer where data of message are stored, count of data to send, datatype, destination processor, tag, communication canal
    MPI_Send(&(numbers[0]), 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
    MPI_Send(&(numbers[1]), 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
    MPI_Send(&(numbers[2]), 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Send(&(numbers[3]), 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Send(&(numbers[4]), 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    MPI_Send(&(numbers[5]), 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    MPI_Send(&(numbers[6]), 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
    MPI_Send(&(numbers[7]), 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
}

// describes work of main process
// loads input, sort first two numbers, and sends it  to other processes
void main_process(){
    vector<int> numbers = load_input();

    // compare first two numbers and sort them
    if (numbers[0] > numbers[1]){
        int tmp = numbers[1];
        numbers[1] = numbers[0];
        numbers[0] = tmp;
    }

    // send data to other processes
    sendInputs(numbers);

    // wait and receive sorted numbers from other processes and print them
    int output;
    MPI_Recv(&output, 1, MPI_INT,10,MPI_ANY_TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%i\n", output);
    MPI_Recv(&output, 1, MPI_INT,16,1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%i\n", output);
    MPI_Recv(&output, 1, MPI_INT,16,2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%i\n", output);
    MPI_Recv(&output, 1, MPI_INT,17,1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%i\n", output);
    MPI_Recv(&output, 1, MPI_INT,17,2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%i\n", output);
    MPI_Recv(&output, 1, MPI_INT,18,1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%i\n", output);
    MPI_Recv(&output, 1, MPI_INT,18,2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%i\n", output);
    MPI_Recv(&output, 1, MPI_INT,13,MPI_ANY_TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%i\n", output);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc,&argv);

    // get rank of each process
    int id;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    // array containing information which process should receive messages from and send messages to
    // contain information only for processes 1-18 (processor 0 has more work to do)
    // -> to get work for process with id, must index task[id - 1]
    // contains information in this order: input 1, input 2, output 1, output 2
    // where input is from it receives data, output is who it sends data, number indicates in which "wire"
    // logic who sends data to who can be seen in documentation
    int task[18][4] = {
            {0, 0, 4, 5},
            {0, 0, 6, 7},
            {0, 0, 6, 7},
            {0, 1, 10, 8},
            {0, 1, 8, 13},
            {2, 3, 10, 9},
            {2, 3, 9, 13},
            {4, 5, 12, 11},
            {6, 7, 12, 11},
            {4, 6, 0, 14},
            {8, 9, 14, 18},
            {8, 9, 16, 15},
            {5, 7, 15, 0},
            {10, 11, 16, 17},
            {12, 13, 17, 18},
            {14, 12, 0, 0},
            {14, 15, 0, 0},
            {11, 15, 0, 0}
    };

    // process with id is considered as main process -> loads data and sends it to other processors
    if(id == 0)
        main_process();
    // other processes do their job, load information about messages for this processor
    else
        run_process(task[id-1][0], task[id-1][1], task[id-1][2], task[id-1][3]);

    MPI_Finalize();
    return 0;
}

