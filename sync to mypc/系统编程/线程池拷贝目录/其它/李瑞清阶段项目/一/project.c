#include "project.h"

list list_init(void)//链表初始化
{
	list head = (list)malloc(sizeof(listnode));
	if(head != NULL)
	{
		head->next = NULL;
	}	
	return head;
}

list create_newnode(char *filename)//创建链表新节点
{
	list new = (list)malloc(sizeof(listnode));
	if(new != NULL)
	{
		bzero(new->filename,SIZE);
		strcpy(new->filename,filename);
	}
		
	return new;
}

void list_add(list head,char *filename)//尾部插入新节点
{
	list p = head;
	list newnode = create_newnode(filename);
	while(p->next != NULL)
	{
		p = p->next;
	}	
	p->next = newnode;	
	
}

void rec_read(char *pathname,list dir,list reg)//递归读取目录
{
	char buf[SIZE];
	bzero(buf,SIZE);
	struct dirent *ep = NULL;
	
	DIR *dp = opendir(pathname);	
	if(dp == NULL)
	{
		perror("open dir failed\n");
		exit(1);
	}
	
	while(1)
	{
		ep = readdir(dp);
		if(ep == NULL)
			break;
		
		if(ep->d_type == DT_DIR && ep->d_name[0] != '.')
		{
			sprintf(buf,"%s%s%s",pathname,"/",ep->d_name);
			list_add(dir,buf);//添加到目录链表
			rec_read(buf,dir,reg);
		}
		if(ep->d_type == DT_REG)
		{
			sprintf(buf,"%s%s%s",pathname,"/",ep->d_name);
			//printf("%s\n",buf);
			list_add(reg,buf);//添加到文件链表
		}
	}
	
}

void dir_judge(char *arg1,char *arg2,list dir,list reg)//目录判断及读取
{
	char buf[SIZE];
	bzero(buf,SIZE);
	struct dirent *ep = NULL;
	DIR *dp1 = opendir(arg1);
	DIR *dp2 = opendir(arg2);
	if(dp1 == NULL)
	{
		perror("打开源目录失败\n");
		exit(1);
	}
	else
	{
		while(1)
		{
			ep = readdir(dp1);
			if(ep == NULL)
				break;
			
			if(ep->d_type == DT_DIR && ep->d_name[0] != '.')//判断是否为目录且不是当前路径.和父路径..
			{
				sprintf(buf,"%s%s%s",arg1,"/",ep->d_name);
				list_add(dir,buf);//添加到目录链表
				rec_read(buf,dir,reg);//循环调用目录读取函数
			}
			
			if(ep->d_type == DT_REG)
			{
				sprintf(buf,"%s%s%s",arg1,"/",ep->d_name);
				list_add(reg,buf);//添加到文件链表
			}
			
		}
	}
	
	if(dp2 == NULL)
		mkdir(arg2,0777);		
}

void *copy_dir(void *arg)//复制目录功能函数
{
	int i;
	char tmp[SIZE];//存放剪切后的目录名
	char buf[SIZE];//存放拼接后的目录名
	struct copydir *p = (struct copydir *)arg;
	
	while(p->dir->next != NULL)
	{
		bzero(tmp,SIZE);
		bzero(buf,SIZE);
		//printf("%s\n",p->dir->next->filename);
		
		for(i = 0;i < (strlen(p->dir->next->filename)-strlen(p->sourcepath));i++)
		{
			tmp[i] = p->dir->next->filename[strlen(p->sourcepath)+i];
		}
		
		sprintf(buf,"%s%s",p->targetpath,tmp);
		mkdir(buf,0777);		
		p->dir = p->dir->next;		
	}	
}

void *copy_reg(void *arg)//复制普通文件功能函数
{
	int i,j,flag,count = 0;
	
	char tmp1[SIZE];
	char tmp2[SIZE];
	char tmp3[SIZE];
	char tmp4[SIZE];
	char tmp5[SIZE];//以上数组存放剪切下的目录字符串
	char yn[5];
	struct copyreg *p = (struct copyreg *)arg;
	struct dirent *ep = NULL;
	
	while(p->reg->next != NULL)
	{
		int fd1 = open(p->reg->next->filename,O_RDONLY);
		if(fd1 == -1)
		{
			printf("%s open failed:",p->reg->next->filename);
			perror("open()\n");
			exit(1);
		}
		int size = lseek(fd1,0,SEEK_END);//获取文件大小
		char buf[size];
		bzero(buf,size);
		lseek(fd1,0,SEEK_SET);//调回偏移量
		
		int nread = read(fd1,buf,size);
		if(nread == -1)
		{
			perror("read failed\n");
			exit(1);
		}
		
		bzero(tmp1,SIZE);
		bzero(tmp2,SIZE);
		bzero(tmp3,SIZE);
		bzero(tmp4,SIZE);
		bzero(tmp5,SIZE);
		bzero(yn,5);
		
		
		for(i = 0;i<strlen(p->reg->next->filename);i++)
		{
			if(p->reg->next->filename[i] == '/')
				j = i;
		}
		for(i = 0;i<strlen(p->reg->next->filename)-j;i++)
		{
			tmp3[i] = p->reg->next->filename[j+i];
		}
		for(i = 0;i<j-strlen(p->sourcepath);i++)
		{
			tmp1[i] = p->reg->next->filename[i+strlen(p->sourcepath)];
		}
		sprintf(tmp2,"%s%s",p->targetpath,tmp1);//拼接成目标目录，方便下面打开
		DIR *dp = opendir(tmp2);
		flag = 0;
		while(1)
		{
			ep = readdir(dp);
			if(ep == NULL)
				break;
			
			//判断是否有重名文件
			if(strcmp((p->reg->next->filename)+j+1,ep->d_name) == 0)
			{
				flag = 1;
				printf("是否要覆盖（%s）,yes or no\n",ep->d_name);
				scanf("%s",yn);
				if(strncmp(yn,"yes",3) == 0)
				{
					sprintf(tmp5,"%s%s",tmp2,tmp3);
					int fd2 = open(tmp5,O_WRONLY|O_CREAT,0777);
					if(write(fd2,buf,size) == -1)
					{
						perror("write failed\n");
						exit(1);
					}
					
				}
				else//不覆盖的话就复制备份文件
				{
					for(i = 0;i<strlen(ep->d_name);i++)
					{
						if(ep->d_name[i] == '.')
							j = i;
					}
					for(i = 0;i<j;i++)
					{
						tmp3[i] = ep->d_name[i];
					}
					tmp3[j] = '\0';
				
					for(i = 0;i<strlen(ep->d_name)-j;i++)
					{
						tmp4[i] = ep->d_name[j+i];
					}
					
					sprintf(tmp5,"%s%s%s%s%s",tmp2,"/",tmp3,"(1)",tmp4);
					int fd2 = open(tmp5,O_WRONLY|O_CREAT,0777);
					if(write(fd2,buf,size) == -1)
					{
						perror("write failed\n");
						exit(1);
					}
				}
			}
		}	
		if(flag == 0)
		{
			sprintf(tmp5,"%s%s",tmp2,tmp3);
			int fd2 = open(tmp5,O_WRONLY|O_CREAT,0777);
			
			if(write(fd2,buf,size) == -1)
			{
				perror("write failed\n");
				exit(1);
			}
		}
		count++;
		usleep(10000);
		if(count == 10)//每复制十个文件输出一次
		{
			printf("||"); 
			fflush(stdout);//刷新缓冲区
			count = 0;
		}
		p->reg = p->reg->next;	
	}
	printf("copy done\n");
}