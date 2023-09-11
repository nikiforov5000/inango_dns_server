#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFERSIZE 1024

void errorMessage(char* msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char** argv) {
    // setbuf(stdout, NULL);
    int sockfd;
    int port;
    int clientlen;
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    struct hostent* hostp;
    char* buf;
    char* hostaddrp;
    int optval;
    int n;

    printf("%s", "checking usage of port\n");
    if (argc != 2) {
        printf("%s", "usage of port\n");
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);
    // port = 53;

    // Create socket

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        errorMessage("Error opening socket");
    }
    printf("sockfd:%d\n", sockfd);


    // Kill server on close
    optval = 1;
    setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));
    printf("%s\n", "kill server complete");
    // fflush(stdout);

    // Bind server's internet address
    bzero((char*)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    printf("bind server's internet address complete\n");
    // fflush(stdout);

    // Associate socket with a port
    if (bind(sockfd,(struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        errorMessage("Error on binding");
        exit(1);
    }

    // Loop wait for a datagram and echo it
    clientlen = sizeof(clientaddr);
    buf = malloc(BUFFERSIZE);
    while(1) {
        printf("in loop\n");
        // fflush(stdout);

        // Receive a UDP datagram from a client
        n = recvfrom(sockfd, buf, BUFFERSIZE, 0, (struct sockaddr*)&clientaddr, &clientlen);
        printf("n:%d\n", n);
        printf("%s\n", buf);
    }


    return 0;
}
