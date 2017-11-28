#include"myhead.h"
int r_num;
int d_num;
int j;

struct thread_pool//线程池
{
	pthread_t *pthread;
	pthread_mutex_t *mutex;
	pthread_cond_t *cond;
	int task_num;//当前任务链表有效任务的数量
	int active_pthread;//活跃线程的数量
	bool pool_state;//标志位表示线程池是否存在
	struct task *head;//任务链表的头
};

struct task//任务链表的创建
{
	void *(*rountine)(void *);
	void *arg;
	struct task *next;
};

struct file_lists
{
	char p_name[50];
	char f_name[5000];
	char name[50];
	struct file_lists *next;
	int d;
};


struct copy_arg //参数结构体
{
	struct file_lists *r_list;
	struct file_lists *d_list;
	char *name;
};

struct copy_arg *copy_init(char *name,struct file_lists *r_list,struct file_lists *d_list)//copy结构体初始化
{
	struct copy_arg *c=malloc(sizeof(struct copy_arg));
	c->r_list=r_list;
	c->d_list=d_list;
	c->name=name;
	return c;
}

struct file_lists *filec_init()//复制结构体的初始化
{
	struct file_lists *f=malloc(sizeof(struct file_lists));
	struct file_lists *q=malloc(sizeof(struct file_lists));
	f->next=q;
	q->next=NULL;
	return f;
} 

 struct file_lists *file_init()//源文件的初始化
{
	struct file_lists *f=malloc(sizeof(struct file_lists));
	f->next=NULL;
	return f;
}

/*---------------------------结构体----------------------------------------------*/

 void add_clist(struct file_lists *f,char *f_buf,char *n_buf,int d,char *p_buf)//读取目录文件和普通文件链表增加
{
	//add_clist(d,d_buf,p_dirent->d_name,d_num,p_buf);
	
	struct file_lists *new=malloc(sizeof(struct file_lists));
	struct file_lists *q ;
	
	q=f;
		//while(q->next->next!=NULL)
		while(q->next->next!=NULL)
		{
			q=q->next;
		}
		
		new->next=q->next;
		strcpy(new->f_name,f_buf);
		strcpy(new->name,n_buf);//传文件名字
		new->d=d;
		if(d!=1)
			strcpy(new->p_name,p_buf);
		q->next=new;
		
}


/*------------------------链表添加函数------------------------------------------------*/

int dir_cread(char *name,struct file_lists *r,struct file_lists *d,int d_num,char *p_buf)//文件目录读取
{
	int i=0;
	d_num++;
	char q_buf[50];
	strcpy(q_buf,p_buf);
	printf("2\n");
	char n_buf[5000];
	char r_buf[5000];
	char d_buf[5000];
	DIR *dir=opendir(name);
	printf("3\n");
	if(dir==NULL)
	{
		perror("open dir failse!");
		return 0;
	}
	
	struct dirent *p_dirent = malloc(sizeof(struct dirent));
	
	
	while((p_dirent=readdir(dir))!=NULL)
	{
	
		if(strcmp(p_dirent->d_name,".")&&strcmp(p_dirent->d_name,".."))//忽略. ..文件
		{
			printf("........\n");
			
			if(p_dirent->d_type==4)
			{
			
				sprintf(d_buf,"%s%s/",name,p_dirent->d_name);
				strcpy(n_buf,p_dirent->d_name);//复制文件名字
				
				add_clist(d,d_buf,p_dirent->d_name,d_num,p_buf);
				
				strcpy(p_buf,p_dirent->d_name);
				
				dir_cread(d_buf,r,d,d_num,p_buf);
				
				//printf("d_buf=%s\n",d_buf);
			}
			
			else if(p_dirent->d_type==8)
			{
				
				sprintf(r_buf,"%s%s",name,p_dirent->d_name);
				
				add_clist(r,name,p_dirent->d_name,d_num,q_buf);
				
			}

		}
	}
	return 0;
}


/*------------------目录读取函数------------------------------------------------------------*/


void task_add(struct thread_pool *p,void *(*rountine)(void *),void *arg)//线程池任务增加
{
	struct task *q;
	
	struct task *new=malloc(sizeof(struct task));
	q=p->head;
	
	
	pthread_mutex_lock(p->mutex);//增锁
	if(p->head->next==NULL)
	{
		p->head->next=new;
		new->rountine=rountine;
		new->arg=arg;
		new->next=NULL;
	}
	else
	{
		while(q->next!=NULL)
		{
			q=q->next;
		}	
		q->next=new;
		new->rountine=rountine;
		new->arg=arg;
		new->next=NULL;
	}
	p->task_num++;//当前任务链表有效任务的数量增加
	pthread_mutex_unlock(p->mutex);//解锁
	
	pthread_cond_broadcast(p->cond);//解除阻塞
}


void closemutex(void *arg)//防止锁死的解锁函数
{
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}


void *thread_fun(void *arg)//线程池任务执行并减去数量
{
	struct thread_pool *p=(struct thread_pool *)arg;//转化成结构体
	
	struct task *q;
	while(1)
	{
		pthread_cleanup_push(closemutex,(void *)&(p->mutex));//调用函数防止锁死
		pthread_mutex_lock(p->mutex);
		while(p->task_num==0&&p->pool_state==true)
		{
			pthread_cond_wait(p->cond,p->mutex);//解锁并阻塞
		}
		while(p->task_num==0&&p->pool_state==false)
		{
			pthread_exit(NULL);
		}
		q=p->head->next;
		p->head->next=q->next;
		p->task_num--;//当前任务链表有效任务的数量减少
		pthread_mutex_unlock(p->mutex);//解锁
		pthread_cleanup_pop(0);//让closemutex出栈，但不执行他
		q->rountine(q->arg);//执行代码
		free(q);
	}	
	
		
}



int pool_init(struct thread_pool *mypool,int threadnum)
{
	int i;
	//初始化线程池结构体
	mypool->pthread=malloc(threadnum*sizeof(pthread_t));
	
	mypool->mutex=malloc(sizeof(pthread_mutex_t));//锁初始化
	mypool->cond=malloc(sizeof(pthread_cond_t));//缓冲初始化
	
	pthread_mutex_init(mypool->mutex,NULL);
	pthread_cond_init(mypool->cond,NULL);
	
	mypool->task_num=0;
	mypool->active_pthread = threadnum;
	mypool->pool_state=true;
	
	mypool->head=malloc(sizeof(struct task));//链表头初始化
	mypool->head->next=NULL;
	for(i=0;i<threadnum;i++)
	{
		pthread_create(&(mypool->pthread[i]),NULL,thread_fun,(void *)mypool);
		
	}
	return 0;
}

int add_thread(struct thread_pool *mypool,int add_num)//新增加的线程
{
	//task_add(mypool,rountine,NULL);
	mypool->active_pthread++;
}

int del_thread(struct thread_pool *mypool,int delnum)//删除线程
{
	pthread_cancel(mypool->pthread[delnum]);
	mypool->active_pthread--;
	return 0;
}

int threadpool_destroy(struct thread_pool *mypool)//线程的回收
{
	int i;
	mypool->pool_state=false;
	mypool->task_num=0;
	for(i=0;i<mypool->active_pthread;i++)
	{
		pthread_join(mypool->pthread[i],NULL);
	}
}

/*------------------------------线程池创建以及初始化---------------------------------------------*/

int copy(char *t_buf,char *g_buf)//文件目录复制函数
{
	int fd1 = open(g_buf,O_RDWR);
	if(fd1 == -1)
	{
		perror("打开文件失败！\n");
		return -1;
	}
	int fd2 = open(t_buf,O_CREAT|O_RDWR,0777);
	if(fd2==-1)
	{
		perror("open false！\n");
		return -1;
	}
			
	int filesize = lseek(fd1,0,SEEK_END); //读取fd1文件的大小
	char buf1[filesize];
	bzero(buf1,0);
			
	lseek(fd1,0,SEEK_SET); //重新将文件的偏移挪回到起始位置
	read(fd1,buf1,filesize);// 读取源文件里面的内容
	write(fd2,buf1,filesize);// 将读取的内容写入到目标文件中去
			
	close(fd1);
	close(fd2);

}


//文件目录判断是不相等以及复制
void copy_read(char *name,int n_num,struct file_lists *r_list1,struct file_lists *r_list2,struct file_lists *d_list1,struct file_lists *d_list2)
{	
	n_num++;
	//printf("n_num:%d\n",n_num);
	if(n_num==1)
	{
		
	}
	int s,p1,p2;
	int y=0;
	char b;
	int n=0;
	
	char f1_buf[500];
	char f2_buf[500];
	char f3_buf[500];
	char r1_buf[500];
	char r2_buf[500];
	char r3_buf[500];
	char name1[500];
	char name_buf[50];
	char d_buf[500];
	struct file_lists *r1=r_list1;
	struct file_lists *d2;
	struct file_lists *r2;
	struct file_lists *r3;
	struct file_lists *flag1;
	struct file_lists *flag2;	
	struct file_lists *d1=d_list1;
	struct file_lists *d3;

	while(d1->next!=NULL)//头目录文件
	{
		
		int i=0;
		//printf("d_list1:%s\n",d_list1->name);
		//printf("d1:%s\n",d1->next->name);
		s=strcmp(d1->next->name,d_list1->name);
		if(s==0)
		{
			break;
		}
		
		d2=d_list2;
		while(d2->next!=NULL)//读取目标链表目录文件
		{
			//printf("d2:%s\n",d2->next->name);
			if(((d2->next->d)==(d1->next->d))&&(strcmp(d1->next->p_name,d2->next->p_name)==0))//当源目录和目标目录级数相等以及各自上一级的名字相等时
			{
				
				if(strcmp(d1->next->name,d2->next->name)==0)//如果目录名字相等
				{
					i++;
/* 					printf("there is a same dir,do you want to cover it?[Y/N]\n");
					scanf("%c",&b); 
					if(b =='y')//判断要覆盖*/
					
						r1=r_list1;
						while(r1->next!=NULL)//读取源目录链表文件
						{
							if((strcmp(r1->next->p_name,d1->next->name))==0)//判断文件的目录名与目录是否一致
							{
								r2=r_list2;
								while(r2->next!=NULL)//读取目标链表文件
								{
										
									p2=strcmp(r1->next->name,r2->next->name);
									p1=strcmp(r2->next->p_name,d2->next->name);
									//判断目标文件的目录名与目录是否一致
									if((p1==0)&&(p2==0)&&((r_list1->next->d)==(r2->next->d)))//判断源文件名字和目标目录是否有重复文件
									{
										sprintf(r2_buf,"%s%s",r2->next->f_name,r2->next->name);											
										sprintf(r1_buf,"%s%s",r1->next->f_name,r1->next->name);	
										sprintf(r3_buf,"%s%s",r2->next->f_name,r1->next->name);	
										copy(r1_buf,r3_buf);
										r1=r1->next;//换
										break;
									} 
									else
										r2=r2->next;
								}		//发现没有相同名字的文件
								sprintf(f1_buf,"%s%s",name,r1->next->name);
								sprintf(f2_buf,"%s%s",r1->next->f_name,r1->next->name);
								copy(f1_buf,f2_buf);
								r1=r1->next;//换
							}
							else//目录名与名字不一致
							{
									r1=r1->next;
							}		
						}
					
					d2=d2->next;
				}
				else
				{
					d2=d2->next;
				}
						
			}
			else
			{
				d2=d2->next;
			}			
		}
		
		if(((d1->next->d)==n_num)&&(n==0))//没有相同名字文件直接复制
		{
				sprintf(d_buf,"%s%s/",name,d1->next->name);
				
				if(mkdir(d_buf,0777)==0)
				{
					while(r1->next!=NULL)
					{
						if(strcmp(r1->next->p_name,d1->next->name)==0)
						{
							//printf("%s,%s,%s\n",d3->next->name,r3->next->name,r3->next->p_name);
							bzero(r1_buf,500);
							bzero(r1_buf,500);
							sprintf(r1_buf,"%s%s",d_buf,r1->next->name);
							sprintf(r2_buf,"%s%s",r1->next->f_name,r1->next->name);
							printf("r1:%s,r2:%s\n",r1_buf,r2_buf);
							copy(r1_buf,r2_buf);
						}
						r1=r1->next;
					} 
					
					
					copy_read(d_buf,n_num,r_list1,r_list2,d_list1,d_list2);
					
					
				}
				
				
				
		}
				d1=d1->next;//换
	
		
	}
	

	printf("1.1,%d\n",n_num);
	r3=r_list1;
	while(r3->next!=NULL)
	{
		if((r3->d==1)&&(n_num==1))
		{
			//printf("r3:%s,name:%s,num:%d\n",r3->next->name,name,n_num);
			sprintf(r1_buf,"%s%s",name,r3->name);
			sprintf(r2_buf,"%s%s",r3->f_name,r3->name);
			//printf("r1:%s,r2:%s\n",r1_buf,r2_buf);
			copy(r1_buf,r2_buf);
		}
		r3=r3->next;
	}
	
	
	printf("2.2,%d\n",n_num);
	return ;
}


void *file_copy(void *arg)//输入到线程池的函数
{
	struct copy_arg *c=(struct copy_arg *)arg;
	char p_buf[50];
	int n_num1=0;
	int n_num2=0;
	struct file_lists *r2=filec_init();
	struct file_lists *d2=filec_init();
	dir_cread(c->name,r2,d2,n_num1,p_buf);
	copy_read(c->name,n_num2,c->r_list,r2,c->d_list,d2);
}
/*---------------------------文件复制函数------------------------------------*/

int main(int argc,char **argv)//主函数
{
	int n;
	int i=0;
	int d_num=0;
	char name[50];
	char p_name[50];
	struct file_lists *r=filec_init();
	struct file_lists *d=filec_init();
	
	dir_cread(argv[1],r,d,d_num,p_name);
	
	struct copy_arg *c=copy_init(argv[2],r,d);
	struct thread_pool *mypool=malloc(sizeof(struct thread_pool));
	
	pool_init(mypool,2);
	
	task_add(mypool,file_copy,(void *)c);
	//task_add(mypool,file,NULL);
	//threadpool_destroy(mypool);
	sleep(3);
	return 0;

	
}
