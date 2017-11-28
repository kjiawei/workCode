#include <stdio.h>
#include <signal.h>

/*
void (*signal(int sig, void (*func)(int)))(int);


typedef void (*signal_hand)(int);
signal_hand signal(int sig, signal_hand func);

*/

void sig_hand_func(int sig)
{
	int i=5;

	while(i--)
	{
		printf("accept signal = %d\n", sig);
		sleep(1);
	}
	return;
}

int main(void)
{

	printf("my PID=%d\n", getpid());
	signal(SIGINT, sig_hand_func);

	while(1)
	{
		printf("hello\n");
		sleep(1);
	}

	return 0;
}
