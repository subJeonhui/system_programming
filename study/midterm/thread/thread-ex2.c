#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int glob_var = 101 ;

void *t_function(void *data)
{
	int i=0;
	pthread_t t_id;

	glob_var++;
	t_id = pthread_self();
	printf("pid = %u, t_id = %lu, i=%d, glob_var = %d\n", getpid(), t_id, i, glob_var);

	while (i++ < 10) {
		sleep(1);
		printf("t_id=%lu says %s \n", t_id, (char *) data) ;
	}
	return ((void *)t_id);
}


int main()
{
	pthread_t p_thread[2];
	int thr_id, status, i ; 
	char *msg[]= { "Hello", "World" } ;

	printf("before pthread_create() pid = %u, glob_var = %d\n",
			getpid(), glob_var);

	for (i=0; i<2; i++) 
		if((thr_id = pthread_create(&p_thread[0], NULL, t_function, (void*) msg[i])) < 0){
			perror("thread create error : "); 
			exit(i); 
		}

	printf("after pthread_create() glob_var = %d\n", glob_var);

	pthread_exit(0);
	return 0;
}
