#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{
	int sock, bytes_recieved;
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr;

	host = gethostbyname("127.0.0.1");

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9000);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	memset(&(server_addr.sin_zero), 0x00, 8);

	if (connect(sock, (struct sockaddr *)&server_addr,
				sizeof(struct sockaddr)) == -1) {
		perror("Connect");
		exit(1);
	}

	while(1) {
		printf("\nSEND : ");
		fgets(send_data, sizeof(send_data), stdin);

		if (strcmp(send_data , "q") != 0 && strcmp(send_data , "Q") != 0)
			send(sock,send_data,strlen(send_data), 0);

		else {
			send(sock,send_data,strlen(send_data), 0);
			close(sock);
			break;
		}
		bytes_recieved=recv(sock,recv_data,1024,0);
		recv_data[bytes_recieved] = '\0';

		if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0) {
			close(sock);
			break;
		}
		else
			printf("\nRECV = %s\n " , recv_data);
	}
	return 0;
}
