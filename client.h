#include <string>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netdb.h>
#include<arpa/inet.h>

class Client {
    public:
    Client(int port, char *host);
    bool connectToServer();
    bool sendCarInfo(std::string &cars);
    std::string getTrafficLights();

    private:
    int sockfd, numbytes;
	struct sockaddr_in their_addr; /* client's address information */
	
    char str[4096];
};