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

#define SHSIZE 1024

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

struct shmbuf *shm;

void inccnt()
{
	printf("Count Incremented as Live Telecast Started\n");
	int cnt = shm->cnt;
	cnt++;
	shm->cnt = cnt;
	printf("Total Live Telecasts so far : %d\n", shm->cnt);
}

int main()
{
	signal(SIGUSR1,inccnt);

	int msgid,flag=2;
	key_t key = ftok("msgkey.txt",47);
	msgid = msgget(key,IPC_CREAT|0666);
	if(msgid < 0)
	{
		perror("msgget");
		exit(0);
	}

	int shmid;
	key_t skey = ftok("shmkey.txt",17);
	shmid = shmget(key,SHSIZE,IPC_CREAT|0666);
	shm = (struct shmbuf *)shmat(shmid,NULL,0);

	shm->cnt = 0;
	shm->epid = getpid();

	FILE *fp1 = popen("./rp1","r");
	FILE *fp2 = popen("./rp2","r");
	FILE *fp3 = popen("./rp3","r");
	FILE *fp4 = popen("./docw","w");
	int fd1 = fileno(fp1);
	int fd2 = fileno(fp2);
	int fd3 = fileno(fp3);
	int fd4 = fileno(fp4);

	int maxfd=-1;

	if(maxfd<fd1)
		maxfd=fd1;
	if(maxfd<fd2)
		maxfd=fd2;
	if(maxfd<fd3)
		maxfd=fd3;

	for(;;)
	{
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(fd1,&readfds);
		FD_SET(fd2,&readfds);
		FD_SET(fd3,&readfds);

		int sret;
		sret = select(maxfd+1,&readfds,NULL,NULL,NULL);
		if(sret > 0)
		{
			struct msgbuf msgr;
			if(FD_ISSET(fd1,&readfds))
			{
				read(fd1,msgr.mtext,sizeof(msgr.mtext));

				if(strlen(msgr.mtext)>=2)
				{
					if(msgr.mtext[0]=='/' && msgr.mtext[1]=='d')
					{
						char tbuf[512];
						memset(&tbuf,0,sizeof(tbuf));
						for(int i=2;i<strlen(msgr.mtext);i++)
						{
							char tmp[2];
							tmp[0]=msgr.mtext[i];
							tmp[1]='\0';
							strcat(tbuf,tmp);
						}
						write(fd4,tbuf,sizeof(tbuf));
						fflush(0);
						continue;
					}
				}
				
				int retval;
				msgr.mtype = flag;
				retval = msgsnd(msgid,&msgr,sizeof(msgr.mtext),0);
				if(retval < 0)
				{
					perror("msgsnd");
				}
			}
			if(FD_ISSET(fd2,&readfds))
			{
				read(fd2,msgr.mtext,sizeof(msgr.mtext));

				if(strlen(msgr.mtext)>=2)
				{
					if(msgr.mtext[0]=='/' && msgr.mtext[1]=='d')
					{
						char tbuf[512];
						memset(&tbuf,0,sizeof(tbuf));
						for(int i=2;i<strlen(msgr.mtext);i++)
						{
							char tmp[2];
							tmp[0]=msgr.mtext[i];
							tmp[1]='\0';
							strcat(tbuf,tmp);
						}
						write(fd4,tbuf,sizeof(tbuf));
						fflush(0);
						continue;
					}
				}
				
				int retval;
				msgr.mtype = flag;
				retval = msgsnd(msgid,&msgr,sizeof(msgr.mtext),0);
				if(retval < 0)
				{
					perror("msgsnd");
				}
			}
			if(FD_ISSET(fd3,&readfds))
			{
				read(fd3,msgr.mtext,sizeof(msgr.mtext));
				
				if(strlen(msgr.mtext)>=2)
				{
					if(msgr.mtext[0]=='/' && msgr.mtext[1]=='d')
					{
						char tbuf[512];
						memset(&tbuf,0,sizeof(tbuf));
						for(int i=2;i<strlen(msgr.mtext);i++)
						{
							char tmp[2];
							tmp[0]=msgr.mtext[i];
							tmp[1]='\0';
							strcat(tbuf,tmp);
						}
						write(fd4,tbuf,sizeof(tbuf));
						fflush(0);
						continue;
					}
				}

				int retval;
				msgr.mtype = flag;
				retval = msgsnd(msgid,&msgr,sizeof(msgr.mtext),0);
				if(retval < 0)
				{
					perror("msgsnd");
				}
			}
		}
	}
}