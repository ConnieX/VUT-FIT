//
// Created by tomas on 06.12.20.
//

#include <iostream>
#include "dolphin_simulation.h"

dolphin_simulation::dolphin_simulation(garbage * garbage) {
    vector<dolphin*> dolphins_new(MAX_DOLPHIN_AGE);
    this->garbage_ptr = garbage;
    int count = 1000000;
    auto t = new dolphin(garbage, 1, count);
    dolphins_new.at(0) = t;

    for(int i = 1; i < MAX_DOLPHIN_AGE; i++){
        dolphins_new.at(i) = dolphins_new.at(i - 1)->create_older_dolphin();
    }
    this->dolphins = dolphins_new;
    this->current_year = 2020;
    auto cnt = sum();
    for(auto dolphin : dolphins_new){
        auto percentage = (double)dolphin->count / cnt;
        dolphin->count = (unsigned)(percentage*7093000);
    }
}


int dolphin_simulation::create_first_gen(unsigned count){
    return (int)((double)count * (1.0/(rand() % 1 + 2))); //Dolphins have kid every 2-3 years
}

void dolphin_simulation::print(){
    cout << "V roce " << current_year << " bude " << sum() << " delfínů." << endl;
}

unsigned int dolphin_simulation::sum() {
    unsigned sum = 0;
    for(auto t: dolphins){
        sum += t->count;
    }
    return sum;
}

unsigned int dolphin_simulation::fertility_count() {
    double sum = 0;
    for(int i = 5; i < 16; i++){
        sum += dolphins.at(i)->count;
    }
    return (unsigned)(sum * 0.5);
}



void dolphin_simulation::advance_year() {
    this->current_year++;
    garbage_ptr->get_garbage(this->current_year);
    auto count = create_first_gen(fertility_count());
    auto first_gen = new dolphin(this->garbage_ptr, 1, count);
    for(auto t: dolphins){
        t->age_one_year();
    }
    auto last = dolphins.back();
    dolphins.pop_back();
    delete last;
    dolphins.insert(dolphins.begin(), first_gen);
}