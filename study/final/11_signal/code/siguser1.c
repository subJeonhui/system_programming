#include <signal.h>
#include <stdio.h>

int main()
{
	printf("Self Process 11_signal : \n");
	//raise(SIGUSR1);
	raise(SIGUSR1);
}
