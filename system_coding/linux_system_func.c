/*************************************************
 File name : linux_system_func.c
 Create date : 2019-2-11 19:04
 Modified date : 2019-2-11 19:04
 Author :jkCodic
 Email : 523090538@qq.com
 Describe:linux系统编程函数,复习/测试使用
 ***********************************************/
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

/*****互斥锁*****/
pthread_mutex_t mutex;
void printf_char(char *string)
{
	char *p = string;
	while(*p != '\0')
	{
		fprintf(stderr,"%c",*p);
		usleep(500);
		p++;
	}
	printf("\n");
}

void *fun1(void *arg)
{
	pthread_mutex_lock(&mutex);//加锁
	printf_char("GSDGLOVGSDQLDSFD4GF5DF75DA5W");
	pthread_mutex_unlock(&mutex);//解锁
	pthread_exit(NULL);
}

void *fun2(void *arg)
{
	pthread_mutex_lock(&mutex);//加锁
	printf_char("sddscxlvjslejslvslflsljflseflksd");
	pthread_mutex_unlock(&mutex);//解锁
	pthread_exit(NULL);
}

void test_mutex(){
	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex,NULL);//互斥锁初始化
	pthread_create(&tid1,NULL,&fun1,NULL);
	pthread_create(&tid2,NULL,&fun2,NULL);
	sleep(2);
	pthread_mutex_destroy(&mutex);//销毁互斥锁
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
}
/*****互斥锁*****/

/*
	共享内存
	高效直接操作物理内存不需要任何的拷贝(向内核申请一块内存映射到进程自身的虚拟地址空间中,直接读写这个进程中的空间)
删除共享内存
       int shmctl(int shmid, int cmd, struct shmid_ds *buf);
int shmid：申请到的内存ID
int cmd:命令  IPC_RMID（删除）
struct shmid_ds *buf：0
*/
void test_shareMemory(){
	char buff[20];
	int shmid;
	shmid = shmget(ftok(".",1),256,IPC_CREAT | 0666);//申请共享内存，ftok获取key值
	/*
	原型int shmget(key_t key, size_t size, int shmflg);
返回值:成功 返回申请到的内存ID  失败：-1
key_t key：内存的键值
size_t size：大小
int shmflg：权限标志IPC_CREAT
	*/
	char *mem_p = shmat(shmid,NULL,0);//映射共享内存到进程空间
	/*
	void *shmat(int shmid, const void *shmaddr, int shmflg);
返回值：成功：映射的地址：
const void *shmaddr：映射起始地址：
int shmflg：权限标志 0代表可读可以写
	*/
	bzero(buff,20);
	fgets(mem_p,256,stdin);
	shmdt(mem_p);
}

/*****条件变量*****/
void test_cond(){

}
/*****条件变量*****/

/*
信号量(semaphore信号灯):不同进程间或一给定进程内部不同线程间同步的机制
二值信号量:值为0或1,与互斥锁类似,资源可用时值为1,不可用时值为0。
计数信号量:值在0到n之间(统计资源,值代表可用资源数)

信号量操作:V释放资源,信号量值加1;P申请资源,信号量值减1(可能引起进程睡眠)
等0操作:既不申请也不释放而是令进程阻塞直到信号量的值为0
*/
void test_sem(){

}

/*
消息队列:消息的列表,用户可以在消息队列中添加消息、读取消息(按照类型来发送/接收),IPC对象一种,由ID来唯一标识(使用时创建)
查看 ipcs -q,删除 ipcrm -q msgid /  ipcrm -Q Key	
*/
#define SIZE 64
//数据接收结构体
struct msg_rv
{
	int mtype;
	char msg[50];
};

//数据发送结构体
struct msg_snd
{
	int mtype;
	char msg[50];
};

void test_msgQueue(){
	struct msg_rv data;
	struct msg_snd snddata;
	data.mtype = 1;
	int msgID = msgget((key_t)2058,IPC_CREAT | 0666);//获取消息队列的ID,原型int msgget(key_t key, int flag);key:和消息队列关联的key值,flag：消息队列的访问权限;成功:消息队列ID,出错:-1 
	if(msgID == -1) return -1;
	char buff[50];
	while(1){
		data.mtype = 89;
		snddata.mtype = 88;
		if(msgrcv(msgid,(void *)&data,sizeof(struct msg_rv),data.mtype,0)==-1) return -1;
		/*
		原型  int msgrcv(int msgid,  void* msgp,  size_t  size,  long msgtype,  int  flag);//消息接收
msqid：消息队列的ID,msgp：接收消息的缓冲区,size：要接收的消息的字节数
msgtype: 0：接收消息队列中第一个消息。
		大于0：接收消息队列中第一个类型为msgtyp的消息.
		小于0：接收消息队列中类型值不小于msgtyp的绝对值且类型值又最小的消息。
flag:0：若无消息函数会一直阻塞 IPC_NOWAIT：若没有消息，进程会立即返回ENOMSG,返回值	成功：接收到的消息的长度 出错：-1
		*/
		if(strncmp(data.msg,"end",3)==0) break;
		bzero(buff,50);
		fgets(buff,50,stdin);
		strcpy(snddata.msg,buff);
		if(strncmp(snddata.msg,"end",3)==0) break;
		msgsnd(msgid,(void *)&snddata,strlen(buff)+4,0);//消息发送
		/*
		原型int msgsnd(int msqid, const void *msgp, size_t size, int flag);msqid：消息队列的ID,msgp：指向消息的指针。
常用消息结构msgbuf如下：struct msgbuf
{
	long mtype;        //消息类型 
	char mtext[N];     //消息正文
}；
size：发送的消息正文的字节数
flag：  IPC_NOWAIT  消息没有发送完成函数也会立即返回,0：直到发送完成函数才返回 返回值	成功：0,出错：-1
		*/
	}
	msgctl(msgid,IPC_RMID,0);//撤消消息队列动作 
	/*原型	int msgctl ( int msgqid, int cmd, struct msqid_ds *buf );	
msqid：消息队列的队列ID
cmd：	IPC_STAT：读取消息队列的属性，并将其保存在buf指向的缓冲区中。
	IPC_SET：设置消息队列的属性。这个值取自buf参数。
	IPC_RMID：从系统中删除消息队列。
buf：消息队列缓冲区
返回值	成功：0 出错：-1*/
}
/*****消息队列*****/

/*
 	a:用pipe产生一个无名管道之后，再连续产生两个子进程A、B，
 	b: 子进程A 在往管道写了一句hello 后就退出了 --------写 fd[1]
  	c: 子进程B读到A写入的数据，再往管道中写入hello world ----读fd[0] 写fd[1]
  	d: 主进程读取最后管道中的数据并打印 ----读fd[0]

	  如果管道里面的数据已经写满了，会产生阻塞，直到数据被读出，才能继续写入
		无名管道的写操作不能保证数据的原子性（完整性）
	对无名管道做文件操作：read()/write()/close()
			    但是不能lseek，会返回失败
			    pipe()打开并创建无名管道，已经设置好打开方式，不能更改。
			    无名管道只存在于进程中，进程结束也随之消失
		局限：只能在亲缘进程中，数据的完整性不能保证
	对于管道不能在共享目录中去编译运行
*/
void test_pipe(){
	int fd[2];//读写端的描述符
	char buf[100];
	const int size=100;
	if(pipe(fd)==-1) perror("pipe error!\n");//pipe()必须在fork()之前才能保证亲缘进程使用同一个管道
	else
	{
		pid_t pid1 = fork();
		if(pid1 == -1) perror("fork1 error!\n");
		else if(pid1 == 0)
		{
			close(fd[0]);
			printf("child1 start write...\n please input:");
			scanf("%d",buf);
			write(fd[1],buf,size);
			close(fd[1]);
		}
		else if(pid1 > 0)
		{
			waitpid(pid1,NULL,0);
			pit_t pid2 = fork();
			if(pid2 == -1) perror("fork2 error!\n");
			else if(pid2 == 0)
			{
				printf("child2 start read...\n ");
				read(fd[0],buf,size);
				close(fd[0]);
				printf("child2 read:%s\n",buf);
				
				printf("child2 start write...\n please input");
				scanf("%s",buf);
				write(f[1],buf,size);//如果写端关闭了，读会立刻返回,如果读端关闭了，在调用write时候内核会产生一个SIGPIPE（write:Broken pipe）
				prinf("child2 finish to write!\n");
				close(fd[1]);
			}
			else if(pid2 > 0)
			{
				waitpid(pid2,NULL,0);
				close(fd[1]);
				printf("parent start read...\n ");
				read(fd[0],buf,size);//读端是有阻塞的，会等到有数据写入才返回
				close(fd[0]);//在读与写端不使用时要用close关闭
				printf("parent read:%s\n",buf);
			}
		}
	}
}

/*
有名管道:在任意两个进程间通信
1、读端是有阻塞的，会等到有数据写稿才返回（读阻塞）
		2、如果写端关闭了，读端的read出没有等待的必须，会直接pass
		3、写端也会阻塞，管道里面已经写满了，产生阻塞（能够保证数据的原子性、完整性）

*/
#define FIFO "myfifo1"
#define FIFO2 "myfifo2"

void test_fifoWrite()
{
	int my_fd,fd;
	char w_buff[30];

	bzero(w_buff,30);
	if(access(FIFO2,F_OK)==-1)
	{
		my_fd = mkfifo(FIFO2,0664);
		if(my_fd == -1)
		{
			perror("failed!\n");
			return -1;
		}
	}
	fd = open(FIFO2,O_WRONLY);
	if(fd==-1)
	{
		printf("open fifo file failed!\n");
		exit(0);
	}
	while(1)
	{
		bzero(w_buff,30);
		fgets(w_buff,30,stdin);
		write(fd,w_buff,strlen(w_buff));
	}
	close(fd);
}
void test_fifoRead()
{
	int my_fd,fd;
	char r_buff[30];

	bzero(r_buff,30);
	if(access(FIFO,F_OK)==-1)//确定文件或文件夹的访问权限。即，检查某个文件的存取方式，比如说是只读方式、只写方式等。如果指定的存取方式有效，则函数返回0，否则函数返回-1
	{
		my_fd = mkfifo(FIFO,0664);
		if(my_fd == -1)
		{
			perror("failed!\n");
			return -1;
		}
	}

	fd = open(FIFO,O_RDONLY);
	if(fd==-1)
	{
		printf("open fifo file failed!\n");
		exit(0);
	}
	//fgets(w_buff,30,stdin);
	while(1)
	{
		bzero(r_buff,30);
		read(fd,r_buff,sizeof(r_buff));
		printf("r_buff = %s\n",r_buff);
	}

	close(fd);
}
/*有名管道*/

/*
信号signal

*/

void f(int sig)
{
	printf("sig = %d\n",sig);
}

void test_signal(){
	sigset_t set;//初始化一个信号集
	printf("pid = %d\n",getpid());
	signal(SIGKILL,f);
	printf("signal wait:\n");
	sigemptyset(&set);//清空信号集
	sigaddset(&set,SIGKILL);//添加信号集
	sigaddset(&set,SIGHUP);//添加信号集

	//阻塞信号集
	sigprocmask(SIG_BLOCK,&set,NULL);

	int i = 20;
	while(i)
	{
		printf("i = %d\n",i);
		sleep(1);
		i--;
	}
	//解除信号阻塞
	sigprocmask(SIG_UNBLOCK,&set,NULL);
}

int main()
{
	test_pipe();
		
	return 0;
	
}