#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>

#define SYS_SIGNAL SIGRTMIN + 1
#define APP_QUIT 1
#define UPDATE_SYSTEM 2
int isWait ;

void SignalRecv(int signum,siginfo_t *info,void*myact)
{
	int msg=info->si_value.sival_int;
	switch(msg) {
		case APP_QUIT:
			isWait = 0;
			break;
		case UPDATE_SYSTEM:
			system("update_system -t JH5058 /mnt/usbStorage &");
			break;
	}
}
int main() 
{
    struct sigaction sa;
    sa.sa_sigaction = SignalRecv;
    sa.sa_flags |= SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SYS_SIGNAL,&sa,NULL);
	isWait = 1;
	while(isWait) {
		sleep(1);
	}
}

