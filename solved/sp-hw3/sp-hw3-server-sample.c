#include "mymessage.h"

int main(int argc, char *argv[]){
    int qid, size;
    int client_1, client_2;
    struct umsg mymsg;
    int type;
	int end = -2;
	
    if((qid = msgget(0x20003021, IPC_CREAT | 0660)) == -1){
        perror("msgget");
        exit(1);
    }
    if((client_1 = msgget(0x20003022, IPC_CREAT | 0660)) == -1){
        perror("msgget");
        exit(1);
    }
    if((client_2 = msgget(0x20003023, IPC_CREAT | 0660)) == -1){
        perror("msgget");
        exit(1);
    }
    printf("message queue 0x20003021 생성\n");

    while(1){
        memset(mymsg.mtext,0,MSIZE);
        // client 1에서 receive
        if((size = msgrcv(qid, &mymsg, (size_t)MSIZE,100,0)) != -1)
        {
            mymsg.mtext[strlen(mymsg.mtext)] = '\0';
            printf("recv from client 1 : %s\n", mymsg.mtext);
			if(!strncmp("quit", mymsg.mtext, 4)){
				end += 1;
			}
            // client 2에게 send
			mymsg.mtype = 200;
			printf("send to client 2 : %s\n", mymsg.mtext);
            if(msgsnd(client_2, &mymsg, strlen(mymsg.mtext), 0) == -1){
                perror("msgsnd");
                exit(2);
		    }
        }
        // client 2에서 receive
        if((size = msgrcv(qid, &mymsg, (size_t)MSIZE, 200, 0)) != -1)
        {
            mymsg.mtext[strlen(mymsg.mtext)] = '\0';
            printf("recv from client 2 : %s\n",mymsg.mtext);
			if(!strncmp("quit", mymsg.mtext, 4)){
				end += 1;
			}
            // client 1에게 send
			mymsg.mtype = 100;
			printf("send to client 1 : %s\n", mymsg.mtext);
            if(msgsnd(client_1, &mymsg, strlen(mymsg.mtext), 0) == -1){
                perror("msgsnd");
                exit(2);
		    }
        }
		if(end == 0){
			msgctl(qid, IPC_RMID, NULL);
			break;
		}
    }
}
