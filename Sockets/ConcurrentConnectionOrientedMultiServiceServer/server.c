#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define BACKLOG 5
#define services 3

int main(int argc,char *argv[])
{
	int PORT[services]={8003,8004,8005};
	int sfd[services],nsfd,maxfd=0,sz;
	struct sockaddr_in servaddr[services],cliaddr;

	for(int i=0;i<services;i++)
	{
		sfd[i]=socket(AF_INET,SOCK_STREAM,0);
		if(sfd[i] < 0)
		{
			perror("socket");
			exit(0);
		}
		else
			printf("Socket %d Created Successfully!\n", i+1);
		if(maxfd < sfd[i])
			maxfd = sfd[i];
	}

	for(int i=0;i<services;i++)
	{
		servaddr[i].sin_family = AF_INET;
		servaddr[i].sin_port = htons(PORT[i]);
		servaddr[i].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	}

	for(int i=0;i<services;i++)
	{
		if(bind(sfd[i],(struct sockaddr *)&servaddr[i],sizeof(servaddr[i]))!=0)
		{
			perror("bind");
			exit(0);
		}
		else
			printf("Socket %d Binded Successfully!\n", i+1);
	}

	for(int i=0;i<services;i++)
	{
		if(listen(sfd[i],BACKLOG)!=0)
		{
			perror("listen");
			exit(0);
		}
		else
			printf("Socket %d Listening..!\n", i+1);
	}

	fd_set readfds;
	FD_ZERO(&readfds);

	for(;;)
	{
		for(int i=0;i<services;i++)
			FD_SET(sfd[i],&readfds);

		// struct timeval timeout;
		// timeout.tv_sec = 10;
		// timeout.tv_usec = 0;

		int sret;
		sret = select(maxfd+1,&readfds,NULL,NULL,NULL);
		if(sret > 0)
		{
			if(FD_ISSET(sfd[0],&readfds))
			{
				memset((struct sockaddr_in *)&cliaddr,0,sizeof(cliaddr));
				sz = sizeof(cliaddr);
				nsfd = accept(sfd[0],(struct sockaddr *)&cliaddr,&sz);
				if(nsfd < 0)
				{
					perror("accept");
					exit(0);
				}
				else
				{
					printf("Accepted By Socket 1\n");
					int c = fork();
					if(c == 0)
					{
						for(int i=0;i<services;i++)
							close(sfd[i]);
						dup2(nsfd,0);
						dup2(nsfd,1);
						execlp("/home/atishay/Profile/two/CN/Sockets/ConcurrentConnectionOrientedMultiServiceServer/s1","s1",NULL);
						exit(0);
					}
					else
						close(nsfd);
				}
			}
			if(FD_ISSET(sfd[1],&readfds))
			{
				memset((struct sockaddr_in *)&cliaddr,0,sizeof(cliaddr));
				sz = sizeof(cliaddr);
				nsfd = accept(sfd[1],(struct sockaddr *)&cliaddr,&sz);
				if(nsfd < 0)
				{
					perror("accept");
					exit(0);
				}
				else
				{
					printf("Accepted By Socket 2\n");
					int c = fork();
					if(c == 0)
					{
						for(int i=0;i<services;i++)
							close(sfd[i]);
						dup2(nsfd,0);
						dup2(nsfd,1);
						execlp("/home/atishay/Profile/two/CN/Sockets/ConcurrentConnectionOrientedMultiServiceServer/s2","s2",NULL);
						exit(0);
					}
					else
						close(nsfd);
				}
			}
			if(FD_ISSET(sfd[2],&readfds))
			{
				memset((struct sockaddr_in *)&cliaddr,0,sizeof(cliaddr));
				sz = sizeof(cliaddr);
				nsfd = accept(sfd[2],(struct sockaddr *)&cliaddr,&sz);
				if(nsfd < 0)
				{
					perror("accept");
					exit(0);
				}
				else
				{
					printf("Accepted By Socket 3\n");
					int c = fork();
					if(c == 0)
					{
						for(int i=0;i<services;i++)
							close(sfd[i]);
						dup2(nsfd,0);
						dup2(nsfd,1);
						execlp("/home/atishay/Profile/two/CN/Sockets/ConcurrentConnectionOrientedMultiServiceServer/s3","s3",NULL);
						exit(0);
					}
					else
						close(nsfd);
				}
			}
		}
		// else
		// {
		// 	printf("No one is selected!\n");
		// }
	}

}