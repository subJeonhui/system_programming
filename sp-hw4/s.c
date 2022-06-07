// 201902763 컴퓨터전자시스템공학부 이전희 정상동작
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024
#define PORT 32763
#define BACKLOG 50
#define HOSTADDR "127.0.0.1"
#define DICTMAX 3

typedef struct dictElement {
    char key[50];
    char value[50];
} element;

element Dictionary[DICTMAX];

typedef struct threadData {
    int cid;
    int nsd;
} ThreadData;

int request(char *key, char *value, int mode) {
    for (int i = 0; i < DICTMAX; i++) {
        if (strcmp(Dictionary[i].key, key) == 0) {
            if (mode == 0) {
                strcpy(value, Dictionary[i].value);
                return 1;
            } else if (mode == 1) {
                strcpy(Dictionary[i].value, value);
                return 1;
            } else if (mode == 2) {
                return 0;
            }
        }
        if (mode == 2 && strlen(Dictionary[i].key) == 0) {
            strcpy(Dictionary[i].key, key);
            strcpy(Dictionary[i].value, value);
            return 1;
        }
    }
    return 0;
}


void split(char *string, char *command, char *key, char *value) {
    int cnt = 0;
    int spl = 0;

    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == ' ' || string[i] == '\n' || string[i] == '\0') {
            if (cnt == 0)
                command[i - spl] = '\0';
            else if (cnt == 1)
                key[i - spl] = '\0';
            else if (cnt == 2)
                value[i - spl] = '\0';
            spl = i + 1;
            cnt += 1;
        } else {
            if (cnt == 0)
                command[i - spl] = string[i];
            else if (cnt == 1)
                key[i - spl] = string[i];
            else if (cnt == 2)
                value[i - spl] = string[i];
        }
    }
}

void *service(void *d) {
    long bytes;
    char data[MAX];
    char sndData[MAX];
    char command[10];
    char key[50];
    char value[50];

    ThreadData *td = (ThreadData *) d;

    int cid = td->cid;
    int nsd = td->nsd;

    while (1) {
        memset(data, 0, MAX);
        bytes = recv(nsd, data, MAX, 0);

        if (bytes < 0) {
            fprintf(stderr, "can’t receive data.\n");
            pthread_exit(NULL);
        }

        printf("client %02d : %s", cid, data);

        split(data, command, key, value);

        memset(sndData, 0, MAX);

        if (strcmp(command, "read") == 0) {
            if (request(key, value, 0) == 1) {
                sprintf(sndData, "%02d read: %s %s\n", cid, key, value);
            } else {
                sprintf(sndData, "%02d read Failed\n", cid);
            }
        } else if (strcmp(command, "update") == 0) {
            if (request(key, value, 1) == 1) {
                sprintf(sndData, "%02d update OK : %s %s\n", cid, key, value);
            } else {
                sprintf(sndData, "%02d update Failed\n", cid); //(key가 없는 경우)
            }
        } else if (strcmp(command, "insert") == 0) {
            if (request(key, value, 2) == 1) {
                sprintf(sndData, "%02d insert OK : %s %s\n", cid, key, value);
            } else {
                sprintf(sndData, "%02d insert Failed\n", cid);
            }
        } else if (strcmp(command, "quit") == 0) {
            sprintf(sndData, "quit_ack");
        } else {
            strcpy(sndData, data);
        }
        if (send(nsd, sndData, strlen(sndData), 0) != strlen(sndData)) { // echo back
            fprintf(stderr, "can’t send data.\n");
            pthread_exit(NULL);
        }

        if (strcmp(command, "quit") == 0) {
            break;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int sd, nsd;
    pthread_t tid;
    int clientNum = 1;
    ThreadData td;
    long cliaddrsize;
    struct sockaddr_in cliaddr, servaddr;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "can’t open socket.\n");
        exit(1);
    }

    bzero ((char *) &servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(HOSTADDR);
    servaddr.sin_port = htons (PORT);

    if (bind(sd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "can’t bind to socket.\n");
        exit(1);
    }

    listen(sd, BACKLOG);

    while (1) {
        cliaddrsize = sizeof(cliaddr);
        if ((nsd = accept(sd, (struct sockaddr *) &cliaddr, (socklen_t *) &cliaddrsize)) < 0) {
            fprintf(stderr, "can’t accept connection.\n");
            exit(1);
        } else {
            td.cid = clientNum++;
            td.nsd = nsd;
            pthread_create(&tid, NULL, (void *) service, (void *) &td);
        }
    }
}
