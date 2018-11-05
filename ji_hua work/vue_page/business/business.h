#ifndef _BUSINESS_H_
#define _BUSINESS_H_

#include <stdio.h>
#include <string.h>

//软件主要业务流程,通过FSM(有限状态机实现)
//参考https://www.cnblogs.com/benxintuzi/p/4931258.html  只做了10来行

class State{
	public:
		virtual State* Transition(char c) = 0;
};

class Business{
	public:
		Business();
		void Reset();
		void Advance(char c);
		int EndState();
		int DoomState();
		
	protected:
	
	private:
		State* p_current;
};

class State1:public State
{
	public:
		State* Transition(char c);
};

#endif