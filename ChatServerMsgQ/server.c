#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msgid,tot=0;
char tmp[512];

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
			if(strcmp(msgr.mtext,"init")==0)
			{
				printf("A new client has been added\n");
				tot++;
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
			strcpy(msgs.mtext,tmp);
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
			for(int i=1;i<=tot;i++)
			{
				if(i==k)
					continue;
				msgs.mtype = i;
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
	key_t key = ftok("key.txt",41);
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