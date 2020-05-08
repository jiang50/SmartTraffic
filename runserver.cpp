#include "server.h"

int main() {
    Server* s = new Server(53044, 10, 10);
    s->runServer();
    return 0;
}