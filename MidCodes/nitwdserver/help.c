#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSIZE 512

int main()
{
	key_t key = ftok("./file",41);
	int shmid;
	shmid = shmget(key,SHMSIZE,IPC_CREAT|0666);
	int *shm;
	shm = (int *)shmat(shmid,NULL,0);

	int pid = *shm;

	for(;;)
	{
		char buff[500];
		memset((char *)buff,0,sizeof(buff));

		recv(0,buff,sizeof(buff),0);
		printf("The Server has received : %s\n", buff);
		if(strcmp(buff,"bye")==0)
			break;
		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='a' && buff[i]<='z')
			{
				buff[i] = buff[i]-'a';
				buff[i] = buff[i]+'A';
			}
			else if(buff[i]>='A' && buff[i]<='Z')
			{
				buff[i] = buff[i]-'A';
				buff[i] = buff[i]+'a';
			}
		}
		
		int stdout = open("/dev/pts/1",O_WRONLY);
		int stdin = open("/dev/pts/1",O_RDONLY);
		write(stdout,buff,sizeof(buff));
		char in[50];
		memset(&in,0,sizeof(in));
		read(stdin,in,sizeof(in));
		send(1,in,sizeof(in),0);
	}
	kill(pid,SIGUSR1);

}