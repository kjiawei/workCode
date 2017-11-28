#include <stdio.h>
#include <pthread.h>


void *func(void *arg)
{
	pthread_detach(pthread_self());		//set detach status

	int state;
	//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &state);


	printf("set end start thread\n");

	unsigned long i=1000000000;

	while(i--);	//2-3s accept cancel
	
	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &state);

	printf("first\n");

	fprintf(stdout, "exit\n");

	sleep(5);

	printf("test\n");

	pthread_exit(NULL);
}


int main(void)
{
	pthread_t tid;


	pthread_create(&tid, NULL, func, "A");


	usleep(100000);

	printf("start\n");
	pthread_cancel(tid);

	pthread_exit(NULL);

	return 0;
}
