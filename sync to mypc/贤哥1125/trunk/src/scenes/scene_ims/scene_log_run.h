#ifndef SCENE_LOG_RUN_H
#define SCENE_LOG_RUN_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"


 //操作类型#/
#define DE_LOGINFO_PAGEDITOR   1   //普通页面编辑#/
#define DE_LOGINFO_ENGINEDITOR 2    //工程页面编辑#/
#define DE_LOGINFO_OPER  3          //日常操作#/



struct logInfo
{
    int id;
    int time;    //时间#/
    int type;        //操作类型#/
    int status;  //当前工作状态#/
    QString opration;  //操作内容#/
    QString note;
    QString objName; //编辑框对象名#/
    QString strOld;  //旧的参数值#/
    QString strNew;  //新的参数值#/
};

class SceneLogRun : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneLogRun(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void initTableLog();
    void cleanTable();
    void setTableLog(int row,logInfo log);
private slots:
    void nextPg();
    void prePg();
private:
    inline void setUi();
    void insertLog(int row,int id,logInfo log);
    void UpdateTableLog(int offSet);
private:
    QPushButton nextPage;
    QPushButton prePage;
    QLabel pageInfo;
    int CurrentPage;
    QTableWidget * tableWidgetLog;
    //QStandardItemModel test;
};

class LogRunModel : public QThread
{
    Q_OBJECT
public:
    LogRunModel(void);
    ~LogRunModel();
    bool addLog(logInfo log);
    bool addLog(QString action=NULL,int type=0,int status=0);
public slots:
    void insertLog(logInfo log);
    void insertLog(void);
    void cleanEngMap(int id);
signals:
    void SigLog(logInfo log);
protected:
    void run();
private:
  QMap<QString,logInfo> *enginLogMap;//工程模式操作记录缓冲区#/
  int enginIndex,enginTabIndex,formulaID;

  bool logHandle(logInfo *log);
  bool logHandle(QMap<QString,logInfo> *map,logInfo *log);
};

#endif
