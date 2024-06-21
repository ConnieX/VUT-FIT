//
// Created by tomas on 06.12.20.
//

#include "killer_whale.h"

killer_whale::killer_whale(garbage *garbage, int age, int count) {
    this->garbage_ptr = garbage;
    this->initial_garbage_count = garbage->get_garbage(0);
    this->age = age;
    this->count = count;
    chance_to_die_by_eating = 22;
}

double killer_whale::chance_to_eat_plastic(int year) const {
    auto garbage_count = this->garbage_ptr->get_garbage(year);
    auto factor = garbage_count / initial_garbage_count;
    auto value = 7;
    value *= factor;
    if(value > 100){
        return 100;
    }
    return value;
}

double killer_whale::survive_count(int year) const{
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
    if(death_count > count){
        return 0;
    }
    return this->count - death_count;
}

killer_whale* killer_whale::create_older_killer_whale() {
    return new killer_whale(this->garbage_ptr, this->age + 1, (int)survive_count(0));
}

void killer_whale::age_one_year() {
    this->age++;
    this->count = (int)(survive_count(0));
}
