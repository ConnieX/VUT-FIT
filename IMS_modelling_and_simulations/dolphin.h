//
// Created by tomas on 06.12.20.
//

#ifndef SRC_DOLPHIN_H
#define SRC_DOLPHIN_H

#define MAX_DOLPHIN_AGE 20

#include "garbage.h"

class dolphin {
public:
    dolphin(garbage *garbage, int age, int count);
    double chance_to_eat_plastic(int year) const;
    double chance_to_get_caught(int year) const;

    dolphin *create_older_dolphin();
    void age_one_year();
    int age;
    unsigned int count;
private:

    double chance_to_die_by_eating;
    double initial_garbage_count;
    garbage* garbage_ptr;
    double survive_count(int year) const;

};


#endif //SRC_DOLPHIN_H
