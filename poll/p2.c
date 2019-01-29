#include <stdio.h>
#include <unistd.h>

int main()
{
	char *buf = "fromP2";
	while(1)
	{
		write(1,buf,sizeof(buf));
		sleep(2);
	}
}