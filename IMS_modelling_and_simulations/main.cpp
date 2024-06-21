#include <cstdlib>
#include "garbage.h"
#include "turtle.h"
#include "turtle_simulation.h"
#include "dolphin_simulation.h"
#include "killer_whale_simulation.h"
#include "whale_simulation.h"
#include <vector>
#include <ctime>


using namespace std;


int main() {
    srand(time(nullptr));
    garbage g;
    g.get_garbage(2020);
    turtle_simulation turtles(&g);
    dolphin_simulation dolphins(&g);
    killer_whale_simulation kw(&g);
    whale_simulation whales(&g);
    for(int i = 0; i < 30; i++){
        turtles.advance_year();
        dolphins.advance_year();
        kw.advance_year();
        whales.advance_year();
    }
    return 0;
}
