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

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	int key = 0x19027631+n;
	int server, qid, size;
	struct umsg sndmsg;
	struct umsg rcvmsg;
	
	if ((qid = msgget(key, IPC_CREAT | 0660)) == -1){
		perror("msgget");
		exit(1);
	} else {
		printf("message queue 0x%08x\n", key);
	}

	if((server = msgget(0x19027631,IPC_CREAT | 0660)) == -1){
		perror("msgget");
		exit(1);
	}

	while (1) {
		printf("Text to send: ");
		fgets(sndmsg.mtext,sizeof(sndmsg.mtext), stdin);
		sndmsg.mtype = n*100;
		sndmsg.mtext[strlen(sndmsg.mtext)] = '\0';
		
		if(msgsnd(server, &sndmsg, strlen(sndmsg.mtext), 0) == -1){
			perror("msgsnd");
			exit(2);
		}else{
			printf("send to client%d : %s", (n == 1 ? 2 : 1), sndmsg.mtext);
		}

		memset(rcvmsg.mtext,0,MSIZE);
		
		if ((size = msgrcv(qid, &rcvmsg, (size_t) MSIZE, (long) 0, 0)) == -1){
			perror("msgrcv");
			exit(2);
		} else {
			printf("recv from client%d : %s", (rcvmsg.mtype == 100 ? 1 : 2), rcvmsg.mtext);
		}

		if (strncmp(rcvmsg.mtext, "quit", 4) == 0){
			rcvmsg.mtype = n*100;
			msgsnd(server, &rcvmsg, strlen(rcvmsg.mtext), 0);
			msgctl(qid, IPC_RMID, NULL);
			printf("remove message queue 0x%08x\n", key);
			break;
		}
	}
	return 0;
}
