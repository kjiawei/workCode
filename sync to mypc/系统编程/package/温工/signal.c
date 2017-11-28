#include <stdio.h>
#include <signal.h>

/*
void (*signal(int sig, void (*func)(int)))(int);


typedef void (*signal_hand)(int);
signal_hand signal(int sig, signal_hand func);

*/

int main(void)
{

	signal(SIGHUP, SIG_IGN);

	while(1)
	{
		printf("hello\n");
		sleep(1);
	}

	return 0;
}
