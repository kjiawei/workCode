#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
	if(argc != 2)
	{
		return -1;
	}

	pid_t spid;

	spid = atoi(argv[1]);

	printf("spid=%d\n", spid);

	kill(spid, SIGINT);

	return 0;
}
