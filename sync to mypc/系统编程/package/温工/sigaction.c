#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void func(int sig, siginfo_t *info, void *context)
{

	printf("accept signal = %d,si_int=%d,send pid=%d \n", sig, info->si_int, info->si_pid);

	return;
}


int main(void)
{
	struct sigaction siga;


	siga.sa_sigaction = func;

	siga.sa_flags = SA_SIGINFO;

	printf("my pid=%d\n", getpid());

	sigaction(SIGINT, &siga, NULL);	//signal


	pause();

	return 0;
}
