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

void fromp1()
{
	printf("Received signal from P1!\n");
}

int main()
{
	signal(SIGUSR1,fromp1);
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
			shm->y = shm->x +1;
			printf("Read x as %d and made y as %d\n", shm->x,shm->y);
			int p = shm->pid;
			shm->pid = getpid();
			kill(p,SIGUSR2);
			sleep(5);
		}
		cnt++;
		sleep(1);
	}

	shmdt(shm);
	shmctl(shmid,IPC_RMID,NULL);
}