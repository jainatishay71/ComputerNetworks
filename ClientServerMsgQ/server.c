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

struct msgbuf
{
	long mtype;
	char mtext[512];
};

void* rcv(void* arg)
{
	while(1)
	{
		struct msgbuf msgr;
		int retval;
		retval = msgrcv(msgid,&msgr,512,1,0);
		if(retval < 0)
		{
			perror("msgrcv");
		}
		if(strcmp(msgr.mtext,"init")==0)
		{
			printf("A new client has been added\n");
			tot++;
		}
		else
		{
			printf("The msg received by the server is : %s\n", msgr.mtext);
		}
	}
}

void* snd(void* arg)
{
	while(1)
	{
		struct msgbuf msgr;
		int retval;
		msgr.mtype = 2;
		char tin[512];
		scanf("%s",tin);
		strcpy(msgr.mtext,tin);
		for(int i=1;i<=tot;i++)
		{
			retval = msgsnd(msgid,&msgr,512,0);
		}
		if(retval < 0)
		{
			perror("msgsnd");
		}
		printf("The msg has been sent from server\n");
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