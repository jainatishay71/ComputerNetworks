#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8001

void func(int sfd)
{
	char buff[50];
	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		printf("Enter the msg to be sent to Server! : ");
		scanf("%s",buff);
		send(sfd,buff,sizeof(buff),0);
		printf("Client has sent a msg to Server!\n");

		if(strcmp(buff,"exit")==0)
			return ;

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
	else
		printf("Socket successful in Client!\n");

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connect Successful!\n");

	memset((struct sockaddr_in *)&cliaddr,0,sizeof(cliaddr));

	sz = sizeof(cliaddr);
	getsockname(sfd,(struct sockaddr *)&cliaddr,&sz);

	printf("Client is %s:%d\n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

	func(sfd);

	close(sfd);

}