#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{
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
		printf("Parent waiting for child to finish!\n");
		wait(&status);
		printf("Parent resumed!\n");
	}
	else
	{
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
	}
}