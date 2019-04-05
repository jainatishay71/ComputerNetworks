#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>

#define BUF_LEN 100

int main(int argc, char** argv)
{
	
	if(argc<2)
	{
		printf("Enter protocol in arguments\n");
		exit(0);
	}

	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));

	if(rsfd==-1)
	{
		perror("socket");
		exit(0);
	}

	char buf[BUF_LEN];
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	socklen_t clilen=sizeof(client);

	if(bind(rsfd,(struct sockaddr *)&client,sizeof(client)) < 0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("Binded\n");

	if(read(rsfd,buf,sizeof(buf)) < 0)
	{
		perror("read");
		exit(0);
	}
	
	struct iphdr *ip;
  	ip=(struct iphdr*)buf;
	printf("%s\n", (buf+(ip->ihl)*4) );

}
