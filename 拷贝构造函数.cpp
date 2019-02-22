#include <stdio.h>

class Trace{
public:
	Trace(){noisy = 0;f = stdout;}
	Trace(FILE* ff){noisy = 0;f = ff;}
	void print(char* s){if(noisy) fprintf(f, "%s\n", s);}
	void on(){noisy = 1;}
	void off(){noisy = 0;}

private:
	int noisy;
	FILE* f;
};

int main(){
	Trace t(stderr);
	t.print("begin main()\n");
	t.print("end main()\n");
}

//拷贝构造函数例子
#include <iostream>
using namespace std;

class CExample{
private:
	int a;

public:
	CExample(int b){
		a = b;
		cout<<"creat: "<<a<<endl;
	}
	CExample(const CExample& C){//拷贝构造函数  
		a = C.a;
		cout<<"copy"<<endl; 
	}//调用情况:①     ②    ③
	~CExample(){cout<<"delete:"<<a<<endl;}//情况:①新增
	void show(){cout<<a<<endl;}
};

void g_Fun(CExample C){cout<<"test"<<endl;}//情况:①新增

CExample g_Fun(){//情况:②新增
	CExample temp(0);
	return temp;
}

int main(){


	//g_Fun();//情况:②新增  对象以值传递的方式从函数返回 [1.先会产生一个临时变量，就叫XXXX吧 2. 然后调用拷贝构造函数把temp的值给XXXX。整个这两个步骤有点像：CExample XXXX(temp); 3.在函数执行到最后先析构temp局部变量  4.等g_Fun()执行完后再析构掉XXXX对象]

	//CExample test(1);传入对象 对象以值传递的方式传入函数参数 对应情况①[1.test对象传入形参时，会先会产生一个临时变量，就叫 C 吧   2.然后调用拷贝构造函数把test的值给C。 整个这两个步骤有点像：CExample C(test);   3.等g_Fun()执行完后, 析构掉 C 对象]
	//g_Fun(test);
	CExample A(100);
	CExample B = A;//CExample B(A)也是一样的,注意这里的对象初始化要调用拷贝构造函数，而非赋值
	B.show();//就类对象而言，相同类型的类对象是通过拷贝构造函数来完成整个复制过程的
	return 0;
}