#include "myhead.h"
void set_y_x(int y, int x)
{
	char ss[20];
	int x1=0,x2=0,y1=0,y2=0;
	strcpy(ss, "\033[yy;xxH");  // 这句话是重点
	x1 = x/10;  // 0
	x2 = x%10;	// 2
	y1 = y/10;  // 0
	y2 = y%10;	// 6	
		
	ss[2] = y1+48;// 0  1 跟'1'
	ss[3] = y2+48;// 6
	ss[5] = x1+48;	// 0
	ss[6] = x2+48;  // 2
	printf("%s",ss);	
}
int main()
{
	printf("nihao!\n");
	set_y_x(7,4);
	printf("hello!\n");
	return 0;
}