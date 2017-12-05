#ifndef H_DAEMON_H
#define H_DAEMON_H

#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/shm.h>

#define SIG_HEARTBEAT 50      //心跳--主进程发给守护进程的心跳信号#/
#define SIG_RELEASE_MUTEX 51  //释放锁--守护进程发给主进程的死锁释放信号#/
#define SIG_APPQUIT  52       //黑屏--主进程异常退出时，发给守护进程#/

extern int messageQueuePid;
extern pid_t childPid;
void PID_Process_Handle();//包括主进程和守护子进程的处理#/
void Parentprocess_handle();//主进程---间隔处理函数#/

#endif
