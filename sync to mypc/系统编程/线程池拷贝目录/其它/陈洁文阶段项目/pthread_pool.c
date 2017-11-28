# include "myhead.h"

extern void * file_operations(void *argv);

struct task
{
	void *(*handler)(void *);
	void *arg;
	struct task *next;
};

struct thread_pool
{
	pthread_t *thread_id;					//线程ID存放的位置
	int task_num;							//任务的数量
	pthread_mutex_t lock;					//互斥锁
	pthread_cond_t cond;	              	//条件变量
	int active_threads;						//活跃的线程数量
	struct task *head;						//任务链表的指针
	bool pool_state;						//标志位表示线程池是否存活,true代表存在
	
};



 
struct task *task_init()		//任务链表的初始化
{
	struct task *head = malloc(sizeof(struct task));
	
	head->handler =	NULL;
	head->arg = NULL;
	head->next = NULL;
	
	return head;
}

//解锁
void *closelock(void *arg)
{
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

/*

	功能：处理任务链表中头节点的下一个有效节点（添加任务从链表的尾部添加，处理任务从头部的下一个节点开始）
	

*/
void *thread_fun(void *arg)		//参数arg是初始化好的线程池
{
	struct task *p = malloc(sizeof(struct task)); // 用于指向挖下来的节点
	struct thread_pool *pool = (struct thread_pool *)arg;
	
	while(1)
	{
		//pthread_cleanup_push(closelock,(void *)&(pool->lock)); 		//调用函数closelock防止死锁
		
		pthread_mutex_lock(&pool->lock); 								//上锁
		
		if(pool->task_num == 0 && pool->pool_state == true)
		{
			pthread_cond_wait( &pool->cond, &pool->lock);
		}
		
		if(pool->task_num == 0 && pool->pool_state == false)
		{
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL);
		}
		
		p = pool->head->next;  						//取出头节点的下一个节点
		pool->head->next = p->next; 				//在任务链表中删除这个节点
		p->handler(p->arg);							//处理及节点中的任务， 通过函数指针调用指向的函数
		pool->task_num --;  						//当前任务数量减一
		
		pthread_mutex_unlock(&pool->lock);  		//解锁
		//pthread_cleanup_pop(0); 					//让closelock出栈，但不执行它

		free(p);  									//释放取出来的任务节点
	}
	
	
}


int init_pool(struct thread_pool *pool,int threads_number) 			 //线程池的初始化    threads_number代表有多少个活跃的线程
{
	
	int i;
	
	pthread_mutex_init(&pool->lock,NULL);		//互斥锁的初始化
	pthread_cond_init(&pool->cond,NULL);		//条件变量的初始化
	
	pool->active_threads = threads_number;
	pool->task_num = 0;
	pool->pool_state = true;
	pool->thread_id = malloc(sizeof(pthread_t) * threads_number);
	pool->head = task_init();
	
	for(i = 0;i < threads_number;i ++)
	{
		pthread_create(&(pool->thread_id[i]),NULL,thread_fun,(void *)pool);
	}
	
	if(pool->head == NULL && pool->thread_id == NULL)
	{
		perror("init pool false!\n");
		return -1;
	}
	
	return 0;
}



//添加任务的函数

int task_insert(struct thread_pool *pool,void *(*p)(void *),void *argument)	//尾部插入
{
	//先准备新的任务节点
	struct task *mytask = malloc(sizeof(struct task));
	struct task *q = pool->head;
	mytask->handler = p;
	mytask->arg = argument;
	mytask->next = NULL;
	
	pthread_mutex_lock(&pool->lock);
	

	
	//往任务链表的尾部插入新的任务节点
	while(q->next != NULL)
	{
		q = q->next;
	}
	q->next = mytask;
	
	//当前任务数量加一
	pool->task_num++;
	printf("pool->task_num is %d\n",pool->task_num);

	pthread_mutex_unlock(&pool->lock);
	pthread_cond_signal(&pool->cond);
	
	return 0;
	
}

//增加线程的数量
int add_thread(struct thread_pool *pool,int additional)
{
	
}

//删除线程的数量
int del_thread(struct thread_pool *pool,int del_num)
{
	
}

//线程池的回收（销毁）
int thread_destroy(struct thread_pool *pool)
{
	int i;
	pool->pool_state = false;
	pool->task_num = 0;
	
	for(i = 0;i < (pool->active_threads);i ++)
	{
		pthread_join(pool->thread_id[i],NULL);
	}
	
	pthread_mutex_destroy(&pool->lock);
	pthread_cond_destroy(&pool->cond);
	
	free(pool->thread_id);
	free(pool->head);
	
}


int main(int argc, char const *argv[])
{
	if(argc<3)
	{
		perror("You input parameters is wrong!Please input again\n");
	}
	struct thread_pool *pool = malloc(sizeof(struct thread_pool));
	
	struct flag *NUM = malloc(sizeof(struct flag));
	
	strcpy(NUM->source_name,argv[1]);
	strcpy(NUM->target_name,argv[2]);
	
	init_pool(pool,1);
	task_insert(pool,file_operations,(void *)NUM);
	while(1);
	
	return 0;
}