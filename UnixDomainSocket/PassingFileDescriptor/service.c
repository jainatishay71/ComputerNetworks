#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SERVICE "pathtwo"
#define BACKLOG 5

void serve(int fsfd)
{
	for(;;)
	{
		char buff[512];
		memset(&buff,0,sizeof(buff));
		if(recv(fsfd,buff,sizeof(buff),0) < 0)
		{
			perror("recv");
			exit(0);
		}
		else
			printf("Received : %s\n", buff);

		if(strcmp(buff,"bye")==0)
		{
			return;
		}

		for(int i=0;i<strlen(buff);i++)
		{
			if(buff[i]>='a' && buff[i]<='z')
			{
				buff[i]-='a';
				buff[i]+='A';
			}
		}

		if(send(fsfd,buff,sizeof(buff),0) < 0)
		{
			perror("send");
			exit(0);
		}
		else
			printf("Request Served!!\n\n");
	}
}

int main()
{
	int usfd,nsfd;
	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket Created Successfully!\n");

	struct sockaddr_un servaddr;

	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path,SERVICE);

	if(bind(usfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		perror("bind1");
		exit(0);
	}
	else
		printf("Socket Binded Successfully!\n");

	if(listen(usfd,BACKLOG) < 0)
	{
		perror("listen");
		exit(0);
	}
	else
		printf("Socket Listening..!\n");

	nsfd = accept(usfd,NULL,NULL);
	if(nsfd < 0)
	{
		perror("accept");
		exit(0);
	}
	else
		printf("Accepted\n");

	close(usfd);
	struct msghdr msg={0};
	struct cmsghdr *cmsg;
	char buf[CMSG_SPACE(sizeof(int))],dup[256];
	memset(buf,0,sizeof(buf));
	struct iovec io={.iov_base=&dup, .iov_len=sizeof(dup)};

	msg.msg_iov=&io;
	msg.msg_iovlen=1;
	msg.msg_control=buf;
	msg.msg_controllen=sizeof(buf);
	
	if(recvmsg(nsfd,&msg,0) < 0)
	{
		perror("recvmsg");
		exit(0);
	}

	cmsg=CMSG_FIRSTHDR(&msg);

	int *fdptr=(int *)CMSG_DATA(cmsg);

	int fsfd = *fdptr;
	serve(fsfd);

	unlink(SERVICE);
}