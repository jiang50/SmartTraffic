#include "SimulatedCity.h"
#include <vector>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
 
using namespace std;

/*
Sychronized:

time    0s            1s,2s,3s,4s,                   5s                           6s, 7s, 8s, 9s,                 10s
event  sendCarInfo                     updateCarInfo->get/updateLight->sendcarinfo                      updateCarInfo->get/updateLight->sendcarinfo

*/

void SimulatedCity::sychronizedRun() {
    printCurState();
    
    if (!sendCarInfo()) {
        cout << "Failed to update car info" << endl;
        return;
    }
    while (finishedCars < numCars) {
    //    this_thread::sleep_for (chrono::seconds(1));
        curTime += 5;
        updateCarInfo();
        getTrafficLights();        
        updateTrafficLights();
        printCurState();
        if (!sendCarInfo()) {
            cout << "Failed to update car info" << endl;
            return;
        }
        
    }
    getFinalStat();
    printFinishInfo();

}

/*
Asychronized:

time        0s       1s    2s    3s    4s           5s           6s     7s     8s      9s        10s
car    sendCarInfo   . . . . . . .       updateCarInfo->sendcarinfo . . . . . . .        updateCarInfo->sendcarinfo . . . . . . . . 
light                          getLight. . . . . updateLight                getLight . . . . . updateLight
*/

void SimulatedCity::asychronizedRun() {
    printCurState();
    sendCarInfo();

    while (finishedCars < numCars) {
        this_thread::sleep_for (chrono::seconds(3));
        curTime += 3;
        getTrafficLights();
        this_thread::sleep_for (chrono::seconds(2));
        curTime += 2;
        updateCarInfo();
        updateTrafficLights();
        sendCarInfo();
        printCurState();
    }
    printFinishInfo();
}

void SimulatedCity::getFinalStat() {
    
    int totalWait = 0, totalTime = 0;
    for (int i = 0 ; i < cars.size() ; i++) {
        totalTime += cars[i].getTotalTime();
        totalWait += cars[i].getWaitTime();
    }
 //   cout << totalTime << " " << totalWait << endl;
    avgTime = totalTime / cars.size();
    avgWait = totalWait / cars.size();
    percentWait = (double)totalWait / totalTime;
}

void SimulatedCity::printFinishInfo() {
    cout << endl;
    cout << "Total time spent: " << curTime << "s" << endl;
    cout << "Average time each car spent to arrive: " << avgTime << "s" <<endl;
    cout << "Average time each car wait: " << avgWait << "s" <<endl;
    cout << "Total percentage of waitting time: " << percentWait << endl;

}

void SimulatedCity::printCurState() {
    cout << curTime << "s: ";
    cout << waittingCars << " cars are waiting, ";
    cout << runningCars << " cars are running, ";
    cout << finishedCars << " cars arrived." << endl;
}

void SimulatedCity::initializeLights(int ml) {
    cout << "Generating map..." << endl;
    trafficLights.resize(mapSize);
    updatedLights.resize(mapSize);
    for (int i = 0 ; i < mapSize ; i++) {
        for (int j = 0 ; j < mapSize ; j++) {
            trafficLights[i].push_back(Intersection(i, j, ml));
            updatedLights[i].push_back(-1); //placeholder
        }
    }
    cout << "Finish generating map" << endl;
}

void SimulatedCity::initializeCars() {
    cout << "Generating cars..." << endl;
    for (int i = 0 ; i < numCars ; i++) {
        cars.push_back(Car(i, len, mapSize));
    }
    cout << "Finish generating cars and routes" << endl;
}

/*
########################################
########### local regular ##############
########################################
*/

SimulatedCityLocalRegular::SimulatedCityLocalRegular(int mapsize, int numcars, int l, int ml) {
    mapSize = mapsize;
    numCars = numcars;
    len = l;
    finishedCars = 0;
    runningCars = numCars;
    waittingCars = 0;
    curTime = 0;
    initializeLights(ml);
    initializeCars();
}


bool SimulatedCityLocalRegular::sendCarInfo() {
    return true;
}

void SimulatedCityLocalRegular::updateCarInfo() {
    int x,y;
    int state;
    finishedCars = 0;
    runningCars = 0;
    waittingCars = 0;
    for (int i = 0 ; i < cars.size() ; i++) {
        x = cars[i].getNextx();
        y = cars[i].getNexty();
        cars[i].update(trafficLights[x][y].getLight());
        state = cars[i].getState();
        if (state == 0) {
            finishedCars++;
        }
        else if (state == 1) {
            runningCars++;
        }
        else {
            waittingCars++;
        }

    }
}


bool SimulatedCityLocalRegular::getTrafficLights() {
    return true;
}


void SimulatedCityLocalRegular::updateTrafficLights() {
    for (int i = 0 ; i < mapSize ; i++) {
        for (int j = 0 ; j < mapSize ; j++) {
            trafficLights[i][j].checkAndSwitch();
        }
    }
}

void SimulatedCityLocalRegular::run() {
    sychronizedRun();
}

/*
################################
#######local smart##############
################################
*/

SimulatedCityLocalSmart::SimulatedCityLocalSmart(int mapsize, int numcars, int l, int ml) {
    mapSize = mapsize;
    numCars = numcars;
    len = l;
    finishedCars = 0;
    runningCars = numCars;
    waittingCars = 0;
    curTime = 0;
    initializeLights(ml);
    initializeCars();
    smartServer = new SmartServer(mapSize);
}

bool SimulatedCityLocalSmart::sendCarInfo() {
    for (int i = 0 ; i < cars.size() ; i++) {
        smartServer->receiveCarInfo(i, cars[i].getDir(), cars[i].getNextx(), cars[i].getNexty(), cars[i].getTime(), cars[i].isArrived());
    }
 //   cout << "finish sending car info" << endl;
    return true;
}

void SimulatedCityLocalSmart::updateCarInfo() {
    int x,y;
    int state;
    finishedCars = 0;
    runningCars = 0;
    waittingCars = 0;
    for (int i = 0 ; i < cars.size() ; i++) {
        x = cars[i].getNextx();
        y = cars[i].getNexty();
        cars[i].update(trafficLights[x][y].getLight());
        state = cars[i].getState();
        if (state == 0) {
            finishedCars++;
        }
        else if (state == 1) {
            runningCars++;
        }
        else {
            waittingCars++;
        }

    }

}

bool SimulatedCityLocalSmart::getTrafficLights() {
    updatedLights = smartServer->getTtrafficLights();
    return true;
}

void SimulatedCityLocalSmart::updateTrafficLights() {
    for (int i = 0 ; i < mapSize ; i++) {
        for (int j = 0 ; j < mapSize ; j++) {
            trafficLights[i][j].update(updatedLights[i][j]);
        }
    }
}

void SimulatedCityLocalSmart::run() {
    sychronizedRun();
}