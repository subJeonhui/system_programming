#include <stdio.h>

void main()
{
	long sum=0, multi=1;
	long i;

	for (i=0; i<0x8FFFFFFF; i++) {
		sum += i;
		multi *= i;
	}
}
