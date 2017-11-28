////////////////////////////
//线程池
//作用：当你设计一个压力性能测试框架的时候，需要连续产生大量的并发操作
//使用实例:
//结构内容:结构体表示
//封装实现：任务(单)链表
////苏:添加任务（信号后）再解锁，
//
//封装函数目标：1.实现什么功能:1.1线程池结构体初始化；
//2.好不好调用 3.复习单链表（数据结构）
//
//需要明确:什么时候用锁(添加任务->封装在线程里->节点)   什么时候用条件变量
//增加任务就唤醒沉睡的线程
//线程里头节点下一节点就是新节点
//对void *类型数据要转换后使用
//函数返回类型:必要才用到结构体(指针)类型
////////////////////////////
#include "myhead.h"

#define MAX_TASK_NUM	1000
#define MAX_ACTIVE_THREADS	20
////////////////////////////
//任务节点:任务是由节点决定的
////////////////////////////
struct task{
	void *(*handler)(void *arg);//函数指针，代表任务,任务实例最终形成一条单向链表
	void *arg;//传递给函数指针的参数
	struct task *next;
};

////////////////////////////////////////////////////////
//线程池实例，包含一个线程池所有信息
//重点：如何分析并封装
//理解：三者的关系->线程(thread_pool),线程实现任务(task),新增节点？
////////////////////////////////////////////////////////
typedef struct thread_pool{
	pthread_t *id;//用于存放多个线程的id号，不是int型,数组或指针（适当添加）
	unsigned task_num;//任务链表中有效任务数量，不是void *   int
	pthread_mutex_t lock;//互斥锁与条件变量协调对任务链表的访问
	pthread_cond_t cond;
	unsigned active_pthread;//活跃线程的数量  不用int,unsigned保证输入的线程数量为正
	bool pool_state;//标志位,表示线程池是否存在
	struct task *head;//任务链表头
}thread_pool;

//////////////////////////////
//解锁
//////////////////////////////
void closemutex(void *arg){
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

//////////////////////////////
//线程处理函数,实现任务的桥梁
//功能:扣出节点并分析指针指向的任务,从头节点的下一个有效节点
//设计思路：知道表头,扣下来后还要增加有效任务节点
//自己想的:用来控制线程创建时函数的设计
//防止意外中断死锁：push   减当中被取消了
//临界区资源->lock&unlock之间代码
//添加任务从链表尾部进行添加
//处理任务从头部下一节点开始
//谁抢到给谁
//////////////////////////////
void *thread_fun(void *arg){//struct thread_pool *mypool
	thread_pool *pool=(thread_pool *)arg;//转换下使用
	//传什么参数,参数arg是初始化好的线程池
	/*  线程处理函数调用task结构体声明新任务节点而不是thread_pool*/
	struct task *q;//指向处理任务节点(被取出来包括的那个节点)
	while(1){
		//调用函数防止死锁
		pthread_cleanup_push(closemutex,(void *)(&(pool->lock)));//(void *)&pool->thread_mutex)
		pthread_mutex_lock(&pool->lock);//上锁   ->优先级比&高
		
		//有效任务为0,线程池关闭(wait阻塞),直到添加任务后task不为0，调用signal唤醒（不在这唤醒，以广播方式通知所有进程解除阻塞）
		while(pool->task_num == 0 && pool->pool_state==true){
			pthread_cond_wait(&pool->cond, &pool->lock);
		}
		//任务链表中没有任务并且线程池也关闭了
		if(pool->task_num==0 && pool->pool_state==false){//if
			pthread_mutex_unlock(&pool->lock);//记住，当初想不到,为什么要这样用；
			pthread_exit(NULL);
		}
		
		/*复习链表的插入,这里是做任务而不是尾插新任务*/
		q = pool->head->next;
		pool->head->next = q->next;//让头节点指向q的下一个节点，后续用free释放
		//q->next = mypool->head->next->next;
		//mypool->head->next = q;//处理q就是执行新任务	
		//当前任务数量减一,thread_pool里的成员还需要进行什么操作吗
		pool->task_num --;//+= 1是否有同样效果?    为什么不用q
		
		//取出头节点后面的一个节点:处理任务并取消
		//当前任务数量减一
		pthread_mutex_unlock(&pool->lock);
		pthread_cleanup_pop(0);//让closemutex出栈,但不执行
	
		//处理取出来的节点的任务
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);//当初想不到
		(q->handler)(q->arg);//q->handler(q->arg);//task里的函数指针,通过传入arg调用handler，再通过函数指针调用指向的函数
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);//当初想不到
		//释放取出来的任务节点
		free(q); 
	}
	//添加任务与互斥锁结合
	/* pthread_mutex_init(&mypool->lock);
	pthread_cond_init(&mypool->cond); */
	
	pthread_exit(NULL);//return new;
}

//////////////////////////////
//线程池初始化
//作用:初始化线程池结构体,创建多个线程
//pthread_create()第三个参数为线程池的功能函数
//法一 申请堆空间，新变量去设置;法二：返回地址，指针改变相当于重新初始化
//////////////////////////////
/* struct task */bool pool_init(thread_pool *mypool,unsigned int threadnum){
	pthread_mutex_init(&mypool->lock,NULL);	
	pthread_cond_init(&mypool->cond,NULL);

	mypool->id = malloc(MAX_ACTIVE_THREADS * sizeof(pthread_t));
	mypool->task_num = 0;//开始没有任务,统计任务链表中有效任务的变量
	mypool->active_pthread = threadnum;
	mypool->pool_state = false;//true;
	mypool->head = malloc(sizeof(struct task));//struct thread_pool;
	
	//判断头节点是否为空
	if(mypool->head == NULL || mypool->id == NULL){
		perror("申请空间失败");
		return false;
	}
	
	//创建threadnum个线程并封装进thread_pool
	int i;
	for(i=0;i < mypool->active_pthread;i++){//threadnum
	//这是一个指针,在thread_pool里面,而不用pthread_t pid;
		if(pthread_create(&((mypool->id)[i]),NULL,thread_fun,(void *)mypool)!=0){//成功返回0
			//pthread_create(&(mypool->id[i]),NULL,thread_fun,(void *)mypool);
			perror("创建线程出错");
			return false;//why
		}
	}
	return true;//0
}

////////////////////////////////////////////
//添加任务函数:添加节点并添加参数,head要共享
//与thread_fun是竞争关系
////////////////////////////////////////////2.函数指针 3.给函数指针的参数：有写没空
/* struct task */int task_add(thread_pool *mypool,void *(*p)(void *arg),
								void *argument){
	//先准备好新的任务节点（封装）
	struct task *new = malloc(sizeof(struct task));
	//判断是否添加成功
	if(new == NULL){
		perror("申请空间失败");
		return false;//定义的还是-1同理
	}
	new->handler = p;//&((*handler)(void *arg)) = (*p)(void *):
	new->arg = argument;
	new->next = NULL;//malloc(sizeof(struct task));	
	pthread_mutex_lock(&mypool->lock);//为什么上锁
	if(mypool->task_num >= MAX_TASK_NUM){//超过就释放
		pthread_mutex_unlock(&mypool->lock);
		fprintf(stderr,"任务超过最大数\n");
		free(new);
		return false;
	}
	//往任务链表尾部插入新的任务节点
	/* mypool->head->next = new; */
	struct task *tmp =mypool->head;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	tmp->next = new;
	mypool->task_num++;//当前任务数量加1
	//唤醒（解除阻塞）并上锁,为什么
	pthread_mutex_unlock(&mypool->lock);
	pthread_cond_signal(&mypool->cond);
	//pthread_cond_broadcast(&mypool->cond);//以广播形式通知所有的线程解除阻塞->销毁用
	
	//统计任务链表中有效任务的变量?
	return true;//new 为什么不是返回新增的任务
}


//////////////////////////////
//线程的复用
//////////////////////////////
int thread_refresh(){
	//功能：线程使用完后重置而不销毁后重建
}

////////////////////////////////
//新增线程
//返回:实际加了多少
//模仿，需要记忆
////////////////////////////////
int thread_add(thread_pool *mypool,unsigned additional){
	//判断新增的参数是否为0
	if(additional == 0){
		return 0;
	}
	//声明新增后总的线程数变量,新增
	unsigned total_threads = mypool->active_pthread + additional;
	
	int i,actual_increment = 0;//不一定添加成功，实际增加
	for(i=mypool->active_pthread;//这时的i不是从0开始了
	i<total_threads&&i<MAX_ACTIVE_THREADS;i++){
		if(pthread_create(&((mypool->id)[i]),NULL,thread_fun,(void *)mypool)!=0){//语句合并
			perror("添加线程失败");
			if(actual_increment == 0){
				return -1;
			}
			break;
		}
		actual_increment++;
/*获取新增线程的属性
	调用pthread_create()生成
	更新: 活跃线程数量  id   总的*/
	}
	mypool->active_pthread += actual_increment;
	return actual_increment;
}

////////////////////////////////
//删除线程
//对传进来的参数进行判断，为0，就不用操作了
////////////////////////////////
int del_thread(thread_pool *mypool,unsigned int delnum){
	if(delnum==0){
		return mypool->active_pthread;
	}
	//定义剩余线程变量
	int remain_threads= mypool->active_pthread-delnum;
	remain_threads=remain_threads>0 ?remain_threads:1;//剩余线程是否大于0,至少保留1个!
	
	//用上面剩余变量和线程id控制删除,注意下标要减1
	int i;
	for(i=mypool->active_pthread-1;i>remain_threads-1;i--){
		errno = pthread_cancel(mypool->id[i]);//errno是系统定义的吗
			if(errno != 0){
				break;
			}//花括号可省略,判断出错的另外一种写法
	}
	//对操作结果进行判断，循环变量仍在最后一位则退出，否则返回存余线程数?
	if(i == mypool->active_pthread - 1)
		return -1;
	else{
		mypool->active_pthread = i+1;//局部变量i怎么理解
		return i+1;
	}
	
	/*之前逻辑:循环调用pthread_cancel();
	解锁->不用*/
}

////////////////////////////////
//线程池销毁
////////////////////////////////
/* int */bool thread_pool_destroy(thread_pool *mypool){//函数返回值用了bool后面的return 就要跟false或true
	//销毁  锁 条件变量  指针id 释放线程池结构体指针mypool
	mypool->pool_state = false;//关闭线程池
	pthread_cond_broadcast(&mypool->cond);//之前没想到是这里用以广播形式通知所有的线程解除阻塞
	int i;
	for(i=0;i<mypool->active_pthread;i++){
		errno = pthread_join(mypool->id[i],NULL);
		if(errno != 0){
			printf("回收线程 id[%d] 错误:%s\n",i,strerror(errno));//之前没想到竟然返回那么多东西,销毁失败还能隔开的吗
		}else{
			printf("[%u] 已回收\n",(unsigned)mypool->id[i]);//用结构体指针显示已销毁的线程数吧
		}
	}
	
	free(mypool->head);
	free(mypool->id);
	free(mypool);
	//mypool->task_num = 0;
	return true;
}

void *fun1(void *arg){
	int n = (int)arg;
	
	printf("[%u][%s]==>新增任务将在%d秒内完成...\n",(unsigned)pthread_self(),__FUNCTION__,n);//__FUNCTION__为所属函数
	sleep(n);
	printf("[%u][%s]==>新增任务完成!\n",(unsigned)pthread_self(),__FUNCTION__);
	return NULL;
}

void *fun2(void *arg){
	int i = 0;
	while(1){
		sleep(1);
		printf("%d秒\n",++i);
	}
	//printf("添加新任务了\n");
}

int main(void){
	pthread_t a;
	pthread_create(&a,NULL,fun2,NULL);
	//1.准备并初始化线程池
	thread_pool *main = malloc(sizeof(thread_pool));
	pool_init(main,2);//第二个参数为需要初始化的线程个数

	//2.投掷任务  //往任务链表添加任务
	printf("新增3个任务\n");
	task_add(main,fun1,(void *)(rand()%10));//第三个参数为传给线程处理函数的参数?  线程池功能函数
	task_add(main,fun1,(void *)(rand()%10));
	task_add(main,fun1,(void *)(rand()%10));
	
	//3.测试当前线程池线程数量
	printf("当前线程数量:%d\n",del_thread(main,0));
	
	sleep(9);
	//4.再次投掷任务
	printf("再新增2个任务\n");
	task_add(main,fun1,(void *)(rand()%10));
	task_add(main,fun1,(void *)(rand()%10));
	
	//5.再次添加线程
	int threadnum=2;//线程数量
	thread_add(main,2);
	
	sleep(5);
	
	//6.移除线程
	printf("从线程池移除3个线程,""移除后线程数量:%d\n",del_thread(main,3));//移除后还剩3?
	
	//7.销毁线程池
	thread_pool_destroy(main);
	
	return 0;
	
	/*
	思路理顺前：
	//1.准备并初始化线程池
	struct thread_pool *main = malloc(sizeof(struct thread_pool));
	pool_init(main,2);//第二个参数为需要初始化的线程个数

	pthread_self();//获取线程自身ID
	//往任务链表添加任务
	//2.封装函数表示任务
	task_add(main,fun1,arg);
	
	int threadnum;//线程数量
	
	//协调多线程防止打架：互斥锁和条件变量控制
	pthread_mutex_init();
	pthread_cond_init();
	
	return 0;
	*/
}

/*
	 _    _    _______
	| |  | |  | ______|    `|  |  |
	| |  | |  | |            `|  |
	| |__| |  | |_____  
	|  __  |  |  _____|
	| |  | |  | |
	| |  | |  | |_____
	|_|  |_|  |_______|
	
	总结:自己再总结设计线程池的一般思路，不断完善，复习巩固之前学的内容，多敲代码，看笔记练习
	函数设计的本意  添加任务/线程->是否成功(线程池初始化也是)    结构体表示抽象概念
*/