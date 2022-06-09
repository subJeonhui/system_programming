#include <stdio.h> // race.c
#include <sys/types.h>

err_sys(char *p) { perror(p); exit(-1); }

int main(void) {
	pid_t  pid;

	if ((pid = fork()) < 0) err_sys("fork error");
	else if (pid == 0)  charatatime("output from child\n");
	else                charatatime("output from parent\n");
	exit(0);
}

charatatime(char *str)
{
	char  *ptr;
	int    c;

	for (ptr = str; c = *ptr++; ) {
		putc(c, stdout);
		fflush(stdout);
		usleep(1);
	}
}
