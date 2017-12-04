/*****************************************************
** 作者： ？？？
** 创始时间：2014-1-18
** 修改人：Huangjunxian
** 修改时间：2014-1-18
** 描述：
**    5058报警模块数据库接口 《头文件》
*****************************************************/

#ifndef DATA_SQLITE_H
#define DATA_SQLITE_H

#include <QtSql>
#include <QString>
#include "packet.h"
#include "Alarm_module.h"
#include "scene_disinfect.h"
#include "scene_log_run.h"

#define SQLDB_NAME          "db/hemodialysis.db"
#define DBTABLE_DATA        "warnDataTable"
#define DBTABLE_STATUSDATA  "statusDataTable"
#define DBTABLE_HISTORY     "warnTable"

#define SQLDB_DISINFECT     "db/disinfect.db"
#define DBTABLE_FILTER      "filter"
#define DBTABLE_RECORD      "record"

#define SQLDB_LOGDATA      "db/logData.db"
#define DBTABLE_RUNLOG      "RunLog"
#define RUNLOG_MAXNUM       500  //最大记录数目
#define RUNLOG_PAGEITEMS    10  //16


#define DE_RUNLOG_TYPE 1    //日志记录表#/
#define DE_CUREDATE_TYPE 2  //数据曲线记录表#/
#define DE_WARNTABLE_TYPE 3 //报警记录表#/
#define DE_PATIENTINFOR_TYPE 4 //病人信息记录表#/
#define DE_CUREDATAINFOR_TYPE 5 //病人治疗数据记录表#/




struct stpatientinfor   //病人信息记录表#/
{
    int idnum;  //ID号
    int curenum; //治疗次号
    unsigned int timesec;  //时间戳
    QString name;   //名字
    int gender;    //性别
    unsigned char age;   //年龄
    unsigned char depart;  //科室
    unsigned char weight;// 体重
    unsigned char diagno;//诊断
    int time;  //治疗时间
    int temp;  //透析液温度
    int na;
    int hco3;
    int ufgoal;  //UF目标
    int bldflow;   //血泵流速
    int subvol;   //置换量
    int heprate;   //肝素速度
    unsigned char treatmode;  //治疗模式
    unsigned int profilemode;  //曲线模式
};

struct stcuredatainfor //病人治疗数据记录表#/
{
    int idnum;  //ID号
    int curenum; //治疗次号
    unsigned int timesec;  //时间戳
    int c701;     //B液
    int c702;     //出水口
    int c709;     //混合电导  c703
    int c704;     //透后电导
    int t201;     //B液
    int t204;     //出水口
    int t205;     //透后
    int t203;     //混合
    int ap;       //动脉
    int vp;       //静脉
    int tmp;      //跨膜
    int pulse;    //脉搏
    int dia;      //舒张
    int sys;      //收缩
};


//数据库缓冲保存结构#/
struct ST_sqldat{
    int time;
    int stavetype;   //保存类型#/

    struct stRunlog
    {
        int type;
        int status;
        QString opration;
        QString note;
    }runLog;

    struct stCuredate
    {
        int tpid;
        int c701;
        int c702;
        int c704;
        int c709;
        int t201;
        int t203;
        int t204;
        int t205;
    }cureDate;

    struct stWaretable
    {
        int who;
        int code;
        int priority;
        int alarmid;
        int dispose;
    }warnTable;

    stpatientinfor patientinfor;

    stcuredatainfor curedatainfor;
};

extern QList<ST_sqldat> ListSqlDat; //数据库缓冲队列#/




int DataSql_Open();
int DataSql_Close();

//--------------------------------------------------------------------报警处理#/

bool DataSql_openHistoryTable(void);

//查询某条报警记录是否存在#/
bool DataSql_Check(int time,int alarmid);

//插入数据库缓冲列表#/
void DataList_InsertwarnTable(int time,int who,int code,int pri,int alarmid,int dispose);

//往报警表插入一条记录#/
bool DataSql_InsertwarnTable(int time,int who,int code,int pri,int alarmid,int dispose);

//修改特定报警条目信息#/
bool DataSql_UpdatewarnTable(int time,int alarmid,int dispose);

//删除选中的一条报警信息#/
bool DataSql_DeletewarnTable(int time,int alarmid);

//获取报警的行数#/
int DataSql_GetrownumwarnTable();

//从历史报警记录表中读取报警数据，并放到QMap中#/
QMap<int,AlarmInfor> DataSql_GetinforwarnTable(int offset, int count, int &numid);



//--------------------------------------------------------------------消毒信息保存和超净滤器使用信息处理#/

//消毒记录保存#/
bool DataSql_InsertDisinfectTable(DisinfcetInfo record);
//读取消毒记录#/
QMap< int,DisinfcetInfo> DataSql_GetinfoDisinfect(int offset, int count);
//获取超净滤器记录表数据#/
QMap< int,FilterTimes> DataSql_GetTimesFilter(int offset, int count);
//修改超净滤器记录表#/
bool DataSql_UpdateFilterTimes(FilterTimes filter);
//获取指定滤器使用情况#/
FilterTimes DataSql_GetFilterTimes(unsigned int id);

//--------------------------------------------------------------------运行日志记录#/

//插入数据库缓冲列表#/
void DataList_InsertRunLogTabel(logInfo rec);
//插入运行日志
bool DataSql_InsertRunLogTable(logInfo record);

QMap< int,logInfo> DataSql_GetRunLog(int offset, int count);

int DataSql_GetRunLogNum();

bool DataSql_UpdateRunLog(logInfo info);


//--------------------------------------------------------------------治疗记录曲线相关#/

//查询治疗数据表中，共有几个治疗，即查询typeid有几个值#/
QList<int> DataSql_GetCureSumID();
//根据治疗次数的ID号typeid将该次治疗的所有数据读取出来#/
//QMap<int,CureProfileDate> DataSql_GetCureDataById(int id);
//删除某一次治疗的全部数据内容#/
bool DataSql_DeleteCureDataById(int id);
//插入数据库缓冲列表#/
void DataList_InsertcureTable(int time,int cureid,int c701,int c702,int c704,int c709,int t201,int t204, int t205, int t203);
//往治疗记录曲线数据表里面插入一条数据#/
bool DataSql_InsertcureTable(int time,int cureid,int c701,int c702,int c704,int c709,int t201,int t204, int t205, int t203);

//数据库缓冲列表处理 num:每次处理条数#/
bool DataSql_ListHandle(int num);


//--------------------------------------------------------------------patientinfor 病人信息相关#/
//往病人信息数据表里面插入一条数据  插入数据库缓冲列表#/
bool DataList_Insertpatientinfor(stpatientinfor infor);

//往病人信息数据表里面插入一条数据  直接操作数据库#/
bool DataSql_Insertpatientinfor(stpatientinfor infor);

//从病人信息数据表里面读取一条数据#/
stpatientinfor DataSql_Readpatientinfor(int idnum,int curenum);

//根据ID删除某一次治疗的病人信息数据内容#/
bool DataSql_DeletepatientinforByIdnum(int idnum,int curenum);

//根据ID号从病人信息数据表里面读取出来该病人所有治疗信息#/
QMap<int,stpatientinfor> DataSql_GetpatientinforByIdnum(int idnum);

//查询病人信息数据表中，某个ID号共有几次治疗,返回治疗次号值#/
QList<int> DataSql_patientinforGetNumByIdnum(int idnum);



//--------------------------------------------------------------------curedatainfor 病人治疗数据记录相关#/
//往病人信息数据表里面插入一条数据  插入数据库缓冲列表#/
bool DataList_Insertcuredatainfor(stcuredatainfor infor);

//往病人信息数据表里面插入一条数据  直接操作数据库#/
bool DataSql_Insertcuredatainfor(stcuredatainfor infor);

//根据ID号和治疗次号将该次治疗的所有数据从病人信息数据表读取出来#/
QMap<int,stcuredatainfor> DataSql_GetcuredatainforByIdnum(int idnum,int curenum);

//根据ID删除某一次治疗的病人数据#/
bool DataSql_DeletecuredatainforByIdnum(int idnum,int curenum);

#endif
