#include "server.h"

using namespace std;

int main(int argc, char** argv) {
    int mapSize = stoi(argv[1]);
    Server* s = new Server(53044, 10, mapSize);
    s->runServer();
    return 0;
}