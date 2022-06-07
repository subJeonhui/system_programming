#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 1024 // max KB input length
#define PORT 32763// server port #
#define HOSTADDR "127.0.0.1" // server IP

int main(int argc, char *argv[]) {
    int sd, send_bytes, n, recv_bytes;

    struct sockaddr_in servaddr;
    char snddata[1024];
    char data[1024];

    bzero ((char *) &servaddr, sizeof(servaddr)); // prepare server address, port
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(HOSTADDR);
    servaddr.sin_port = htons (PORT);
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "can’t open socket.\n");
        exit(1);
    }

    if (connect(sd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "can’t connect to server.\n");
        exit(1);
    } // auto binding for this client and connect request

    while (fgets(snddata, MAX, stdin) != NULL) { // get a string from KB
        send_bytes = strlen(snddata);

        if (send(sd, snddata, send_bytes, 0) != send_bytes) { // to server
            fprintf(stderr, "can’t send data.\n");
            exit(1);
        }

        memset(data, 0, MAX);
        recv_bytes = recv(sd, data, MAX, 0);
        fputs(data, stdout); // display
        fflush(stdout);
        if(strcmp(data,"quit_ack") == 0)
            break;
        memset(data, 0, MAX);
    }
    close(sd);
    return 0;
}
