#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{	
	FILE *f = popen("/home/atishay/CN/Basics/pipes/PR","r");
	int fd = fileno(f);
	char buff[20];
	read(fd,buff,20);
	printf("Content read from PR is %s \n",buff);
	sleep(1);
	return 0;
}