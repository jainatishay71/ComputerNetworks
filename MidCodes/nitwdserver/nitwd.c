#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#define services 2
#define BACKLOG 5
#define SHMSIZE 512

int curr[]={0,0};

void sig_handler()
{
	printf("A child has ended!\n");
	curr[0]--;
}

void *saylen(void *arg)
{
	int *val = (int *)arg;
	int nsfd = *val;
	for(;;)
	{
		char buff[500],tbuff[500];
		memset(&buff,0,sizeof(buff));
		if(recv(nsfd,buff,sizeof(buff),0) < 0)
		{
			perror("recv");
			exit(0);
		}
		else
		{
			printf("String received from client is : %s\n", buff);
			if(strcmp(buff,"bye")==0)
			{
				curr[1]--;
				return NULL;
			}	
			int len = strlen(buff);
			memset(&buff,0,sizeof(buff));
			memset(&tbuff,0,sizeof(tbuff));
			while(len)
			{
				int r=len%10;
				char tmp[2];
				tmp[0]=r+'0';
				tmp[1]='\0';
				strcat(tbuff,tmp);
				len/=10;
			}
			for(int i=strlen(tbuff)-1;i>=0;i--)
			{
				char tmp[2];
				tmp[0]=tbuff[i];
				tmp[1]='\0';
				strcat(buff,tmp);
			}
			if(send(nsfd,buff,sizeof(buff),0) < 0)
			{
				perror("send");
				exit(0);
			}
			else
			{
				printf("Sent successfully!\n");
			}
		}
	}
}

int main()
{
	key_t key = ftok("./file",41);
	int shmid;
	shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	int *shm;
	shm = (int *)shmat(shmid,NULL,0);

	*shm = getpid();

	signal(SIGUSR1,sig_handler);
	int sfd[services],port[]={8000,8001},mxfd=-1,limit[]={2,2},sz;
	pthread_t th[limit[1]];
	struct sockaddr_in servaddr[services];
	struct sockaddr_in cliaddr;

	for(int i=0;i<services;i++)
	{
		sfd[i] = socket(AF_INET,SOCK_STREAM,0);
		if(sfd[i] < 0)
		{
			perror("socket");
			exit(0);
		}
		else
		{
			printf("Socket %d created successfully!\n", i+1);
			if(mxfd < sfd[i])
				mxfd = sfd[i];
		}	
	}

	for(int i=0;i<services;i++)
	{
		servaddr[i].sin_family = AF_INET;
		servaddr[i].sin_port = htons(port[i]);
		servaddr[i].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	}

	for(int i=0;i<services;i++)
	{
		if(bind(sfd[i],(struct sockaddr *)&servaddr[i],sizeof(servaddr[i])) < 0)
		{
			perror("bind");
			exit(0);
		}
		else
			printf("Socket %d binded successfully!\n", i+1);
	}

	for(int i=0;i<services;i++)
	{
		if(listen(sfd[i],BACKLOG) < 0)
		{
			perror("listen");
			exit(0);
		}
		else
			printf("Socket %d listening!\n", i+1);
	}

	for(;;)
	{
		fd_set readfds;
		FD_ZERO(&readfds);

		for(int i=0;i<services;i++)
			FD_SET(sfd[i],&readfds);

		int sret;
		sret = select(mxfd+1,&readfds,NULL,NULL,NULL);
		if(sret > 0)
		{
			if(FD_ISSET(sfd[0],&readfds))
			{
				if(curr[0] < limit[0])
				{
					memset(&cliaddr,0,sizeof(cliaddr));
					sz = sizeof(cliaddr);
					int nsfd = accept(sfd[0],(struct sockaddr *)&cliaddr,&sz);
					if(nsfd < 0)
					{
						perror("accept");
						exit(0);
					}
					else
					{
						printf("Accepted by Socket 1\n");
						curr[0]++;
						int c = fork();
						if(c == 0)
						{
							printf("Inside Child!\n");
							close(sfd[0]);
							dup2(nsfd,0);
							dup2(nsfd,1);
							execlp("/home/atishay/Profile/two/CN/MidCodes/nitwdserver/help","help",NULL);
						}
						else
							close(nsfd);
					}
				}
			}
			if(FD_ISSET(sfd[1],&readfds))
			{
				if(curr[1] < limit[1])
				{
					memset(&cliaddr,0,sizeof(cliaddr));
					sz = sizeof(cliaddr);
					int nsfd = accept(sfd[1],(struct sockaddr *)&cliaddr,&sz);
					if(nsfd < 0)
					{
						perror("accept");
						exit(0);
					}
					else
					{
						printf("Accepted by Socket 2\n");
						pthread_create(&th[curr[1]],NULL,saylen,(void *)&nsfd);
						curr[1]++;
					}
				}
			}
		}
		else
		{
			printf("No one is ready!\n");
		}
	}
	shmdt(shm);								//to detach
	shmctl(shmid,IPC_RMID,NULL);			//to remove from shared memory
}