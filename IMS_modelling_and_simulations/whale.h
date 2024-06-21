//
// Created by tomas on 06.12.20.
//

#ifndef SRC_WHALE_H
#define SRC_WHALE_H


#include "garbage.h"
#define MAX_WHALE_AGE 200

class whale {
public:
    whale(garbage *garbage, int age, int count);
    double chance_to_eat_plastic(int year) const;

    whale *create_older_whale();
    void age_one_year();
    int age;
    unsigned int count;
private:
    double chance_to_die_by_eating;
    double initial_garbage_count;
    garbage* garbage_ptr;
    double survive_count(int year) const;

    double chance_to_get_caught(int year) const;
};


#endif //SRC_WHALE_H
