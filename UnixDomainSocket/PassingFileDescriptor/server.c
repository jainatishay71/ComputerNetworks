#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PATH "pathone"
#define SERVICE "pathtwo"
#define BACKLOG 5

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
	strcpy(servaddr.sun_path,PATH);

	if(bind(usfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		perror("bind");
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

	int usfd1;
	usfd1 = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd1 < 0)
	{
		perror("socket");
		exit(0);
	}
	else
		printf("Socket1 Created Successfully!\n");

	struct sockaddr_un servaddr1;

	servaddr1.sun_family = AF_UNIX;
	strcpy(servaddr1.sun_path,SERVICE);

	if(connect(usfd1,(struct sockaddr *)&servaddr1,sizeof(servaddr1)) < 0)
	{
		perror("connect");
		exit(0);
	}
	else
		printf("Connected Successfully To Service!\n");

	struct msghdr msg={0};
	struct cmsghdr *cmsg;
	char buf[CMSG_SPACE(sizeof(int))],*dup="hello world";
	memset(buf,0,sizeof(buf));
	struct iovec io={.iov_base=&dup, .iov_len=sizeof(dup)};

	msg.msg_iov=&io;
	msg.msg_iovlen=1;
	msg.msg_control=buf;
	msg.msg_controllen=sizeof(buf);

	cmsg=CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level=SOL_SOCKET;
	cmsg->cmsg_type=SCM_RIGHTS;
	cmsg->cmsg_len=CMSG_LEN(sizeof(int));

	int *fdptr=(int *)CMSG_DATA(cmsg);
	*fdptr=nsfd;

	if(sendmsg(usfd1,&msg,0) < 0)
	{
		perror("sendmsg");
		exit(0);
	}
	else
		printf("File Descriptor Sent To Service!\n");

	if(close(usfd1) < 0)
	{
		perror("close");
		exit(0);
	}


	close(usfd);

	unlink(PATH);
	exit(EXIT_SUCCESS);
	
}