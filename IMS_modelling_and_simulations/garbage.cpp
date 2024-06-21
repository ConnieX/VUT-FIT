//
// Created by tomas on 02.12.20.
//

#include <iostream>
#include "garbage.h"

garbage::garbage() {
    currentYear = 2020;
    garbageTons = 194.27;
    production = 390.39;
    productionGrowth = 0.039;
    garbageFactor = 0.03;

    origYear = currentYear;
}

/// calculates garbage for a year. Year must be >= the year in last calculation
/// \param year Year to calcualte in. (>= 2016)
/// \return number of garbage in the ocean
double garbage::get_garbage(int year) {
    if(currentYear > year){
        return garbageTons;
    }
    if(currentYear == year){
        origYear = currentYear;
        //cout << "V roce " << year << " bude v ocenánch " << garbageTons << " miliónů metrických tun plastu." << endl;
        return garbageTons;
    }
    if(year - origYear  >= 5){
        if((currentYear % 5) == 0){
            cout << "V roce " << currentYear << " bude v ocenánch " << garbageTons << " miliónů metrických tun plastu." << endl;
        }
    }else{
        //cout << "V roce " << currentYear << " bude v ocenánch " << garbageTons << " miliónů metrických tun plastu." << endl;
    }
    production += production * productionGrowth;
    garbageTons += production * garbageFactor;
    currentYear++;
    return get_garbage(year);
}
