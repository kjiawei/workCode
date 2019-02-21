
/* sem_com.c */

#include "sem_com.h"

int init_sem(int sem_id, int init_value)
{
	union semun sem_union;
	sem_union.val= init_value;
	if (semctl (sem_id, 0, SETVAL, sem_union) == -1)//动作
	{
		perror("Initialize semaphore");		
		return -1;
	}
	/*原型int semctl(int semid, int semnum,int cmd…);	
semid：信号量集ID
semnum: 要修改的信号量编号
cmd：	GETVAL：获取信号量的值
SETVAL：设置信号量的值
IPC_RMID：从系统中删除信号量集合,返回值	成功:0 出错:-1
*/
	return 0;
}

int del_sem(int sem_id)
{
	union semun sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
	{
		perror("Delete semaphore");
		return -1; 
	}
}

int sem_p(int sem_id)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0; /*id=0 表示的申请到的信号量集合里面的第一个信号量*/
	sem_b.sem_op = -1; /* P operation*/
	sem_b.sem_flg = SEM_UNDO;// 进程结束的时候恢复成调用前状态，最关键是释放
	
	if (semop(sem_id, &sem_b, 1) == -1) 
	{
		perror("P operation");
		return -1;
	}
	return 0;
}

int sem_v(int sem_id)
{
	struct sembuf sem_b;
	
	sem_b.sem_num = 0; /* id 第一个信号量*/
	sem_b.sem_op = 1; /* V operation */	
	sem_b.sem_flg = SEM_UNDO; 

/*
原型int semop(int semid, struct sembuf  opsptr,  size_t  nops);	
semid：信号量集ID
struct sembuf
{
	short  sem_num;=>> 要操作的信号量的编号
	short  sem_op;   =>>     0 :  等待，直到信号量的值变成0                
				 1  : 放资源，V操作              
				-1 :  分配资源，P操作                    
    short  sem_flg;   =>> 0/IPC_NOWAIT/SEM_UNDO
};
nops:  要操作的信号量的个数,返回值	成功：0 出错：-1
*/
	if (semop(sem_id, &sem_b, 1) == -1)//信号里操作
	{
		perror("V operation");
		return -1;
	}
	return 0;
}