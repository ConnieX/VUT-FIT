cmake_minimum_required(VERSION 3.17)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp garbage.cpp garbage.h turtle.cpp turtle.h turtle_simulation.cpp turtle_simulation.h dolphin.cpp dolphin.h dolphin_simulation.cpp dolphin_simulation.h killer_whale.cpp killer_whale.h killer_whale_simulation.cpp killer_whale_simulation.h whale.cpp whale.h whale_simulation.cpp whale_simulation.h)