//
// Created by tomas on 06.12.20.
//

#include <iostream>
#include "whale_simulation.h"

whale_simulation::whale_simulation(garbage * garbage) {
    vector<whale*> dolphins_new(MAX_WHALE_AGE);
    this->garbage_ptr = garbage;
    int count = 1000000;
    auto t = new whale(garbage, 1, count);
    dolphins_new.at(0) = t;

    for(int i = 1; i < MAX_WHALE_AGE; i++){
        dolphins_new.at(i) = dolphins_new.at(i - 1)->create_older_whale();
    }
    this->whales = dolphins_new;
    this->current_year = 2020;
    auto cnt = sum();
    for(auto dolphin : dolphins_new){
        auto percentage = (double)dolphin->count / cnt;
        dolphin->count = (unsigned)(percentage*500000);
    }
}

int whale_simulation::create_first_gen(unsigned count){
    return (int)((double)count * (1.0/(rand() % 1 + 1))); //Dolphins have kid every 2-3 years
}


void whale_simulation::print(){
    cout << "V roce " << current_year << " bude " << sum() << " delfínů." << endl;
}

unsigned int whale_simulation::sum() {
    unsigned sum = 0;
    for(auto t: whales){
        sum += t->count;
    }
    return sum;
}

unsigned int whale_simulation::fertility_count() {
    double sum = 0;
    for(int i = 20; i < 40; i++){
        sum += whales.at(i)->count;
    }
    return (unsigned)(sum * 0.5);
}

void whale_simulation::advance_year() {
    this->current_year++;
    garbage_ptr->get_garbage(this->current_year);
    auto count = create_first_gen(fertility_count());
    auto first_gen = new whale(this->garbage_ptr, 1, count);
    for(auto t: whales){
        t->age_one_year();
    }
    auto last = whales.back();
    whales.pop_back();
    delete last;
    whales.insert(whales.begin(), first_gen);
}
