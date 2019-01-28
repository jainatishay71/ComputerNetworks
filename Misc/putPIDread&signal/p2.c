#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE 100

void fromp1()
{
	printf("Signal received from P1!\n");
}

int main()
{
	int shmid;
	int *shm;

	signal(SIGUSR1,fromp1);

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
		printf("P2 has put its pid in shared memory!\n");
		sleep(5);
		shmdt(shm);
		shmctl(shmid,IPC_RMID,NULL);
	}
	else
	{
		int p = *shm;
		printf("P2 is now sending signal to P1!\n");
		kill(p,SIGUSR2);
	}
}