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

#define APORT 8010
#define BPORT 8020
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




	int sc = 42;



	key_t key = ftok("forkey.txt",41);
	int msgid;
	msgid = msgget(key,IPC_CREAT|0666);





	int sfd1,sz;
	sfd1 = socket(AF_INET,SOCK_DGRAM,0);
	if(sfd1 < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("sfd1 created successfully\n");

	struct sockaddr_in servaddr1,cliaddr1;

	servaddr1.sin_family = AF_INET;
	servaddr1.sin_port = htons(APORT);
	servaddr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);







	int sfd2,nsfd2;
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
	servaddr2.sin_port = htons(BPORT);
	servaddr2.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(sfd2,(struct sockaddr *)&servaddr2,sizeof(servaddr2)) < 0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("sfd2 binded!\n");

	if(listen(sfd2,5) < 0)
	{
		perror("listen");
		exit(0);
	}
	else
		printf("sfd2 listening..!\n");

	for(;;)
	{
		memset(&cliaddr2,0,sizeof(cliaddr2));
		sz = sizeof(cliaddr2);
		nsfd2 = accept(sfd2,(struct sockaddr *)&cliaddr2,&sz);
		if(nsfd2 < 0)
		{
			perror("accept");
			exit(0);
		}
		else
			printf("accepted!\n");

		char ticket[1024];
		memset(&ticket,0,sizeof(ticket));
		if(recv(nsfd2,ticket,sizeof(ticket),0) < 0)
		{
			perror("recv");
			exit(0);
		}
		else
			printf("checking from agent..!\n");

		char abuff[1024];
		memset(&abuff,0,sizeof(abuff));
		strcpy(abuff,"enquiry");
		sz = sizeof(servaddr1);
		sendto(sfd1,abuff,sizeof(abuff),0,(struct sockaddr *)&servaddr1,sz);
		sendto(sfd1,ticket,sizeof(ticket),0,(struct sockaddr *)&servaddr1,sz);
		memset(&abuff,0,sizeof(abuff));
		recvfrom(sfd1,abuff,sizeof(abuff),0,(struct sockaddr *)&servaddr1,&sz);
		if(strcmp(abuff,"correct")==0)
		{
			printf("ticket verified\n");


			if(sendto(sfd3,ticket,sizeof(ticket),0,(struct sockaddr *)&servaddr3,sizeof(servaddr3)) < 0)
			{
				perror("sendto");
				exit(0);
			}



			char sno[40];
			memset(&sno,0,sizeof(sno));
			sprintf(sno,"%d",sc);
			sc++;
			if(send(nsfd2,sno,sizeof(sno),0) < 0)
			{
				perror("send");
				exit(0);
			}
			else
				printf("Seat no sent %s alloted to the passenger having ticket %s\n", sno,ticket);




			struct msgbuf msg;
			msg.mtype = 2;
			if(ticket[0]=='I')
				msg.mtype = 3;
			msg.data = cliaddr2;

			if(msgsnd(msgid,&msg,sizeof(msg.data),0) < 0)
			{
				perror("msgsnd");
				exit(0);
			}
			else
				printf("msg sent!\n");
		}
		else
		{
			printf("ticket does not exist\n");
		}
	}
	
}				