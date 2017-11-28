#include "myhead.h"

//分离属性(可分:自生自灭 不可:必须join)  pthread_attr_setdetachestate
//PTHREAD_CREATE_DETACHED
//作用:线程自己回收自己,主函数不用关心退出状态；多线程并发处理时可以自己管理自己的生命周期

void *fun1(void *str){
	printf("分离属性演示\n");
	pthread_exit("线程退出");
}

int main(void){
	void *str=NULL;
	pthread_attr_t pattr;//线程属性结构体变量
	//pthread_attr_init(&pattr);//初始化   默认不可分离(主线程确保退出)，使用时将create函数第二参数设置为&pattr
	//pthread_attr_setdetachstate(&pattr,PTHREAD_CREATE_DETACHED);//设置分离属性
	//方法二：int pthread_detach(pthread_t thread); &pattr在create函数传入就可以了,在join前要sleep(2)才能体现作用
	
	//生效属性
	pthread_t pid;//创建线程
	pthread_create(&pid,NULL,fun1,NULL);
	
	pthread_join(pid,&str);//确保等待子线程退出然后把它回收，与wait作用相同
	//打印回收信息
	sleep(1);//不加打印第一句话不出
	printf("退出信息:%s\n",(char *)str);//str在join函数里给到fun1了
	
	//pthread_attr_destroy();
	return 0;
}