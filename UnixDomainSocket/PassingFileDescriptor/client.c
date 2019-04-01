#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PATH "pathone"

void func(int usfd)
{
	for(;;)
	{
		char buff[512];
		memset(&buff,0,sizeof(buff));

		printf("Enter Something : ");
		scanf("%[^\n]s",buff);
		while(getchar()!='\n');

		if(send(usfd,buff,sizeof(buff),0) < 0)
		{
			perror("send");
			exit(0);
		}
		else
			printf("Request Sent!\n");

		if(strcmp(buff,"bye")==0)
		{
			return;
		}
		memset(&buff,0,sizeof(buff));
		if(recv(usfd,buff,sizeof(buff),0) < 0)
		{
			perror("recv");
			exit(0);
		}
		else
			printf("Received : %s\n", buff);
	}
}

int main()
{
	int usfd;
	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket Created Successfully!\n");

	struct sockaddr_un servaddr;

	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path,PATH);

	if(connect(usfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connected Successfully!\n");

	func(usfd);
	close(usfd);

	unlink(PATH);
}