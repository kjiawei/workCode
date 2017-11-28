#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void func(int sig, siginfo_t *info, void *context)
{
	int i=10;
	while(i--)
	{
		printf("accept signal = %d,si_int=%d,send pid=%d \n", sig, info->si_int, info->si_pid);
		sleep(1);
	}
	return;
}

void func1(int sig)
{
	printf("accept SIGHUP\n");

	return;
}


int main(void)
{
	struct sigaction siga;


	siga.sa_sigaction = func;


	sigemptyset(&siga.sa_mask);

	sigaddset(&siga.sa_mask, SIGHUP);

	siga.sa_flags = SA_SIGINFO;

	printf("my pid=%d\n", getpid());

	sigaction(SIGINT, &siga, NULL);	//signal
	signal(SIGHUP, func1);

	pause();

	return 0;
}
