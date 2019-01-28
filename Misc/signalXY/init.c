#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#define SHMSIZE 100

struct vars
{
	int x,y,pid;
};

int main(int argc,char *argv[])
{
	if(argc < 3)
	{
		printf("Not enough arguments specified!\n");
		exit(1);
	}
	int shmid;
	struct vars *shm;
	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",37);
	shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	shm = (struct vars *)shmat(shmid,NULL,0);
	shm->pid = -1;

	int cx = (int)atoi(argv[1]);
	int cy = (int)atoi(argv[2]);

	shm->x = cx;
	shm->y = cy;

	// shmdt(shm);
	// shmctl(shmid,IPC_RMID,NULL);
}