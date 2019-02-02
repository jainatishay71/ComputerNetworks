#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgid,tot[50];
char tmp[512];
int group[50][50];

struct msgbuf
{
	long mtype;
	char mtext[512];
};

void* rcv(void* arg)
{
	while(1)
	{
		if(strcmp(tmp,"")==0)
		{
			struct msgbuf msgr;
			int retval;
			retval = msgrcv(msgid,&msgr,512,123,0);
			if(retval < 0)
			{
				perror("msgrcv");
			}
			if(strncmp("init",msgr.mtext,4)==0)
			{
				char y[50],text[512],z[50];
				strcpy(text,msgr.mtext);
				int ind;
				for(int i=5;i<strlen(text);i++)
				{
					if(text[i]=='|')
					{
						ind=i+1;
						break;
					}	
					y[i-5]=text[i];
				}
				int cid = atoi(y);

				for(int i=ind;i<strlen(text);i++)
				{
					z[i-ind]=text[i];
				}
				int gid = atoi(z);

				printf("A new client with cid %d has requested to get added to group %d\n",cid,gid);

				group[gid][tot[gid]] = cid;
				tot[gid]++;

				strcpy(tmp,"");
			}
			else
			{
				printf("The msg received by the server is : %s\n", msgr.mtext);
				strcpy(tmp,msgr.mtext);
			}
		}
	}
}

void* snd(void* arg)
{
	while(1)
	{
		struct msgbuf msgs;
		int retval;
		if(strlen(tmp)>0)
		{
			char y[50]="",frwd[512]="";
			int q=0;
			for(int i=0;i<strlen(tmp);i++)
			{
				if(q==0)
					y[i]=tmp[i];
				else
					frwd[i-q]=tmp[i];
				if(tmp[i]=='|')
					q=i+1;
			}
			strcpy(msgs.mtext,frwd);
			int k=atoi(y);
			int grpid,qf=0;
			for(int i=0;i<50;i++)
			{
				for(int j=0;j<50;j++)
				{
					if(group[i][j]==k)
					{
						grpid = i;
						qf=1;
						break;
					}
					if(group[i][j]==0)
						break;
				}
				if(qf)
					break;
			}
			for(int i=0;i<tot[grpid];i++)
			{
				if(group[grpid][i]==k)
					continue;
				msgs.mtype = group[grpid][i];
				retval = msgsnd(msgid,&msgs,512,0);
			}
			if(retval < 0)
			{
				perror("msgsnd");
			}
			strcpy(tmp,"");
		}
	}
}


int main()
{
	key_t key = ftok("key.txt",23);
	msgid = msgget(key,0666|IPC_CREAT);

	if(msgid < 0)
	{
		perror("msgget");
	}
	
	pthread_t p1,p2;
	pthread_create(&p1,NULL,rcv,NULL);
	pthread_create(&p2,NULL,snd,NULL);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
}