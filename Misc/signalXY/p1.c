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

void fromp2()
{
	printf("Received signal from P2!\n");
}

int main()
{
	signal(SIGUSR2,fromp2);
	int shmid;
	struct vars *shm;
	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",37);
	shmid = shmget(key,SHMSIZE,0666);
	shm = (struct vars *)shmat(shmid,NULL,0);
	
	int cnt = 0;
	while(cnt < 10)
	{
		if(shm->pid == -1)
		{
			shm->pid = getpid();
			sleep(5);
		}
		else
		{
			shm->x = shm->y +1;
			printf("Read y as %d and made x as %d\n", shm->y,shm->x);
			int p = shm->pid;
			shm->pid = getpid();
			kill(p,SIGUSR1);
			sleep(5);
		}
		cnt++;
		sleep(1);
	}
	shmdt(shm);
	shmctl(shmid,IPC_RMID,NULL);
}