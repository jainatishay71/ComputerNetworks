#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>

// These semaphores are visible in /dev/shm directory

int main(int argc,char *argv[])
{
	if(argc < 4)
	{
		printf("Not enough arguments are specified!\n");
		exit(1);
	}
	int sem1 = (int)atoi(argv[1]);
	int sem2 = (int)atoi(argv[2]);
	int sem3 = (int)atoi(argv[3]);

	printf("%d %d %d\n", sem1,sem2,sem3);

	sem_t *semm1 = sem_open("/semap1",O_CREAT,0666,sem1);
	if(semm1 == SEM_FAILED)
	{
		perror("sem_open");
		exit(0);
	}
	sem_t *semm2 = sem_open("/semap2",O_CREAT,0666,sem2);
	if(semm2 == SEM_FAILED)
	{
		perror("sem_open");
		exit(0);
	}
	sem_t *semm3 = sem_open("/semap3",O_CREAT,0666,sem3);
	if(semm3 == SEM_FAILED)
	{
		perror("sem_open");
		exit(0);
	}
}
