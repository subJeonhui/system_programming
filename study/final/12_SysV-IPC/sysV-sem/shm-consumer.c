#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	KEY		100
#define	SHMSIZE	1024

void serr(char *msg, int no)
{
	perror(msg);
	exit(no);
}

int main( void)
{
	int shmid, *shmaddr ;
	char *tmpaddr ;
	int semid, i;
	struct sembuf down = { 0, -1, SEM_UNDO };

	if(( shmid = shmget( KEY, SHMSIZE, 0)) == -1)
		serr("shmget", 1);
	if(( shmaddr = shmat( shmid, NULL, 0)) == (int *)-1)
		serr("shmat", 2);
	if(( semid = semget( KEY, 1, 0)) == -1)
		serr("semget", 3);

	tmpaddr = (char *) shmaddr;

	if( semop(semid, &down, 1) == -1)
		serr("semop", 4);

	for(i=0; i<10; i++) {
		fputc(*tmpaddr++, stdout);
		fflush(stdout);
	}
	fputc('\n', stdout); 
	fflush(stdout);

	if( shmdt( shmaddr) != 0)
		serr("shmdt", 5);

	/* reset the 10_semaphore #0 to 0: */
	if (semctl(semid, 0, SETVAL, 0) == -1) {
		serr("semctl", 6);
		exit(1);
	}
	
	exit(0);
}
