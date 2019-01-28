#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void func()
{
	printf("You pressed Ctrl+C but I won't QUIT :)\n");
	// signal(SIGINT,SIG_DFL);            // Uncomment this to restore the default functionality of SIGINT
}

int main()
{
	signal(SIGINT,func);
	while(1)
	{
		printf("Hey there!\n");
		sleep(1);
	}
}
