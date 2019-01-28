#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	printf("Inside P2\n");
	char buff[20];
	scanf("%s",buff);
	printf("The read content by P2 is : %s \n", buff);
	return 0;
}