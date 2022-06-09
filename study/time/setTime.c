#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main( int argc, char *argv[])
{
	time_t curtime, modtime;

	if ( argc != 3) {
		perror("argument error");
		printf("format : [+|-] sec");
		exit(1);
	}
	modtime = (time_t) time(&curtime);

	printf ("current time : %d sec from 1970, 1, 1 00 : 00 : 00\n", (int) curtime);

	if (!strcmp(argv[1], "-"))
		modtime = curtime - atoi(argv[2]);
	else if (!strcmp(argv[1], "+"))
		modtime = curtime + atoi(argv[2]);
	else {
		perror ("argument error");
		exit(2);
	}
	if (stime(&modtime))	{
		perror("stime error"); 
	}
	printf ("modified time : %d sec from 1970, 1, 1 00 : 00 : 00\n",(int) modtime);
}
