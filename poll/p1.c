#include <stdio.h>
#include <unistd.h>

int main()
{
	char *buf = "fromP1";
	while(1)
	{
		write(1,buf,sizeof(buf));
		sleep(1);
	}
}