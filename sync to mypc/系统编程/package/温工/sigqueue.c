#include <stdio.h>
#include <signal.h>




int main(int argc, const char *argv[])
{

	union sigval value;

	pid_t pid;

	pid = atoi(argv[1]);

	value.sival_int = 18;

	sigqueue(pid, SIGINT, value);

	return 0;
}
