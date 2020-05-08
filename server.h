#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include "SmartServer.h"
#include<netdb.h>
#include<arpa/inet.h>
#include<time.h>
#include <string>
#include <unistd.h>

class Server {
    public:
    Server(int port, int bl, int size);
    void runServer();
    void processCarInfo(std::string cars);
    std::string getTrafficLights();

    private:
    int mapSize;
    int sockfd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr; /* client's address info */
	
    int backlog;
	// char dst[INET_ADDRSTRLEN];
	// char buffer[1024] = {0}; 
    SmartServer* smartServer;

};