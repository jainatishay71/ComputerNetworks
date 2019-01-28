#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE 100

struct args
{
	int x,y;
};

int main(int argc,char *argv[])
{
	if(argc < 4)
	{
		printf("Not all arguments are specified!\n");
		exit(1);
	}
	int shmid;
	struct args *shm;
	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",17);
	shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	shm = shmat(shmid,NULL,0);

	int pno = (int)atoi(argv[1]);
	int cx = (int)atoi(argv[2]);
	int cy = (int)atoi(argv[3]);

	// printf("%d %d %d \n", pno,cx,cy);

	shm->x = -1;
	shm->y = -1;

	if(pno == 1)
	{
		if(shm->x == -1 && shm->y == -1)
		{
			shm->x = cx;
			shm->y = cy;
		}
		int cnt=0;
		while(cnt<10)
		{
			shm->x = shm->y+1;
			printf("Read y as %d and made x as %d\n", shm->y,shm->x);
			sleep(1);
			cnt++;
		}
	}
	else
	{
		if(shm->x == -1 && shm->y == -1)
		{
			shm->x = cx;
			shm->y = cy;
		}
		int cnt=0;
		while(cnt<10)
		{
			shm->y = shm->x+1;
			printf("Read x as %d and made y as %d\n", shm->x,shm->y);
			sleep(1);
			cnt++;
		}
	}

	shmdt(shm);
	shmctl(shmid,IPC_RMID,NULL);
}