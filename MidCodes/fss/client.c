#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>


// Use exit only in the end when execution is finished, as server sends signal to existing pid.
// No more services can be added once exit is used in client

#define PORT 8010
int usfd;
struct sockaddr_in uservaddr;

void sig_handler()
{
	char buff[50];
	memset((char *)buff,0,sizeof(buff));
	strcat(buff,"enquiry");

	if(sendto(usfd,buff,sizeof(buff),0,(struct sockaddr *)&uservaddr,sizeof(uservaddr))<0)
	{
		perror("sendto");
		exit(0);
	}

	int sz = sizeof(uservaddr);
	if(recvfrom(usfd,buff,sizeof(buff),0,(struct sockaddr *)&uservaddr,&sz)<0)
	{
		perror("recvfrom");
		exit(0);
	}
	else
		printf("\nOne new service added. The available services are : %s\n", buff);
}

void func(int sfd)
{
	char buff[50];
	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		printf("Enter the msg to be sent to Server! : ");
		scanf("%[^\n]s",buff);
		while(getchar()!='\n');

		if(strcmp(buff,"enquiry")==0)
		{
			if(sendto(usfd,buff,sizeof(buff),0,(struct sockaddr *)&uservaddr,sizeof(uservaddr))<0)
			{
				perror("sendto");
				exit(0);
			}
			else
				printf("Client has sent the Request!\n");

			int sz = sizeof(uservaddr);
			if(recvfrom(usfd,buff,sizeof(buff),0,(struct sockaddr *)&uservaddr,&sz)<0)
			{
				perror("recvfrom");
				exit(0);
			}
			else
				printf("The available services are : %s\n", buff);
		}
		else
		{
			send(sfd,buff,sizeof(buff),0);
			printf("Client has sent a msg to Server!\n");

			if(strcmp(buff,"exit")==0)
				return;

			memset((char *)buff,0,sizeof(buff));
			recv(sfd,buff,sizeof(buff),0);
			printf("Received from Server : %s\n", buff);
		}
	}
}

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		printf("Not enough arguments!\n");
		exit(0);
	}

	signal(SIGUSR2,sig_handler);



	// for enquiry

	int pid;
	char spid[50];
	memset(&spid,0,sizeof(spid));
	pid = getpid();

	printf("My pid is : %d \n\n", pid);

	sprintf(spid,"%d",pid);

	usfd = socket(AF_INET,SOCK_DGRAM,0);
	if(usfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Enquiry Socket successful in Client!\n");

	memset((struct sockaddr_in *)&uservaddr,0,sizeof(uservaddr));
	
	uservaddr.sin_family = AF_INET;
	uservaddr.sin_port = htons(PORT);
	uservaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	char buff[50];
	memset((char *)buff,0,sizeof(buff));

	printf("Sending Enquiry Message!\n");
	strcpy(buff,"enquiry");
	strcat(buff,"|");
	strcat(buff,spid);

	if(sendto(usfd,buff,sizeof(buff),0,(struct sockaddr *)&uservaddr,sizeof(uservaddr))<0)
	{
		perror("sendto");
		exit(0);
	}
	else
		printf("Client has sent the Request!\n");

	int sz = sizeof(uservaddr);
	if(recvfrom(usfd,buff,sizeof(buff),0,(struct sockaddr *)&uservaddr,&sz)<0)
	{
		perror("recvfrom");
		exit(0);
	}
	else
		printf("The available services are : %s\n", buff);

	int sfd,port;
	port = atoi(argv[1]);
	struct sockaddr_in servaddr,cliaddr;

	memset((struct sockaddr_in *)&servaddr,0,sizeof(servaddr));

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket successful in Client!\n");

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connect Successful!\n");

	func(sfd);

	close(sfd);
}