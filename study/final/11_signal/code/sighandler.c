#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

static void sigcatcher(int); // 새로운 signal handler
static void sigalrmcatcher(int);

void (*was)(int); // 이전 signal handler을 저장할 포인터 변수
void (*wasAlrm)(int);

int main(void) {
    // signal handler 변경
    // signal(int singal, sighandler_t handler)
    // return ( old sinal handler / SIG_ERR )
    if ((was = signal(SIGINT, sigcatcher)) == SIG_ERR) {
        perror("SIGINT");
        exit(1);
    }
    if ((wasAlrm = signal(SIGALRM, sigalrmcatcher)) == SIG_ERR) {
        perror("SIGALRM");
        exit(1);
    }
    while (1) {
        printf("pause\n");
        pause();
    }
    // pause() signal이 올 때까지 기다림
}

static void sigcatcher(int signo) {
    switch (signo) {
        case SIGINT :
            // SIGINT가 들어오면 PID와 메세지 출력
            printf("PID %d caught 11_signal SIGINT.\n", getpid());
            // 이전 signal handler로 되돌림
            signal(SIGINT, was); // dependent on linux, bsd versions
            kill(getpid(), SIGALRM);
            break;
        default :
            // 다른 signal이 들어왔을 경우
            fprintf(stderr, "something wrong\n");
            exit(1);
    }
}

static void sigalrmcatcher(int signo) {
    switch (signo) {
        case SIGALRM :
            // SIGALRM이 들어오면 PID와 메세지 출력
            printf("PID %d caught 11_signal SIGALRM.\n", getpid());
            // 이전 signal handler로 되돌림
            signal(SIGALRM, wasAlrm); // dependent on linux, bsd versions
            kill(getpid(), SIGINT);
            break;
        default :
            // 다른 signal이 들어왔을 경우
            fprintf(stderr, "something wrong\n");
            exit(1);
    }
}
