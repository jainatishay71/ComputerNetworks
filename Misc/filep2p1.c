#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define SHMSIZE 100

int flag = -1;

void fromp1()
{
	printf("Received signal from P1\n");
	flag = 1;
}

struct shmtype
{
	int pid;
	int fd;
	int cnt;	
};

int main()
{
	signal(SIGUSR1,fromp1);

	int shmid;
	struct shmtype *shm;

	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",67);
	shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	shm = (struct shmtype *)shmat(shmid,NULL,0);
	shm->pid = getpid();
	shm->fd = -1;
	shm->cnt = -1;

	while(flag != 1)
	{
		printf("Waiting for the signal!\n");
		sleep(1);
	}

	char buff[1024];
	int cnt = shm->cnt;
	int fd = shm->fd;

	printf("\n\n");
	while(read(fd,buff,1))
	{
		if(buff[0] == '\n')
			cnt++;
		if(cnt==15)
		{
			printf("\n");
			break;
		}
		printf("%c", (char)buff[0]);
	}
	printf("\n\n");

	shmdt(shm);
	shmctl(shmid,IPC_RMID,NULL);
}