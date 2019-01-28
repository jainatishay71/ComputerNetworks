#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE 100

int main()
{
	int shmid;
	int *shm;

	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",41);
	if(key<0)
	{
		perror("ftok");
		exit(1);
	}
	shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	shm = (int *)shmat(shmid,NULL,0);
	*shm = -1;
}