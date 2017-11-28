#include "thread_pool.h"
#include "project.h"

int main(int argc,char *argv[])
{
	int number;
	thread_pool mypool = (thread_pool)malloc(sizeof(struct thread_pool));
	
	struct copydir *directory = (struct copydir *)malloc(sizeof(struct copydir));
	struct copyreg *regular = (struct copyreg *)malloc(sizeof(struct copyreg));
	//初始化复制目录及复制文件结构体
	directory->dir = list_init(); 
	regular->reg = list_init(); 
	strcpy(directory->sourcepath,argv[1]);
	strcpy(regular->sourcepath,argv[1]);
	strcpy(directory->targetpath,argv[2]);
	strcpy(regular->targetpath,argv[2]);
	
	if(argc != 3)
	{
		printf("请按照格式%s dir1 dir2输入\n",argv[0]);
		exit(1);
	}
	
	dir_judge(argv[1],argv[2],directory->dir,regular->reg);	
	
	printf("请输入初始化的线程数量\n");
	scanf("%d",&number);
	pool_init(mypool,number);

	add_task(mypool,copy_dir,(void *)directory);
	add_task(mypool,copy_reg,(void *)regular);
	
	threadpool_destroy(mypool);
	free(directory);
	free(regular);
	
	return 0;	
}