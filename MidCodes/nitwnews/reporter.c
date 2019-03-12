#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[])
{
	while(1)
	{
		char buff[500];
		memset(&buff,0,sizeof(buff));
		read(0,buff,sizeof(buff));
		write(1,buff,sizeof(buff));
		sleep(1);
	}
}