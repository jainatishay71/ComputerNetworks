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
	int sfd,nsfd,sz;
	struct sockaddr_in servaddr,cliaddr;

	memset((struct sockaddr_in *)&servaddr,0,sizeof(servaddr));

	sfd = socket(AF_INET,SOCK_DGRAM,0);
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

	for(;;)
	{
		char buff[50];
		memset((char *)buff,0,sizeof(buff));

		int sz = sizeof(servaddr);
		if(recvfrom(sfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,&sz)<0)
		{
			perror("recvfrom");
			exit(0);
		}
		else
			printf("The Server has received : %s\n", buff);

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
		if(sendto(sfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr))<0)
		{
			perror("sendto");
			exit(0);
		}
		else
			printf("Request Served!\n\n");
	}

	close(sfd);
}