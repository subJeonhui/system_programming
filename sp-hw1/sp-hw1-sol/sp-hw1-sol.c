#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>


void do_child_work(int fd[], int ofd, int (*work)(int c))
{
	char byte;
	ssize_t n;

	close(fd[1]);
	while (1) {
		n = read(fd[0], &byte, 1);
		if (n==-1 || n==0) break;
		
		byte = (char) work((int) byte);
		write(1, &byte, 1); // to stdout 
		write(ofd, &byte, 1); 
	}

	close(ofd);
	close(fd[0]);

	exit(0);
}



void main(void)
{
	pid_t c1pid, c2pid;
	int pipe1[2], pipe2[2];
	int ofd1, ofd2;
	char *msg1 = "Hello World!!! \n";
	char *msg2 = "Goodbye My friend!!! \n";


	pipe(pipe1);
	c1pid = fork();
	if (c1pid==0) {
		ofd1 = creat("./out1.txt", 0644);
		do_child_work(pipe1, ofd1, toupper);
	}

	pipe(pipe2);
	c2pid = fork(); 
	if (c2pid==0) { 
		ofd2 = creat("./out2.txt", 0644);
		do_child_work(pipe2, ofd2, tolower);
	}

	close(pipe1[0]);
	close(pipe2[0]);

	write(pipe1[1], msg1, strlen(msg1));
	write(pipe2[1], msg2, strlen(msg2));

	close(pipe1[1]);
	close(pipe2[1]);

	waitpid(c1pid, NULL, 0);
	waitpid(c2pid, NULL, 0);
}
