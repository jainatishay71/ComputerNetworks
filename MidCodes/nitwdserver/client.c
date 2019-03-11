#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("Not Enough Arguments!\n");
		exit(0);
	}
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket created successfully!\n");

	struct sockaddr_in servaddr;
	int port = atoi(argv[1]);

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connection successful!\n");

	for(;;)
	{
		char buff[500],tbuff[500];
		memset(&buff,0,sizeof(buff));
		printf("Enter a string : \n");
		scanf("%[^\n]s",buff);
		while(getchar()!='\n');
		send(sfd,buff,sizeof(buff),0);
		printf("String sent successfully!\n");
		if(strcmp(buff,"bye")==0)
			return 0;
		memset(&tbuff,0,sizeof(tbuff));
		recv(sfd,tbuff,sizeof(tbuff),0);
		printf("Result is : %s\n", tbuff);
	}
}