#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#define PORT 8000
#define SHMSIZE 512

struct shmbuf
{
	int f1,f2,f3;
	int pid1,pid2,pid3;
};

int main()
{
	int port[] = {8040,8050,8060};

	key_t key = ftok("forkey.txt",41);
	int shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	struct shmbuf *shm;

	shm = shmat(shmid,NULL,0);

	sem_t *semm = sem_open("/semap",0);

	// int tmp;
	// sem_getvalue(semm,&tmp);
	// printf("semaphore value is : %d\n", tmp);

	int sfd,nsfd,sz;
	struct sockaddr_in servaddr,cliaddr;
	sfd = socket(AF_INET,SOCK_STREAM,0);

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	bind(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	listen(sfd,5);

	for(;;)
	{
		int flag = 0;
		char buff[50];
		memset(&buff,0,sizeof(buff));
		sem_wait(semm);
		if(shm->f1 == 1)
		{
			sprintf(buff,"%d",port[0]);
			flag = 1;
		}
		else if(shm->f2 == 1)
		{
			sprintf(buff,"%d",port[1]);
			flag = 1;
		}
		else if(shm->f3 == 1)
		{
			sprintf(buff,"%d",port[2]);
			flag = 1;
		}
		if(flag)
		{
			sz = sizeof(cliaddr);
			printf("connecting to platform : %s\n", buff);
			nsfd = accept(sfd,(struct sockaddr *)&cliaddr,&sz);
			send(nsfd,buff,sizeof(buff),0);
		}
		sem_post(semm);
		sleep(2);
	}
}