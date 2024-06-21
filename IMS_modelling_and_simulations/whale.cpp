//
// Created by tomas on 06.12.20.
//

#include "whale.h"
#include "turtle.h"

whale::whale(garbage* g, int age, int count)  {
    this->garbage_ptr = g;
    this->initial_garbage_count = g->get_garbage(0);
    this->age = age;
    this->count = count;
    chance_to_die_by_eating = 22;
}

double whale::chance_to_get_caught(int year) const {
    auto garbage_count = this->garbage_ptr->get_garbage(year);
    auto factor = garbage_count / initial_garbage_count;
    auto value = 34 * factor;
    if(value > 100){
        return 100;
    }
    return value;
}


double whale::chance_to_eat_plastic(int year) const {
    auto garbage_count = this->garbage_ptr->get_garbage(year);
    auto factor = garbage_count / initial_garbage_count;
    auto value = 37 * factor;
    if(value > 100){
        return 100;
    }
    return value;
}

double whale::survive_count(int year) const{
    auto death_count = 0;
    //apparently return this->count * chance_to_eat * chance_to_die is not an option?!
    auto chance = chance_to_eat_plastic(year);
    auto net_chance = chance_to_get_caught(year);
    for(int i = 0; i < this->count; i++){
        if(rand() % 100 > chance){
            continue;
        }
        if(rand() % 100 > chance_to_die_by_eating){
            continue;
        }
        death_count++;
    }
    for(int i = 0; i < this->count; i++){
        if(rand() % 100 > net_chance){
            continue;
        }
        death_count++;
    }
    if(death_count > count){
        return 0;
    }
    return this->count - death_count;
}

whale* whale::create_older_whale() {
    return new whale(this->garbage_ptr, this->age + 1, (int)survive_count(0));
}


void whale::age_one_year() {
    this->age++;
    this->count = (int)(survive_count(0));
}
