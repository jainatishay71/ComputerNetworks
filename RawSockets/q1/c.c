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
	socklen_t clilen=sizeof(client);
	printf("receive\n");

	if(recvfrom(rsfd,buf,BUF_LEN,0,(struct sockaddr*)&client,( socklen_t*)&clilen)<0)
	{
		perror("recv");
		exit(0);
	}
	struct iphdr *ip;
  	ip=(struct iphdr*)buf;
	printf("%s\n", (buf+(ip->ihl)*4) );

}
