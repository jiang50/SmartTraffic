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
#include<time.h>
#include <string>

class Server {
    public:
    Server(int port, int backlog);
    void processCarInfo(std::string cars);
    std::string getTrafficLights();

    private:
    int sockfd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr; /* client's address info */
	int sin_size;
	char dst[INET_ADDRSTRLEN];
	char buffer[1024] = {0}; 
    
};