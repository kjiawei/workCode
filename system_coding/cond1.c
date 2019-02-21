#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int balance = 0;

pthread_mutex_t m;
pthread_cond_t v;

void *routine(void *args)
{

	pthread_mutex_lock(&m);

	while(balance < 100)
		pthread_cond_wait(&v, &m);

	fprintf(stderr, "t%d: balance = %d\n", (int)args, balance);
	balance -= 100;


	pthread_mutex_unlock(&m);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: %s <threads-number>\n", argv[0]);
		return 1;
	}

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&v, NULL);
	pthread_mutex_lock(&m);

	pthread_t tid;
	int i, thread_nums = 5;
	for(i=0; i<thread_nums; i++)
	{
		pthread_create(&tid, NULL, routine, (void *)i);
	}

	

	balance += (thread_nums * 100);
	pthread_cond_broadcast(&v);
	pthread_mutex_unlock(&m);

	pthread_exit(NULL);
}
