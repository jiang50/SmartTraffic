#include "SmartServer.h"
#include <iostream>

using namespace std;

SmartServer::SmartServer(int _size) : mapSize(_size) {
    database.resize(mapSize * mapSize);
    for (int i = 0 ; i < database.size() ; i++) {
        database[i] = {0, 0, 0, 0};
    }
}

void SmartServer::receiveCarInfo(int id, int dir, int x, int y, int time, bool isArrived) {
    if (isArrived) {
        if (cars.count(id)) {
            database[cars[id].second][cars[id].first]--;
            update(cars[id].second);
            cars.erase(id);
        }
        return;
    }
    int loc = encode(x, y);
    if (cars.count(id)) {
        if (cars[id].second != loc || cars[id].first != dir) {
            database[cars[id].second][cars[id].first]--;
            
            if (database[cars[id].second][cars[id].first] < 0) {
                cout << "Number of cars is less than 0!" << endl;
            }
            update(cars[id].second);
            database[loc][dir]++;
            update(loc);
            cars[id] = {dir, loc};
        }
    }
    else {
        cars[id] = {dir, loc};
        database[loc][dir]++;
        update(loc);

    }
 //   cout << id << " " << loc << endl;

}

vector<vector<int>> SmartServer::getTtrafficLights() {
    vector<vector<int>> res(mapSize, vector<int>(mapSize));
    int loc;
    for (int i = 0 ; i < mapSize ; i++) {
        for (int j = 0 ; j < mapSize ; j++) {
            loc = encode(i, j);
            if (cache.count(loc)) {
                res[i][j] = cache[loc];
            }
            else {
                res[i][j] = 2;
            }
        }
    }
    return res;

}

int SmartServer::encode(int x, int y) {
    return x * mapSize + y;
}

void SmartServer::update(int loc) {
    if (database[loc][0] + database[loc][2] < database[loc][1] + database[loc][3]) {
        cache[loc] = 1;
    }
    else if (database[loc][0] + database[loc][2] > database[loc][1] + database[loc][3]) {
        cache[loc] = 0;
    }
}