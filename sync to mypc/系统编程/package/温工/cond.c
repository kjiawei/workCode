#include <stdio.h>	//printf
#include <pthread.h>
#include <signal.h>


pthread_mutex_t mt;
pthread_cond_t cond;

int count = 0;

void *start_thread(void *arg)
{
	printf("child <-%d->\n", (int)arg);

	pthread_mutex_lock(&mt);

	while(count < 100)
	{
		printf("start wait\n");
		pthread_cond_wait(&cond, &mt);	//首先解mt的锁，然后去睡觉，等待他人唤醒，唤醒后再去上mt的锁
	}

	printf("child <-%d->count == %d\n", (int)arg, count);

	count -= 100;
	
	pthread_mutex_unlock(&mt);

	return (void *)0;
}


void func(int sig)
{

	printf("in main thread \n");

	pthread_mutex_lock(&mt);

	count += 300;
	
	pthread_mutex_unlock(&mt);

	pthread_cond_broadcast(&cond);		//去唤醒用cond这个条件变量睡觉的人，以广播的形式（群发）
	//pthread_cond_signal(&cond);		//去唤醒一个用cond这个变量睡觉的人，单发

	return;
}


int main(void)
{
	pthread_t tid[10];
	int i=10;

	signal(SIGINT, func);

	pthread_mutex_init(&mt, NULL);
	pthread_cond_init(&cond, NULL);	//初始化条件变量

	while(i--)
		pthread_create(&tid[i], NULL, start_thread, (void *)i);

	while(1)
		pause();						//一直让它睡眠

	return 0;
}
