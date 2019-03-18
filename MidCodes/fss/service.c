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
	struct sockaddr_in cliaddr;
	memset((struct sockaddr_in *)&cliaddr,0,sizeof(cliaddr));
	int sz,nsfd;
	sz = sizeof(cliaddr);
	nsfd = accept(sfd,(struct sockaddr *)&cliaddr,&sz);
	if(nsfd < 0)
	{
		perror("accept");
		exit(0);
	}
	else
		printf("Accepted Successfully!\n");
	
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

	pthread_t th;
	pthread_create(&th,NULL,serve,NULL);
}

int main(int argc,char *argv[])
{
	signal(SIGUSR1,sig_handler);

	sfd = atoi(argv[0]);
	for(;;)
	{
		pthread_t th;
		pthread_create(&th,NULL,serve,NULL);
		pthread_join(th,NULL);
	}
}