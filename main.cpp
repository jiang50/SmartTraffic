#include <iostream>

#include "SimulatedCity.h"

using namespace std;

int main(int argc, char** argv) {
    /*
    arg1 : mapsize
    arg2 : num of cars
    arg3 : len of route of cars
    arg4 : mode (local_regular, local_smart, cloud_smart, remote_smart)
    */

   if (argc != 5) {
       cout << "Invalid command!" << endl;
       return 1;
   }
   int mapSize = stoi(argv[1]);
   int numCars = stoi(argv[2]);
   int len = stoi(argv[3]);
   string mode = argv[4];
   string host = "smart-traffic-node.azurewebsites.net";
   SimulatedCity* sc;
   if (mode == "local_regular") {
       sc = new SimulatedCityLocalRegular(mapSize, numCars, len, 20);
   }
   else if (mode == "local_smart") {
       sc = new SimulatedCityLocalSmart(mapSize, numCars, len, 20);
   }
   else if (mode == "remote_smart") {
       sc = new SimulatedCityRemote(mapSize, numCars, len, 20, 10000);
   }
   else if (mode == "cloud_smart") {
       sc = new SimulatedCityCloud(mapSize, numCars, len, 20, host);
   }

   else {
       cout << "Invalid mode!" << endl;
   }

    sc->run();
    // Car c1(0, 10, 10);
    // c1.displayRoute();
    // c1.update(true);
    // c1.curState();
    // c1.update(false);
    // c1.curState();
    // c1.update(true);
    // c1.curState();
    // c1.update(true);
    // c1.curState();
    return 0;
}