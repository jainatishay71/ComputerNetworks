#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define NAME "socket"

void func(int sfd)
{
	char buff[50];
	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		printf("Enter the msg to be sent to Server! : ");
		scanf("%[^\n]s",buff);
		while(getchar()!='\n');
		send(sfd,buff,sizeof(buff),0);
		printf("Client has sent a msg to Server!\n");

		if(strcmp(buff,"exit")==0)
			return;

		memset((char *)buff,0,sizeof(buff));
		recv(sfd,buff,sizeof(buff),0);
		printf("Received from Server : %s\n", buff);
	}
}

int main(int argc,char *argv[])
{
	int usfd;
	struct sockaddr_un servaddr,cliaddr;

	memset((struct sockaddr_in *)&servaddr,0,sizeof(servaddr));

	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket successful in Client!\n");

	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path,NAME);

	if(connect(usfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connect Successful!\n");

	func(usfd);

	close(usfd);
}