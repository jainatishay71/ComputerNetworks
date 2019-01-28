#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
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
		if(cnt==5)
		{
			printf("\n");
			break;
		}
		printf("%c", (char)buff[0]);
	}	
}