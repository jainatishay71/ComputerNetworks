#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8001
#define BACKLOG 5

void serve(int nsfd)
{
	char buff[50];

	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		recv(nsfd,buff,sizeof(buff),0);
		printf("The Server has received : %s\n", buff);
		if(strcmp(buff,"exit")==0)
			return ;
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='a' && buff[i]<='z')
			{
				buff[i] = buff[i]-'a';
				buff[i] = buff[i]+'A';
			}
			else
			{
				buff[i] = buff[i]-'A';
				buff[i] = buff[i]+'a';
			}
		}
		send(nsfd,buff,sizeof(buff),0);
	}
}

int main(int argc,char *argv[])
{
	int sfd,nsfd,sz,c;
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
			printf("Server Acception Successful\n");

		getpeername(sfd,(struct sockaddr *)&cliaddr,&sz);

		printf("Connected client is %s:%d\n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

		c = fork();
		if(c == 0)
		{
			printf("Inside Child!\n");
			close(sfd);
			serve(nsfd);
			exit(0);
		}
		else
		{
			close(nsfd);
		}
	}

	close(sfd);
}