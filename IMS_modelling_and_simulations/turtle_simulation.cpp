//
// Created by tomas on 06.12.20.
//


#include "turtle_simulation.h"


turtle_simulation::turtle_simulation(class garbage* garbage){
    vector<turtle*> turtles_new(MAX_TURTLE_AGE);
    this->garbage_ptr = garbage;
    auto t = new turtle(garbage, 1, 1000000);
    turtles_new.at(0) = t;
    for(int i = 1; i < MAX_TURTLE_AGE; i++){
        turtles_new.at(i) = turtles_new.at(i - 1)->create_older_turtle();
    }
    this->turtles = turtles_new;
    this->current_year = 2020;
    auto cnt = sum();
    for(auto turtle : turtles_new){
        auto percentage = (double)turtle->count / cnt;
        turtle->count = (unsigned)(percentage*6461000);
    }
}

int turtle_simulation::create_first_gen(unsigned count){
    int turtle_count = 0;
    count *= 1.0/(rand() % 4 + 2);
    for(int i = 0; i < count; i++){ //For every turtle
        int nestCount = (rand() % 6 + 2); //get number of nests between 2-8
        for(int j = 0; j < nestCount; j++) {
            int eggs = rand() % 115 + 65; // in every nest there is 65-180;
            turtle_count += (eggs * 0.8); //Not all eggs get hatched (around 80% will hatch)
        }
    }
    turtle_count /= 1000; // Only around 0.1% survives to be 1 year long
    return turtle_count;
}



unsigned int turtle_simulation::sum() {
    unsigned sum = 0;
    for(auto t: turtles){
        sum += t->count;
    }
    return sum;
}

unsigned int turtle_simulation::fertility_count() {
    double sum = 0;
    for(int i = 18; i < 48; i++){
        sum += turtles.at(i)->count;
    }
    return (unsigned)(sum * 0.75);
}


void turtle_simulation::full_print(){
    int age = 5;
    int sum = 0;
    for(auto t: turtles){
        if(t->age <= age){
            sum += t->count;
        }
        if(age == t->age){
            cout << "Počet želv ve věku " << age - 4 << "-" << age << " let je " << sum << endl;
            age += 5;
            sum = 0;
        }
    }
}

void turtle_simulation::print(){
    cout << "V roce " << current_year << " bude " << sum() << " želv." << endl;
}

void turtle_simulation::advance_year() {
    this->current_year++;
    garbage_ptr->get_garbage(this->current_year);
    auto count = create_first_gen(fertility_count());
    auto first_gen = new turtle(this->garbage_ptr, 1, count);
    for(auto t: turtles){
        t->age_one_year();
    }
    auto last = turtles.back();
    turtles.pop_back();
    delete last;
    turtles.insert(turtles.begin(), first_gen);
}

