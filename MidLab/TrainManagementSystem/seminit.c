#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <signal.h>
#include <semaphore.h>

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		printf("Not Enough Arguments!\n");
		exit(0);
	}
	int semval = atoi(argv[1]);

	sem_t *semm = sem_open("/semap",O_CREAT,0666,semval);
}