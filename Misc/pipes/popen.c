#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{	
	if(argc < 2)
	{
		printf("Not enough arguments!\n");
		exit(1);
	}
	FILE *f = popen("/home/atishay/CN/Basics/pipes/P2","w");
	int fd = fileno(f);
	write(fd,argv[1],strlen(argv[1]));
	printf("Content written from program1\n");
	sleep(1);
	return 0;
}