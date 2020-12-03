#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 100
#define MAX_LISTEN_QUEUE 100

int main(int argc, char *argv[]){
    int sockfd, serverPortNum, sendBytes, rcvBytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE], server_response[256];;

    // create a socket for theo client
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error: ");
        exit(-1);
    }

    serverPortNum = atoi(argv[2]);

    // create the remote server socket information structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    // inet_aton("127.0.0.1", &servaddr.sin_addr); 
    servaddr.sin_port = htons(serverPortNum);

    // connection theo client to the server socket
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("Problem in connecting to the server");
        exit(-1);
    }

    while(1){
        fflush(stdin);
        fgets(sendline, MAXLINE,stdin);
        sendline[strlen(sendline) -1] = '\0';
        sendBytes = send(sockfd, sendline , strlen(sendline) +1, 0);
        if(sendBytes < 0){
            perror("Error: ");
            return 0;
        }

        rcvBytes = recv(sockfd, &server_response, MAXLINE, 0);
        if(rcvBytes <0){
            perror("Error: ");
            return 0;
        }
        server_response[rcvBytes] = '\0';
        printf("%s\n", server_response);
    }
}