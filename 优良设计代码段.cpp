全局变量的替换方案
1.

inline提高效率 函数需要短小精悍 看编译器优化处理  编译时替换
inline int add(int x,int y,int z){
	return x + y + z;
}//类似template


#include <iostream>
#include <string>

//模板是类和对象的蓝图--泛型编程

//交换两个数
template <class T>//字母T将在接下来的函数里代表一种不确定的数据类型
void swap(T &a,T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

int main()
{
	int i1 = 100;
	int i2 = 200;
	std:cout<<"调用前,i1="<<i1<<",i2="<<i2<<"\n";
	swap(i1,i2);//usage2:swap<int>(i1,i2);
	std:cout<<"调用后",i1="<<i1<<",i2="<<i2<<"\n";

	std:string s1 = "123";
	std:string s2 = "456";
	std:cout<<"调用前,s1="<<s1<<",s2="<<s2<<"\n";
	swap(s1,s2);
	std:cout<<"调用后",s1="<<s1<<",s2="<<s2<<"\n";
}

//类模板  栈例子-小甲鱼 计算器例子 列波浪
template <class T>

class MyClass{
	private push(T value){

	}
	T pop();
};

MyClass<T>::MyClass()
{
	
}

template <class T>//这里可以忽略吗
T MyClass<T>::pop()
{

}
