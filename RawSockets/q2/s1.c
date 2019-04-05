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

#define BACKLOG 5


void complement(int nsfd)
{
	char buff[50];

	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		recv(nsfd,buff,sizeof(buff),0);
		printf("The Server has received : %s\n", buff);
		if(strcmp(buff,"exit")==0)
			return;
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='a' && buff[i]<='z')
			{
				buff[i] = buff[i]-'a';
				buff[i] = buff[i]+'A';
			}
			else if(buff[i]>='A' && buff[i]<='Z')
			{
				buff[i] = buff[i]-'A';
				buff[i] = buff[i]+'a';
			}
		}
		send(nsfd,buff,sizeof(buff),0);
	}
}

void uppercase(int nsfd)
{
	char buff[50];

	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		recv(nsfd,buff,sizeof(buff),0);
		printf("The Server has received : %s\n", buff);
		if(strcmp(buff,"exit")==0)
			return;
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='a' && buff[i]<='z')
			{
				buff[i] = buff[i]-'a';
				buff[i] = buff[i]+'A';
			}
		}
		send(nsfd,buff,sizeof(buff),0);
	}
}

void lowercase(int nsfd)
{
	char buff[50];

	for(;;)
	{
		memset((char *)buff,0,sizeof(buff));
		recv(nsfd,buff,sizeof(buff),0);
		printf("The Server has received : %s\n", buff);
		if(strcmp(buff,"exit")==0)
			return;
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='A' && buff[i]<='Z')
			{
				buff[i] = buff[i]-'A';
				buff[i] = buff[i]+'a';
			}
		}
		send(nsfd,buff,sizeof(buff),0);
	}
}

int main(int argc,char *argv[])
{
	if(argc < 3)
	{
		printf("Not Enough Arguments!\n");
		exit(0);
	}

	int tmp = atoi(argv[2]);
	int port = 8000 + tmp;
	char services[][512] = {"Uppercase : Use Port 8001" , "Lowercase : Use Port 8002" , "Complement : Use Port 8003"};

	int sfd,nsfd,szz;
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

	if(bind(sfd,(struct sockaddr *)&tservaddr,sizeof(tservaddr))!=0)
	{
		perror("bind");
		exit(0);
	}

	if(listen(sfd,BACKLOG)!=0)
	{
		perror("listen");
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

	struct sockaddr_in servaddr,cliaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.2");

	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(rsfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("Binded Successfully!\n");

	char buff[512],store[512];
	int sz = sizeof(cliaddr);
	memset(&buff,0,sizeof(buff));
	memset(&store,0,sizeof(store));
	if(recvfrom(rsfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,&sz) < 0)
	{
		perror("recvfrom");
		exit(0);
	}
	struct iphdr *ip;
	ip = (struct iphdr *)buff;
	strcpy(store,(buff+(ip->ihl)*4));
	if(strcmp(store,"enquiry")==0)
	{
		memset(&buff,0,sizeof(buff));
		strcpy(buff,services[tmp-1]);
		if(sendto(rsfd,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr)) < 0)
		{
			perror("sendto");
			exit(0);
		}
	}

	memset((struct sockaddr_in *)&tcliaddr,0,sizeof(tcliaddr));

	szz = sizeof(tcliaddr);
	nsfd = accept(sfd,(struct sockaddr *)&tcliaddr,&szz);
	if(nsfd < 0)
	{
		perror("accept");
		exit(0);
	}
	else
		printf("Server Acception Successful\n");

	if(tmp == 1)
		uppercase(nsfd);
	else if(tmp == 2)
		lowercase(nsfd);
	else
		complement(nsfd);

}