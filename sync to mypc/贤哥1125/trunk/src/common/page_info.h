#ifndef PAGE_INFO_H
#define PAGE_INFO_H

#include <list>
#include <string.h>
#include <iostream>

using namespace std;

class PieceVisible
{
public:
    PieceVisible() {
        pieceShow = true;
    }
    virtual void setCusVisible(bool vis=false, char* para=NULL)=0;

public:
    bool pieceShow;
};

class PageInfo
{
public:
    PageInfo() {
        pageShow = false;
        memset(title, 0, 64);
    }
    PageInfo(const char*  name) {
        strcpy(title, name);
    }

    ~PageInfo() {
        listPiece.clear();
    }

    bool IsShow() {
        return pageShow;
    }

    bool SetShow(bool pageShow = false, char* para=NULL) {
        list<PieceVisible*>::iterator it;
        for (it=listPiece.begin(); it != listPiece.end(); ++it) {
            (*it)->setCusVisible(pageShow, para);
        }
        return true;
    }

    bool Append(PieceVisible* piece=NULL) {
        if (piece == NULL) {
            return false;
        } else {
            listPiece.push_back(piece);
        }
        return true;
    }

public:
    char title[64];

private:
    bool pageShow;
    list<PieceVisible*> listPiece;
};

#include <QHash>
class PageManage
{
public:
    PageManage();
    ~PageManage();

    bool PageSwitch(const char* pageNew=NULL, char* para=NULL);
    bool PieceAppend(const char* name=NULL, PieceVisible* piece=NULL);

    PageInfo* PageFind(const char*  name=NULL);
    PageInfo* PageAppend(const char*  name=NULL);

    char*  PageCurrent() {
        return pageCurrent;
    }

private:
    char pageCurrent[64];
    QHash<QString, PageInfo*> hashPage;
};

bool PageAppendPiece(const char*  name=NULL, PieceVisible* piece=NULL);
bool PageShow(const char* pageNew=NULL, char* arg=NULL);
char* PageCurrent(void);

#endif
