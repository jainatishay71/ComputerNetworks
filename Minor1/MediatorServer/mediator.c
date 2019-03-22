#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8000
#define BACKLOG 5
#define services 3

int main(int argc,char *argv[])
{
	int sfd,nsfd,sz;
	int port[] = {8010,8020,8050};
	char match[services][50];
	strcpy(match[0],"one");
	strcpy(match[1],"two");
	strcpy(match[2],"three");
	struct sockaddr_in servaddr,cliaddr;

	memset((struct sockaddr_in *)&servaddr,0,sizeof(servaddr));

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket Created Successfully!\n");

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("Socket Binded Successfully!\n");

	if(listen(sfd,BACKLOG)!=0)
	{
		perror("listen");
		exit(0);
	}
	else
		printf("Socket Listening..!\n");

	for(;;)
	{
		memset((struct sockaddr_in *)&cliaddr,0,sizeof(cliaddr));
		sz = sizeof(cliaddr);

		nsfd = accept(sfd,(struct sockaddr *)&cliaddr,&sz);
		if(nsfd < 0)
		{
			perror("accept");
			exit(0);
		}
		else
			printf("Mediator Acception Successful\n");

		getpeername(sfd,(struct sockaddr *)&cliaddr,&sz);
		printf("Connected client is %s:%d\n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

		int cport = ntohs(cliaddr.sin_port);
		char scport[50];
		strcpy(scport,"");
		sprintf(scport,"%d",cport);

		char buff[50];

		memset((char *)buff,0,sizeof(buff));
		recv(nsfd,buff,sizeof(buff),0);
		printf("The Mediator has received : %s\n", buff);

		int vsfd;
		struct sockaddr_in vservaddr;

		memset((struct sockaddr_in *)&vservaddr,0,sizeof(vservaddr));

		vsfd = socket(AF_INET,SOCK_STREAM,0);
		if(vsfd < 0)
		{
			perror("socket");
			exit(0);
		}

		for(int i=0;i<services;i++)
		{
			if(strcmp(buff,match[i])==0)
			{
				vservaddr.sin_family = AF_INET;
				vservaddr.sin_port = htons(port[i]);
				vservaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
			}
		}

		if(connect(vsfd,(struct sockaddr *)&vservaddr,sizeof(vservaddr))!=0)
		{
			perror("connect");
			exit(0);
		}

		send(vsfd,scport,sizeof(scport),0);
		printf("\nMediator has sent client's details to the service program!\n");
	}
}