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

int main()
{
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

	char buff[1024],ticket[1024];
	memset(&buff,0,sizeof(buff));
	memset(&ticket,0,sizeof(ticket));
	strcpy(buff,"hello");
	sz = sizeof(servaddr1);
	sendto(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&servaddr1,sz);
	memset(&buff,0,sizeof(buff));
	recvfrom(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,&sz);
	printf("\n%s\n", buff);
	memset(&buff,0,sizeof(buff));
	scanf("%s",buff);
	sendto(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&servaddr1,sz);
	memset(&buff,0,sizeof(buff));
	recvfrom(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,&sz);
	printf("Ticket is : %s\n", buff);
	strcpy(ticket,buff);






	int sfd2,sz2;
	sfd2 = socket(AF_INET,SOCK_STREAM,0);
	if(sfd2 < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("sfd2 created successfully\n");

	int temp = 1;
	setsockopt(sfd2,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));

	struct sockaddr_in servaddr2,cliaddr2;

	servaddr2.sin_family = AF_INET;
	servaddr2.sin_port = htons(BPORT);
	servaddr2.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sfd2,(struct sockaddr *)&servaddr2,sizeof(servaddr2)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connected to Boarding Pass\n");

	sz2 = sizeof(cliaddr2);
	getsockname(sfd2,(struct sockaddr *)&cliaddr2,&sz2);
	printf("My Values are %s:%d\n", inet_ntoa(cliaddr2.sin_addr),ntohs(cliaddr2.sin_port));
	int fport = ntohs(cliaddr2.sin_port);

	if(send(sfd2,ticket,sizeof(ticket),0) < 0)
	{
		perror("send");
		exit(0);
	}
	else
		printf("ticket sent!\n");

	char sno[40];
	memset(&sno,0,sizeof(40));
	if(recv(sfd2,sno,sizeof(sno),0) < 0)
	{
		perror("recv");
		exit(0);
	}
	else
		printf("Alloted Seat No is : %s\n", sno);




	int sfd3,nsfd3,sz3;
	sfd3 = socket(AF_INET,SOCK_STREAM,0);
	if(sfd3 < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("sfd3 created successfully\n");

	temp = 1;
	setsockopt(sfd3,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp));

	struct sockaddr_in servaddr3,cliaddr3;

	servaddr3.sin_family = AF_INET;
	servaddr3.sin_port = htons(fport);
	servaddr3.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(sfd3,(struct sockaddr *)&servaddr3,sizeof(servaddr3)) < 0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("sfd3 binded successfully\n");

	if(listen(sfd3,5) < 0)
	{
		perror("listen");
		exit(0);
	}
	else
		printf("sfd3 listening..!\n");

	sz3 = sizeof(cliaddr3);
	nsfd3 = accept(sfd3,(struct sockaddr *)&cliaddr3,&sz3);
	if(nsfd3 < 0)
	{
		perror("accept");
		exit(0);
	}
	else
		printf("Accepted by passenger\n");

	printf("sno is : %s\n", sno);
	
	if(send(nsfd3,sno,sizeof(sno),0) < 0)
	{
		perror("send");
		exit(0);
	}
	else
		printf("Seat no sent to airways!\n");
}				