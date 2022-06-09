#include <signal.h>
#include <stdio.h>

int main()
{
	printf("Self Process signal : \n");
	//raise(SIGUSR1);
	raise(SIGUSR1);
}
