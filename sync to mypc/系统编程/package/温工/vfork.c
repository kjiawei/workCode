#include <stdio.h>	//printf
#include <sys/types.h>	//vfork
#include <unistd.h>	//vfork
#include <stdlib.h>	//exit


int main(void)
{

	pid_t pid;

	int x=0;

	printf("begin\n");

	pid = vfork();
	if(pid < 0)
	{
		perror("create child failed\n");
	}

	sleep(1);

	if(pid == 0)	//child
	{
		x=1;
		printf("in child process\n");
		exit(0);
	}

	if(pid > 0)	//parent
	{
		printf("x=%d\n", x);
		printf("in parent process\n");
	}

	return 0;
}
