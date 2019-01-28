#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void fromp()
{
	printf("Recieved signal from Parent!\n");
}

void fromc()
{
	printf("Recieved signal from Child!\n");
}

int main()
{
	printf("parent process id %d\n", getpid());

	signal(SIGUSR1,fromp);
	signal(SIGUSR2,fromc);

	int c,status;
	c=fork();
	if(c>0)
	{
		printf("Sending signal from Parent!\n");
		kill(c,SIGUSR1);
		printf("Back in Parent\n");
		sleep(5);
	}
	else if(c==0)
	{
		int p = getppid();
		printf("Sending signal from Child!\n");
		kill(p,SIGUSR2);
		printf("Back in Child\n");
		sleep(5);
	}
}