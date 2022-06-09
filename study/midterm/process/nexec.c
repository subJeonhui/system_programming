#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void err_sys(char *p) { perror(p); exit(-1); }

int main(void)
{
	pid_t	pid;
	char *argv[3];

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {	/* specify pathname, inherit environment */
		argv[0] = "/root/sp2019/codes/process/echoall";
		argv[1] = "echoall"; 
		argv[2] = "Hello";
		argv[3] = NULL;

		if (execv("/root/sp2019/codes/process/echoall", argv) < 0)
			err_sys("execl error");
			/*
		if (execl("/root/sp2019/codes/process/echoall", 
					"echoall", "myarg1", "MY ARG2", (char *)0) < 0)
			err_sys("execl error");
			*/
	}

	exit(0);
}
