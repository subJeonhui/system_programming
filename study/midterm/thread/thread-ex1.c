#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS		5 

void *PrintHello(void *threadid) 
{	

	while (1) printf("\n%d: Hello World!\n", (int) threadid); 
	pthread_exit(NULL); 
} 

int main (int argc, char *argv[]) 
{   pthread_t threads[NUM_THREADS];  
	int rc, t; 

	for(t=0; t < NUM_THREADS; t++) { 
		printf("Creating thread %d\n", t); 
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t); 
		if (rc) { printf("ERROR; …%d\n", rc);  exit(-1); }
	} 
	pthread_exit(NULL);
} 
