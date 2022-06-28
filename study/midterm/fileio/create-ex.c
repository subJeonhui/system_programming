#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char buf1[] = "Test1 data \n";
	char buf2[] = "Test2 data \n";

	if ((fd = creat ("test.txt", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
		printf("creat error");
		return 1;
	}
	write(fd, buf1, 10);
	if(lseek(fd, 6, SEEK_SET) == -1) {
		printf("lseek error");
		return 2 ;
	}
	write(fd, buf2, 10);
	return 0;
}
