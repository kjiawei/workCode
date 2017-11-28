//线程的取消
#include "myhead.h"
void *fun(void *arg)
{
	// 先将该线程设置成不能被取消
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	int i=10;
	printf("thread is start!\n");
	while(i)
	{
		i--;
		printf("now i is:%d\n",i);
		sleep(1);
	}
	// 将线程设置成能被取消   
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL); 立即取消
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);  // 延时取消
	
	printf("really cancel!!!!\n"); // 由于printf是个取消点，所有延时取消的时候会执行这句代码
	printf("really  really cancel!!!!!\n");
	pthread_exit(NULL);
}
int main()
{
	pthread_t myid;
	printf("I am in main thread!\n");
	// 创建一个线程
	pthread_create(&myid,NULL,fun,NULL);// 创建了线程，跟主函数并发执行
	// 先腾出时间给线程运行一会
	sleep(2);
	// 取消线程
	pthread_cancel(myid);  // 发送了取消的命令
	printf("main prepare exit!\n");
	// 回收线程
	pthread_join(myid,NULL);
	return 0;
	
}