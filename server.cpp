#include "server.h"
#include <iostream>
#include <sstream>
using namespace std;

Server::Server(int port, int bl, int size) {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
    backlog = bl;
    mapSize = size;
    my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
	bzero(&(my_addr.sin_zero), 8);
    smartServer = new SmartServer(size);
    
}


void Server::runServer() {
    if (::bind(sockfd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, backlog) == -1) {
		perror("listen");
		exit(1);
	}

    socklen_t sin_size;
    char dst[INET_ADDRSTRLEN];
    char buffer[1024] = {0};
    char *finish = (char*)"Done";

    while (1) {
        sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
			perror("accept");
			continue;
		}
		
		inet_ntop(AF_INET, &(their_addr.sin_addr), dst, INET_ADDRSTRLEN);
		printf("server: got connection from %s\n", dst);
        int valread;
        valread = read( new_fd , buffer, 5); 
 //   	printf("receive message which is %s\n",buffer ); 
        if (strstr(buffer, "light")!=NULL) {
            vector<vector<int>> traffcLight = smartServer->getTtrafficLights();
            string mess;
            for (int i = 0 ; i < mapSize ; i++) {
                for (int j = 0 ; j < mapSize ; j++) {
                    cout << traffcLight[i][j];
                    mess += to_string(traffcLight[i][j]);
                    mess += " ";
                }
                cout << endl;
            }
            cout << endl;
            if( (valread = send(new_fd, mess.c_str(), mess.length(),0))== -1) {
                perror("send");
            }
            if( (valread = send(new_fd, finish, strlen(finish),0))== -1) {
                perror("send");
            }
            cout << "Finish sending traffic lights" << endl;
        }
        else if (strstr(buffer, "car##")!=NULL) {
            string carInfo;
            bool printfirst = true;
            while (1) {
                if ((valread=recv(new_fd, buffer, 1024, 0)) == -1) {
                    perror("recv");
                    exit(1);
                }
                
                string newstr(buffer);

                if (printfirst) {
                    cout << newstr << endl;
                    printfirst = false;
                }

                string iphead = "127."; //weird
                int p;
                if ((p = newstr.find(iphead)) != std::string::npos) {
                    newstr = newstr.substr(0,p);
                }
                if (printfirst) {
                    cout << newstr << endl;
                }
                // cout << newstr << endl;
                // cout << endl;
                carInfo += newstr;
                if (strstr(buffer, "Done")!=NULL) break;
                
            }
            string delimiter = "\n";
            
            size_t pos = 0;
            string token;

            pos = carInfo.find("Done");
            carInfo = carInfo.substr(0, pos);
            while ((pos = carInfo.find(delimiter)) != std::string::npos) {
                token = carInfo.substr(0, pos);
            //    cout << token << endl;
                istringstream iss(token);
                int id, dir, xy, x, y, time, arrived;
                bool isarrived = false;
                iss >> id >> dir >> xy >> time >> arrived;
                if (arrived == 1) {
                    isarrived = true;
                }
                smartServer->receiveCarInfo(id, dir, xy / mapSize, xy % mapSize, time, isarrived);
        //        std::cout << token << std::endl;
                carInfo.erase(0, pos + delimiter.length());
            }

        }
        else  {
            continue;
        }


    }
}