#include "car.h"
#include "intersection.h"
#include "SmartServer.h"


/*
put car and intersection initialization outside class
*/

class SimulatedCity {
    public:
    
    virtual bool sendCarInfo() = 0;
    virtual void updateCarInfo() = 0;
    virtual bool getTrafficLights() = 0;
    virtual void updateTrafficLights() = 0;
    virtual void run() = 0;

    void sychronizedRun();
    void asychronizedRun();
    
    void printCurState();
    void printFinishInfo();
    void getFinalStat();

    protected:
    void initializeLights(int ml);
    void initializeCars();

    int mapSize;
    int numCars;
    int finishedCars;
    int runningCars;
    int waittingCars;
    int len;
    int curTime;

    int avgTime;
    int avgWait;
    double percentWait;

    std::vector<std::vector<Intersection>> trafficLights;
    std::vector<std::vector<int>> updatedLights;
    std::vector<Car> cars;


};


class SimulatedCityLocalRegular : public SimulatedCity {
    public:
    SimulatedCityLocalRegular(int mapsize, int numcars, int l, int ml);

    bool sendCarInfo();
    void updateCarInfo();
    bool getTrafficLights();
    void updateTrafficLights();
    void run();

};



class SimulatedCityLocalSmart : public SimulatedCity {
    public:
    SimulatedCityLocalSmart(int mapsize, int numcars, int l, int ml);

    bool sendCarInfo();
    void updateCarInfo();
    bool getTrafficLights();
    void updateTrafficLights();
    void run();

    private:
    SmartServer *smartServer;
};

/*

class SimulatedCityRemote : public SimulatedCity {

};

class SimulatedCityCloud : public SimulatedCity {

};

*/