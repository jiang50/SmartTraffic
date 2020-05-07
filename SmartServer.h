#include <vector>
#include <unordered_map>

class SmartServer {
    public:
    SmartServer(int _size);
    void receiveCarInfo(int id, int dir, int x, int y, int time, bool isArrived);
    std::vector<std::vector<int>> getTtrafficLights();

    private:
    int encode(int x, int y);

    int mapSize;
    std::unordered_map<int, std::pair<int, int>> cars; 
    std::vector<std::vector<int>> database; 
    std::unordered_map<int, int> cache;
    void update(int loc);


};


/*

0 : dir next timestamp arrived

*/

/*
    0top   1right    2down     3left   light
0    23     4        4         8         
1
2
3
4
...
*/