//
// Created by tomas on 02.12.20.
//

#ifndef SRC_TURTLE_H
#define SRC_TURTLE_H


#include "garbage.h"
#include <vector>
#include <iostream>


#define MAX_TURTLE_AGE 120


class turtle {
public:
    turtle(garbage *garbage, int age, int count);
    double chance_to_eat_plastic(int year) const;

    turtle *create_older_turtle();
    void age_one_year();
    int age;
    unsigned int count;
private:

    double chance_to_die_by_eating;
    double initial_garbage_count;
    garbage* garbage_ptr;
    double survive_count(int year) const;
};


#endif //SRC_TURTLE_H
