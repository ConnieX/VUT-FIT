//
// Created by tomas on 06.12.20.
//

#include <iostream>
#include "killer_whale_simulation.h"



killer_whale_simulation::killer_whale_simulation(class garbage* garbage){
    vector<killer_whale*> turtles_new(MAX_KILLER_WHALE_AGE);
    this->garbage_ptr = garbage;
    auto t = new killer_whale(garbage, 1, 1000000);
    turtles_new.at(0) = t;
    for(int i = 1; i < MAX_KILLER_WHALE_AGE; i++){
        turtles_new.at(i) = turtles_new.at(i - 1)->create_older_killer_whale();
    }
    this->killer_whales = turtles_new;
    this->current_year = 2020;
    auto cnt = sum();
    for(auto turtle : turtles_new){
        auto percentage = (double)turtle->count / cnt;
        turtle->count = (unsigned)(percentage*50000);
    }
}



int killer_whale_simulation::create_first_gen(unsigned count){
    return (int)((double)count * (1.0/(rand() % 3 + 2))); //3-5
}

void killer_whale_simulation::print(){
    cout << "V roce " << current_year << " bude " << sum() << " kosatek." << endl;
}

unsigned int killer_whale_simulation::sum() {
    unsigned sum = 0;
    for(auto t: killer_whales){
        sum += t->count;
    }
    return sum;
}

unsigned int killer_whale_simulation::fertility_count() {
    double sum = 0;
    for(int i = 13; i < MAX_KILLER_WHALE_AGE - 5; i++){
        sum += killer_whales.at(i)->count;
    }
    return (unsigned)(sum * 0.5);
}

void killer_whale_simulation::advance_year() {
    this->current_year++;
    garbage_ptr->get_garbage(this->current_year);
    auto count = create_first_gen(fertility_count());
    auto first_gen = new killer_whale(this->garbage_ptr, 1, count);
    for(auto t: killer_whales){
        t->age_one_year();
    }
    auto last = killer_whales.back();
    killer_whales.pop_back();
    delete last;
    killer_whales.insert(killer_whales.begin(), first_gen);
}