//为其他对象提供一种代理以控制对这个对象的访问
//最大的好处便是逻辑与实现的彻底解耦  https://www.cnblogs.com/lang5230/p/5322668.html
class Subject{//抽象主体角色，抽象类或者接口，是一个普通的业务类型定义
public:
	virtual ~Subject() = 0;
	virtual void Request() = 0;//具体代理的任务

protected:
	Subject();
};

class ConcreteSubject:public Subject{//具体主体角色，也叫作被委托角色，被代理角色。业务逻辑的具体执行者
public:
	ConcreteSubject();
	~ConcreteSubject();
	void Request();
};

class Proxy:public Subject{//代理主体角色，委托类，代理类。
public:
	Proxy();
	Proxy(Subject* _sub);
	void Request()//实现对委托者的委托任务执行与补偿
	{
		bef();
		this->_sub->Request();
		end();
	}

	void bef(){}
	void end(){}
	~Proxy();

private:
	Subject* _sub;
};