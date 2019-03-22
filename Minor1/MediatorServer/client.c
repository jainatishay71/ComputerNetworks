#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8000
#define BACKLOG 5

void func(int sfd)
{
	char buff[50];
	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		printf("Enter the msg to be sent to Server! : ");
		scanf("%[^\n]s",buff);
		while(getchar()!='\n');
		if(strcmp(buff,"")==0)
			continue;
		send(sfd,buff,sizeof(buff),0);

		if(strcmp(buff,"exit")==0)
			return;

		memset((char *)buff,0,sizeof(buff));
		recv(sfd,buff,sizeof(buff),0);
		printf("Received from Server : %s\n", buff);
	}
}

int main(int argc,char *argv[])
{
	int sfd,sz;
	struct sockaddr_in servaddr,cliaddr;

	memset((struct sockaddr_in *)&servaddr,0,sizeof(servaddr));

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		perror("socket");
		exit(0);
	}

	int temp = 1,tsz;
	tsz = sizeof(temp);
	if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&temp,tsz) < 0)
	{
		perror("setsockopt");
		exit(0);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
	{
		perror("connect");
		exit(0);
	}

	sz = sizeof(cliaddr);

	getsockname(sfd,(struct sockaddr *)&cliaddr,&sz);
	printf("Client is %s:%d\n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

	int port = ntohs(cliaddr.sin_port);

	char buff[50];
	memset((char *)buff,0,sizeof(buff));
	printf("Enter the service no in smallcaps english! : ");
	scanf("%s",buff);
	send(sfd,buff,sizeof(buff),0);

	int nsfd,csz;
	struct sockaddr_in cservaddr,ccliaddr;

	memset((struct sockaddr_in *)&cservaddr,0,sizeof(cservaddr));

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		perror("socket");
		exit(0);
	}

	int tempp = 1,tpsz;
	tpsz = sizeof(tempp);
	if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&tempp,tpsz) < 0)
	{
		perror("setsockopt");
		exit(0);
	}

	cservaddr.sin_family = AF_INET;
	cservaddr.sin_port = htons(port);
	cservaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(sfd,(struct sockaddr *)&cservaddr,sizeof(cservaddr))!=0)
	{
		perror("bind");
		exit(0);
	}

	if(listen(sfd,BACKLOG)!=0)
	{
		perror("listen");
		exit(0);
	}

	memset((struct sockaddr_in *)&ccliaddr,0,sizeof(ccliaddr));

	csz = sizeof(ccliaddr);
	nsfd = accept(sfd,(struct sockaddr *)&ccliaddr,&csz);
	if(nsfd < 0)
	{
		perror("accept");
		exit(0);
	}

	func(nsfd);
}