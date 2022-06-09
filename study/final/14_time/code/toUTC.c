#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main( void)
{
	time_t curtime, mkdtime;
	struct tm *tmbuf;

	if (time(&curtime) < 0) {
		perror("14_time error");
		exit(1);
	}

	printf ("curtime 14_time from 19970, 1, 1 00 : 00 : 00 : %d sec \n\n", (int) curtime);

	printf ("we make tm struct and print 14_time information \n");

	tmbuf = (struct tm *) localtime(&curtime);

	printf ("%4d year", tmbuf->tm_year + 1900);
	printf ("%4d month", tmbuf->tm_mon + 1);
	printf ("%4d day", tmbuf->tm_mday);
	printf ("%4d hour", tmbuf->tm_hour);
	printf ("%4d minute", tmbuf->tm_min);
	printf ("%4d sec\n\n", tmbuf->tm_sec);

	mkdtime = mktime(tmbuf);

	printf("Next is result of mktime which make second from tm struct \n");

	printf ("mktime result : %d\n", (int) mkdtime);
}
