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

#define BACKLOG 5
int sfd;

void *serve(void *arg)
{
	int *val = (int *)arg;
	int nsfd = *val;
	for(;;)
	{
		char buff[50];
		memset(&buff,0,sizeof(buff));
		if(recv(nsfd,buff,sizeof(buff),0) < 0)
		{
			perror("recv");
			exit(0);
		}
		else
			printf("Received!\n");
		if(strcmp(buff,"exit")==0)
		{
			pthread_exit(NULL);
		}
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='a' && buff[i]<='z')
			{
				buff[i]-='a';
				buff[i]+='A';
			}
		}
		if(send(nsfd,buff,sizeof(buff),0) < 0)
		{
			perror("send");
			exit(0);
		}
		else
			printf("Sent Successfully!\n");
	}
}

void sig_handler()
{
	int nsfd,sz;
	struct sockaddr_in cliaddr;
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

	pthread_t th;
	pthread_create(&th,NULL,serve,(void *)&nsfd);
}

int main(int argc,char *argv[])
{
	signal(SIGUSR1,sig_handler);

	int port = atoi(argv[0]);
	int nsfd,sz;
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
	servaddr.sin_port = htons(port);
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

		pthread_t th;
		pthread_create(&th,NULL,serve,(void *)&nsfd);
	}
}