//
// Created by tomas on 06.12.20.
//

#include "dolphin.h"

dolphin::dolphin(garbage *garbage, int age, int count) {
    this->garbage_ptr = garbage;
    this->initial_garbage_count = garbage->get_garbage(0);
    this->age = age;
    this->count = count;
    chance_to_die_by_eating = 22;
}

double dolphin::chance_to_get_caught(int year) const {
    auto garbage_count = this->garbage_ptr->get_garbage(year);
    auto factor = garbage_count / initial_garbage_count;
    auto value = 7 * factor;
    if(value > 100){
        return 100;
    }
    return value;
}


double dolphin::chance_to_eat_plastic(int year) const {
    auto garbage_count = this->garbage_ptr->get_garbage(year);
    auto factor = garbage_count / initial_garbage_count;
    auto value = 47; //dolphins have 47% chance to eat plastic
    value *= factor;
    if(value > 100){
        return 100;
    }
    return value;
}

double dolphin::survive_count(int year) const{
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

dolphin* dolphin::create_older_dolphin() {
    return new dolphin(this->garbage_ptr, this->age + 1, (int)survive_count(0));
}

void dolphin::age_one_year() {
    this->age++;
    this->count = (int)(survive_count(0));
}
