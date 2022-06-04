// 201902763 이전희 완성
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSIZE 256

struct umsg {
	int mtype;
	char mtext[MSIZE];
};

int main(int argc, char *argv[]){
	int key = 0x19027631;
	int server, qid1, qid2, size;
	struct umsg msg, sndmsg;
		
	if ((server = msgget(key, IPC_CREAT | 0660)) == -1){
		perror("msgget");
		exit(1);
	} else {
		printf("message queue 0x%08x\n", key);
	}
	
	if ((qid1 = msgget(0x19027632, IPC_CREAT | 0660)) == -1){
		perror("msgget");
		exit(1);
	}

	if ((qid2 = msgget(0x19027633, IPC_CREAT | 0660)) == -1){
		perror("msgget");
		exit(1);
	}

	while (1) {
		memset(msg.mtext, 0, MSIZE);
		if ((size = msgrcv(server, &msg, (size_t) MSIZE, (long) 0, 0)) == -1){
			perror("msgrcv");
			exit(2);
		}else{
			printf("recv from client%d : %s", (msg.mtype == 100 ? 1 : 2), msg.mtext);
			if((msg.mtype == 100) ? (qid2 != 0):(qid1 != 0)){
				if (msgsnd((msg.mtype == 100 ? qid2 : qid1), &msg, strlen(msg.mtext), 0) == -1){
					perror("msgsnd");
					exit(2);
				}else{
					printf("send to clint%d : %s", (msg.mtype == 100 ? 2 : 1), msg.mtext);
					if (strncmp(msg.mtext, "quit", 4) == 0){
						if(msg.mtype == 100){
							qid2 = 0;
						}else{
							qid1 = 0;
						}
					}
				}
			}else{
				msgctl(server, IPC_RMID,NULL);
				printf("remove message queue 0x%08x\n", key);
				break;
			}
		}

	}
	return 0;
}
