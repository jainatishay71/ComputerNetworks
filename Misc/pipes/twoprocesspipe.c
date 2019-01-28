#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{
	if(argc < 2)
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
	int fd;
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
		printf("P1 has finished writing to pipe!\n");
	}
	else
	{
		close(pp[1]);
		printf("Inside Child!\n");
		execlp("/home/atishay/CN/Basics/pipes/P2","P2",NULL);
	}
}