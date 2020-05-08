#include "intersection.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <iostream>
#include <utility>

using namespace std;

Intersection::Intersection(int x, int y, bool l, int len, int ml) : locx(x), locy(y), light(l), lenOfCurLight(len), maxlen(ml) {} 

Intersection::Intersection(int x, int y, int ml) : locx(x), locy(y), maxlen(ml) {
    lenOfCurLight = 0;
    srand (2 * x + y + time(NULL));
    int r = rand() % 2;
    int lr = rand() % (ml / 5);
    if (r == 0) {
        light = true;
    }
    else {
        light = false;
    }
    lenOfCurLight = lr * 5;

}

void Intersection::checkAndSwitch() {
    lenOfCurLight += 5;
    if (lenOfCurLight == maxlen) {
        light = !light;
        lenOfCurLight = 0;
    }
    
}

void Intersection::update(int lightFromCloud) {
    if (lightFromCloud == 2) {
        checkAndSwitch();
    }
    if (light == lightFromCloud) {
        lenOfCurLight += 5;
    }
    else {
        light = lightFromCloud;
        lenOfCurLight = 0;
    }
}

bool Intersection::getLight() {
    return light;
}