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

int main(int argc,char *argv[])
{
	int rsfd=socket(AF_INET,SOCK_RAW,atoi(argv[1]));

	if(rsfd < 0)
	{
		perror("socket");
		exit(0);
	}

	char buf[512];
	memset(&buf,0,sizeof(buf));
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(read(rsfd,buf,sizeof(buf))<0)
	{
		perror("recv");
		exit(0);
	}

	struct iphdr *ip;
  	ip=(struct iphdr*)buf;
	printf("%s\n", (buf+(ip->ihl)*4) );

}