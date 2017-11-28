# include "myhead.h"



struct f_file
{
	char f_name[100];
	struct f_file *next;
};

struct dir_file
{
	char dir_name[100];
	struct dir_file *next;
};


struct f_file *f_init()
{
	struct f_file *head = malloc(sizeof(struct f_file));
	
	bzero(head->f_name,0);
	head->next = NULL;
	
	return head;
}

struct dir_file *dir_init()
{
	struct dir_file *head = malloc(sizeof(struct dir_file));
	
	bzero(head->dir_name,0);
	head->next = NULL;
	
	return head;
}

int f_insert(struct f_file *head,char *f_path) //文件链表的添加
{
	struct f_file *p = head;
	struct f_file *new = f_init();
	
	strcpy(new->f_name,f_path);
	
	while(p->next != NULL)
	{
		p = p->next;
	}
	
	p->next = new;
	//printf("f_list->f_name is %s\n",p->next->f_name);
	return 0;
	
}

int dir_insert(struct dir_file *head,char *dir_path) //目录文件的添加
{
	struct dir_file *p = head;
	struct dir_file *new = dir_init();
	
	strcpy(new->dir_name,dir_path);
	
	while(p->next != NULL)
	{
		p = p->next;
	}
	
	p->next = new;
	//printf("dir_list->dir_name is %s\n",p->next->dir_name);
	
	return 0;
	
}

//读目录
int read_dir(struct f_file *f_head,struct dir_file *dir_head,struct dir_file *s_list,char *source_name,char *target_name)
{
	DIR *dir_stream; 			//指向目录流的指针
	struct dirent *mydir;		//保存你正在读取的某个文件的信息
	struct stat *q = malloc(sizeof(struct stat));	
	int stat1;	
	char buf[100],buf1[100]; 
	int len;
	char *p = NULL;
	
	
	dir_stream = opendir(source_name);
	
	if(dir_stream == NULL)
	{
		perror("open dir false!\n");
		exit(0);
	}

	stat1 = stat(source_name,q); //判断源目录是否存在
	if(stat1 == -1)	
	{
		printf("你所输入的源路径不存在！请重新输入！\n");
		exit(0);
	}
	
	
	
	while((mydir = readdir(dir_stream))!=NULL)
	{
		if((strcmp(mydir->d_name ,".") != 0) && (strcmp(mydir->d_name , "..") != 0))
		{
			bzero(buf,0);
			bzero(buf1,0);
			if((mydir->d_type==DT_REG))//如果是文件
			{
				sprintf(buf,"%s/%s",source_name,mydir->d_name);
				//printf("buf is %s\n",buf);
				f_insert(f_head,buf);
			}
			 if((mydir->d_type==DT_DIR))//如果是目录
			{
				
				sprintf(buf,"%s%s",target_name,mydir->d_name);
				strcpy(target_name,buf);
				printf("target_name is %s\n",target_name);
				dir_insert(dir_head,buf); 
				
				
				sprintf(buf1,"%s%s",source_name,mydir->d_name);
				strcpy(source_name,buf1);
				dir_insert(s_list,buf1);
				//printf("buf1 is %s\n",buf1);
				printf("source_name is %s\n",source_name);
				read_dir(f_head,dir_head,s_list,source_name,target_name);
			} 
		}
		
	}
}



//文件的拷贝
int f_cpy(struct f_file *f_head,struct dir_file *dir_head,struct dir_file *s_head) 
{
	struct f_file *f_list = f_head;
	struct dir_file *dir_list = dir_head->next;
	struct dir_file *s_list = s_head->next;
	int fd1,fd2;
	int filesize;
	char buf[100],buf2[100];
	int i,j;
	int len,len1;
		
	while(f_list->next != NULL)
	{
		f_list = f_list->next;
		
		len = strlen(s_list->dir_name);
		
		//printf("s_list->dir_name is %s\n",s_list->dir_name);
		//printf("f_list->f_name is %s\n",f_list->f_name);

		if(strncmp(s_list->dir_name,f_list->f_name,len-1) == 0)
		{
			sprintf(buf,"%s%s",dir_list->dir_name,&f_list->f_name[len]);
			//printf("buf is %s\n",buf);
		}
		else
		{
			dir_list = dir_list->next;
			s_list = s_list->next;
			f_list = f_head;
		}
		
		
		//文件拷贝功能
		//printf("f_list->f_name is  %s\n",f_list->f_name);
		fd1 = open(f_list->f_name,O_RDWR);
		if(fd1 == -1)
		{
			perror("打开文件失败！\n");
			return -1;
		}
		fd2 = open(buf,O_CREAT|O_RDWR,0777);
		if(fd2==-1)
		{
			perror("open false！\n");
			return -1;
		}
		
		filesize = lseek(fd1,0,SEEK_END); //读取fd1文件的大小
		char buf1[filesize];
		bzero(buf1,0);
		
		for(j = 0;j <= 10;j ++)
		{
			printf("\033[1;31m");  //红色
			printf("| ");
			printf("\033[0m");	//关闭所有属性
			usleep(50000);
		}
		printf("拷贝完成！\n");
		lseek(fd1,0,SEEK_SET); //重新将文件的偏移挪回到起始位置
		read(fd1,buf1,filesize);// 读取源文件里面的内容
		write(fd2,buf1,filesize);// 将读取的内容写入到目标文件中去
		
		close(fd1);
		close(fd2);
	}


}


//目录的拷贝
int dir_cpy(struct dir_file *dir_head,char *target_buf) 
{
	struct dir_file *p = dir_head;
	int ret,i;
	char buf[100];
	while(p->next != NULL)
	{
		p = p->next;
		/* //printf("p->dir_name is%s\n",p->dir_name);
		bzero(buf,100);
		for(i = (strlen(p->dir_name)-1);i >= 0;i -- )
		{
			if( (p->dir_name[i] == '/') && (i != (strlen(p->dir_name) -1)))
			{
				//len1 = strlen(f_list->f_name) - strlen(&f_list->f_name[i]);
				sprintf(buf,"%s%s",target_buf,&p->dir_name[i]);
				//printf("buf is %s\n",buf);
				strcpy(target_buf,buf);
				break;
			}
		}  */
		
		if(access(p->dir_name,F_OK) == -1)
		{
			
			ret = mkdir(p->dir_name,0777);
			if(ret == -1)
			{
				perror("mkdir false!\n");
				return -1;
			}  
		}
	}
	
	return 0;
} 

int f_printf(struct f_file *f_list)
{
	struct f_file *p = f_list;
	
	while(p->next != NULL)
	{
		p = p->next;
		printf("p->f_name is %s\n",p->f_name);
	}
	
	return 0;
}

int dir_printf(struct dir_file *dir_list)
{
	struct dir_file *p = dir_list;
	
	while(p->next != NULL)
	{
		p = p->next;
		printf("p->dir_name is %s\n",p->dir_name);
	}
	
	return 0;
}
void * file_operations(void *arg)
{
	struct flag *S = (struct flag *)arg;
	int i;
	char source_buf[100]= {0}; //存放源目录
	char target_buf[100]= {0}; //存放目标目录
	char buf[100] = {0};
	strcpy(source_buf,S->source_name);
	strcpy(target_buf,S->target_name);
	
	struct f_file *f_list = f_init();
	struct dir_file *dir_list = dir_init();//存放目标目录的路径
	struct dir_file *s_list = dir_init();//存放源目录的路径

	for(i = (strlen(source_buf)-1);i >= 0;i -- )
	{
		if( (source_buf[i] == '/') && (i != (strlen(source_buf) -1)))
		{
			//len1 = strlen(f_list->f_name) - strlen(&f_list->f_name[i]);
			sprintf(buf,"%s%s",target_buf,&source_buf[i]);
			//printf("buf is %s\n",buf);
			dir_insert(dir_list,buf);
			strcpy(target_buf,buf);
			break;
		}
	} 	
	dir_insert(s_list,source_buf);
	
	read_dir(f_list,dir_list,s_list,source_buf,target_buf);
	dir_cpy(dir_list,target_buf);
	
	//f_printf(f_list);
	//dir_printf(dir_list);
	//dir_printf(s_list);
	
	f_cpy(f_list,dir_list,s_list);
}


