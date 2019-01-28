#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

char* itoa(int n)
{
	char p[20];
	int k=0;
	while(n)
	{
		int r = n%10;
		p[k] = r+48;
		k++;
		n/=10;
	}
	char s[20];
	k=0;
	for(int i=strlen(p)-1;i>=0;i--)
	{
		s[k++] = p[i];
	}
	s[k] = '\0';
	char* send;
	send = (char*)malloc(sizeof(char)*20);
	strcpy(send,s);
	return send;
}

int main(int argc,char* argv[])
{
	if(argc < 7)
	{
		printf("Not enough arguments!\n");
		exit(1);
	}
	int pp[2],c;
	if(pipe(pp) == -1)
	{
		perror("pipe");
		exit(1);
	}
	int fd,status;
	printf("Currently in upp!\n");
	dup2(pp[1],fd);
	dup2(pp[0],0);
	c = fork();
	if(c == -1)
	{
		perror("fork");
		exit(1);
	}
	if(c > 0)
	{
		close(pp[0]);
		write(pp[1],argv[1],strlen(argv[1]));
		sleep(1);
	}
	else
	{
		close(pp[1]);
		char path[40];
		strcpy(path,"/home/atishay/Desktop/CNLab/");
		int h = (int)atoi(argv[6]);
		strcat(path,argv[h]);
		h++;
		char *obt = itoa(h);
		argv[6] = obt;
		execv(path,argv);
	}
}