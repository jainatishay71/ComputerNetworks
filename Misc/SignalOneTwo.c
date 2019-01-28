#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void fromc()
{
	printf("Recieved signal from Child!\n");
}

void fromp()
{
	printf("Had to write this otherwise code is not working!\n");
}

int main()
{
	signal(SIGUSR2,fromc);
	signal(SIGUSR1,fromp);

	printf("pid of the Parent process is %d \n", getpid());
	int c,status;
	c=fork();
	if(c>0)
	{
		printf("Sending signal from Parent!\n");
		kill(c,SIGUSR1);
		wait(&status);
		printf("Back in Parent!\n");
	}
	else
	{
		printf("In child!\n");
		execlp("/home/atishay/CN/Basics/SignalTwoOne","SignalTwoOne",NULL);
		printf("Child Executed!\n");
	}
}