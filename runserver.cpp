#include "server.h"

int main() {
    Server* s = new Server(53044, 10, 100);
    s->runServer();
    return 0;
}