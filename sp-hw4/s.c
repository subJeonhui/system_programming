// 201902763 컴퓨터전자시스템공학부 이전희 정상동작
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX 1024
#define PORT 32763
#define BACKLOG 5

int main (int argc, char *argv[])
{
	int sd, nsd, pid, bytes, cliaddrsize;
	struct sockaddr_in cliaddr, servaddr;
	char data[MAX];
	
	if (( sd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf( stderr, "can’t open socket.\n");
		exit(1);
	}
	
	bzero ((char*) &servaddr, sizeof( servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (PORT);

	if (bind (sd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		fprintf (stderr, "can’t bind to socket.\n");
		exit(1);
	} // bind itself to the socket
	
	listen (sd, BACKLOG); // declare the client-queue length

	while (1) { // a typical server waiting loop
		cliaddrsize = sizeof (cliaddr);
		if (( nsd = accept (sd, (struct sockaddr *) &cliaddr, &cliaddrsize)) < 0) {
			fprintf (stderr, "can’t accept connection.\n");
			exit(1);
		} // upon return: client addr. is known and a new socket is created
		if ((pid = fork()) < 0) { // fork error, a new thread may be used!
			fprintf (stderr, "can’t fork process.\n"); exit(1);
		}

		if (pid == 0) { // the new child server for the connected client
			close (sd); // old socket is not necessary for me
			while(1) {
				bytes = recv (nsd, data, MAX, 0); // from client
				if (bytes == 0) // client quit
					break;
				else if (bytes < 0) { // error
					fprintf (stderr, "can’t receive data.\n"); exit(1);
				}
				if (send (nsd, data, bytes, 0) != bytes) { // echo back
					fprintf (stderr, "can’t send data.\n"); exit(1);
				}
			} // end while, client quits
			return 0; // child server exit.
		} // else : parent

		close (nsd); // parent: close the new socket
	}
}
