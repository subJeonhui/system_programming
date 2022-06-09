#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>

#define	KEY	100
#define	SHMSIZE 1024
#define SFLAG	( IPC_CREAT | S_IRUSR | S_IWUSR)

void serr( char *call, int no)
{
	perror( call);
	exit(no);
}

int main( void)
{
	char c;
	int shmid, *shmaddr ;
	char *tmpaddr ;
	int semid, resource = 1;
	struct shmid_ds buf ;
	struct sembuf up = { 0, 1, SEM_UNDO};
	struct sembuf reset = { 0, 0, SEM_UNDO};

	if((shmid = shmget(KEY, SHMSIZE, SFLAG)) == -1)
		serr("shmget", 1);

	if((semid = semget(KEY, 1, SFLAG)) == -1)
		serr("semget", 2);

	if((shmaddr = (int *)shmat( shmid, NULL, 0)) == (int *)-1)
		serr("shmat", 4);

	tmpaddr = (char *) shmaddr;

	for(c='0'; c<='9'; c++)
		*tmpaddr++ = c;
	*tmpaddr = (char) 0 ;

	if( semop( semid, &up, 1) == -1)
		serr("semop", 5);

	if( shmdt( shmaddr) != 0)
		serr("shmdt", 6);

	/*
	if( shmctl(shmid, IPC_RMID, (struct shmid_ds *) &buf) == -1)
			serr("shmctl", 7);

	if( semctl(semid, 0, IPC_RMID) == -1) 
			serr("semctl", 8);
	*/

	exit(0);
}
