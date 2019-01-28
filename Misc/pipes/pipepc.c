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
	int pp[2],c,status;
	if(pipe(pp) == -1)
	{
		perror("pipe");
		exit(1);
	}
	c = fork();
	if(c == -1)
	{
		perror("fork");
		exit(1);
	}
	if(c > 0)
	{
		// sleep(5);
		close(pp[0]);
		write(pp[1],argv[1],strlen(argv[1]));
		printf("Parent has finished writing in the pipe!\n");
		wait(&status);
	}
	else
	{
		close(pp[1]);
		char buff[20];
		int r = read(pp[0],buff,200);
		buff[r] = '\0';
		printf("Child has finished reading from the pipe!\n");
		printf("\nRead content is :%s\n",buff);
	}
	return 0;
}