#include <vector>
#include <string>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

enum State {RUNNING, STOP};

class Car {
    public:
    Car(int id, int len, int mapsize);
    void update(bool light);
    std::string generateMessage();
    void displayRoute();
    void curState();
    std::string num2dir(int n);
    int getWaitTime();
    int getTotalTime();
    int getDir();
    int getNextx();
    int getNexty();
    int getState();
    int getTime();
    int isArrived();
    std::string serialize(); // id direction next timestamp arrive

    private:
    int id;
    int mapSize;
    State state;
    int dir; // direction heading to
    std::vector<std::pair<int, int>> routes;
    int next; // the next intersection heading to
    int distance; // the distance to next intersection
    bool arrived;
    int curTime;
    int finishedTime;

};