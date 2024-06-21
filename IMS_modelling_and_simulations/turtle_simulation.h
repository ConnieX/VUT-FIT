//
// Created by tomas on 06.12.20.
//

#ifndef SRC_TURTLE_SIMULATION_H
#define SRC_TURTLE_SIMULATION_H


#include "turtle.h"

class turtle_simulation {
public:
    explicit turtle_simulation(garbage*);
    void advance_year();
    unsigned int sum();
    void full_print();
    void print();
private:
    vector<turtle*> turtles;
    class garbage * garbage_ptr;
    int current_year;
    unsigned int fertility_count();

    int create_first_gen(unsigned count);

};


#endif //SRC_TURTLE_SIMULATION_H
