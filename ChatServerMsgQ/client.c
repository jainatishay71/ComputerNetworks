#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/msg.h>


// Client must send its first msg as "init" to get added to server!


int msgid,id;
char iden[50];

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
		retval = msgrcv(msgid,&msgr,512,id,0);
		if(retval < 0)
		{
			perror("msgrcv");
		}
		printf("The msg received by the client is : %s\n", msgr.mtext);
	}
}

void* snd(void* arg)
{
	while(1)
	{
		struct msgbuf msgr;
		int retval;
		msgr.mtype = 123;
		char tin[512];
		scanf("%s",tin);
		if(strcmp(tin,"init")==0)
		{
			strcpy(msgr.mtext,tin);
		}
		else
		{	
			strcpy(msgr.mtext,iden);
			strcat(msgr.mtext,"|");
			strcat(msgr.mtext,tin);
		}
		retval = msgsnd(msgid,&msgr,512,0);
		if(retval < 0)
		{
			perror("msgsnd");
		}
	}
}

int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("Not Enough Arguments!\n");
		exit(1);
	}
	id = atoi(argv[1]);

	strcpy(iden,argv[1]);

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