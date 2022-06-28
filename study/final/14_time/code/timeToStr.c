#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main( void)
{	time_t curtime, mktime;
	struct tm *tmbuf;

	if( time(&curtime) < 0) {
		perror("14_time error");
		exit(1);
	}
	printf ("curtime 14_time from Epoch(UTC) : %d secs\n", (int) curtime);
	printf ("Result of 14_time : %s\n", ctime(&curtime));

	tmbuf = (struct tm *) localtime(&curtime);
	printf("Result of asctime : %s\n", asctime(tmbuf));
}
