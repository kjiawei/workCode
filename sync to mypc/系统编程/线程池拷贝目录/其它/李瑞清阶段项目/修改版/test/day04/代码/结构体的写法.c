#include "myhead.h"

struct student
{
	int num;
	char name[10];
	struct teacher
	{
		int num;
		char name[10];
	} teacher1;  // 开阔眼界
/* struct
	{
		//int num;
		//char name[10];
		int data;
	} teacher; */
};

int main()
{
	struct student stu1;
	stu1.num=15;
	stu1.teacher1.num=18;
	printf("stu1.num is:%d\n",stu1.num);
	printf("stu1.teacher1.num is:%d\n",stu1.teacher1.num);
	return 0;
	
}

