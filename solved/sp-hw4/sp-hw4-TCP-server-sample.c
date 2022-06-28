#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024
#define PORT 312345
#define BACKLOG 50

int count = 1;

void *thread_func(void *nnsd)
{
	int rnsd,bytes;
	char data[MAX];
	char send_data[MAX];
	int count_key = 0;

	char kv[100][2][50];
	char (*kv2)[2][50] = kv;

	rnsd = *((int *)nnsd);
	while(1){
		data[0] = '\0';
		send_data[0] = '\0';
		bytes = recv(rnsd,data,MAX,0);
		char* ptr = strtok(data," ");
		if (strncmp(ptr,"quit",4)==0){
			for(int i=0;ptr[i]!=0;i++){
				if(ptr[i] == '\n'){
					ptr[i]=0;
					break;
				}
			}
			printf("0%d client : quit\n",count);
			sprintf(send_data,"quit_ack\n");
			send(rnsd,send_data,strlen(send_data),0);
			break;
		}
		else if(strncmp(ptr,"read",4)==0){
			char* key = strtok(NULL," ");
			for(int j=0;key[j]!=0;j++){
				if(key[j]=='\n'){
					key[j] = 0;
					break;
				}
			}
			if(count_key == 0){
				sprintf(send_data,"read Failed\n");
				send(rnsd,send_data,strlen(send_data),0);
				printf("read Failed\n");
			}
			else{
				for(int k=0;k<count_key;k++){
					if(strcmp(key,kv2[k][0])==0){
						sprintf(send_data,"0%d read: %s %s\n",count,key,kv2[k][1]);
						send(rnsd,send_data,strlen(send_data),0);
						printf("client 0%d : read %s\n",count,key);
						break;
					}
					else{
						if(k == count_key-1 && strcmp(kv2[k][0],key)!=0){
							sprintf(send_data,"read Failed\n");
							send(rnsd,send_data,strlen(send_data),0);
							printf("read Failed\n");
							break;
						}
					}
				}
			}
		}	
		else if(strncmp(ptr,"insert",6)==0){
			char *key = strtok(NULL," ");
			char *value = strtok(NULL," ");
			for(int l=0;value[l]!=0;l++){
				if(value[l]=='\n'){
					value[l]=0;
					break;
				}
			}
			if (count_key==0){
				strcpy(kv2[0][0],key);
				strcpy(kv2[0][1],value);
				count_key++;
				sprintf(send_data,"0%d insert OK : %s %s\n",count,key,value);
				send(rnsd,send_data,strlen(send_data),0);
				printf("client 0%d : insert %s %s\n",count,key,value);
			}
			else if(count_key==100){
				sprintf(send_data,"0%d insert Failed\n",count);
				send(rnsd,send_data,strlen(send_data),0);
				printf("0%d insert Failed\n",count);
			}
			else{
				int index = count_key;
				for(int m=0;m<index;m++){
					if(strcmp(key,kv2[m][0])==0){
						sprintf(send_data,"0%d insert Failed\n",count);
						send(rnsd,send_data,strlen(send_data),0);
						printf("0%d insert Failed\n",count);
					}
					else{
						if(m==count_key-1 && strcmp(kv2[m][0],key)!=0){
							strcpy(kv2[count_key][0],key);
							strcpy(kv2[count_key][1],value);
							count_key++;
							sprintf(send_data,"0%d insert OK : %s %s\n",count,key,value);
							send(rnsd,send_data,strlen(send_data),0);
							printf("client 0%d : insert %s %s\n",count,key,value);
						}
					}
				}
			}
		}
		else if(strncmp(ptr,"update",6)==0){
			char *key = strtok(NULL," ");
		    char *value = strtok(NULL," ");
			for(int n=0;value[n]!=0;n++){
				if(value[n]=='\n'){
					value[n]=0;
					break;
				}
            }
			if(count_key == 0){
				sprintf(send_data,"0%d update Failed\n",count);
				send(rnsd,send_data,strlen(send_data),0);
				printf("0%d update Failed\n",count);
			}
			else{
				for(int o=0;o<count_key;o++){
					if(strcmp(key,kv2[o][0])==0){
						strcpy(kv2[o][1],value);
						sprintf(send_data,"0%d update OK : %s %s\n",count,key,value);
						send(rnsd,send_data,strlen(send_data),0);
						printf("client 0%d : update %s %s\n",count,key,value);
						break;
					}
					else{
						if(o==count_key-1 && strcmp(kv2[o][0],key)!=0){
							sprintf(send_data,"0%d update Failed\n",count);
							send(rnsd,send_data,strlen(send_data),0);
							printf("0%d update Failed\n",count);
						}
					}
				}
			}	
		}
	}
	close(rnsd);
	pthread_exit((void*)pthread_self());
}



int main(int argc,char *argv[]){
	int sd,nsd,pid,bytes,cliaddsize;
	pthread_t pid_arr[50];
	int rval[50];
	 	
	struct sockaddr_in cliaddr,servaddr;
	char data[MAX];

	sd = socket(AF_INET,SOCK_STREAM,0);
	bzero ((char*)&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	
	bind(sd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	listen(sd,BACKLOG);
	
	while(count < 50){
		cliaddsize = sizeof(cliaddr);
		nsd = accept(sd,(struct sockaddr *)&cliaddr,&cliaddsize);
		pthread_create(&pid_arr[count],NULL,thread_func,(void *)&nsd);	
		pthread_join(pid_arr[count],(void **)&rval[count]);
		count++;
	}
	close(sd);
	return 0;
}
