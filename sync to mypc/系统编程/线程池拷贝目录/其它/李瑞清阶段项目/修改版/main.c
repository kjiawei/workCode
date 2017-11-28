#include "thread_pool.h"
#include "project.h"

int main(int argc,char *argv[])
{
	int number,i;
	thread_pool mypool = (thread_pool)malloc(sizeof(struct thread_pool));
	
	struct copydir *directory = (struct copydir *)malloc(sizeof(struct copydir));
	struct copyreg *regular = (struct copyreg *)malloc(sizeof(struct copyreg));
	//初始化复制目录及复制文件结构体
	directory->dir = list_init(); 
	regular->reg = list_init(); 
	regular->count = 0; 
	strcpy(directory->sourcepath,argv[1]);
	strcpy(regular->sourcepath,argv[1]);
	strcpy(directory->targetpath,argv[2]);
	strcpy(regular->targetpath,argv[2]);
	
	if(argc != 3)
	{
		printf("请按照格式%s dir1 dir2输入\n",argv[0]);
		exit(1);
	}
	
	int count = dir_judge(argv[1],argv[2],directory->dir,regular->reg);	
	//regular->reg = regular->reg->next;
	
	printf("请输入初始化的线程数量\n");
	scanf("%d",&number);
	pool_init(mypool,number);

	add_task(mypool,copy_dir,(void *)directory);
	sleep(1);
	
	for(i = 0;i<count;i++)
	{	
		add_task(mypool,copy_reg,(void *)regular); 
	}
	
	threadpool_destroy(mypool);
	free(directory);
	free(regular);
	
	printf("copy done\n");
	return 0;	
}