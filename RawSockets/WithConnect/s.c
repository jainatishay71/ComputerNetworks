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

//need to use sudo for raw socket to work
int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("Enter protocol in arguments!\n");
		exit(0);
	}

	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));
	if(rsfd==-1)
	{
		perror("socket");
		exit(0);
	}

	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	printf("Opt\n");

	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=htonl(INADDR_LOOPBACK);

	char buff[]="hello";	

	unsigned int client_len=sizeof(client);
	printf("Sending\n");

	if(connect(rsfd,(struct sockaddr *)&client,sizeof(client)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connected Successfully!\n");

	if(write(rsfd,buff,sizeof(buff)) < 0)
	{
		perror("write");
		exit(0);
	}
	else
		printf("Written Successfully!\n");
}
