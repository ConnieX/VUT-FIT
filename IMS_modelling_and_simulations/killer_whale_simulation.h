//
// Created by tomas on 06.12.20.
//

#ifndef SRC_KILLER_WHALE_SIMULATION_H
#define SRC_KILLER_WHALE_SIMULATION_H


#include "garbage.h"
#include "killer_whale.h"
#include <vector>

class killer_whale_simulation {
public:
    explicit killer_whale_simulation(garbage*);
    void advance_year();
    unsigned int sum();
    void full_print();
    void print();
private:
    vector<killer_whale*> killer_whales;
    class garbage * garbage_ptr;
    int current_year;
    unsigned int fertility_count();
    int create_first_gen(unsigned count);

};


#endif //SRC_KILLER_WHALE_SIMULATION_H
