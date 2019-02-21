#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


//线程执行函数
void *thread_start(void *arg)
{
	
	printf("reveice char %s\n",(char *)arg);

	printf("hi man!\n");
	
	char *p = malloc(20);
	fgets(p,20,stdin);
	pthread_exit((void *)p);
}

int main(void)
{
	//线程识别变量
	pthread_t pthread_id;
	int ret;
	
	char msg[] = "helloworld"; 
	//线程创建
	ret = pthread_create(&pthread_id,NULL,&thread_start,(void *)msg);
	if(ret !=0)
	{
		printf("pthread_create failed!\n");
		return;
	}

	

	printf("hellowrold!\n");
	sleep(6);
	
	char *ret_val;
	pthread_join(pthread_id,(void **)&ret_val);
	printf("ret_val = %s\n",ret_val);
	
	return 0;
}