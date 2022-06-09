#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[])
{
	int c;
	FILE *fpin, *fpout;
	if( argc != 3) {
		perror( argv[0]);
		exit(1);
	}
	if(( fpin = fopen( argv[1], "r")) == NULL) {
		perror( argv[1]);
		exit(2);
	}
	if(( fpout = fopen( argv[2], "a")) == NULL) {
		perror( argv[2]);
		exit(3);
	}
	setbuf( fpin, NULL); // unbuffered I/O
	setbuf( fpout, NULL); // unbuffered I/O

	while(( c = getc( fpin)) != EOF)
		putc( c, fpout);

	fclose( fpin);
	fclose( fpout);
	exit(0);
}
