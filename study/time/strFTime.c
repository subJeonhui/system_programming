#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main( void)
{
	struct tm time_str;
	char str[80];
	size_t strsize = 80;
	int year, month, day;

	time_str.tm_sec = 0;
	time_str.tm_min = 0;
	time_str.tm_hour = 0;

	printf ("year : ");
	scanf ("%d", &year);
	printf ("month : ");
	scanf ("%d", &month);
	printf ("day : ");
	scanf ("%d", &day);

	time_str.tm_mday = day;
	time_str.tm_mon = month - 1;
	time_str.tm_year = year - 1900;
	time_str.tm_isdst = - 1;

	if( mktime (&time_str) == -1)
		perror("mktime");
	if (strftime(str, strsize, "%A %b %d %j %U %W", &time_str) <= 0)
		perror("strftime");
	printf ("%s\n", str);
}


