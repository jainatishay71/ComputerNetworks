#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8010

int main(int argc,char *argv[])
{
	int sfd;
	struct sockaddr_in servaddr,cliaddr;

	memset((struct sockaddr_in *)&servaddr,0,sizeof(servaddr));

	sfd = socket(AF_INET,SOCK_DGRAM,0);
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

	for(;;)
	{
		char buff[50];
		memset((char *)buff,0,sizeof(buff));

		printf("Enter the msg to be sent to server : ");
		scanf("%[^\n]s",buff);
		while(getchar()!='\n' );

		if(sendto(sfd,buff,sizeof(buff),0,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
		{
			perror("sendto");
			exit(0);
		}
		else
			printf("Client has sent the Request!\n\n");

		int sz = sizeof(servaddr);
		if(recvfrom(sfd,buff,sizeof(buff),0,(struct sockaddr *)&servaddr,&sz)<0)
		{
			perror("recvfrom");
			exit(0);
		}
		else
			printf("The Client has received : %s\n", buff);
	}

	close(sfd);
}