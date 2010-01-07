#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int prt = 27745;
char ip[] = "127.0.0.1";
int s1;

int la_connect()
{
	struct sockaddr_in sock;
	sock.sin_addr.s_addr=inet_addr(ip);
	sock.sin_port = htons(prt);
	sock.sin_family = AF_INET;
	printf("[>] Entering in Astral via libastral (%s:%d)...\n",ip,prt);
	s1 = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(s1, &sock, sizeof(sock))==0){
		printf("[!] Enter complete!\n");
		return 0;
	}
	else
	{
		printf("[X] Sorry, your brain destructed by libastral =(\n");
		return -1;
	}
}
