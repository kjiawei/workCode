#include <stdio.h>
#include <pthread.h>

void *fun_cancel(void *arg)
{
	int i = 100;
	while(1)
	{
		printf("i = %d\n",i);
		sleep(1);
	}
	pthread_exit(NULL);
}


int main(void)
{
	pthread_t pthread_id;
	pthread_attr_t attr;
	
	int ret = pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	if(ret !=0)
	{
		printf("pthread_attr_init failed!\n");
	}
	pthread_create(&pthread_id,&attr,&fun_cancel,NULL);
	while(1);
	
	
	pthread_attr_destroy(&attr);


	
	return 0;
}