#include "mymessage.h"

int main(int argc, char *argv[])
{
	int qid, size;
    int server;
	long type;
	struct umsg mymsg;
    int a;

    if(argc != 2){
		fprintf(stderr, "usage : %s [key] [type]\n", argv[0]);
		perror("msqq");
		exit(1);
	}

    if((server = msgget(0x20003021, IPC_CREAT | 0660)) == -1){
        perror("msgget");
        exit(1);
    }

	//message queue 열기
	if((a = (long)atoi(argv[1])) == 1){
		qid = msgget(0x20003022, IPC_CREAT|0660);
		printf("message queue 0x20003022 생성\n");
	}else if((a = (long)atoi(argv[1])) == 2){
		qid = msgget(0x20003023, IPC_CREAT|0660);
		printf("message queue 0x20003023 생성\n");
	}

	type = (long)a*100;	
	mymsg.mtype = type;

	while(1){
		if(type == 100){
			printf("Text to send : ");
			fgets(mymsg.mtext, sizeof(mymsg.mtext), stdin);
			printf("\n");
			mymsg.mtext[strlen(mymsg.mtext)] = '\0';
			printf("send to client 2 : %s\n", mymsg.mtext);
			if(msgsnd(server, &mymsg, strlen(mymsg.mtext), 0) == -1){
				perror("msgsnd");
				exit(2);
			}

			memset(mymsg.mtext, 0, MSIZE);
			if((size = msgrcv(qid, &mymsg, (size_t)MSIZE, type, 0)) == -1){
				perror("msgrcv");
				exit(2);
				}else{
					
					mymsg.mtext[strlen(mymsg.mtext)] = '\0';
					printf("recv from client 2 : %s\n", mymsg.mtext);
				}

			if(!strncmp("quit", mymsg.mtext, 4)){
				msgctl(qid, IPC_RMID, NULL);
				break;
			}
		}else if(type == 200){
			memset(mymsg.mtext, 0, MSIZE);
			if((size = msgrcv(qid, &mymsg, (size_t)MSIZE, type, 0)) == -1){
				perror("msgrcv");
				exit(2);
			}else{
				mymsg.mtext[strlen(mymsg.mtext)] = '\0';
				printf("recv from client 1 : %s\n",mymsg.mtext);
			}
			printf("Text to send : ");
			fgets(mymsg.mtext, sizeof(mymsg.mtext), stdin);
			printf("\n");

			mymsg.mtext[strlen(mymsg.mtext)] = '\0';
			
			printf("send to client 1 : %s\n", mymsg.mtext);
			if(msgsnd(server, &mymsg, strlen(mymsg.mtext), 0) == -1){
				perror("msgsnd");
				exit(2);
			}
			if(!strncmp("quit", mymsg.mtext, 4)){
				msgctl(qid, IPC_RMID, NULL);
				break;
			}
		}
        
	}

	return 0;
}
