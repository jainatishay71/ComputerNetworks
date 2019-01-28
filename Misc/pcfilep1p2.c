#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SHMSIZE 100

struct shmtype
{
	int pid;
	int fd;
	int cnt;	
};

void proto()
{
	printf("Prototype of SIGUSR1\n");
}

int main()
{
	signal(SIGUSR1,proto);

	int shmid;
	struct shmtype *shm;

	key_t key = ftok("/home/atishay/CN/Basics/forkey.txt",67);
	shmid = shmget(key,SHMSIZE,0666);
	shm = (struct shmtype *)shmat(shmid,NULL,0);

	int fd = open("processfile.txt",O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}
	int cnt=0;
	char buff[1024];
	
	while(read(fd,buff,1))
	{
		if(buff[0] == '\n')
			cnt++;
		if(cnt==4)
		{
			printf("\n");
			break;
		}
		printf("%c", (char)buff[0]);
	}	
	printf("\n\n");

	int c,status;
	c=fork();
	if(c>0)
	{
		printf("P1 is now sending signal to P2!\n");
		int p = shm->pid;
		shm->pid = -1;
		shm->fd = fd;
		shm->cnt = cnt;
		close(fd);
		kill(p,SIGUSR1);
		return 0;

		// wait(&status);
		sleep(10);
		printf("P1 resumed!\n");
		// shmdt(shm);
		// shmctl(shmid,IPC_RMID,NULL);
	}
	else
	{
		printf("\n\n");
		execlp("/home/atishay/CN/Basics/filep2p1","filep2p1",NULL);
	}
}