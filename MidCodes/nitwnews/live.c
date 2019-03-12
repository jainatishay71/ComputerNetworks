#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8010
#define BACKLOG 5

void telecast(int nsfd)
{
	for(;;)
	{
		char news[512];
		memset(&news,0,sizeof(news));
		printf("Enter the news : ");
		scanf("%[^\n]s", news);
		while(getchar()!='\n');
		if(send(nsfd,news,sizeof(news),0) < 0)
		{
			perror("send");
			exit(0);
		}
		else
			printf("Sent Successfully!\n");
		if(strcmp(news,"bye")==0)
		{
			printf("This telecast is over!\n");
			return;
		}
	}
}

int main()
{
	int sfd,sz;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		perror("sfd");
		exit(0);
	}
	else
		printf("Socket Created Successfully!\n");

	struct sockaddr_in servaddr,cliaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("Binded Successfully!\n");

	if(listen(sfd,BACKLOG) < 0)
	{
		perror("listen");
		exit(0);
	}
	else
		printf("Listening...!\n");

	for(;;)
	{
		memset(&cliaddr,0,sizeof(cliaddr));
		sz = sizeof(cliaddr);
		int nsfd = accept(sfd,(struct sockaddr *)&cliaddr,&sz);
		if(nsfd < 0)
		{
			perror("accept");
			exit(0);
		}
		else
			printf("Accepted By Live Telecast!\n");

		telecast(nsfd);
		close(nsfd);
	}
	
}