#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
	int i;
	char cmdstr[1024];

	strcpy(cmdstr, "/bin/ls ");

	for(i=1;i < ac;i++) {
		strcat(cmdstr, av[i]);
		strcat(cmdstr, " ");
	}
	fprintf(stdout, "cmdstr = \"%s\"\n", cmdstr);

	system(cmdstr);

	exit(0);
}
