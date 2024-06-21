//
// Created by tomas on 06.12.20.
//

#ifndef SRC_WHALE_SIMULATION_H
#define SRC_WHALE_SIMULATION_H


#include "garbage.h"
#include "whale.h"
#include <vector>
class whale_simulation {
public:
    explicit whale_simulation(garbage*);
    void advance_year();
    unsigned int sum();
    void full_print();
    void print();
private:
    vector<whale*> whales;
    class garbage * garbage_ptr;
    int current_year;
    unsigned int fertility_count();

    int create_first_gen(unsigned count);
};


#endif //SRC_WHALE_SIMULATION_H
