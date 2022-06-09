#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
	pid_t pid; 

	pid = fork();

	if (pid==0) {
		printf("I am child %d \n", getpid());
		for (;;);
	}
	else {
		printf("I am parent %d \n", getpid());
		for (;;);
	}
}
