#include <QHash>
#include "data_custom.h"
#include "global_var.h"

QHash<int, DataCustom *> hashParameter;

bool dataHashInit(void)
{
    hashParameter.clear();
    return true;
}

bool dataHashQuit(void)
{
    QHash<int, DataCustom*>::const_iterator it;
    for (it= hashParameter.constBegin(); it != hashParameter.constEnd(); it++) {
        delete it.value();
    }
    hashParameter.clear();
    return true;
}

DataCustom *dataNodeFind(int name)
{
    if (name == 0) {
        return NULL;
    }
    //从hash表列表中查找关键字为name的值，如果找不到，则返回一个默认的值#/
    return hashParameter.value(name);
}

DataCustom *dataNodeAppend(int name, void **dataPtr, const size_t len)
{
    if ((name == 0) || (len == 0)) {
        return NULL;
    }

    DataCustom *hashNode = hashParameter.value(name);
    if (hashNode == NULL) {
        hashNode = new DataCustom();
        hashParameter.insert(name, hashNode);
    }
    //在数据包管理类中创建空间存放数据结构内容#/
    hashNode->DataCreate(dataPtr, len, name);
    return hashNode;
}

bool dataRead(int name, void* dest)
{
    if ((name == 0) || (dest == NULL)) {
        return false;
    }
//    sem.acquire(1);
    DataCustom *dataNode = dataNodeFind(name);
    if (dataNode == NULL) {
//        sem.release(1);
        return false;
    }
//    sem.release(1);
    return dataNode->DataReadCopy(dest);
}

bool dataWrite(int name, void* dest)
{
     DataCustom *dataNode = hashParameter.value(name);
     if(dataNode==NULL)
         return false;

     return dataNode->DataWriteCopy(dest);
}

bool dataNodeAddObject(int name, ActionCustom* obj)
{
    if ((name == 0) || (obj == NULL)) {
        return false;
    }

    DataCustom *nodePtr = hashParameter.value(name);
    if (nodePtr == NULL) {
        return false;
    }

    nodePtr->Append(obj);
    //nodePtr->Update(obj);

    return true;
}

bool dataNodeDelObject(int name, ActionCustom* obj)
{
    if ((name == 0) || (obj == NULL)) {
        return false;
    }

    DataCustom *nodePtr = hashParameter.value(name);
    if (nodePtr == NULL) {
        return false;
    }

    nodePtr->Remove(obj);
    return true;
}

void DataCustomSiglot::SlotsUpdate(int name, void *dataPtr)
{
    DataCustom *ptr = dataNodeFind(name);//查找一个name对应的数据节点区#/
    if (ptr != NULL) {
        ptr->Update(dataPtr);         //更新该节点的设置或者参数#/
    }
}

DataCustomSiglot dataCustomSiglot;
void dataNodeUpdate(int name, void *dataPtr)
{
    sem.acquire(1);
    //如果是其他子线程调用了dataCustomSiglot这个类变量，则通过发送信号的方式调用SlotsUpdate#/
    //因为如果其他线程中直接调用SlotsUpdate可能不成功，因为子线程中可能没有定义SlotsUpdate中#/
    //用到的相应类型和变量#/
    if (dataCustomSiglot.ThreadIdMatch() == false) {

        dataCustomSiglot.TestOccurSignal(name, dataPtr);
    } else {
        dataCustomSiglot.SlotsUpdate(name, dataPtr);
    }
    sem.release(1);
}
