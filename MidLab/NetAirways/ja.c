#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/msg.h>

#define DPORT 8050

struct msgbuf
{
	long mtype;
	struct sockaddr_in data;
};

int main()
{
	int sfd3,sz3;
	sfd3 = socket(AF_INET,SOCK_DGRAM,0);
	if(sfd3 < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("sfd3 created successfully\n");

	struct sockaddr_in servaddr3,cliaddr3;

	servaddr3.sin_family = AF_INET;
	servaddr3.sin_port = htons(DPORT);
	servaddr3.sin_addr.s_addr = htonl(INADDR_LOOPBACK);


	key_t key = ftok("forkey.txt",41);
	int msgid;
	msgid = msgget(key,IPC_CREAT|0666);


	for(;;)
	{
		struct msgbuf msgr;
		if(msgrcv(msgid,&msgr,sizeof(msgr.data),2,0) < 0)
		{
			perror("msgrcv");
			exit(0);
		}
		else
			printf("msg received!\n");

		printf("%s:%d\n", inet_ntoa(msgr.data.sin_addr),ntohs(msgr.data.sin_port));
		int fport = ntohs(msgr.data.sin_port);



		int sfd2;
		sfd2 = socket(AF_INET,SOCK_STREAM,0);
		if(sfd2 < 0)
		{
			perror("socket");
			exit(0);
		}
		else
			printf("sfd2 created successfully\n");

		struct sockaddr_in servaddr2,cliaddr2;

		servaddr2.sin_family = AF_INET;
		servaddr2.sin_port = htons(fport);
		servaddr2.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		if(connect(sfd2,(struct sockaddr *)&servaddr2,sizeof(servaddr2)) < 0)
		{
			perror("connect");
			exit(0);
		}
		else
			printf("Connected to Passenger\n");

		char sno[40];
		memset(&sno,0,sizeof(40));
		if(recv(sfd2,sno,sizeof(sno),0) < 0)
		{
			perror("recv");
			exit(0);
		}
		else
		{
			printf("Alloted Seat No is : %s\n", sno);
			close(sfd2);

			if(sendto(sfd3,sno,sizeof(sno),0,(struct sockaddr *)&servaddr3,sizeof(servaddr3)) < 0)
			{
				perror("sendto");
				exit(0);
			}
		}	
	}
}