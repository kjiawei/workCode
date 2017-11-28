#include <stdio.h>	//printf
#include <unistd.h>	//fork,getpid,exec
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
		char *argv[] = {"./hello", "abc", "123", NULL};

		//execl("./hello", "./hello", "abc", "123", NULL);
		//execv("./hello", argv);
		execlp("ls", "ls", "-l", NULL);
	}

	int status;
	pid_t waitid;

	waitid = waitpid(-1, &status, 0);
	

	return 0;
}
