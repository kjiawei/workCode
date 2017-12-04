#ifndef COMMON_FUNC_H
#define COMMON_FUNC_H
#include <QObject>
#include <QEvent>
#include <QMap>

void Debug_init();

void Debug_insert(const char *str);

void Debug_record(const char *str);

void Debug_setuptrap();

//死机后重生处理函数---保存参数#/
void Rebirth_Handle(void);

//HJX实现非阻塞性的互斥锁
void Try_Map_mutex(void);

int SystemShellCmd(const char *file, char **arg);

void us_sleep(const int us);

void CallTsCalibrate(void);

void GetScreenShots(void);

bool MarkDisinfectFinish(bool disinfectFinish);

bool GetDisinfectState(void);

//改变血泵的速度#/
void CHANGED_BP_SPEED(unsigned short int bpspeed);

//改变超滤泵的速度#/
void CHANGED_UF_SPEED(bool curerate,int ufspeed);

//超滤泵和时间同步处理函数#/
void UF_SYNTIME_HANDLE(unsigned int starttime,unsigned int curtime,unsigned int endtime,unsigned int finishuf,unsigned int totaluf,unsigned int ufsetrate);

//NA曲线处理函数#/
void NA_PROFILE_HANDLE(unsigned int starttime,unsigned int curtime,unsigned int endtime);

//UF曲线处理函数#/
void UF_PROFILE_HANDLE(unsigned int starttime,unsigned int curtime,unsigned int endtime);

//透析液曲线处理函数#/
void DIA_PROFILE_HANDLE();
//肝素曲线处理函数#/
void HEP_PROFILE_HANDLE();
//电导曲线处理函数#/
void COND_PROFILE_HANDLE();
//温度曲线处理函数#/
void TEMP_PROFILE_HANDLE();
//HCO3曲线处理函数#/
void HCO3_PROFILE_HANDLE();


//写XML文件特定节点内容#/
void Write_XML_Node(QString path,QString strmode,QString strnum);
//读XML文件特定节点内容#/
QMap<QString, QString>  Read_XML_Node(QString path);

class FunctionsKeyEater : public QObject
{
    Q_OBJECT
public:
    FunctionsKeyEater(QObject *parent=0):QObject(parent){}

signals:
    void mouseEvent(int id);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};


#endif
