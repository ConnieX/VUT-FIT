//
// Created by tomas on 06.12.20.
//

#ifndef SRC_KILLER_WHALE_H
#define SRC_KILLER_WHALE_H

#define MAX_KILLER_WHALE_AGE 50
#include "garbage.h"

class killer_whale {
public:
    killer_whale(garbage *garbage, int age, int count);
    double chance_to_eat_plastic(int year) const;

    killer_whale *create_older_killer_whale();
    void age_one_year();
    int age;
    unsigned int count;
private:

    double chance_to_die_by_eating;
    double initial_garbage_count;
    garbage* garbage_ptr;
    double survive_count(int year) const;

};


#endif //SRC_KILLER_WHALE_H
