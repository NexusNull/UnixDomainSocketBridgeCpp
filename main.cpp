#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

void error(const char *);

int main(int argc, char *argv[]) {
    int sockfd, servlen, n;
    struct sockaddr_un serv_addr;
    char buffer[82];

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, argv[1]);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        error("Creating socket");
    while (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0) {
        sleep(1);
    }

    std::cout << "Connected\n";
    sleep(1);
    while (true) {
        n = read(sockfd, buffer, 5);

        if (n <= 0 || strcmp(buffer, "close") == 0)
            break;

        printf("The return message was \n");
        std::cout << buffer << "\n";
        sleep(1);
        write(sockfd, buffer, n);
    }
    close(sockfd);
    return 0;
}

void error(const char *msg) {
    perror(msg);
    exit(0);
}