#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <string>
#include <iostream>

using namespace std;

void error(const char *msg) { perror(msg); exit(0); }

int main(int argc,char *argv[])
{
    /* first what are we going to send and where are we going to send it? */
    int portno =        80;
    char *host =        (char*)"smart-traffic.azurewebsites.net";
 //   char *host = (char*)"smart-traffic-node.azurewebsites.net";
    char *message = (char*)"GET /api/SmartTraffic_HttpTrigger1?name=shang HTTP/1.1\r\nHost: smart-traffic.azurewebsites.net\r\n\r\n";
 //   char *message = (char*)"POST /api/car_info HTTP/1.1\r\nHost: smart-traffic-node.azurewebsites.net\r\nContent-Type: text/plain\r\nContent-Length: 39\r\n\r\n0 1234 2 5 0\n1 3214 1 5 1\n2 4321 0 5 1\n";
 //  char *message = (char*)"GET /api/SmartTraffic_HttpTrigger1?name=shangl HTTP/1.1\r\nHost: smart-traffic.azurewebsites.net\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.129 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: en-US,en;q=0.9,la;q=0.8\r\n\r\n";
//Host: smart-traffic.azurewebsites.net
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
  //  char message[1024],response[4096];
    char response[4096];

  //  if (argc < 3) { puts("Parameters: <apikey> <command>"); exit(0); }

    /* fill in the parameters */
 //   sprintf(message,message_fmt);
    printf("Request:\n%s\n",message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("connected\n");
    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);
    printf("Sent\n");
    /* receive the response */
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    string receivedInfo;
    char str[1000];
    int numbytes;
    while (1) {
        if ((numbytes=recv(sockfd, str, 1000, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        
     //   printf("%s", str);
        string newstr(str);
        receivedInfo += newstr;
        cout << receivedInfo << endl;
        break;
     //   
    }
    

    return 0;
}
