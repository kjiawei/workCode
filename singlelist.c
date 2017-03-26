/*
  author: jerry     Email:523090538@qq.com    
  help:myhead.h  in another file
*/
#include "myhead.h" 

struct student {
	char name[SIZE];
	float score;
};

typedef struct singlelist{
	struct student stu[SIZE];
	struct singlelist *next;
}single_list,*listp;//redirect

#define SIZE 20

//init my list,null of head
struct singlelist *singlelist_init(){
	struct singlelist *head=malloc(sizeof(struct singlelist));
	head->next=NULL;//diff with ->createNode
	return head;
}

//create newnode
struct singlelist *createNode(){
	struct singlelist *newNode=malloc(sizeof(struct singlelist));
	printf("请输入新插入的学生的名字！\n");
	scanf("%s",newnode->stu.name);
	printf("请输入新插入的学生的分数！\n");
	scanf("%f",&(newnode->stu.score));
  newNode->next=NULL;
	return newNode;
}

//insert tail
int singlelist_insert_tail(struct siglelist *head){
  struct singlelist *p=head;//遍历判断使用
  struct singlelist *newnode=createNode();
  while(p->next!=NULL){
    p=p->next;
  }
  p->next=newnode;
  return 0;
}

//insert somewhere(front) of the list
int singlelist_insert_mid(struct singlelist *head,char *name){
  struct singlelist *p=head;//遍历判断使用
  
  while(p->next!=NULL){
    if(strcmp(p->next->stu.name,name)==0){
      break;
		  p=p->next;//马上指向下一位
    }
  }
  // 判断name在链表中根本就不存在
	if(p->next==NULL)
	{
		printf("对不起，没有这个人%s！\n",name);
		return -1;
	}
  listp newnode=createNode();//use the definenation
  newnode->next=p->next;
	p->next=newnode;
	return 0;
}
//delete the node of the list
int remove_node(struct siglelist *head,char *name)
{
	if(head==NULL)
	{
		return -1;
	}
	struct siglelist *p=head;
	struct siglelist *q=head->next;// 使用q去遍历单链表
	
	while(q!=NULL)
	{
		if(strcmp(q->stu.name,name)==0) // 判断p的下一个节点里面的名字是否符合要求
			break;
			
		p=p->next;
		q=q->next;
	}
	//准备删除节点
	p->next=q->next;
	q->next=NULL;
	free(q);
	return 0;
}
//print the list
int siglelist_print(struct siglelist *head)
{
	struct siglelist *p=head;
	//遍历单链表，打印
	while(p->next!=NULL)
	{
		p=p->next;
		printf("目前我访问到的节点对应的学生名字：%s 分数：%f\n",p->stu.name,p->stu.score);
		
	}
	return 0;
}

int main(void){
  char buf[20];
	bzero(buf,20);
	struct singlelist *myhead=singlelist_init();
	//修改元素 排序
  
  siglelist_insert_tail(myhead);
  siglelist_insert_tail(myhead);
  siglelist_insert_tail(myhead);
  
  // 中间插入
	printf("请输入你打算插入的那个学生的名字：前面插入！\n");
	scanf("%s",buf);
	siglelist_mid(myhead,buf);
	siglelist_print(myhead);
	// 删除某个节点
	remove_node(myhead,"li");
	siglelist_print(myhead);
  
	return 0;
}
