#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
	struct servent *appl_name;
	char name[4] = "ftp";
	char proto[4] = "tcp";
	int port;

	appl_name = getservbyname(name, proto);

	if (!appl_name)
		printf("unknown application %s\n", name);
	else
	{
		port = ntohs(appl_name->s_port);
		printf("sizeof port : %d \n", sizeof(appl_name->s_port));
		printf("The service %s was successful at port %d \n", appl_name->s_name, port);
	} 
}
