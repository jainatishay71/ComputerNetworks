#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8020
#define BACKLOG 5
int ssfd;

void *serve(void *arg)
{
	char buff[50];

	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		recv(ssfd,buff,sizeof(buff),0);
		printf("The Server has received : %s\n", buff);
		if(strcmp(buff,"exit")==0)
			return NULL;
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='A' && buff[i]<='Z')
			{
				buff[i] = buff[i]-'A';
				buff[i] = buff[i]+'a';
			}
		}
		send(ssfd,buff,sizeof(buff),0);
	}
}

int main(int argc,char *argv[])
{
	int sfd,nsfd,sz;
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
			printf("Service Acception Successful\n");

		char buff[50];
		memset((char *)buff,0,sizeof(buff));
		recv(nsfd,buff,sizeof(buff),0);
		printf("The Service has received : %s\n", buff);

		int port = atoi(buff);

		struct sockaddr_in sservaddr;
		memset((struct sockaddr_in *)&sservaddr,0,sizeof(sservaddr));

		ssfd = socket(AF_INET,SOCK_STREAM,0);
		if(ssfd < 0)
		{
			perror("socket");
			exit(0);
		}

		sservaddr.sin_family = AF_INET;
		sservaddr.sin_port = htons(port);
		sservaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		if(connect(ssfd,(struct sockaddr *)&sservaddr,sizeof(sservaddr))!=0)
		{
			perror("connect");
			exit(0);
		}

		pthread_t th;
		pthread_create(&th,NULL,serve,NULL);
	}
}