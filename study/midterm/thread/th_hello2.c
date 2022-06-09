#include <stdio.h>
#include <pthread.h>

int global;

void *print_hello()
{
	int i=0;

	while (i<10000) {
		printf("%s : %d \n", "My Hello", i++);
	}

	pthread_exit((void *)pthread_self());
}

void *printmsg(void *msg)
{
	int i=0;

	while (i<10000) {
		printf("%s : %d \n", (char *) msg, i++);
	}

	pthread_exit((void *)pthread_self());
}

void main()
{
	pthread_t	t1, t2;
	unsigned long rval1, rval2;
	int mydata;

	//pthread_create(&t1, NULL, printmsg, "Hello");
	pthread_create(&t1, NULL, print_hello, NULL);
	pthread_create(&t2, NULL, printmsg, "World");
	printf("t1: %lu, t2: %lu \n", t1, t2);

	pthread_join(t1, (void **)&rval1);
	pthread_join(t2, (void **)&rval2);

	printf("t1: %lu, t2: %lu \n", t1, t2);
	printf("rval1: %lu, rval2: %lu \n", (unsigned long)rval1, (unsigned long)rval2);
}
