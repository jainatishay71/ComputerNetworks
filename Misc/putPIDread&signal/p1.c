#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE 100

void fromp2()
{
	printf("Signal received from P2!\n");
}

int main()
{
	int shmid;
	int *shm;

	signal(SIGUSR2,fromp2);

	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",41);
	if(key<0)
	{
		perror("ftok");
		exit(1);
	}
	shmid = shmget(key,SHMSIZE,0666);
	shm = (int *)shmat(shmid,NULL,0);
	if(*shm == -1)
	{
		*shm = getpid();
		printf("P1 has put its pid in shared memory!\n");
		sleep(5);
		shmdt(shm);
		shmctl(shmid,IPC_RMID,NULL);
	}
	else
	{
		int p = *shm;
		printf("P1 is now sending signal to P2!\n");
		kill(p,SIGUSR1);
	}
}