#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int pp[2];
	pipe(pp);
	write(pp[1],"Hello",5);
	char buff[20];
	read(pp[0],buff,20);
	buff[5]='\0';
	printf("%s\n", buff);
}