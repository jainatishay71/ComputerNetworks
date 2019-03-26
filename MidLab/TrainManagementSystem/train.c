#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#define PORT 8000

int main()
{
	int sfd,nsfd,sz;
	struct sockaddr_in servaddr;
	sfd = socket(AF_INET,SOCK_STREAM,0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connected\n");

	char buff[50];
	memset(&buff,0,sizeof(buff));
	recv(sfd,buff,sizeof(buff),0);

	int tport = atoi(buff);

	int psfd;
	struct sockaddr_in pservaddr;
	psfd = socket(AF_INET,SOCK_STREAM,0);

	pservaddr.sin_family = AF_INET;
	pservaddr.sin_port = htons(tport);
	pservaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(psfd,(struct sockaddr *)&pservaddr,sizeof(pservaddr)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connected to platform with port : %d\n", tport);

	for(;;)
	{
		char msg[50];
		memset(&msg,0,sizeof(msg));
		printf("Enter the msg : ");
		scanf("%s",msg);
		if(send(psfd,msg,sizeof(msg),0) < 0)
		{
			perror("send");
		}
		if(strcmp(msg,"exit")==0)
		{
			break;
		}
		memset(&msg,0,sizeof(msg));
		if(recv(psfd,msg,sizeof(msg),0) < 0)
		{
			perror("recv");
		}
		printf("train received : %s\n", msg);
	}
}