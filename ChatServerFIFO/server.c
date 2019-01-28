#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define PATH "SFIFO"

int tot=0;

void func()
{
	printf("A FIFO has been closed\n");
	tot--;
}

int main(int argc,char* argv[])
{
	signal(SIGPIPE,func);
	if(argc < 2)
	{
		printf("Not enough arguments!\n");
		exit(1);
	}
	int fd,noc;
	noc = atoi(argv[1]);
	if(mkfifo(PATH,0666)!=0)
	{
		perror("mkfifo");
	}
	int cnt=0;
	fd = open(PATH,O_RDONLY);
	char clients[noc][400];
	int cfd[noc];
	while(1)
	{
		char buf[400];
		read(fd,buf,sizeof(buf));
		if(buf[strlen(buf)-1]=='|' && buf[strlen(buf)-2]=='|')
		{
			char *buff;
			buff = malloc(sizeof(char)*400);
			for(int i=0;i<strlen(buf);i++)
			{
				if(buf[i] == '|')
					break;
				char tmp[2];
				tmp[0] = buf[i];
				tmp[1] = '\0';
				strcat(buff,tmp);
			}
			mkfifo(buff,0666);
			printf("A client added successfully!\n");
			strcpy(clients[cnt],buff);
			cfd[cnt] = open(buff,O_WRONLY);
			int jo = write(cfd[cnt],"hello",5);
			if(jo < 0)
			{
				perror("write");
			}
			cnt++;tot++;
		}
		else
		{
			int u=0;
			char g[400]="",e[400]="";
			for(int i=0;i<strlen(buf);i++)
			{
				if(buf[i]=='|')
				{
					u=i;
					break;
				}
				char tmp[2];
				tmp[0] = buf[i];
				tmp[1] = '\0';
				strcat(g,tmp);
			}
			g[strlen(g)]='\0';
			for(int i=u+1;i<strlen(buf);i++)
			{
				char tmp[2];
				tmp[0] = buf[i];
				tmp[1] = '\0';
				strcat(e,tmp);
			}
			e[strlen(e)]='\0';
			for(int i=0;i<cnt;i++)
			{
				if(strcmp(clients[i],g)==0)
				{
					continue;
				}
				int top = open(clients[i],O_RDWR|O_NONBLOCK);
				if(top < 0)
				{
					perror("open");
					continue;
				}
				int jo = write(top,e,sizeof(e));
				if(jo < 0)
				{
					perror("write");
					tot--;
					printf("top is %d\n", top);
				}
			}
		}
		if(tot==0)
			break;
	}
}