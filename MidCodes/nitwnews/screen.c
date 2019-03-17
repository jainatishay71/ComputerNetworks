#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PATH "SFIFO"

int main()
{
	mkfifo(PATH,0666);
	int ffd = open(PATH,O_RDONLY);
	if(ffd < 0)
	{
		perror("open");
		exit(0);
	}

	for(;;)
	{
		char buff[512];
		memset(&buff,0,sizeof(buff));
		int sz = read(ffd,buff,sizeof(buff));
		if(sz < 0)
		{
			perror("read");
			exit(0);
		}
		else if(sz > 0)
		{
			printf("The msg received by the client is : %s\n", buff);
			fflush(0);
		}
	}
}