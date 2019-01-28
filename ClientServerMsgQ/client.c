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


int msgid;

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
		retval = msgrcv(msgid,&msgr,512,2,0);
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
		msgr.mtype = 1;
		char tin[512];
		scanf("%s",tin);
		strcpy(msgr.mtext,tin);
		retval = msgsnd(msgid,&msgr,512,0);
		if(retval < 0)
		{
			perror("msgsnd");
		}
		printf("The msg has been sent from client\n");
	}
}

int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("Not Enough Arguments!\n");
		exit(1);
	}

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