#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>


void func(int sfd)
{
	char buff[50];
	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		printf("Enter the msg to be sent to Server! : ");
		scanf("%[^\n]s",buff);
		while(getchar()!='\n');
		send(sfd,buff,sizeof(buff),0);
		printf("Client has sent a msg to Server!\n");

		if(strcmp(buff,"exit")==0)
			return;

		memset((char *)buff,0,sizeof(buff));
		recv(sfd,buff,sizeof(buff),0);
		printf("Received from Server : %s\n", buff);
	}
}



int main(int argc,char *argv[])
{
	if(argc < 3)
	{
		printf("Not Enough Arguments!\n");
		exit(0);
	}


	int rsfd;
	rsfd = socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket Created Successfully!\n");

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.2");

	if(connect(rsfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connected Successfully!\n");

	char buff[512];
	int sz = sizeof(servaddr);
	memset(&buff,0,sizeof(buff));
	strcpy(buff,"enquiry");
	if(sendto(rsfd,buff,sizeof(buff),0,(struct sockaddr *)&servaddr,sz) < 0)
	{
		perror("sendto");
		exit(0);
	}

	for(int i=0;i<atoi(argv[2]);i++)
	{
		char store[512],buf[512];
		memset(&buf,0,sizeof(buf));
		memset(&store,0,sizeof(store));
		if(recvfrom(rsfd,buf,sizeof(buf),0,(struct sockaddr *)&servaddr,&sz) < 0)
		{
			perror("recvfrom");
			exit(0);
		}
		struct iphdr *ip;
		ip = (struct iphdr *)buf;
		printf("%s\n",(buf+(ip->ihl)*4));
	}

	for(;;)
	{
		printf("\nEnter the port to avail service : ");

		int port;
		scanf("%d", &port);


		int sfd;
		struct sockaddr_in tservaddr,tcliaddr;

		memset((struct sockaddr_in *)&tservaddr,0,sizeof(tservaddr));

		sfd = socket(AF_INET,SOCK_STREAM,0);
		if(sfd < 0)
		{
			perror("socket");
			exit(0);
		}

		tservaddr.sin_family = AF_INET;
		tservaddr.sin_port = htons(port);
		tservaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		if(connect(sfd,(struct sockaddr *)&tservaddr,sizeof(tservaddr))!=0)
		{
			perror("connect");
			exit(0);
		}

		func(sfd);

		close(sfd);
	}
	
}