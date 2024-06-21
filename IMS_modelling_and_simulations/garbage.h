//
// Created by tomas on 02.12.20.
//

#ifndef SRC_GARBAGE_H
#define SRC_GARBAGE_H

#include <cstdio>
#include <cstdlib>

using namespace std;

class garbage {
public:
    garbage();
    double get_garbage(int year);


private:
    int currentYear;
    int origYear;
    double garbageTons;
    double productionGrowth;
    double production;
    double garbageFactor;
};


#endif //SRC_GARBAGE_H
