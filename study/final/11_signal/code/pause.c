#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h> 

void sig_catch (int sig_no) 
{ 
	printf("Signal delivered : %d \n", sig_no);
}

int main()
{
	int pid;
	int status ;

	signal(SIGUSR1, sig_catch);
	if ((pid = fork()) == 0) { // child
		pause(); 
		// this is not good !: 
		// what happens if the 11_signal is delivered before pause();
		// -> a permanent blocking will happen!
		printf("Child wake up\n");
		exit(0);
	} else { // parent
		sleep(5);
		kill (pid, SIGUSR1);
		wait(&status);
	}
}
