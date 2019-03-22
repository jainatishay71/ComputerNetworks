#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

int pid,msgid;

struct msgbuf
{
	long mtype;
	char data[512];
};

void sig_handler()
{
	struct msgbuf msgr;
	memset(&msgr,0,sizeof(msgr));

	if(msgrcv(msgid,&msgr,512,pid,0) < 0)
	{
		perror("msgrcv");
		exit(0);
	}
	else
		printf("\nMsg Received is : %s\n", msgr.data);
}

int main()
{
	signal(SIGUSR1,sig_handler);

	key_t key = ftok("forkey.txt",41);
	if(key < 0)
	{
		perror("ftok");
		exit(0);
	}

	msgid = msgget(key,IPC_CREAT|0666);
	if(msgid < 0)
	{
		perror("msgget");
		exit(0);
	}

	pid = getpid();
	printf("My pid is : %d\n", pid);

	char spid[50],first[100];
	strcpy(spid,"");
	strcpy(first,"init|");
	sprintf(spid,"%d",pid);
	strcat(first,spid);

	struct msgbuf msg;
	msg.mtype = 2;
	strcpy(msg.data,first);

	if(msgsnd(msgid,&msg,512,0) < 0)
	{
		perror("msgsnd");
		exit(0);
	}

	for(;;)	
	{
		printf("Enter the msg to be sent : ");
		char msgs[512];
		strcpy(msgs,"");
		scanf("%[^\n]s",msgs);
		while(getchar()!='\n');

		memset(&msg,0,sizeof(msg));
		msg.mtype = 2;
		strcpy(msg.data,msgs);
		strcat(msg.data,"|");
		strcat(msg.data,spid);

		if(msgsnd(msgid,&msg,512,0) < 0)
		{
			perror("msgsnd");
			exit(0);
		}
	}
}