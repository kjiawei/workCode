#include "myhead.h"
/*创建两个线程并设置不同的静态优先级
动态:nice*/
void *fun1(void *str){
   nice(5);//数字小，优先级高
   while(1){printf("thread 1-----\n");sleep(1);}
   pthread_exit(NULL);
}
void *fun2(void *str){
   nice(20);
   while(1){printf("thread 2-----\n");sleep(1);}
   pthread_exit(NULL);
}
int main(void){
    pthread_t mythread1;
    pthread_t mythread2;

    struct sched_param param1;//pthread_attr_setschedparam()第二个参数用
    struct sched_param param2;
    bzero(&param1,sizeof(struct sched_param));
    bzero(&param2,sizeof(struct sched_param));
    param1.sched_priority = 0;//非实时线程 静态优先级为0
    param2.sched_priority = 0;

    pthread_attr_t myattr1;//定义线程属性变量
    pthread_attr_t myattr2;
    pthread_attr_init(&myattr1);//初始化
    pthread_attr_init(&myattr2);
    pthread_attr_setschedparam(&myattr1,&param1);//调用设置静态优先级的函数
    pthread_attr_setschedparam(&myattr2,&param2);

    //创建两个线程
    pthread_create(&mythread1,&myattr1,fun1,NULL);
    pthread_create(&mythread2,&myattr2,fun2,NULL);

    /*//设置不同的静态优先级
    pthread_attr_setschedparam(&myattr1,param1);
    pthread_attr_setschedparam(&myattr2,param2);

    //获取并打印不同的静态优先级
    long int p1,p2;
    pthread_attr_getschedparam(&myattr1,&param1);
    pthread_attr_getschedparam(&myattr2,&param2);
    printf("p1 %ld:\n",p1);
    printf("p2 %ld:\n",p2);*/

    //回收
    pthread_join(mythread1,NULL);
    pthread_join(mythread2,NULL);
    //销毁
    pthread_attr_destroy(&myattr1);
    pthread_attr_destroy(&myattr2);
    return 0;
}