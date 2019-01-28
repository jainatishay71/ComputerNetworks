#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define PATH "SFIFO"

char spp[400];

void* fun1(void* arg)
{
	char rcv[400];
	int fdd = open(spp,O_RDONLY);
	if(fdd < 0)
	{
		perror("open");
	}
	while(1)
	{
		int jo = read(fdd,rcv,sizeof(rcv));
		if(jo < 0)
		{
			perror("write");
		}
		if(strlen(rcv)>0)
			printf("%s\n", rcv);
		sleep(1);
	}
}

void* fun2(void* arg)
{
	int fd = open(PATH,O_WRONLY);
	while(1)
	{
		char in[50];
		scanf("%s",in);
		char snd[400];
		strcpy(snd,spp);
		strcat(snd,"|");
		strcat(snd,in);
		if(strlen(in)>0)
		{	int jo = write(fd,snd,sizeof(snd));
			if(jo < 0)
			{
				perror("write");
			}
		}
		sleep(1);
	}
}

char *itoa(int n)
{
	char *b,*h;
	b = malloc(sizeof(char)*400);
	h = malloc(sizeof(char)*400);
	while(n)
	{
		int r = n%10;
		char tmp[2];
		tmp[0] = r+48;
		tmp[1] = '\0';
		strcat(b,tmp);
		n/=10;
	}
	for(int i=strlen(b)-1;i>=0;i--)
	{
		char tmp[2];
		tmp[0] = b[i];
		tmp[1] = '\0';
		strcat(h,tmp);
	}
	return h;
}

int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("Not enough arguments!\n");
		exit(1);
	}
	int fd,retval;
	fd = open(PATH,O_WRONLY);
	int p = getpid();
	char *sp;
	sp = malloc(sizeof(char)*400);
	sp = itoa(p);
	strcpy(spp,sp);
	printf("%s\n\n", spp);
	strcat(sp,"||");
	int jo = write(fd,sp,sizeof(sp));
	if(jo < 0)
	{
		perror("write");
	}

	mkfifo(spp,0666);

	pthread_t p1,p2;

	pthread_create(&p1,NULL,&fun1,NULL);
	pthread_create(&p2,NULL,&fun2,NULL);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
}