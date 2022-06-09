#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

void *threadFunc(void *arg)
{	
	int count = 0;
	printf("new thread started ....\n");
	int retval;

	//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	
	//pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	while(1)
	{
		printf("count = %d \n", count++);
		sleep(1);
		//pthread_testcancel();

		printf("1st print.... \n");
		fflush(stdout);

		// for testing pthread_exit() return value
		if(count == 10)
			break;
		printf("2nd print.... \n");
		fflush(stdout);
	}

	pthread_exit((void *) count);	
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int retval;
	void *res;

	pthread_create(&tid, NULL, threadFunc, NULL);


	sleep(3);

	pthread_cancel(tid);

	pthread_join(tid, &res);

	if(res != 0)
	{
		perror("pthread_join : ");
//		exit(EXIT_FAILURE);
	}	

	if(res == PTHREAD_CANCELED)
	{
		printf("thread cancled\n");
	}
	else
	{
		printf("thread is normal exit retval = %ld \n", (long)res);
	}

	exit(EXIT_SUCCESS);
}
