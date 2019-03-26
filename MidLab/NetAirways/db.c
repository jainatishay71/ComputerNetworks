#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/msg.h>

#define DPORT 8050

int main()
{
	int sfd1,sz;
	sfd1 = socket(AF_INET,SOCK_DGRAM,0);
	if(sfd1 < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("sfd1 created successfully\n");

	struct sockaddr_in servaddr1,cliaddr1;

	servaddr1.sin_family = AF_INET;
	servaddr1.sin_port = htons(DPORT);
	servaddr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(sfd1,(struct sockaddr *)&servaddr1,sizeof(servaddr1)) < 0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("sfd1 binded successfully\n");

	for(;;)
	{
		char buff[1024];
		memset(&buff,0,sizeof(buff));
		recvfrom(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,&sz);
		printf("On Display Board : %s\n\n", buff);
	}
}