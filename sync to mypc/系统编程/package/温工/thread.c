#include <stdio.h>
#include <pthread.h>

static int x=0;		//data

void *start_routine(void *arg)
{
	char *ret = "hello";	//rodata

	sleep(3);

	printf("arg=%d, x=%d\n", (int)arg, x);

	
	//线程的退出函数，它会将ret的值返回给回收这条线程的人
	pthread_exit((void *)ret);
}


int main(void)
{

	pthread_t tid;		//用来存放线程的ID
	int rtv;
	void *ret;
	pthread_attr_t attr;	//线程的属性结构体

	pthread_attr_init(&attr);	//初始化线程属性结构体

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);	//设置线程的分离属性为分离状态

	/*
		创建线程：
			tid：创建线程成功会把线程的ID存放到这里
			attr：线程的属性设置
			start_routine：线程创建之后跑的内容
			arg：传入给线程函数的值
	*/
	
	rtv = pthread_create(&tid, &attr, start_routine, (void *)1);	
	if(rtv != 0)
	{
		printf("create thread failed\n");
		return -1;
	}

	//摧毁线程的属性结构体
	pthread_attr_destroy(&attr);

	//线程的接合函数，如果指定tid的线程不是分离状态，它会一直卡在这里，等待线程退出，接收其返回值放到ret里面
	pthread_join(tid, &ret);

	printf("in parent thread x=%d\n", ++x);
	
	printf("ret value =%s\n", (char *)ret);
	
	
	//线程的退出函数
	pthread_exit(NULL);

	return 0;
}
