#ifndef Config_cpp
#define Config_cpp
#include "Config.hpp"

int rd(int min, int max){
    return Rand() % (max - min + 1) + min;
}

int dist(int x1, int y1, int x2, int y2){
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool collide(int x1, int y1, int x2, int y2){
    return 1;
}

#endif