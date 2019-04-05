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

#define PORT 8000
#define BACKLOG 5

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		printf("Not Enough Arguments!\n");
		exit(0);
	}

	int sfd,nsfd,sz;
	struct sockaddr_in servaddr,cliaddr;

	memset((struct sockaddr_in *)&servaddr,0,sizeof(servaddr));

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket Created Successfully!\n");

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(bind(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("Socket Binded Successfully!\n");

	if(listen(sfd,BACKLOG)!=0)
	{
		perror("listen");
		exit(0);
	}
	else
		printf("Socket Listening..!\n");

	nsfd = accept(sfd,NULL,NULL);
	if(nsfd < 0)
	{
		perror("accept");
		exit(0);
	}
	else
		printf("Server Acception Successful\n");

	char buff[512];
	memset(&buff,0,sizeof(buff));
	if(recv(nsfd,buff,sizeof(buff),0) < 0)
	{
		perror("recv");
		exit(0);
	}

	char file[512],ip[512];
	memset(&file,0,sizeof(file));
	memset(&ip,0,sizeof(ip));

	int x=0;
	for(int i=0;i<strlen(buff);i++)
	{
		if(buff[i]=='|')
		{
			x=1;
			continue;
		}
		if(x==0)
		{
			char tmp[2];
			tmp[0]=buff[i];
			tmp[1]='\0';
			strcat(file,tmp);
		}
		else
		{
			char tmp[2];
			tmp[0]=buff[i];
			tmp[1]='\0';
			strcat(ip,tmp);
		}
	}

	printf("%s %s\n", file,ip);

	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd < 0)
	{
		perror("socket");
		exit(0);
	}

	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL

	cliaddr.sin_family=AF_INET;
	cliaddr.sin_addr.s_addr=inet_addr(ip);


	if(connect(rsfd,(struct sockaddr *)&cliaddr,sizeof(cliaddr)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connected!\n");


	FILE *fp = fopen(file,"r");
	if(fp == NULL)
	{
		perror("fopen");
		exit(0);
	}
	int fd = fileno(fp);
	
	if(dup2(fd,0) < 0)
	{
		perror("dup2");
		exit(0);
	}

	if(dup2(rsfd,1) < 0)
	{
		perror("dup2");
		exit(0);
	}


	int c;
	c = fork();
	if(c > 0)
	{
		close(nsfd);
	}
	else
	{
		execlp("/home/atishay/Profile/two/CN/RawSocket/q1/s2","s2",NULL);
	}

	close(sfd);
}