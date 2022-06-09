#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{ 
	int fd[2]; // two descripters for pipe
	int input, output; // copy input and output
	int n; char buf[10];

	pipe(fd); // fd[0] for reading, fd[1] for writing
	if (fork() == 0) { // child process executes
		input = open ("input", 0); // open for reading
		close( fd[0] ); // close read pipe channel
		while ( (n = read (input, buf, 10)) != 0) // until EOF
			write (fd[1], buf, n); // to pipe
		close(input);
		close(fd[1]);
		exit(0);
	} else { // parent process executes
		close (fd[1]); // close unnecessary wrting channel
		output = creat("output", 0666);
		while ( (n = read(fd[0], buf, 10)) != 0) // until pipe closed
			write(output, buf, n);
		close (output); close(fd[0]);
		wait(NULL);
	}
}
