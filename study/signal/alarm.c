#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int alarmed = 0;
int remain;

void sig_catcher(int signo)
{
	alarmed = 1;
	printf("SIGALRM catched...\n");
	alarm(0);
}

int main()
{
	int pid;


	signal(SIGALRM, sig_catcher);
	alarm(3);
	
	while(alarmed == 0);
	printf("after alarm in main\n");
}
