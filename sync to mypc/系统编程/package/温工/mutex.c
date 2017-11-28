#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int global = 0;

pthread_mutex_t	mt;

void *start_thread1 (void *arg)
{
	int i=5;

	pthread_mutex_lock(&mt);	//p  mt == 0 
	while(1)
	{
		if(global > 5)
			break;
		printf("arg=%s,global=%d\n", (char *)arg, global);
		global++;
		sleep(1);
	}

	pthread_mutex_unlock(&mt);	//v

}

void *start_thread2 (void *arg)
{
	int i=5;

	printf("begin\n");
	pthread_mutex_lock(&mt);	//p
	printf("end\n");
	while(i--)
	{
		printf("arg=%s,global=%d\n", (char *)arg, global);
		global++;
		sleep(1);
	}

	pthread_mutex_unlock(&mt);	

}

int main(void)
{

	pthread_t tid1, tid2;
	int rtv;

	pthread_mutex_init(&mt, NULL);	//1


	pthread_create(&tid2, NULL, start_thread2, "B");

	pthread_create(&tid1, NULL, start_thread1, "A");


	pause();

	return 0;
}
