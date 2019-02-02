#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <sys/msg.h>

// P3 sends messages only to P2 and receives only from P1

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
	}
}

int main(int argc,char* argv[])
{
	key_t key = ftok("key.txt",37);
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