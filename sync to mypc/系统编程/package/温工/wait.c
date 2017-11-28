#include <stdio.h>	//printf
#include <unistd.h>	//fork,getpid
#include <sys/types.h>	//getpid,wait
#include <sys/wait.h>	//wait
#include <stdlib.h>	//exit

int main(void)
{

	pid_t pid;

	pid = fork();

	if(pid == -1)
	{
		perror("create child process failed\n");
		return -1;
	}

	if(pid == 0)	//child
	{
		printf("in chiled\n");
		exit(EXIT_SUCCESS);	
	}

	
	printf("in parent ...\n");

	int status;
	pid_t waitid;

	sleep(1);

	//waitid = wait(&status);	//waitpid
	waitid = waitpid(-1, &status, 0);
	
	printf("waitid=%d, pid=%d,status=%d\n", waitid, pid, WEXITSTATUS(status));

	return 0;
}
