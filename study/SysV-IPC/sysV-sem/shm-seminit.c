#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY		100

int main(void)
{
	int semid;

	/* create a semaphore set with 1 semaphore: */
	if ((semid = semget(KEY, 1, 0666 | IPC_CREAT)) == -1) {
		perror("semget");
		exit(1);
	}

	/* initialize the semaphore #0 to 0: */
	if (semctl(semid, 0, SETVAL, 0) == -1) {
		perror("semctl");
		exit(1);
	}
	return 0;
}
