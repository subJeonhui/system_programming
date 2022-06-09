#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sig_catch (int sig_no)
{ // nothing to do
}

int main()
{
	int pid;
	sigset_t mysigset, oldsigset;
	sigemptyset(&mysigset); // empty the set
	sigaddset(&mysigset, SIGUSR1); // add SIGUSR1 to the set
	signal(SIGUSR1, sig_catch); // set a user-defined signal handler
	sigprocmask(SIG_BLOCK, &mysigset, &oldsigset); // block SIGUSR1

	if ((pid = fork()) == 0) { // child
		// pause(); // not good
		sigsuspend(&oldsigset); // unblock and pause!
		printf("Child wake up\n");
		exit(0);
	} else { // parent
		sleep(5);
		kill (pid, SIGUSR1); // send the SIGUSR1 to the child
		wait();
	}
}
