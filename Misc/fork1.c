#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define k 4

int main()
{
	int a[k],i=0,flag[k],status;
	for(int u=0;u<k;u++)
		flag[u]=0;
	printf("pid of parent is %d\n", getpid());
	while(!flag[0] || !flag[1] || !flag[2] || !flag[3])
	{
		if(i<k)
		{
 			a[i]=fork();
 			if(a[i]>0)
			{
				printf("parent waiting on %d child!\n",i);
				waitpid(a[i],&status,WNOHANG);
				flag[i]=1;
				printf("parent resumed on termination of %d child process\n",i);
				i++;
			}
			else
			{
				printf("child process having pid %d\n", getpid());
				break;
			}
		}
	}
}