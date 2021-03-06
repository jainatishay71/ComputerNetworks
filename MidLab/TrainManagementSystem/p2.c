#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#define PORT 8050
#define SHMSIZE 512

struct shmbuf
{
	int f1,f2,f3;
	int pid1,pid2,pid3;
};

void sig_handler()
{
	printf("A Traing is arriving!\n");
}

int main()
{
	signal(SIGUSR1,sig_handler);

	key_t key = ftok("forkey.txt",41);
	int shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	struct shmbuf *shm;

	shm = shmat(shmid,NULL,0);
	shm->pid2 = getpid();

	sem_t *semm = sem_open("/semap",0);

	// int tmp;
	// sem_getvalue(semm,&tmp);
	// printf("semaphore value is : %d\n", tmp);

	sem_wait(semm);
	shm->f2 = 1;
	sem_post(semm);

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
		sz = sizeof(cliaddr);
		nsfd = accept(sfd,(struct sockaddr *)&cliaddr,&sz);
		if(nsfd < 0)
		{
			perror("accept");
		}
		else
			printf("accepted at 2\n");
		kill(shm->pid3,SIGUSR1);
		kill(shm->pid1,SIGUSR1);
		sem_wait(semm);
		shm->f2 = 0;
		sem_post(semm);
		for(;;)
		{
			char buff[50];
			memset(&buff,0,sizeof(buff));
			recv(nsfd,buff,sizeof(buff),0);
			printf("Received : %s\n", buff);
			if(strcmp(buff,"exit")==0)
				break;

			strcat(buff," from platform 2");
			send(nsfd,buff,sizeof(buff),0);
		}
		sem_wait(semm);
		shm->f2 = 1;
		sem_post(semm);
	}
}