#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

char buf[10000000];

void set_fl(int fd, int flags) {
	int val;
	if ( (val = fcntl(fd, F_GETFL, 0)) < 0)
		perror("fcntl F_GETFL error");
	val |= flags; /* turn on flags */
	if (fcntl(fd, F_SETFL, val) < 0)
		perror("fcntl F_SETFL error");
}

void clr_fl(int fd, int flags) {
	int val;
	if ( (val = fcntl(fd, F_GETFL, 0)) < 0)
		perror("fcntl F_GETFL error");
	val &= ~flags; /* turn flags off */
	if (fcntl(fd, F_SETFL, val) < 0)
		perror("fcntl F_SETFL error");
}

int main(void) {
	int ntowrite, nwrite; char *ptr;
	int i;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "read %d bytes\n", ntowrite);
	set_fl(STDOUT_FILENO, O_NONBLOCK); /* set nonblocking */
	for (ptr = buf; ntowrite > 0; ) {
		errno = 0;
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "\n nwrite = %d, errno = %d \n", nwrite, errno);
		ptr = buf;
		if (nwrite > 0) {
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}
	clr_fl(STDOUT_FILENO, O_NONBLOCK);
	exit(0);
}
