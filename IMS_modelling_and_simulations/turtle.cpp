//
// Created by tomas on 02.12.20.
//

#include "turtle.h"

turtle::turtle(garbage* g, int age, int count)  {
    this->garbage_ptr = g;
    this->initial_garbage_count = g->get_garbage(0);
    this->age = age;
    this->count = count;
    chance_to_die_by_eating = 22;
}

double turtle::chance_to_eat_plastic(int year) const {
    auto garbage_count = this->garbage_ptr->get_garbage(year);
    auto factor = garbage_count / initial_garbage_count;
    auto value = 0.0;
    if(age < 5){
        value = 54;
    }else if(age < 20){
        value = 23;
    }else{
        value = 16;
    }
    value *= factor;
    if(value > 100){
        return 100;
    }
    return value;
}

double turtle::survive_count(int year) const{
    auto death_count = 0;
    //apparently return this->count * chance_to_eat * chance_to_die is not an option?!
    auto chance = chance_to_eat_plastic(year);
    for(int i = 0; i < this->count; i++){
        if(rand() % 100 > chance){
            continue;
        }
        if(rand() % 100 > chance_to_die_by_eating){
            continue;
        }
        death_count++;
    }
    return this->count - death_count;
}

turtle* turtle::create_older_turtle() {
    return new turtle(this->garbage_ptr, this->age + 1, (int)survive_count(0));
}


void turtle::age_one_year() {
    this->age++;
    this->count = (int)(survive_count(0));
}




