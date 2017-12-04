#include "page_info.h"
#include "global_var.h"

//页面切换功能的文件#/

PageManage::PageManage()
{
    hashPage.clear();
    memset(pageCurrent, 0, 64);
}

//

PageManage::~PageManage()
{
    QHash<QString, PageInfo*>::const_iterator it;
    for (it= hashPage.constBegin(); it != hashPage.constEnd(); it++) {
        delete it.value();
    }
    hashPage.clear();

    memset(pageCurrent, 0, 64);
    /*
     * some work, later
     */
}

PageInfo* PageManage::PageFind(const char* name)
{
    if (name == NULL) {
        return NULL;
    }
    return hashPage.value(name);
}

PageInfo* PageManage::PageAppend(const char* name)
{
    if ((name!=NULL) && ((strlen(name)>=64) || (strlen(name)==0))) {
        return NULL;
    }

    PageInfo* page = hashPage.value(name);
    if (page == NULL) {
        page = new PageInfo(name);
        hashPage.insert(name, page);
    }
    return page;
}

bool PageManage::PieceAppend(const char* name, PieceVisible* piece)
{
    if (NULL == name || NULL == piece) {
        return false;
    }

    PageInfo* page = PageFind(name);
    if (NULL == page) {
        page = PageAppend(name);
        if (NULL == page) {
            return false;
        }
    }

    piece->setCusVisible(false);
    return page->Append(piece);
}

bool PageManage::PageSwitch(const char* pageNew, char* para)
{
    PageInfo* page = NULL;

    if (pageNew == NULL) {
        return false;
    }

    if (strcmp(pageCurrent, pageNew) == 0) {
        return true;
    }

    page = PageFind(pageCurrent);
    if(page != NULL) {
        page->SetShow(false, para);
    }
   
    page = PageFind(pageNew);
    if (page == NULL) {
        return false;
    }
    strcpy(pageCurrent, pageNew);
    page->SetShow(true, para);

    return true;
}

bool PageAppendPiece(const char* name, PieceVisible* piece)
{
    return pageManage.PieceAppend(name, piece);
}

bool PageShow(const char* pageNew, char* arg)
{
    return pageManage.PageSwitch(pageNew, arg);
}

char* PageCurrent(void)
{
    return pageManage.PageCurrent();
}
