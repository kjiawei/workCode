#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

char buff[100];
sem_t sem_r;//信号量的数据类型,本质上是一个长整型的数
sem_t sem_w;

void *fun1(void *arg)
{
	while(1)
	{
		sem_wait(&sem_w); //减，阻塞
		scanf("%s",buff);
		printf("write :%s\n",buff);
		sem_post(&sem_r); //加
	}
	pthread_exit(NULL);
}

void *fun2(void *arg)
{
	while(1)
	{
		sem_wait(&sem_r); //原子操作(从信号量的值减去一个“1”，但它永远会先等待该信号量为一个非零值才开始做减法。也就是说，如果你对一个值为2的信号量调用sem_wait(),线程将会继续执行，介信号量的值将减到1。如果对一个值为0的信号量调用sem_wait()，这个函数就会地等待直到有其它线程增加了这个值使它不再是0为止。如果有两个线程都在sem_wait()中等待同一个信号量变成非零值，那么当它被第三个线程增加一个“1”时，等待线程中只有一个能够对信号量做减法并继续执行，另一个还将处于等待状态),被用来阻塞当前线程直到信号量sem的值大于0，解除阻塞后将sem的值减一，表明公共资源经使用后减少。函数sem_trywait ( sem_t *sem )是函数sem_wait（）的非阻塞版本，它直接将信号量sem的值减一
		sleep(1);
		printf("read :%s!\n",buff);
		sem_post(&sem_w);//原子操作(同时对同一个信号量做加“1”操作的两个线程是不会冲突的；而同时对同一个文件进行读、加和写操作的两个程序就有可能会引起冲突。信号量的值永远会正确地加一个“2”－－因为有两个线程试图改变它),增加信号量的值,当有线程阻塞在这个信号量上时，调用这个函数会使其中的一个线程不在阻塞，选择机制同样是由线程的调度策略决定的
	}
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t tid1,tid2;
	sem_init(&sem_r,0,0);
	/*设置好它的共享选项，并指定一个整数类型的初始值。pshared参数控制着信号量的类型。如果 pshared的值是０，就表示它是当前里程的局部信号量；否则，其它进程就能够共享这个信号量。我们现在只对不让进程共享的信号量感兴趣。（这个参数受版本影响），pshared传递一个非零将会使函数调用失败
	原型int sem_init  (sem_t *__sem, int __pshared, unsigned int __value);　　
sem为指向信号量结构的一个指针；
pshared不为０时此信号量在进程间共享，否则只能为当前进程的所有线程共享；
value给出了信号量的初始值
	*/
	sem_init(&sem_w,0,1);
	
	pthread_create(&tid1,NULL,&fun1,NULL);
	pthread_create(&tid2,NULL,&fun2,NULL);
	
	while(1);
	sem_destory(&sem_p);//释放信号量
	sem_destory(&sem_v);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
		
}