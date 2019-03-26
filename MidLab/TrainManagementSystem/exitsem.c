#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <signal.h>
#include <semaphore.h>

int main(int argc,char *argv[])
{
	sem_t *semm = sem_open("./semap",0);

	sem_close(semm);
	sem_unlink("/semap");
}