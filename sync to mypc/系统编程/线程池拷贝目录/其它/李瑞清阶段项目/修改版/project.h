#ifndef _PROJECT_H_
#define _PROJECT_H_

#include "myhead.h"

#define SIZE 500

typedef struct list//文件名链表
{
	char filename[SIZE];
	struct list *next;
}listnode,*list;

struct copydir
{
	list dir;
	char sourcepath[SIZE];//存放源目录的路径
	char targetpath[SIZE];//存放目标目录路径
};

struct copyreg
{
	int count;
	list reg;
	char sourcepath[SIZE];
	char targetpath[SIZE];
};

extern list list_init(void);
extern list create_newnode(char *filename);
extern void list_add(list head,char *filename);
extern int rec_read(char *pathname,list dir,list reg);
extern int dir_judge(char *arg1,char *arg2,list dir,list reg);
extern void *copy_dir(void *arg);
extern void *copy_reg(void *arg);

#endif