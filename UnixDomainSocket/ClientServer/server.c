#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define NAME "socket"
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
			return;
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='a' && buff[i]<='z')
			{
				buff[i] = buff[i]-'a';
				buff[i] = buff[i]+'A';
			}
			else if(buff[i]>='A' && buff[i]<='Z')
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
	int usfd,nsfd,sz;
	struct sockaddr_un servaddr;

	memset((struct sockaddr_un *)&servaddr,0,sizeof(servaddr));

	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket Created Successfully!\n");

	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path,NAME);

	if(bind(usfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("Socket Binded Successfully!\n");

	if(listen(usfd,BACKLOG)!=0)
	{
		perror("listen");
		exit(0);
	}
	else
		printf("Socket Listening..!\n");


	nsfd = accept(usfd,NULL,NULL);
	if(nsfd < 0)
	{
		perror("accept");
		exit(0);
	}
	else
		printf("Server Acception Successful\n");

	serve(nsfd);

	close(usfd);
	unlink(NAME);
}