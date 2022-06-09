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
		write(1, &byte, 1); 
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
	int ofd1, ofd2, ifd;
	char line[200];
	FILE *fin, *fout1, *fout2;


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

	ifd = open("./sample.txt", O_RDONLY);

	fin = fdopen(ifd, "r");
	fout1 = fdopen(pipe1[1], "w");
	fout2 = fdopen(pipe2[1], "w");

	int i = 0;
	while (!feof(fin)) {
		if (fgets(line, sizeof(line), fin)==NULL) break;
		printf("%s", line); fflush(stdout);
		if (i++%2==0) { 
			//fputs(line, fout1); fflush(fout1);
			write(pipe1[1], line, strlen(line));
		}
		else {
			//fputs(line, fout2); fflush(fout2);
			write(pipe2[1], line, strlen(line));
		}
	}
	close(ifd);

	close(pipe1[1]);
	close(pipe2[1]);

	waitpid(c1pid, NULL, 0);
	waitpid(c2pid, NULL, 0);
}
