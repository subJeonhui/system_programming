#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

static void sigcatcher(int);
void (*was)(int);

int main(void)
{
	if( (was = signal( SIGINT, sigcatcher )) == SIG_ERR) {
		perror("SIGINT");
		exit(1);
	}
	while(1) pause();
}

static void sigcatcher( int signo )
{
	switch( signo) {
		case SIGINT :
			printf("PID %d caught 11_signal SIGINT.\n", getpid());
			signal(SIGINT, was); // dependent on linux, bsd versions
			break;
		default :
			fprintf(stderr, "something wrong\n");
			exit(1);
	}
}
