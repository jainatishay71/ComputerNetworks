#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// void fromc()
// {
// 	printf("Recieved signal from Child!\n");
// }

void fromp()
{
	printf("Recieved signal from Parent!\n");
}

int main()
{
	// signal(SIGUSR2,fromc);
	signal(SIGUSR1,fromp);

	printf("Hello from the other program!\n");

	int p = getppid();
	kill(p,SIGUSR2);
}