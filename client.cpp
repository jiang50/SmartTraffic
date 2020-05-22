#include "client.h"
#include <iostream>

using namespace std;

Client::Client(int port, const char *host) {
    struct hostent *he;
    if ((he=gethostbyname(host)) == NULL) {
		herror("gethostbyname");
		exit(1);
	}

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

    their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(port);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);
}

bool Client::connectToServer() {
    if (connect(sockfd,(struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
    return true;
}

bool Client::sendCarInfo(std::string &cars) {
 //   connectToServer();
    // char *message = (char*)"car##"; 
    // char *finish = (char*)"Done";
//    cout << cars;
    // if( (numbytes = send(sockfd, message, strlen(message),0))== -1) {
    //     perror("send");
    // }
    cout << cars << endl;
    if( (numbytes = send(sockfd, cars.c_str(), cars.length(),0))== -1) {
        perror("send");
    }
    // if( (numbytes = send(sockfd, finish, strlen(finish),0))== -1) {
    //     perror("send");
    // }
    string rcv;   
   
    if ((numbytes=recv(sockfd, str, 1000, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    printf("%s", str);
    // string newstr(str);
    // rcv += newstr;
    
    close(sockfd);
    return true;

}

string Client::getTrafficLights() {
    string res;
 //   connectToServer();
  //  char *light = (char*)"light"; 
    char *message = (char*)"GET /api/getLight HTTP/1.1\r\nHost: smart-traffic-node.azurewebsites.net\r\n\r\n";
    if(send(sockfd, message, strlen(message), 0) == -1) {
        perror("send");
    }
    
    string rcv;   
    while (1) {
        if ((numbytes=recv(sockfd, str, 1000, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        string newstr(str);
        rcv += newstr;
        if (strstr(str, "Done")!=NULL) break;
        
    }
    size_t pos = 0;
    pos = rcv.find("Done");
    rcv = rcv.substr(0, pos);
  //  cout << "received lights: " << rcv << endl;
    close(sockfd);
    return rcv;
}