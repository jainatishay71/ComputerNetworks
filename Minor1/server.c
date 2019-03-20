#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/msg.h>
#include <string.h>

#define MAX 20

struct msgbuf
{
	long mtype;
	char data[512];
};

int main()
{
	key_t key = ftok("forkey.txt",41);
	if(key < 0)
	{
		perror("ftok");
		exit(0);
	}

	int msgid = msgget(key,IPC_CREAT|0666);
	if(msgid < 0)
	{
		perror("msgget");
		exit(0);
	}

	int pidarr[MAX],cnt=0;
	for(;;)
	{
		struct msgbuf msg;
		memset(&msg,0,sizeof(msg));
		
		if(msgrcv(msgid,&msg,512,2,0) < 0)
		{
			perror("msgrcv");
			exit(0);
		}

		if(strlen(msg.data) >= 4)
		{
			if(msg.data[0]=='i' && msg.data[1]=='n' && msg.data[2]=='i' && msg.data[3]=='t')
			{
				char spid[50];
				strcpy(spid,"");
				int q=0;
				for(int i=0;i<strlen(msg.data);i++)
				{
					if(msg.data[i] == '|')
					{
						q=1;
						continue;
					}
					if(q==1)
					{
						char tmp[2];
						tmp[0] = msg.data[i];
						tmp[1] = '\0';
						strcat(spid,tmp);
					}
				}
				int cpid = atoi(spid);
				pidarr[cnt++] = cpid;
				printf("New client with pid %d registered\n", cpid);
			}
			else
			{
				char spid[50],tdata[100];
				strcpy(spid,"");
				strcpy(tdata,"");
				int q=0;
				for(int i=0;i<strlen(msg.data);i++)
				{
					if(msg.data[i] == '|')
					{
						q=1;
						continue;
					}
					if(q==1)
					{
						char tmp[2];
						tmp[0] = msg.data[i];
						tmp[1] = '\0';
						strcat(spid,tmp);
					}
					else
					{
						char tmp[2];
						tmp[0] = msg.data[i];
						tmp[1] = '\0';
						strcat(tdata,tmp);
					}
				}
				int cpid = atoi(spid);
				for(int i=0;i<cnt;i++)
				{
					if(pidarr[i] == cpid)
						continue;
					struct msgbuf cmsg;
					memset(&cmsg,0,sizeof(cmsg));
					cmsg.mtype = pidarr[i];
					strcpy(cmsg.data,tdata);
					if(msgsnd(msgid,&cmsg,512,0) < 0)
					{
						perror("msgsnd");
						exit(0);
					}
					if(kill(pidarr[i],SIGUSR1) < 0)
					{
						perror("kill");
						exit(0);
					}
				}
				printf("Sent to all!\n");
			}
		}
		else
		{
			char spid[50],tdata[100];
			strcpy(spid,"");
			strcpy(tdata,"");
			int q=0;
			for(int i=0;i<strlen(msg.data);i++)
			{
				if(msg.data[i] == '|')
				{
					q=1;
					continue;
				}
				if(q==1)
				{
					char tmp[2];
					tmp[0] = msg.data[i];
					tmp[1] = '\0';
					strcat(spid,tmp);
				}
				else
				{
					char tmp[2];
					tmp[0] = msg.data[i];
					tmp[1] = '\0';
					strcat(tdata,tmp);
				}
			}
			int cpid = atoi(spid);
			for(int i=0;i<cnt;i++)
			{
				if(pidarr[i] == cpid)
					continue;
				struct msgbuf cmsg;
				memset(&cmsg,0,sizeof(cmsg));
				cmsg.mtype = pidarr[i];
				strcpy(cmsg.data,tdata);
				if(msgsnd(msgid,&cmsg,512,0) < 0)
				{
					perror("msgsnd");
					exit(0);
				}
				if(kill(pidarr[i],SIGUSR1) < 0)
				{
					perror("kill");
					exit(0);
				}
			}
			printf("Sent to all!\n");
		}
	}
}