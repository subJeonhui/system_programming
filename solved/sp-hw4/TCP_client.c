#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
		int sd;
		int port = atoi(argv[1]);

		if((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
				perror("socket");
				return -1;
		}


		struct sockaddr_in servaddr;
		bzero((char*)&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		//servaddr.sin_port = htons(31337);
		servaddr.sin_port = htons(port);
		char snddata[100] = {0, };
		char recvdata[100] = {0, };

		if(connect(sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) <0)
		{
				perror("connect");
				return -1;
		}
		else
		{
			while(fgets(snddata, 100, stdin) != NULL)
			{

				if (strlen(snddata)==0) continue;


				if(send(sd, snddata, strlen(snddata), 0) == -1)
				{
					perror("send");
					return -1;
				}

				if(recv(sd, recvdata, 100, 0) == -1)
				{
					perror("recv");
					return -1;
				}

				printf("%s", recvdata);

				if(strncmp(recvdata, "quit_ack", 8) == 0)
				{
					close(sd);
					return 0;
				}

				memset(recvdata, 0, sizeof(recvdata));
			}
		}
}
