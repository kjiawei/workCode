#include <stdio.h>
#include <pthread.h>

void *fun_cancel(void *arg)
{
	int i = 100;
	while(1)
	{
		printf("i = %d\n",i);
	}
	pthread_exit(NULL);
}


int main(void)
{
	pthread_t pthread_id;
	
	pthread_create(&pthread_id,NULL,&fun_cancel,NULL);
	
	sleep(5);
	pthread_cancel(pthread_id);
	
	pthread_join(pthread_id,NULL);
	return 0;
}