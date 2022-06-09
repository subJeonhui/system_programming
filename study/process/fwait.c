#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid; 
	int status;

	pid = fork();

	if (pid==0) {
		printf("I am child %d \n", getpid());
		exit(77);
	}
	else {
		wait(&status);
		printf("I am parent %d \n", getpid());
		printf("exit code from child: %d \n", WEXITSTATUS(status));
	}
}
