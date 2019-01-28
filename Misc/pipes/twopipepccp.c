#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{
	if(argc < 3)
	{
		printf("Not enough arguments!\n");
		exit(1);
	}
	int pp1[2],pp2[2],c,status;
	if(pipe(pp1) == -1)
	{
		perror("pipe");
		exit(1);
	}
	if(pipe(pp2) == -1)
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
		close(pp1[0]);
		write(pp1[1],argv[1],strlen(argv[1]));
		printf("Parent has finished writing in the pipe pp1!\n");
		close(pp2[1]);
		char buff[20];
		int r = read(pp2[0],buff,20);
		buff[r] = '\0';
		printf("Parent has finished reading from the pipe pp2!\n");
		printf("\nRead content is :%s \n",buff);
		wait(&status);
	}
	else
	{
		close(pp1[1]);
		char buff[20];
		int r = read(pp1[0],buff,200);
		buff[r] = '\0';
		printf("Child has finished reading from the pipe pp1!\n");
		printf("\nRead content is :%s \n",buff);
		close(pp2[0]);
		write(pp2[1],argv[2],strlen(argv[2]));
		printf("Child has finished writing in the pipe pp2!\n");
	}
	return 0;
}