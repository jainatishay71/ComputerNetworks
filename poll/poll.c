#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>

int main()
{
	struct pollfd pt[2];
	FILE *fp1 = popen("/home/atishay/Profile/two/CN/poll/p1","r");
	FILE *fp2 = popen("/home/atishay/Profile/two/CN/poll/p2","r");

	pt[0].fd = fileno(fp1);
	pt[1].fd = fileno(fp2);

	pt[0].events = 0;
	pt[0].events|=POLLIN;
	pt[1].events = 0;
	pt[1].events|=POLLIN;

	pt[0].revents = 0;
	pt[1].revents = 0;

	int timeout = 3000;

	while(1)
	{
		int ret,pret;
		char buff[512];
		pret = poll(pt,2,timeout);
		if(pret < 0)
		{
			perror("poll");
		}
		else if(pret > 0)
		{
			for(int i=0;i<2;i++)
			{
				if(pt[i].revents & POLLIN)
				{
					ret = read(pt[i].fd,buff,sizeof(buff));
					if(ret < 0)
					{
						perror("read");
					}
					else
					{
						printf("%s\n", buff);
					}
				}
			}
		}
	}
}