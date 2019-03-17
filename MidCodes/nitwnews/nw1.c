#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#define SHSIZE 1024
#define PATH "SFIFO"

void hold(int sfd,int ffd)
{
	for(;;)
	{
		char news[512];
		memset(&news,0,sizeof(news));
		if(recv(sfd,news,sizeof(news),0) < 0)
		{
			perror("recv");
			exit(0);
		}
		else
		{
			if(strcmp(news,"bye")==0)
			{
				printf("Telecast Over\n");
				return;
			}
			printf("News : %s\n", news);
			write(ffd,news,sizeof(news));
			fflush(0);
		}
	}
}

struct msgbuf
{
	long mtype;
	char mtext[512];
};

struct shmbuf
{
	int epid;
	int nropid;
	int nrtpid;
	int nrthpid;
	int cnt;
};

int main()
{
	sem_t *semm1 = sem_open("/semap1",0);
	sem_t *semm2 = sem_open("/semap2",0);
	sem_t *semm3 = sem_open("/semap3",0);

	int a,b,c;
	if(sem_getvalue(semm1,&a) < 0)
	{
		perror("sem_getvalue");
		exit(0);
	}
	if(sem_getvalue(semm2,&b) < 0)
	{
		perror("sem_getvalue");
		exit(0);
	}
	if(sem_getvalue(semm3,&c) < 0)
	{
		perror("sem_getvalue");
		exit(0);
	}

	printf("%d %d %d\n", a,b,c);

	mkfifo(PATH,0666);
	int ffd = open(PATH,O_WRONLY);
	if(ffd < 0)
	{
		perror("open");
		exit(0);
	}

	int msgid;
	key_t key = ftok("msgkey.txt",47);
	msgid = msgget(key,IPC_CREAT|0666);
	if(msgid < 0)
	{
		perror("msgget");
		exit(0);
	}

	int shmid;
	struct shmbuf *shm;
	key_t skey = ftok("shmkey.txt",17);
	shmid = shmget(key,SHSIZE,IPC_CREAT|0666);
	shm = (struct shmbuf *)shmat(shmid,NULL,0);

	shm->cnt = 0;
	shm->nropid = getpid();

	for(;;)
	{
		struct msgbuf msgr;
		int retval,q=0;
		
		sem_wait(semm1);
		retval = msgrcv(msgid,&msgr,512,0,0);
		printf("Received : %s\n", msgr.mtext);
		write(ffd,msgr.mtext,sizeof(msgr.mtext));
		fflush(0);
		for(int i=0;i<strlen(msgr.mtext);i++)
		{
			if((msgr.mtext[i]<'0' || msgr.mtext[i]>'9') && msgr.mtext[i]!='\n')
			{
				q=1;
				break;
			}
		}
		if(q==0)
		{
			kill(shm->epid,SIGUSR1);

			printf("Live Telecast is going to start....\n");
			int sfd;
			sfd = socket(AF_INET,SOCK_STREAM,0);
			if(sfd < 0)
			{
				perror("socket");
				exit(0);
			}
			else
				printf("Socket Created Successfully!\n");

			struct sockaddr_in servaddr;
			servaddr.sin_family = AF_INET;
			servaddr.sin_port = htons(atoi(msgr.mtext));
			servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

			if(connect(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
			{
				perror("connect");
				exit(0);
			}
			else
				printf("Connection Successful!\n");

			hold(sfd,ffd);
		}
		sem_post(semm2);
	}
}