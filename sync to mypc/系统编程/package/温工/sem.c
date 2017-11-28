#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

char buf[100];

sem_t sem;

void *read_thread (void *arg)
{
	while(1)
	{
		sem_wait(&sem);	//p
		printf("arg=%s, readbuf=%s\n", (char *)arg, buf);
	}
}

void *write_thread (void *arg)
{
	while(1)
	{
		gets(buf);
		sem_post(&sem);	//v
	}
}

int main(void)
{

	pthread_t tid1, tid2;
	int rtv;
	rtv = sem_init(&sem, 0, 0);
	if(rtv == -1)
	{
		perror("init sem failed\n");
		return -1;
	}

	

	pthread_create(&tid1, NULL, read_thread, "A");

	pthread_create(&tid2, NULL, write_thread, "B");


	pause();

	return 0;
}
