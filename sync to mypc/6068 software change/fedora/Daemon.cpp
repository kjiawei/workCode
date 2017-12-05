#include "Daemon.h"
#include <stdio.h>
#include "global_var.h"

#define BUFSZ 100

struct message{//
    long msg_type;
    char msg_text[BUFSZ];
};

pid_t childPid = 0;  //子进程ID G_cpid
pid_t fatherPid = 0;  //父进程ID ppid
bool isRestartMainProcess = false;  //子进程重启主程序restart_flag
bool isChildProcessStart = false;  //子进程开始运行childstart_flag
int messageQueuePid = 0;  //消息队列PID keypid

void SIG_RelMutex_Handle(int sig)//主进程---释放锁信号操作
{
    if( sig == SIG_RELEASE_MUTEX )
    {
        global_mutex.tryLock();
        global_mutex.unlock();

        Map_mutex.tryLock();
        Map_mutex.unlock();
        //sem.release(1);
        G_CanSend_Mutex.tryLock();  //CAN 数据发送互斥锁#/
        G_CanSend_Mutex.unlock();
        //G_log_mutex.tryLock();
        //G_log_mutex.unlock();     //操作日志互斥#/
        printf("SIG_RELEASE_MUTEX \n");
    }
}

static char HeartBeat = 0;
//守护子进程---心跳操作#/
void SIG_HEARTBEAT_Handle(int sig)
{
    if(sig==SIG_HEARTBEAT)
    {
        HeartBeat = 0;
        isChildProcessStart = true;
        printf("SIG_HEARTBEAT \n");
    }
}

/*
//守护子进程---主进程异常退出信号操作#/
void SIG_APPQUIT_Handle(int sig)
{
    if(sig==SIG_APPQUIT)
    {
        printf("SIG_APPQUIT_Handle--IN\n");

        if(ppid>1)
        {
            kill(ppid,SIGKILL);
        }

   //         usleep(1000);
#ifdef HW_ARM
            execl("/usr/local/app/hemodialysis","/usr/local/app/hemodialysis","-qws","restart",NULL);
#else
            execl("./hemodialysis","./hemodialysis","-qws","restart",NULL);
#endif
            usleep(100);

            printf("SIG_APPQUIT_Handle--OUT\n");

 //           exit(1);
    }
}
*/

//守护子进程---主进程异常退出信号操作#/
void SIG_APPQUIT_Handle(int sig)
{
    if( sig == SIG_APPQUIT )
    {
       isRestartMainProcess = true;

        if(fatherPid > 1)
        {
            kill(fatherPid,SIGKILL);
        }
        printf("SIG_APPQUIT_Handle--OUT\n");
    }
}

//测试用#/
void SIG_Test(int sig)
{
    if(sig==SIG_HEARTBEAT)
    {
        printf("SIG_HEARTBEAT sig=%d\n",sig);
    }
    else if(sig==SIG_RELEASE_MUTEX)
    {
        printf("SIG_RELEASE_MUTEX sig=%d\n",sig);
    }
    else if(sig==SIG_APPQUIT)
    {
        printf("SIG_APPQUIT sig=%d\n",sig);
    }
}

//守护进程---子进程处理函数#/
void childprocess_handle()
{
    pid_t res;
    if(fatherPid<=0)
    {
        fatherPid = getppid();
    }
    signal(SIG_HEARTBEAT,SIG_HEARTBEAT_Handle);
    signal(SIG_APPQUIT,SIG_APPQUIT_Handle);
    while(1)
    {
        sleep(1);
        HeartBeat++;

        if(isChildProcessStart==false)
            continue;

        if((HeartBeat%10==0)&&(fatherPid>1))  //每8秒后给主进程发释放信号#/
        {
            kill(fatherPid,SIG_RELEASE_MUTEX);
        }

        if(HeartBeat>30&&HeartBeat<40)
        {
            if(getppid()!=fatherPid) //主进程退出后，子进程也退出#/
            {
                printf("childprocess exit-1\n");
                exit(0);
            }
            else if(getppid()==fatherPid)
            {
                kill(fatherPid,11);
                HeartBeat=40;
            }
            //HeartBeat=0;
        }

        if(isRestartMainProcess==true)
        {
            res=fork();
            if(res==-1)
            {
                perror("fork error!");
            }
            else if(res==0)  //子进程----处理#/
            {
                 printf("childprocess coming\n");
#ifdef HW_ARM
            execl("/usr/local/app/hemodialysis","/usr/local/app/hemodialysis","-qws","restart",NULL);
#else
            execl("./hemodialysis","./hemodialysis","-qws","restart",NULL);
#endif
            us_sleep(1000);
            exit(1);
            }
            us_sleep(1000);
            isRestartMainProcess = false;
            printf("childprocess exit-2\n");
            exit(0);
        }
    }  //while(1)#/
}

//主进程和守护进程(子)的处理#/
void PID_Process_Handle()
{
    signal(SIG_RELEASE_MUTEX,SIG_RelMutex_Handle);
/*
    int key=0;
    int len=0;

    signal(SIG_HEARTBEAT,SIG_Test);
    signal(SIG_RELEASE_MUTEX,SIG_Test);
    signal(SIG_APPQUIT,SIG_Test);


    //生成标准的Key#/
    if((key=ftok("tmp",10))<=0)
    {
        perror("ftok");
        key=180;
    }

     //创建消息队列#/
    if(keypid=msgget(key,IPC_CREAT|0666)==-1)
    {
        perror("msgget");
    }

    //添加消息队列#/
    G_msg.msg_type=88;
    len=strlen(G_msg.msg_text);

    if(msgsnd(keypid,&G_msg,len,IPC_NOWAIT)<0)
    {
        perror("msgsnd");
    }

    //读取消息队列#/
    if(msgrcv(keypid,&G_msg,len,0,IPC_NOWAIT)<0)
    {
        perror("msgrcv");
    }

    //移走消息队列#/
    if(msgctl(keypid,IPC_RMID,NULL)<0)
    {
        perror("msgctl");
    }

    */
    pid_t res;
    res=fork();
    if(res==-1)
    {
        perror("fork");
    }
    else if(res==0)  //子进程----处理#/
    {
        childprocess_handle();
    }
    else if(res>0)  //父进程---处理#/
    {
        childPid = res;
        printf("child PID=%d \n",childPid);
        printf("My PID=%d \n",getpid());
    }
}

//主进程---间隔处理函数#/
void Parentprocess_handle()
{
    if(childPid>1)
    {
        kill(childPid,SIG_HEARTBEAT);
    }

}



