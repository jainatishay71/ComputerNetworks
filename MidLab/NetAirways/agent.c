#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/msg.h>

#define APORT 8010

int main()
{
	srand(time(NULL));

	char dbtick[50][1024];
	int cnt=0;

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

	if(bind(sfd1,(struct sockaddr *)&servaddr1,sizeof(servaddr1)) < 0)
	{
		perror("bind");
		exit(0);
	}
	else
		printf("sfd1 binded successfully\n");

	for(;;)
	{
		char buff[1024];
		memset(&buff,0,sizeof(buff));
		memset(&cliaddr1,0,sizeof(cliaddr1));
		sz = sizeof(cliaddr1);
		recvfrom(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,&sz);
		if(strcmp(buff,"hello")==0)
		{
			memset(&buff,0,sizeof(buff));
			strcpy(buff,"Available options are: 1)Jet Airways	2)Indian Airlines");
			sendto(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,sz);	
			memset(&buff,0,sizeof(buff));
			recvfrom(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,&sz);
			if(strcmp(buff,"1")==0)
			{
				char tick[1024];
				memset(&tick,0,sizeof(tick));
				strcpy(tick,"JA");
				int r = rand()%10000+1;
				char dum[1024];
				memset(&dum,0,sizeof(dum));
				sprintf(dum,"%d",r);
				strcat(tick,dum);
				sendto(sfd1,tick,sizeof(tick),0,(struct sockaddr *)&cliaddr1,sz);	
				printf("A new ticket is generated : %s\n", tick);
				strcpy(dbtick[cnt++],tick);
			}
			else if(strcmp(buff,"2")==0)
			{
				char tick[1024];
				memset(&tick,0,sizeof(tick));
				strcpy(tick,"IA");
				int r = rand()%10000+1;
				char dum[1024];
				memset(&dum,0,sizeof(dum));
				sprintf(dum,"%d",r);
				strcat(tick,dum);
				sendto(sfd1,tick,sizeof(tick),0,(struct sockaddr *)&cliaddr1,sz);	
				printf("A new ticket is generated : %s\n", tick);
				strcpy(dbtick[cnt++],tick);
			}
		}
		else if(strcmp(buff,"enquiry")==0)
		{
			memset(&buff,0,sizeof(buff));
			recvfrom(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,&sz);
			int flag = 0;
			for(int i=0;i<cnt;i++)
			{
				if(strcmp(dbtick[i],buff)==0)
				{
					flag = 1;
					break;
				}
			}
			if(flag)
			{
				memset(&buff,0,sizeof(buff));
				strcpy(buff,"correct");
				sendto(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,sz);	
			}
			else
			{
				memset(&buff,0,sizeof(buff));
				strcpy(buff,"incorrect");
				sendto(sfd1,buff,sizeof(buff),0,(struct sockaddr *)&cliaddr1,sz);
			}
		}
	}

}				