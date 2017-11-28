#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_rwlock_t rwm;

int global = 0;

void *write_thread(void *arg)
{
	int i = 5;
	pthread_rwlock_wrlock(&rwm);

	while(i--)
	{
		global++;
		printf("arg = %s,lobal = %d\n", (char *)arg, global);
		sleep(1);
	}

	pthread_rwlock_unlock(&rwm);
}


void *read_thread(void *arg)
{
	int i=5;

	pthread_rwlock_rdlock(&rwm);

	while(i--)
	{
		printf("arg = %s,lobal = %d\n", (char *)arg, global);
		sleep(1);
	}
	
	pthread_rwlock_unlock(&rwm);

}

int main(void)
{

	pthread_t tid1, tid2, tid3, tid4;
	int rtv;
	
	pthread_rwlock_init(&rwm, NULL);

	pthread_create(&tid3, NULL, write_thread, "C");
	pthread_create(&tid1, NULL, read_thread, "A");
	//pthread_create(&tid2, NULL, read_thread, "B");
	//pthread_create(&tid4, NULL, write_thread, "D");



	pause();

	return 0;
}
