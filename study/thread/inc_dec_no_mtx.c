#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *Incrementer();
void *Decrementer();
int counter = 0 ;

//pthread_mutex_t mVar=PTHREAD_MUTEX_INITIALIZER;

int main()
{
   pthread_t ptid,ctid;

 //  pthread_mutex_init(&mVar, NULL) ;
   pthread_create(&ptid,NULL,Incrementer,NULL);
   pthread_create(&ctid,NULL,Decrementer,NULL);

   pthread_join(ptid,NULL);
   pthread_join(ctid,NULL);

   return 0;
}

void *Incrementer()
{
	int oldcnt, diff ;

   for(;;) {
 //	pthread_mutex_lock(&mVar);
	oldcnt = counter ;
 	counter++ ;
	diff = counter - oldcnt ;
	if (diff==0 || diff>1) {
		printf("race condition found in INC (old: %d, new: %d) \n", oldcnt, counter);
		exit(0);
	}
 	printf("Inc : %d \n", counter);
 //	pthread_mutex_unlock(&mVar);
   }
}

void *Decrementer()
{
    int oldcnt, diff ;

   for(;;) {
 //	pthread_mutex_lock(&mVar);
	oldcnt = counter ;
 	counter-- ;
	diff = oldcnt - counter ;
	if (diff==0 || diff>1) {
		printf("race condition found in DEC (old: %d, new: %d) \n", oldcnt, counter);
		exit(0);
	}
 	printf("Dec : %d \n", counter);
 //	pthread_mutex_unlock(&mVar);
   }
}
