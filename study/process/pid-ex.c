#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int pid;

	printf("Original process : pid = %d, ppid = %d, pgid = %d, pgrp = %d\n", getpid(), getppid(), getpgrp(), getpgid(getpid()));
	pid = fork();
	if(pid != 0) {
		printf("Parent process : pid = %d, ppid = %d, pgrp = %d, pgid = %d\n", getpid(), getppid(), getpgrp(), getpgid(getpid()));
	}
	else {
		printf("Child process : pid = %d, ppid = %d, pgrp = %d, pgid = %d\n", getpid(), getppid(), getpgrp(), getpgid(getpid()));
	}
	return 0;
}
