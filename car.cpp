#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <iostream>
#include <utility>
#include "car.h"
using namespace std;

Car::Car(int _id, int len, int mapsize) {
    id = _id;
    mapSize = mapsize;
    curTime = 0;
    int x,y;
    srand (time(NULL) + id);
    dir = rand() % 4;
    distance = 100;
    arrived = false;
    state = RUNNING;
    next = 0;
    x = rand() % mapSize;
    y = rand() % mapSize;
    routes.push_back({x,y});
    len--;

    int nextDir;
    int prevDir = dir;
    int curx = x, cury = y;
    while (len > 0) {
        nextDir = rand() % 4;
        if (nextDir == 3 - prevDir) continue;
        if (nextDir == 0) {
            if (curx == 0) continue;
            curx--;
        }
        else if (nextDir == 1) {
            if (cury == mapSize - 1) continue;
            cury++;
        }
        else if (nextDir == 3) {
            if (curx == mapSize - 1) continue;
            curx++;
        }
        else if (nextDir == 2) {
            if (cury == 0) continue;
            cury--;
        }
        routes.push_back({curx, cury});
        len--;
        prevDir = nextDir;

    }
 //   displayRoute();

}

void Car::update(bool light) { // light == true means cars from left and right can pass
 //   cout << "light: " << light << endl;
    if (arrived) return;
    curTime += 5;
    if (distance == 100) {
        distance = 50; // go to mid of the road
        return;
    }
    
    bool isHor = false;
    if (dir == LEFT || dir == RIGHT) {
        isHor = true;
    }
    bool canPass = false;
    if (!(light ^ isHor)) canPass = true;
    if (!canPass) {
        state = STOP;
        distance = 0;
    }
    else if (canPass && next == routes.size() - 1) {
        arrived = true;
        finishedTime = curTime;
        return;
    }
    else {
        int x = routes[next].first;
        int y = routes[next].second;
        next++;
        int curx = routes[next].first;
        int cury = routes[next].second;
        if (x == curx && cury < y) {
            dir = LEFT;
        }
        else if (x == curx && cury > y) {
            dir = RIGHT;
        }
        else if (y == cury && curx < x) {
            dir = UP;
        }
        else {
            dir = DOWN;
        }

        if (distance == 50) {
            distance = 100;
        }
        else {
            distance = 50;
            state = RUNNING;
        }

    }
 //   curState();
    
}

void Car::displayRoute() {
    cout << num2dir(dir) << " ";
    for (auto it : routes) {
        cout << "(" << it.first << ", " << it.second << ") ";
    }
    cout << endl;
}

string Car::num2dir(int n) {
    if (n == LEFT) {
        return "left";
    }
    if (n == RIGHT) {
        return "right";
    }
    if (n == UP) {
        return "up";
    }
    if (n == DOWN) {
        return "down";
    }
    return "None";
}

void Car::curState() {
    cout << "Direction: " << num2dir(dir) << " ";
    cout << "Next intersection: " << "(" << routes[next].first << ", " << routes[next].second << ") ";
    cout << "Distance to next intersection: " << distance << " ";
    cout << "Status: " << state << endl;
}

int Car::getWaitTime() {
    return curTime - 10 * routes.size();
}


int Car::getTotalTime() {
    return curTime;
}

int Car::getNextx() {
    return routes[next].first;
}

int Car::getNexty() {
    return routes[next].second;
}

int Car::getDir() {
    return dir;
}

int Car::getTime() {
    return curTime;
}

int Car::isArrived() {
    return arrived;
}

int Car::getState() {
    if (arrived) {
        return 0;
    }
    else if (state == RUNNING) {
        return 1;
    }
    else {
        return 2;
    }
}

string Car::serialize() {
    string res;
    res += to_string(id);
    res += " ";
    res += to_string(dir);
    res += " ";
    int tl = mapSize * getNextx() + getNexty();
    res += to_string(tl);
    res += " ";
    res += to_string(curTime);
    res += " ";
    if (arrived) {
        res += '1';
    }
    else {
        res += '0';
    }
    res += '\n';
    return res;
}