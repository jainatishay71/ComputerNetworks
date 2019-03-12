#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc,char *argv[])
{
	FILE *fp = fopen("store.txt","w");
	while(1)
	{
		char buff[500];
		memset(&buff,0,sizeof(buff));
		if(read(0,buff,sizeof(buff)) < 0)
		{
			perror("read");
			exit(0);
		}
		if(fprintf(fp, "%s", buff) < 0)
		{
			perror("fprintf");
			exit(0);
		}
		fflush(0);
		sleep(1);
	}
}