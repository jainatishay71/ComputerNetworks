#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
	char buf[50];
	for(;;)
	{
		memset((char *)buf,0,sizeof(buf));
		recv(0,buf,sizeof(buf),0);
		printf("Serving from S3, received %s\n", buf);

		for(int i=0;i<strlen(buf);i++)
		{
			if(i==0 || i==strlen(buf)-1)
			{
				if(buf[i]>='a' && buf[i]<='z')
				{
					buf[i] = buf[i]-'a';
					buf[i] = buf[i]+'A';
				}
			}
			else
			{
				if(buf[i]>='A' && buf[i]<='Z')
				{
					buf[i] = buf[i]-'A';
					buf[i] = buf[i]+'a';
				}
			}
		}

		send(1,buf,sizeof(buf),0);
		printf("Request Served!\n\n");
	}
}