#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

int main()
{
	int shmid;
	struct vars *shm;
	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",31);
	shmid = shmget(key,SHMSIZE,0666);
	shm = shmat(shmid,NULL,0);

	sem_t *semm1 = sem_open("/semap1",O_EXCL);
	sem_t *semm2 = sem_open("/semap2",O_EXCL);


	int cnt=0;
	while(cnt<10)
	{
		sem_wait(semm2);
		shm->x = shm->y + 1;
		printf("Read y as %d and made x as %d\n", shm->y,shm->x);
		sem_post(semm1);
		sleep(1);
		cnt++;
	}

	sem_close(semm1);
	sem_close(semm2);

	sem_unlink("/semap1");
	sem_unlink("/semap2");

	shmdt(shm);
	shmctl(shmid,IPC_RMID,NULL);
}
