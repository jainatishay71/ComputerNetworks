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

#define PORT 8010
#define MAX 15
#define BACKLOG 5
char services[512];

struct param
{
	char portno[10];
	char path[50];
};

struct param one;

int main()
{
	strcpy(services,"");
	int arrsfd[MAX],cnt=0,clicnt=0;
	int cpid[MAX],clipid[MAX];
	arrsfd[cnt] = STDIN_FILENO;
    cpid[cnt] = 0;
    cnt++;

	arrsfd[cnt] = socket(AF_INET,SOCK_DGRAM,0);
    cpid[cnt] = 0;
	if(arrsfd[cnt] < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket Created Successfully!\n");

    struct sockaddr_in servaddr[MAX];
    struct sockaddr_in cliaddr;
    servaddr[cnt].sin_family = AF_INET;
    servaddr[cnt].sin_port = htons(PORT);
    servaddr[cnt].sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(bind(arrsfd[cnt],(struct sockaddr *)&servaddr[cnt],sizeof(servaddr[cnt])) < 0)
    {
    	perror("bind");
    	exit(0);
    }
    else
    	printf("Socket Binded Successfully!\n");

    cnt++;

    for(;;)
    {
    	int maxfd=-1;
    	fd_set readfds;
    	FD_ZERO(&readfds);
    	for(int i=0;i<cnt;i++)
    	{
    		FD_SET(arrsfd[i],&readfds);
    		if(maxfd<arrsfd[i])
    			maxfd=arrsfd[i];
    	}
    	int sret;
    	sret = select(maxfd+1,&readfds,NULL,NULL,NULL);
    	if(sret < 0)
    	{
    		perror("select");
    		exit(0);
    	}
    	if(sret > 0)
    	{
    		if(FD_ISSET(arrsfd[0],&readfds))
    		{
                char buff[512];
                memset(&buff,0,sizeof(buff));
                scanf("%[^\n]s",buff);
                while(getchar()!='\n');
                memset(&one,0,sizeof(one));
                int q=0,u=0;
                for(int i=0;i<strlen(buff);i++)
                {
                    if(buff[i]==' ' && q==0 && u==0)
                    {
                        q=1;
                        continue;
                    }
                    if(buff[i]==' ' && q==1 && u==0)
                    {
                        u=1;
                        continue;
                    }
                    if(q==0 && u==0)
                    {
                        char tmp[2];
                        tmp[0]=buff[i];
                        tmp[1]='\0';
                        strcat(one.portno,tmp);
                    }
                    if(q==1 && u==0)
                    {
                        char tmp[2];
                        tmp[0]=buff[i];
                        tmp[1]='\0';
                        strcat(one.path,tmp);
                    }
                }
                strcat(services,one.portno);
                strcat(services,",");

    			int tport = atoi(one.portno),tsfd;
    			arrsfd[cnt] = socket(AF_INET,SOCK_STREAM,0);
    			tsfd = arrsfd[cnt];
    			if(arrsfd[cnt] < 0)
    			{
    				perror("socket");
    				exit(0);
    			}

    			servaddr[cnt].sin_family = AF_INET;
			    servaddr[cnt].sin_port = htons(tport);
			    servaddr[cnt].sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    			if(bind(arrsfd[cnt],(struct sockaddr *)&servaddr[cnt],sizeof(servaddr[cnt])) < 0)
    			{
    				perror("bind");
    				exit(0);
    			}

    			if(listen(arrsfd[cnt],BACKLOG)!=0)
				{
					perror("listen");
					exit(0);
				}

    			int c = fork();
    			if(c == 0)
    			{
    				char *argv[] = {"hello",NULL};
    				char tsfds[50];
    				sprintf(tsfds,"%d",tsfd);
    				argv[0] = tsfds;
    				if(execv(one.path,argv) < 0)
    				{
    					perror("execv");
    					exit(0);
    				}
    			}
    			else
    			{
    				cpid[cnt] = c;
    				cnt++;
                    for(int i=0;i<clicnt;i++)
                    {
                        if(kill(clipid[i],SIGUSR2) < 0)
                        {
                            perror("kill");
                            exit(0);
                        }
                    }
    			}
    		}
    		
    		// for enquiry

    		else if(FD_ISSET(arrsfd[1],&readfds))
    		{
    			char buff[50];
				memset((char *)buff,0,sizeof(buff));

				int sz = sizeof(cliaddr);
				if(recvfrom(arrsfd[1],buff,sizeof(buff),0,(struct sockaddr *)&cliaddr,&sz) < 0)
				{
					perror("recvfrom");
					exit(0);
				}
				else
					printf("The Server has received : %s\n", buff);

                char spid[50];
                strcpy(spid,"");
                int q=0;
                for(int i=0;i<strlen(buff);i++)
                {
                    if(buff[i]=='|')
                    {
                        q=1;
                        continue;
                    }
                    if(q==1)
                    {
                        char tmp[2];
                        tmp[0]=buff[i];
                        tmp[1]='\0';
                        strcat(spid,tmp);
                    }
                }
                if(strcmp(spid,"")!=0)
                    clipid[clicnt++] = atoi(spid);

				if(sendto(arrsfd[1],services,sizeof(services),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr)) < 0)
				{
					perror("sendto");
					exit(0);
				}
				else
					printf("Enquiry Served!\n\n");
    		}

            else
            {
                for(int i=2;i<cnt;i++)
                {
                    if(FD_ISSET(arrsfd[i],&readfds))
                    {
                        kill(cpid[i],SIGUSR1);
                        break;
                    }
                }
            }

    	}
    }
}