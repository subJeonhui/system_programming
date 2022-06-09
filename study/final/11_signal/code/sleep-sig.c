#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int alarmed = 0;
int remain;

void sig_catcher(int signo)
{
	printf("SIGNAL %d catched...\n", signo);
}

int main()
{
	int pid;
	int remained;


	signal(SIGUSR1, sig_catcher);

	if (fork()==0) {
		sleep(2);
		kill(getppid(), SIGUSR1);
		exit(0);
	}

	remained = sleep(10);
	printf("remained sleep 14_time: %d \n", remained);
}
