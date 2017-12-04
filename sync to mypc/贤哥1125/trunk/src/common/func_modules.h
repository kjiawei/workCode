#ifndef FUNC_MODULES_H
#define FUNC_MODULES_H

#include <list>
#include <iostream>
#include "packet.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

using namespace std;

class FuncModule
{
public:
    bool swFlag;
    FuncModule(){}
    virtual void FuncModule_Switch(const char*, const bool)=0;
};

class FuncModuleSeries
{
public:
    FuncModuleSeries() {
        memset(funcName, 0, 20);
        listObj.clear();
    }
    ~FuncModuleSeries(){
        listObj.clear();
    }

    /* append obj to the list */
    int appendObj(FuncModule* obj=NULL) {
        if (obj == NULL) {
            return -1;
        } else {
            listObj.push_back(obj);
        }
        return 0;
    }

    /* remove obj to the list */
    int removeObj(FuncModule* obj=NULL) {
        if (obj == NULL) {
            return -1;
        } else {
            listObj.remove(obj);
        }
        return 0;
    }

    /* to exec the FuncModule_Switch */
    int Update(bool sw) {
        list<FuncModule*>::iterator it;
        for (it=listObj.begin(); it != listObj.end(); ++it) {
            (*it)->FuncModule_Switch(funcName, sw);
        }
        return 0;
    }

    int Update(void) {
        return Update(false);
    }


private:
    char funcName[20];
    list<FuncModule*> listObj;
};

bool funcModuleInit(void);
bool funcModuleQuit(void);

FuncModuleSeries *funcModuleSeriesFind(const char *name=NULL);
FuncModuleSeries *funcModuleSeriesAppend(const char *name=NULL);
bool seriesAppendFuncModule(const char *name=NULL, FuncModule* obj=NULL);
bool seriesAppendDelModule(const char *name=NULL, FuncModule* obj=NULL);

#endif
