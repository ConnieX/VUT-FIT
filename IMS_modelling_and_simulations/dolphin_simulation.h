//
// Created by tomas on 06.12.20.
//

#ifndef SRC_DOLPHIN_SIMULATION_H
#define SRC_DOLPHIN_SIMULATION_H

#include "garbage.h"
#include "dolphin.h"
#include <vector>

class dolphin_simulation {
public:
    explicit dolphin_simulation(garbage*);
    void advance_year();
    unsigned int sum();
    void full_print();
    void print();
private:
    vector<dolphin*> dolphins;
    class garbage * garbage_ptr;
    int current_year;
    unsigned int fertility_count();

    int create_first_gen(unsigned count);

};


#endif //SRC_DOLPHIN_SIMULATION_H
