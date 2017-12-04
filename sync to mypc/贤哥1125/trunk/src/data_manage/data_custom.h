#ifndef DATA_CUSTOM_H
#define DATA_CUSTOM_H

#include <list>
#include <iostream>
#include "packet.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "common_func.h"
#include <QDebug>

#include "config_ui.h"
using namespace std;

#define CompareAssignBool(x, y, f)     do{if(x!=y){x=y;f=true;}}while(0)

//数据包对应的操作处理基类#/
//如在特定页面有对数据包进行相应处理的，则需继承该类，并实现ActionHandle函数#/
//然后将该派生类的指针用DataCustom类的Append函数添加到该数据包的操作处理队列中#/
class ActionCustom
{
public:
    ActionCustom(){}
    virtual void ActionHandle(const void*, int)=0;
};

//数据包管理类#/
//可以认为一个个数据结构是一个个的数据包，为了统一对这些数据包进行管理而实现该类#/
//可创建数据包空间，对数据包命名，有数据包操作处理队列#/
class DataCustom //:public QObject#/
{
public:
    DataCustom() //构造函数#/
    {
//        memset(dataName, 0, 32);
        dataName=0;
        dataLen = 0;
        dataPtr = NULL;
        write_mutex=false;
        mutex_count=0;
        listObj.clear(); //数据包操作处理队列#/
    }
    virtual ~DataCustom(){ //析构函数#/
        DataFree();
        listObj.clear();
    }

    /* malloc the data */
    //创建数据存放空间，并且初始化为0#/
    int DataCreate(void **dp, const size_t len, int name)
    {
        if (dataPtr != NULL) { //如果之前已经指向别的数据结构，则释放#/
            //if (strcmp(name, dataName) == 0)
            if(name==dataName)
            {
                if (dp != NULL)
                    *dp = dataPtr;
                return 0;
            }
            else
            {
                free(dataPtr);
            }
        }

        dataPtr = malloc(len);
        if (dataPtr == NULL) {
            return -1;
        }
        if (dp != NULL)
            *dp = dataPtr;
        dataLen = len;
//        strcpy(dataName, name);
        dataName=name;
        memset(dataPtr, 0, len);
        return 0;
    }

    /* free the data*/
    void DataFree(void) //释放指向的数据结构#/
    {
        if (dataPtr!=NULL) {
            free(dataPtr);
            dataPtr=NULL;
        }
    }

    /* append obj to the list */
    int Append(ActionCustom* obj=NULL) //将指向数据结构，关联的操作处理添加到队列#/
    {
        if (obj == NULL) {
            return -1;
        } else {
            listObj.push_back(obj);
        }
        return 0;
    }

    /* append obj to the list */
    int Remove(ActionCustom* obj=NULL)
    {
        if (obj == NULL) {
            return -1;
        } else {
            listObj.remove(obj);
        }
        return 0;
    }

    /* copy the data for read */
    bool DataReadCopy(void *ptr) //将指向的数据结构内容读取出来#/
    {
        if (ptr == NULL) {
            return false;
        }

        memcpy(ptr, dataPtr, dataLen);

        return true;
    }

    /* copy the data for write */
    bool DataWriteCopy(void *ptr)  //将特定内容写入到指向的数据结构中#/
    {
        if (ptr == NULL) {
            return false;
        }
        //互斥处理++#/
        while(write_mutex)
        {
            mutex_count++;
            if(mutex_count>=6)
            {
                write_mutex=false;
                break;
            }
            else
            {
                us_sleep(10000);
            }
        }
        mutex_count=0;
        write_mutex=true;
        //互斥处理--#/
        memcpy(dataPtr, ptr, dataLen);
        write_mutex=false;
        return true;
    }

    /* to exec the ActionHandle */
    int Update(void) //进行数据包绑定的所有操作处理#/
    {
        list<ActionCustom*>::iterator it; //建立一个Actionustom指针的迭代器#/
        for (it=listObj.begin(); it != listObj.end(); ++it)
        {
            //指向ActionCustom的派生类中的ActionHandle函数，进行特定数据包绑定的操作处理#/
            (*it)->ActionHandle(dataPtr, dataName);
//            us_sleep(2000);
        }
        return 0;
    }

    int Update(void *pData) //更新数据包的数据内容，再进行操作处理#/
    {

#if DEBUG_OTHER_FLAG
    Debug_insert("Data_Update-in01");
#endif
    /*
        //互斥处理++#/
        while(write_mutex)
        {
            mutex_count++;
            if(mutex_count>=6)
            {
                write_mutex=false;
                break;
            }
            else
            {
                us_sleep(10000);
            }
        }
        mutex_count=0;
        write_mutex=true;
    //互斥处理--#/
#if DEBUG_OTHER_FLAG
    Debug_insert("Data_Update-in02");
#endif
*/

        memcpy(dataPtr, pData, dataLen);
        Update();

        /*
        write_mutex=false;
#if DEBUG_OTHER_FLAG
    Debug_insert("Data_Update-out");
#endif
    */
//        return Update();
        return 0;
    }

    int Update(ActionCustom* obj) //以指定的操作处理方式处理该数据包内容#/
    {
        if (dataPtr != NULL) {
            obj->ActionHandle(dataPtr, dataName);
        }
        return 0;
    }

private:
    /* dataName: data type*/
//    char dataName[35]; //数据包名称#/
    int dataName;//数据包对应ID号#/
    /* dataLen: data length*/
    size_t dataLen; //数据包长度#/
    void *dataPtr;  //数据包指针#/
    mutable bool write_mutex;  //HJX为写保护而设#/
    char mutex_count;    //进行加锁循环计数，防止死锁#/
    list<ActionCustom*> listObj; //数据包操作处理队列#/

};

bool dataHashInit(void); //QHash初始化#/
bool dataHashQuit(void); //对QHash中数据包的空间进行释放#/

//每一个设备节点对应一种数据结构，即数据包。并且有其特定的操作处理#/
//即一个数据包和设备可以抽象为一个数据节点#/

//根据数据包名称，查找并返回对应的数据包管理类#/
DataCustom *dataNodeFind(int name);

//首先通过数据包名称在QHash队列中查找数据包对应的管理类DataCustom，并返回#/
//如果没有查找到，则通过数据包结构体变量和数据包自定义名称，new一个DataCustom#/
//并用数据包名称做关键字KEY，将DataCustom添加到QHash队列中，并返回#/
DataCustom *dataNodeAppend(int name, void **dataPtr=NULL, const size_t len=0);

//通过数据包名称（即KEY）在QHash中查找到对应的数据包管理类，然后将操作处理派生类指针添加进去#/
bool dataNodeAddObject(int name, ActionCustom* obj=NULL);

//通过数据包名称（即KEY）在QHash中查找到对应的数据包管理类，然后将操作处理派生类指针从中移除#/
bool dataNodeDelObject(int name, ActionCustom* obj=NULL);

//通过数据包名称和数据结构指针更新数据节点内容和操作处理#/
//可用于不同的线程中#/
void dataNodeUpdate(int name, void *dataPtr=NULL);

/* read the data */
//通过数据包名称在QHash中查找到对应的数据包管理类，并将其中的数据内容读取出来放到指针dest指向的地方#/
bool dataRead(int name, void* dest=NULL);
bool dataWrite(int name, void* dest=NULL);


/* Class DataCustomSiglot is used to interaction data with the UI conveniently by threads */
#include <QObject>
#include <QThread>
#include <QDebug>

//数据包更新类#/
//实现了通过数据包名称和数据结构指针更新数据包内容和操作处理#/
//并且子线程通过调用该类的信号实现数据节点更新#/
//主线程（即创建该类变量的线程）通过直接调用该类槽函数实现#/
class DataCustomSiglot : public QObject
{
    Q_OBJECT
public:
    DataCustomSiglot()
    {
        //获取创建本类的线程ID号，以方便标示不同线程#/
        ThreadId = (unsigned long int)QThread::currentThreadId();
        connect(this, SIGNAL(OccurSignal(int, void*)),
            this, SLOT(SlotsUpdate(int, void*)), Qt::QueuedConnection);
    }

    bool ThreadIdMatch()
    {
        //获取调用本类的当前线程ID号#/
        if (ThreadId != (unsigned long int)QThread::currentThreadId()) {
            return false;
        }
        return true;
    }

    //根据数据包名称和数据结构变量指针，发送信号#/
    void TestOccurSignal(int name, void *dataPtr=NULL)
    {
        {
            emit OccurSignal(name, dataPtr);
        }
    }

public slots:
    //根据数据包名称和数据结构变量指针，更新数据包数据内容，并且进行数据包对应的所有操作处理#/
    void SlotsUpdate(int name, void *dataPtr=NULL);

signals:
    void OccurSignal(int name, void *dataPtr);

private:
    unsigned long int ThreadId;

};

#endif
