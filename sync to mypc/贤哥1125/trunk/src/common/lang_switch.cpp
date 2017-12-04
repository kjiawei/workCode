#include "lang_switch.h"
#include "global_var.h"

//语言切换#/

//把需要更新语言的页面，添加到链表里面去#/
LangSetText::LangSetText()
{
    listText.push_back(this);
}

//需要更新语言时，把每一个页面的CustomSetText函数执行一次，把整个应用程序刷新语言。。。。#/
void ReUpdateLangText()
{
    list<LangSetText*>::iterator it;
    for (it=listText.begin(); it != listText.end(); ++it) {
        (*it)->CustomSetText();
    }
}
