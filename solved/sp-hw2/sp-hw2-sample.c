#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

# define NUM_PROCCESS 10
# define BUFFER_SIZE 1024

struct Operation{
	char acc_no[6];
	char optype;
	int amount;
};

struct Account{
	char acc_no[6];
	char name[10];
	int balance;
}; 

void GetInfoCmdLine(char *buff, struct Operation *container);
int IsExistAccount(int fd, char *buffer);
int GetAccBalance(int fd, int rec_no);
void SetAccBalance(int fd, int rec_no, int amount, int curr_balance);
int RecordLock(int fd, int rec_no, int len, int type);
void PrintCommandResult(pid_t pid, struct Operation *op, char *cmd, int curr_acc_balance);
void Sleep();

int main(int argc, char *argv[])
{
	struct Account acc;
	FILE *facc, *fcmd;
	int fd_acc, fd_cmd;
	int child_status;
	char buff[BUFFER_SIZE];
	pid_t pid[NUM_PROCCESS];

	if((fd_acc = open("./account.dat",O_RDWR | O_CREAT)) == -1){
		perror("open account.dat");
		return 1;
	}

	if((facc = fdopen(fd_acc, "r+")) == NULL){
		perror("fdopen account.dat");
		return 2;
	}
	
	for(int i=0; i<NUM_PROCCESS; i++){
		pid[i] = fork();
		if(pid[i] == 0){

			if((fd_cmd = open("./operation.dat",O_RDONLY | O_CREAT)) == -1){
				perror("open operation.dat");
				return 1;
			}

			if((fcmd = fdopen(fd_cmd, "r")) == NULL){
				perror("fdopen operation.dat");
				return 2;
			}

			while(feof(fcmd) == 0){
				if(fgets(buff,BUFFER_SIZE,fcmd) != NULL){
					int rec_no;
					int curr_acc_balance = -1;
					struct Operation op;
					GetInfoCmdLine(buff, &op);
					rec_no = (op.acc_no[0] - '0') -1;

					switch(op.optype){

						case 'w':
							RecordLock(fd_acc, rec_no, sizeof(struct Account), F_WRLCK);

							curr_acc_balance = IsExistAccount(fd_acc, op.acc_no);
							if(curr_acc_balance != -1){
								SetAccBalance(fd_acc, rec_no, -1 * op.amount, curr_acc_balance);
								curr_acc_balance -= op.amount;
							}
							PrintCommandResult(getpid(), &op, "withraw", curr_acc_balance);

							RecordLock(fd_acc, rec_no, sizeof(struct Account), F_UNLCK);
							Sleep();
							break;

						case 'd':
							RecordLock(fd_acc, rec_no, sizeof(struct Account), F_WRLCK);

							curr_acc_balance = IsExistAccount(fd_acc, op.acc_no);
							if(curr_acc_balance != -1){
								SetAccBalance(fd_acc, rec_no, op.amount, curr_acc_balance);
								curr_acc_balance += op.amount;
							}
							PrintCommandResult(getpid(), &op, "deposit", curr_acc_balance);

							RecordLock(fd_acc, rec_no, sizeof(struct Account), F_UNLCK);
							Sleep();
							break;

						case 'i':
							RecordLock(fd_acc, rec_no, sizeof(struct Account), F_RDLCK);

							curr_acc_balance = IsExistAccount(fd_acc, op.acc_no);
							if(curr_acc_balance != -1){
								printf("계좌 %s 의 현재 금액 : %d\n", op.acc_no, curr_acc_balance);
							}
							PrintCommandResult(getpid(), &op, "inquiry", curr_acc_balance);

							RecordLock(fd_acc, rec_no, sizeof(struct Account), F_UNLCK);
							Sleep();
							break;
					}
				}
			}
			fclose(fcmd);
			close(fd_cmd);
			exit(i);
		}
	}

	for(int i=0; i<NUM_PROCCESS; i++){
		wait(&child_status);
	}

	for(int i=0; i<5; i++){
		int pos = i * sizeof(struct Account);
		lseek(fd_acc, pos, SEEK_SET);

		int n = read(fd_acc, &acc, sizeof(struct Account));
		printf("acc_no: %s balance: %d\n", acc.acc_no, acc.balance);
	}

	fclose(facc);
	close(fd_acc);

	return 0;
}

// operation.dat을 한 줄씩 계좌번호, 유형, 금액으로 나눠 저장한다.
void GetInfoCmdLine(char *buff, struct Operation *container){
	sscanf(buff,"%s %c %d",container->acc_no, &container->optype, &container->amount);
}

int IsExistAccount(int fd, char *buffer){
	struct Account acc;

	for(int i=0; i<5; i++){
		int pos = i * sizeof(struct Account);
		lseek(fd, pos, SEEK_SET);

		int n = read(fd, &acc, sizeof(struct Account));
		if(strcmp(acc.acc_no, buffer) == 0){
			return acc.balance;
		}
	}	
	return -1;
}

int GetAccBalance(int fd, int rec_no){
	struct Account acc;

	int pos = rec_no * sizeof(struct Account);
	lseek(fd, pos, SEEK_SET);

	int n = read(fd, &acc, sizeof(struct Account));
	return acc.balance;
}

void SetAccBalance(int fd, int rec_no, int amount, int curr_balance){
	struct Account acc;

	int pos = rec_no * sizeof(struct Account);
	curr_balance += amount;
	acc.balance = curr_balance;

	lseek(fd, pos, SEEK_SET);
	write(fd, &acc, sizeof(struct Account));
}

int RecordLock(int fd, int rec_no, int len, int type){
	struct flock fl;

	fl.l_type = type;
	fl.l_whence = SEEK_SET;
	fl.l_start = rec_no * len;
	fl.l_len = len;

	switch(type){
		case F_RDLCK:
		case F_WRLCK:
			return fcntl(fd, F_SETLKW, &fl);
		case F_UNLCK:
			return fcntl(fd, F_SETLK, &fl);
		default:
			return -1;
	}
}

void PrintCommandResult(pid_t pid, struct Operation *op, char *cmd, int curr_acc_balance){
	char *error_msg = "계좌정보 없음";
	char char_cmd = cmd[0];

	switch(char_cmd){
		case 'd':
		case 'w':
			if(curr_acc_balance != -1){
				printf("pid: %d acc_no: %s %s: %d balance: %d\n", pid, op->acc_no, cmd, op->amount, curr_acc_balance); 
			}
			else{
				printf("pid: %d acc_no: %s %s: %d %s\n", pid, op->acc_no, cmd, op->amount, error_msg);
			}
			break;

		case 'i':
			if(curr_acc_balance != -1){
				printf("pid: %d acc_no: %s %s balance: %d\n", pid, op->acc_no, cmd, curr_acc_balance); 
			}
			else{
				printf("pid: %d acc_no: %s %s %s\n", pid, op->acc_no, cmd, error_msg); 
			}
			break;
	}
}

void Sleep(){
	srand(time(NULL));
	useconds_t rand_num = rand()%1000001;
	usleep(rand_num);
}
