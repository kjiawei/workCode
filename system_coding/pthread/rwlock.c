#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

pthread_rwlock_t rwlock;

void printf_char(char *string)
{
	char *p = string;
	while(*p != '\0')
	{
		fprintf(stderr,"%c",*p);
		usleep(500);
		p++;
	}
	printf("\n");
	
}

void *fun1(void *arg)
{
	pthread_rwlock_wrlock(&rwlock);//Ð´Ëø-->»¥³âËø
	printf_char("GSDGLOVGSDQLDSFD4GF5DF75DA5W");
	pthread_rwlock_unlock(&rwlock);//½âËø
	pthread_exit(NULL);
}

void *fun2(void *arg)
{
	pthread_rwlock_wrlock(&rwlock);//Ð´Ëø
	printf_char("sddscxlvjslejslvslflsljflseflksd");
	pthread_rwlock_unlock(&rwlock);//½âËø
	
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t tid1,tid2;
	
	pthread_rwlock_init(&rwlock,NULL);
	
	
	pthread_create(&tid1,NULL,&fun1,NULL);
	pthread_create(&tid2,NULL,&fun2,NULL);
	
	sleep(2);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_rwlock_destroy(&rwlock);
		
}