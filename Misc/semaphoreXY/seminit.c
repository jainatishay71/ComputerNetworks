#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SHMSIZE 100

struct vars
{
	int x,y;
};

int main(int argc,char *argv[])
{
	if(argc < 5)
	{
		printf("Not enough arguments are specified!\n");
		exit(1);
	}
	int shmid;
	struct vars *shm;
	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",31);
	shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	shm = shmat(shmid,NULL,0);

	int cx = (int)atoi(argv[1]);
	int cy = (int)atoi(argv[2]);
	int sem1 = (int)atoi(argv[3]);
	int sem2 = (int)atoi(argv[4]);

	shm->x = cx;
	shm->y = cy;

	sem_t *semm1 = sem_open("/semap1",O_CREAT,0666,sem1);
	sem_t *semm2 = sem_open("/semap2",O_CREAT,0666,sem2);
}
