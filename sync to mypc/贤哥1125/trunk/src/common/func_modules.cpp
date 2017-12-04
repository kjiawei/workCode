#include "global_var.h"
#include "func_modules.h"

bool funcModuleInit(void)
{
    return true;
}

bool funcModuleQuit(void)
{
    QHash<QString, FuncModuleSeries*>::const_iterator it;
    for (it= hashFuncModule.constBegin(); it != hashFuncModule.constEnd(); it++) {
        delete it.value();
    }
    hashFuncModule.clear();

    return true;
}

FuncModuleSeries *funcModuleSeriesFind(const char *name)
{
    if (name == NULL) {
        return NULL;
    }
    return hashFuncModule.value(QString(name));
}

FuncModuleSeries *funcModuleSeriesAppend(const char *name)
{
    if (name == NULL) {
        return NULL;
    }

    FuncModuleSeries *hashNode = hashFuncModule.value(QString(name));
    if (hashNode == NULL) {
        hashNode = new FuncModuleSeries();
        hashFuncModule.insert(QString(name), hashNode);
    }
    return hashNode;
}

bool seriesAppendFuncModule(const char *name, FuncModule* obj)
{
    if ((name == NULL) || (obj == NULL)) {
        return false;
    }

    FuncModuleSeries *nodePtr = hashFuncModule.value(QString(name));
    if (nodePtr == NULL) {
        return false;
    }

    nodePtr->appendObj(obj);
    nodePtr->Update(obj);

    return true;
}

bool seriesAppendDelModule(const char *name, FuncModule* obj)
{
    if ((name == NULL) || (obj == NULL)) {
        return false;
    }

    FuncModuleSeries *nodePtr = hashFuncModule.value(QString(name));
    if (nodePtr == NULL) {
        return false;
    }

    nodePtr->removeObj(obj);
    return true;
}
