#include"myhead.h"
#include"kernel_list.h"
static pthread_mutex_t lock2;
static int flag=0;
static int over=0;
struct fdmsg
{
	char path[256];
	char obpath[256];
	char name[256];
	int size;
	mode_t mymode;
	struct thread_pool *mypool;
};

struct k_list
{
	struct fdmsg file;
	struct list_head mylist;
};

struct task
{
	void *(*handler)(void *);
	void *arg;
	struct task *next;
};

struct k_list *create_new()
{
	struct k_list *new=malloc(sizeof(struct k_list));
	bzero(new,sizeof(struct k_list));
	new->mylist.next=NULL;
	new->mylist.prev=NULL;
	return new;
}

struct task *task_create()
{
	struct task *new=malloc(sizeof(struct task));
	new->handler=NULL;
	new->arg=NULL;
	new->next=NULL;
	return new;
};

struct thread_pool
{
	char dir[256];
	char obdir[256];
	int effect_task;
	int effect_file;
	int wing_pth;
	int thread_on_num;
	bool on_off;
	struct task *task_head;
	struct k_list *file_head;
	struct k_list *current_msg;
	pthread_t arr_id[100];
	pthread_t current_id;
	pthread_mutex_t lock1;
	pthread_cond_t cond1;
};

struct task *task_del(struct task *head,struct task *del)
{
	while(head->next!=del)
	head=head->next;
	head->next=del->next;
	del->next=NULL;
}

void *execute(void *arg)                      //功能函数
{
	pthread_t selfid;
	selfid=pthread_self();
	struct thread_pool *mypool=(struct thread_pool *)arg;
	struct task *work_point;
	void (*unlock)(void *)=(void *)pthread_mutex_unlock;
	void *(*work_handle)(void *);
	void *work_arg;
	// struct task *work_task;
	// work_task=mypool->task_head->next;
	while(1)
	{
	pthread_cleanup_push(unlock,(void *)&mypool->lock1);
	pthread_mutex_lock(&mypool->lock1);
	while(mypool->on_off==false || mypool->effect_task==0) 
		{
			pthread_cond_wait(&mypool->cond1,&mypool->lock1);
			if(mypool->on_off==false) 
			{
				mypool->current_id=selfid;
				pthread_exit(0);
			}
			pthread_mutex_unlock(&mypool->lock1); 
		}
	mypool->effect_task--;
	work_handle=mypool->task_head->next->handler;
	work_arg=mypool->task_head->next->arg;
	work_point=task_del(mypool->task_head,mypool->task_head->next);
	mypool->wing_pth++;
	work_handle(work_arg); //执行对应函数功能
	mypool->wing_pth--;
	pthread_mutex_unlock(&mypool->lock1);
	pthread_cleanup_pop(0);
	}
}

int thread_pool_init(struct thread_pool *mypool,struct task *head,struct k_list *fhead,char *dir,char *obdir,struct k_list *current_msg)      //线程池初始化函数
{
	
	bzero(mypool->arr_id,sizeof(mypool->arr_id));
	int i;
	INIT_LIST_HEAD(&fhead->mylist);
	INIT_LIST_HEAD(&current_msg->mylist);
	flag=1;
	over=1;
	mypool->on_off=true;
	mypool->task_head=head;
	mypool->file_head=fhead;
	mypool->current_msg=current_msg;
	mypool->effect_task=0;
	mypool->thread_on_num=10;
	strcpy(mypool->dir,dir);
	strcpy(mypool->obdir,obdir);
	pthread_mutex_init(&mypool->lock1,NULL);
	pthread_mutex_init(&lock2,NULL);
	pthread_cond_init(&mypool->cond1,NULL);
	mypool->wing_pth=0;
	for(i=0;i<10;i++)
	{
		pthread_create(&mypool->arr_id[i],NULL,execute,(void *)mypool);
	}
}

struct task task_add(void *(*func)(void *),void *arg,struct thread_pool *mypool) //添加任务链表函数
{
	struct task *new;
	struct task *tail;
	tail=mypool->task_head;
	new=task_create();
	new->handler=func;
	new->arg=arg;
	new->next=NULL;
	if(mypool->task_head->next==NULL) 
	{
		mypool->task_head->next=new;
	}
	else
	{
		while(tail=tail->next)
		{
			if(tail->next==NULL)
			{
				tail->next=new;
				break;
			}
		}
	}
	mypool->effect_task++;
	pthread_cond_signal(&mypool->cond1);
}	

int del_thread(int num,struct thread_pool *mypool)			//删除线程函数
{
	int i=0;
	if(mypool->thread_on_num<num)
	{
		printf("\n");
		printf("需要回收线程大于现有有效线程总数，无法回收\n");
		return -1;
	}
	if(mypool->thread_on_num==0)
	{
		printf("\n");
		printf("有效线程总数已经为零\n");
		return -1;
	}
	if((mypool->thread_on_num-num)<mypool->wing_pth)
	{
		printf("\n");
		printf("回收线程中有正在工作的线程，请稍后再次输入\n");
		return -1;
	}
	mypool->on_off=false;
	for(;i<num;i++)
	{
		pthread_cond_signal(&mypool->cond1);
		usleep(2000);
		pthread_join(mypool->current_id,NULL);
		mypool->thread_on_num--;
	}
		mypool->on_off=true;
	printf("\n");
	printf("回收完毕\n");
	return 0;
}

int add_thread(int num,struct thread_pool *mypool)		//添加线程函数
{
	int i=0;
	int m;
	while(1)
	{
		if(num<=0)
		{
			printf("输入错误，请重新输入:\n");
			scanf("%d",&num);
		}
		else break;
	}
	while(mypool->arr_id[i]!=0) i++;
	mypool->on_off=false;
	m=i+num;
	for(;i<m;i++)
	{
		pthread_create(&mypool->arr_id[i],NULL,execute,(void *)mypool);
		mypool->thread_on_num++;
	}
	sleep(1);
	mypool->on_off=true;
	printf("\n");
	printf("添加完成\n");
}

int out(struct thread_pool *mypool)		//回收线程池资源函数
{
	int i;
	struct task *p=mypool->task_head->next;
	struct task *q;
	for(i=0;i<mypool->thread_on_num;i++)
	{
		pthread_cancel(mypool->arr_id[i]);
		
	}
	mypool->on_off=false;
	while(p!=NULL)  
	{
		q=p;
		p=p->next;
		q->next=NULL;
		free(q);
	}
	mypool->task_head->next=NULL;
	pthread_mutex_destroy(&lock2);
	pthread_mutex_destroy(&mypool->lock1);
	pthread_cond_destroy(&mypool->cond1);		
	return 0;
}

void *copy(void *arg)		//复制函数
{
	int fd;
	int ret;
	char p;
	char *buf;
	void *back;
	struct fdmsg *file=(struct fdmsg *)arg;
	void (*unlock)(void *)=(void *)pthread_mutex_unlock;;
	struct k_list *current_msg=malloc(sizeof(struct k_list));
	fd=open(file->path,O_RDONLY);
		if(fd==-1)
		{
			printf("文件%s读取失败\n",file->path);
			perror("open wrong");
			return ;
		}
		buf=malloc(file->size);
		ret=read(fd,buf,file->size);
		// printf("大小%d\n",pop->file.size);
		// printf("文件内容%s\n",buf);
		if(ret==-1)
		{
			perror("read wrong");
			return;
		}
		close(fd);
		fd=open(file->obpath,O_CREAT | O_EXCL | O_WRONLY,0755);
		if(fd==-1)
		{
			while(1)
			{
				pthread_cleanup_push(unlock,(void *)&lock2);
				pthread_mutex_lock(&lock2);
				usleep(2000);
				printf("目标文件%s已经存在，是否覆盖(y/n)",file->obpath);
				scanf("%c",&p);
				getchar();
				pthread_mutex_unlock(&lock2);
				pthread_cleanup_pop(0);
				if(p=='y')
				{
					fd=open(file->obpath,O_CREAT | O_TRUNC | O_WRONLY,0755);
					if(fd==-1)
					{
						perror("create wrong");
						return;
					}
					break;
				}
				else if(p=='n')
				{
					break;
				}
			}
			if(p=='n')
			{
				return;
			}
		}
		strcpy(current_msg->file.obpath,file->obpath);
		list_add_tail(&current_msg->mylist,&file->mypool->current_msg->mylist);
		ret=write(fd,buf,file->size);
		list_del(&current_msg->mylist);
		free(current_msg);
		current_msg=NULL;
		if(ret==-1)
		{
			perror("write");
			return;
		}
		
		free(buf);
		buf=NULL;
		
		printf("文件%s复制完成\n",file->path);
		close(fd);
		return;
}

int read_dir(char *dir,char *obdir,struct thread_pool *mypool)		//读取目录函数
{
	int ret;
	int nsize;
	int obnnize;
	DIR *fd;
	char sondir_path[256];
	char obsondir_path[256];
	struct stat file_msg;
	struct dirent *dir_msg;
	struct fdmsg *new;
	nsize=strlen(dir);
	obnnize=strlen(obdir);
	fd=opendir(dir);		//用于第一次使用该函数判断用户输入的源目录路径是否正确
	if(fd==NULL)
	{
		printf("源目录路径错误，复制失败\n");
		perror("opendir");
		return -1;
	}
	ret=mkdir(obdir,0755);	//用于第一次使用该函数判断用户输入的目标目录路径是否正确
	if(opendir(obdir)==NULL)
	{
		perror("make sondir");
		return -1;
	}
	dir_msg=readdir(fd);
	while(dir_msg!=NULL)
	{
		if(*(dir_msg->d_name)=='.')
		{
			dir_msg=readdir(fd);
			continue;
		}
		if(dir_msg->d_type!=DT_DIR)
		{
			new=malloc(sizeof(struct fdmsg));
			if(*(dir+nsize-1)=='/')	sprintf(new->path,"%s%s",dir,dir_msg->d_name);	//判断预防用户输入地址格式不同
			else sprintf(new->path,"%s/%s",dir,dir_msg->d_name);
			if(*(obdir+obnnize-1)=='/') sprintf(new->obpath,"%s%s",obdir,dir_msg->d_name);
			else sprintf(new->obpath,"%s/%s",obdir,dir_msg->d_name);
			stat(new->path,&file_msg);
			new->size=file_msg.st_size;
			new->mypool=mypool;
			task_add(copy,new,mypool);
			new=NULL;
			
		}
		else if(dir_msg->d_type==DT_DIR)
		{
			if(*(dir+nsize-1)=='/')	sprintf(sondir_path,"%s%s",dir,dir_msg->d_name);	//判断预防用户输入地址格式不同
			else sprintf(sondir_path,"%s/%s",dir,dir_msg->d_name);
			if(*(obdir+obnnize-1)=='/') sprintf(obsondir_path,"%s%s",obdir,dir_msg->d_name);
			else sprintf(obsondir_path,"%s/%s",obdir,dir_msg->d_name);
			ret=mkdir(obdir,0755);			
			if(ret==-1)		//此判断用于第一次使用该函数时的判断目用户输入的标目录路径是否正确
			{
				if(opendir(obdir)==NULL)
				{
					printf("目标路径错误，请重新输入");
					perror("obdir");
					return -1;
				}
			}
			read_dir(sondir_path,obsondir_path,mypool);    //递归调用read_dir函数
		}
		dir_msg=readdir(fd);
	}
	return 0;
}


void *check_task(void *arg)			//实时监测复制情况线程
{
	sleep(1);
	int i;
	struct thread_pool *mypool=(struct thread_pool *)arg;
	while(1)
	{
		if(mypool->effect_task==0 && mypool->wing_pth==0)
		{
			printf("复制完毕\n");
			over=0;
			return;
		}
		else if(flag==0)
		{
			printf("复制中断\n");
			return;
		}
		printf("正在工作的线程有:%d\n",mypool->wing_pth);
		printf("剩余任务数有:%d\n",mypool->effect_task);
		sleep(1);
	}
}

void *read_task(void *arg)
{
	struct thread_pool *mypool=(struct thread_pool *)arg;
	read_dir(mypool->dir,mypool->obdir,mypool);
}

void control(int sig)			//中断信号函数
{
	flag=0;
	printf("flag = %d\n",flag);
}

int main(int argv,char *argc[])
{
	char order[256]={0};
	int task_num;
	int thread_num;
	pthread_t check_t;
	pthread_t read_t;
	struct thread_pool *mypool;
	mypool=malloc(sizeof(struct thread_pool));
	struct task *task_head;
	struct k_list *file_head;
	struct k_list *current_msg;
	struct k_list *pop;
	task_head=task_create();
	file_head=create_new();
	current_msg=create_new();
	thread_pool_init(mypool,task_head,file_head,argc[1],argc[2],current_msg);
	signal(SIGINT,control);
	pthread_create(&check_t,NULL,check_task,(void *)mypool);
	pthread_create(&read_t,NULL,read_task,(void *)mypool);
	while(1)
	{
		if(flag==0)
		{
		out(mypool);
		 list_for_each_entry(pop,&current_msg->mylist,mylist)
		{
		 printf("%s复制失败\n",pop->file.path);
		 sprintf(order,"rm %s",pop->file.obpath);
		 system(order);
		}
		break;
		}
		else if(over==0)
		{
			out(mypool);
			break;
		}
	}
	pthread_join(check_t,NULL);
	return 0;
}