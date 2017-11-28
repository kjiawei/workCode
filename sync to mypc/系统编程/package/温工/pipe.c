#include <stdio.h>	//printf
#include <sys/wait.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int pfd[2];
	int rtv;
	pid_t pid;

	//rtv = pipe(pfd);	//parent child share data
	rtv = pipe2(pfd, O_NONBLOCK);	//parent child share data
	if(rtv == -1)
	{
		perror("create pipe failed\n");
		return -1;
	}

	pid = fork();

	if(pid == -1)
	{
		perror("create child process failed\n");
		return -1;
	}

	if(pid == 0)	//child
	{
		char buf[100];

		close(pfd[1]);	//close write

		read(pfd[0], buf, sizeof(buf));	//return value read data size

		printf("buf=%s\n", buf);

		exit(EXIT_SUCCESS);
	}

	close(pfd[0]);	//close read

	sleep(1);

	write(pfd[1], "hello", 6);

	int status;

	wait(&status);

	return 0;
}
