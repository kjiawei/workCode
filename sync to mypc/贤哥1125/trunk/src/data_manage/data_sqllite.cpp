/*****************************************************
** 作者： ？？？
** 创始时间：2014-1-18
** 修改人：Huangjunxian
** 修改时间：2014-1-18
** 描述：
**    5058报警模块数据库接口 《头文件》
*****************************************************/
#include <time.h>
#include <QDebug>
#include <QObject>
#include "data_sqllite.h"
#include "global_var.h"
#include "Alarm_module.h"
#include "scene_record_curve.h"


QList<ST_sqldat> ListSqlDat; //数据库缓冲队列#/

//打开数据库#/
int DataSql_Open()
{
    db = QSqlDatabase::addDatabase("QSQLITE", SQLDB_NAME);
    db.setDatabaseName(SQLDB_NAME);
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection."), QMessageBox::Cancel);
        return false;
    }

    disinfect_db = QSqlDatabase::addDatabase("QSQLITE", SQLDB_DISINFECT);
    disinfect_db.setDatabaseName(SQLDB_DISINFECT);
    if (!disinfect_db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection."), QMessageBox::Cancel);
        return false;
    }

    runLog_db = QSqlDatabase::addDatabase("QSQLITE", SQLDB_LOGDATA);
    runLog_db.setDatabaseName(SQLDB_LOGDATA);
    if (!runLog_db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open runLog database"),
            qApp->tr("Unable to establish a database connection."), QMessageBox::Cancel);
        return false;
    }

    ListSqlDat.clear();

    return 0;
}

//关闭数据库#/
int DataSql_Close()
{

    if (db.isOpen()) {
        db.close();
    }

    if (disinfect_db.isOpen()) {
        disinfect_db.close();
    }

    if(runLog_db.isOpen())
    {
        runLog_db.close();
    }

    return 0;
}

//只能查询对应code的一条记录#/
bool DataSql_Query(QString tableName, QString query_name, QString query_code,
                   int index, QMap<int, QVariant> *mapValue)
{
    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("SELECT * FROM ")
            .append(tableName)
            .append(" WHERE ")
            .append(query_name)
            .append(" = ")
            .append(query_code);

    bool isRet = database.exec(queryStr);

    while(database.next()){
        for(int id = 0; id < index; id ++) {
            mapValue->insert(id, database.value(id));
        }
    }
    return isRet;
}


QString DataSql_Query(QString tableName, QString selectName, QString queryName, QString queryResult)
{
    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("SELECT ")
            .append(selectName)
            .append(" FROM ")
            .append(tableName)
            .append(" WHERE ")
            .append(queryName)
            .append(" = ")
            .append(queryResult);

    database.exec(queryStr);

    database.next();

    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QString str = codec->fromUnicode(database.value(0).toString());

    return str;
}

QString DataSql_Query(QString tableName, QString selectTitle, QString selectInfo, QString selectPro, QString queryName, QString queryResult)
{
    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("SELECT ")
            .append(selectTitle)
            .append(", ")
            .append(selectInfo)
            .append(", ")
            .append(selectPro)
            .append(" FROM ")
            .append(tableName)
            .append(" WHERE ")
            .append(queryName)
            .append(" = ")
            .append(queryResult);

    database.exec(queryStr);

    database.next();

    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QString str = codec->fromUnicode(database.value(0).toString()); //title#/
    str.append("=")
       .append(codec->fromUnicode(database.value(1).toString()))    //解决方法#/
       .append("=")
       .append(codec->fromUnicode(database.value(2).toString()));   //优先级#/
    return str;
}


//创建历史报警记录表#/
bool DataSql_openHistoryTable(void) {

    QSqlQuery query("",db);
    QString queryStr ;
    queryStr.append("create table ")
            .append(DBTABLE_HISTORY)
            .append("(")
            .append("_id INTEGER PRIMARY KEY AUTOINCREMENT, ")
            .append("time varchar(20), ")
            .append("address varchar(10), ")
            .append("code varchar(20), ")
            .append("priority varchar(5), ")
            .append("note varchar(512), ")
 //           .append("help varchar(1024),")
            .append("dispose varchar(5))");

    return query.exec(queryStr);
}

/*---------------new creator-------------------*/

//查询某条报警记录是否存在#/
bool DataSql_Check(int time,int alarmid)
{
#if DEBUG_DATA_SQL
    Debug_insert("DataSql_Check-in");
#endif
    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("SELECT * ")
            .append("FROM ")
            .append(DBTABLE_HISTORY)
            .append(" WHERE time='")
            .append(QString::number(time))
            .append("' AND alarmid='")
            .append(QString::number(alarmid))
            .append("'");
     database.exec(queryStr);

#if DEBUG_DATA_SQL
    Debug_insert("DataSql_Check-out");
#endif
     return database.next();
}

//插入数据库缓冲列表#/
void DataList_InsertwarnTable(int time,int who,int code,int pri,int alarmid,int dispose)
{
    ST_sqldat sqldat;

    sqldat.time=time;
    sqldat.stavetype=DE_WARNTABLE_TYPE;

    sqldat.warnTable.who=who;
    sqldat.warnTable.code=code;
    sqldat.warnTable.priority=pri;
    sqldat.warnTable.alarmid=alarmid;
    sqldat.warnTable.dispose=dispose;

    ListSqlDat.append(sqldat);
}

//往报警表插入一条记录#/
bool DataSql_InsertwarnTable(int time,int who,int code,int pri,int alarmid,int dispose)
{
    QSqlQuery query("",db);
    QString queryStr ;
    queryStr.append("INSERT INTO ")
            .append(DBTABLE_HISTORY)
            .append("(time, who, code, priority, alarmid, dispose)")
            .append(" values('")
            .append(QString::number(time))
            .append("', '")
            .append(QString::number(who))
            .append("', '")
            .append(QString::number(code))
            .append("', '")
            .append(QString::number(pri))
            .append("', '")
            .append(QString::number(alarmid))
            .append("', '")
            .append(QString::number(dispose))
            .append("')");
    return query.exec(queryStr);
}

//修改特定报警条目信息#/
bool DataSql_UpdatewarnTable(int time,int alarmid,int dispose)
{
    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("UPDATE ")
            .append(DBTABLE_HISTORY)
            .append(" SET ")
            .append("dispose='")
            .append(QString::number(dispose))
            .append("' WHERE time='")
            .append(QString::number(time))
            .append("' AND alarmid='")
            .append(QString::number(alarmid))
            .append("'");

    return database.exec(queryStr);
}

//删除选中的一条报警信息#/
bool DataSql_DeletewarnTable(int time,int alarmid)
{
#if DEBUG_DATA_SQL
    Debug_insert("DataSql_DeletewarnTable-in");
#endif
    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("DELETE ")
            .append(" FROM ")
            .append(DBTABLE_HISTORY)
            .append(" WHERE time='")
            .append(QString::number(time))
            .append("' AND alarmid='")
            .append(QString::number(alarmid))
            .append("'");
#if DEBUG_DATA_SQL
    Debug_insert("DataSql_DeletewarnTable-out");
#endif
    return database.exec(queryStr);
}

//获取报警的行数#/
int DataSql_GetrownumwarnTable()
{
#if DEBUG_DATA_SQL
    Debug_insert("DataSql_GetrownumwarnTable-in");
#endif
    int num;
    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("SELECT ")
            .append(" COUNT() AS rownum")
            .append(" FROM ")
            .append(DBTABLE_HISTORY);

    database.exec(queryStr);
    database.next();
    num=database.value(0).toInt();
#if DEBUG_DATA_SQL
    Debug_insert("DataSql_GetrownumwarnTable-out");
#endif
    return num;
}


//从历史报警记录表中读取报警数据，并放到QMap中#/
QMap<int,AlarmInfor> DataSql_GetinforwarnTable(int offset, int count, int &numid)
{
    AlarmInfor alarm;
    QMap<int , AlarmInfor> alarmmap;
#if DEBUG_DATA_SQL
    Debug_insert("DataSql_GetinforwarnTable-in");
#endif
    alarmmap.clear();
    QSqlQuery database("", db);
    QString queryStr;

    int num=0,rem=0;

    num=count/200;    //以200条为一次读取单位
    rem=count%200;    //不够200条剩余的

    while(num)
    {
        queryStr.append("SELECT ")
                .append("time, who, code, priority, alarmid, dispose")
                .append(" FROM ")
                .append(DBTABLE_HISTORY)
//                .append(" order by time desc")
                .append(" LIMIT '")
                .append(QString::number(200))
                .append("' OFFSET '")
                .append(QString::number(offset))
                .append("'");

        database.exec(queryStr);

        while(database.next()) {
            alarm.time=database.value(0).toInt();
            alarm.who=database.value(1).toInt();
            alarm.code=database.value(2).toInt();
            alarm.pri=database.value(3).toInt();
            alarm.alarmid=database.value(4).toInt();
            alarm.dispose=database.value(5).toInt();

            alarmmap.insert(numid,alarm);
            numid++;
        }
        queryStr.clear();
        num--;
        offset+=200;
    }

    queryStr.append("SELECT ")
            .append("time, who, code, priority, alarmid, dispose")
            .append(" FROM ")
            .append(DBTABLE_HISTORY)
//            .append(" order by time desc")
            .append(" LIMIT '")
            .append(QString::number(rem))
            .append("' OFFSET '")
            .append(QString::number(offset))
            .append("'");

    database.exec(queryStr);

    while(database.next()) {
        alarm.time=database.value(0).toInt();
        alarm.who=database.value(1).toInt();
        alarm.code=database.value(2).toInt();
        alarm.pri=database.value(3).toInt();
        alarm.alarmid=database.value(4).toInt();
        alarm.dispose=database.value(5).toInt();

        alarmmap.insert(numid,alarm);
        numid++;
    }

#if DEBUG_DATA_SQL
    Debug_insert("DataSql_GetinforwarnTable-out");
#endif
    return alarmmap;
}



bool DataSql_InsertDisinfectTable(DisinfcetInfo record)
{
    QSqlQuery query("",disinfect_db);
    QString queryStr ;
    queryStr.append("INSERT INTO ")
            .append(DBTABLE_RECORD)
            .append("(time, type)")
            .append(" values('")
            .append(QString::number(record.time))
            .append("', '")
            .append(QString::number(record.type))
            .append("')");

    return query.exec(queryStr);
}

QMap< int,DisinfcetInfo> DataSql_GetinfoDisinfect(int offset, int count)
{
    QMap<int , DisinfcetInfo> map;
    DisinfcetInfo info;
    int i=0;
    map.clear();
    QSqlQuery database("", disinfect_db);
    QString queryStr;
    queryStr.append("SELECT ")
            .append("time, type")
            .append(" FROM ")
            .append(DBTABLE_RECORD)
            .append(" LIMIT '")
            .append(QString::number(count))
            .append("' OFFSET '")
            .append(QString::number(offset))
            .append("'");

    database.exec(queryStr);

    while(database.next()) {
     //   info.timeStr=database.value(0).toString();
        info.time=database.value(0).toInt();
        info.type=database.value(1).toInt();
        map.insert(i,info);
        i++;
    }

    return map;
}


QMap< int,FilterTimes> DataSql_GetTimesFilter(int offset, int count)
{
    offset=offset;
    count=count;

    QMap<int , FilterTimes> map;
    FilterTimes info;
    int i=0;
    map.clear();
    QSqlQuery database("", disinfect_db);
    QString queryStr;
    queryStr.append("SELECT ")
            .append("filterID,useable time,used time, remain time")
            .append(" FROM ")
            .append(DBTABLE_FILTER);

    database.exec(queryStr);

    while(database.next()) {
        info.id=database.value(0).toUInt();
        info.usedableTime=database.value(1).toUInt();
        info.usedTime=database.value(2).toUInt();
        info.remainTime=database.value(3).toUInt();
        map.insert(i,info);
        i++;
    }

    return map;
}

//修改
bool DataSql_UpdateFilterTimes(FilterTimes filter)
{

    QSqlQuery database("", disinfect_db);
    QString queryStr;
    queryStr.append("UPDATE ")
            .append(DBTABLE_FILTER)
            .append(" SET ")
            .append("useableTime='")
            .append(QString::number(filter.usedableTime))
            .append("',usedTime='")
            .append(QString::number(filter.usedTime))
            .append("',remainTime='")
            .append(QString::number(filter.remainTime))
            .append("' WHERE filterID='")
            .append(QString::number(filter.id))
            .append("'");

    return database.exec(queryStr);

/*
    QSqlQuery database("", disinfect_db);
    QString queryStr;
    queryStr.append("UPDATE ")
            .append(DBTABLE_FILTER)
            .append(" SET ")
            .append("useableTime=?")
            .append(",usedTime=?")
            .append(",remainTime=?")
            .append(" WHERE filterID=?");

    database.prepare(queryStr);
    database.addBindValue(QString::number(filter.usedableTime));
    database.addBindValue(QString::number(filter.usedTime));
    database.addBindValue(QString::number(filter.remainTime));
    database.addBindValue(QString::number(filter.id));
    return  database.exec();
    */

}

FilterTimes DataSql_GetFilterTimes(unsigned int id)
{
    FilterTimes info;
    QSqlQuery database("", disinfect_db);
    QString queryStr;
    queryStr.append("SELECT * ")
            .append("FROM ")
            .append(DBTABLE_FILTER)
            .append(" WHERE filterID='")
            .append(QString::number(id))
            .append("'");
     database.exec(queryStr);

     memset(&info,0,sizeof(info));
     if(database.next()) {
         info.id=database.value(0).toUInt();
         info.usedableTime=database.value(1).toUInt();
         info.usedTime=database.value(2).toUInt();
         info.remainTime=database.value(3).toUInt();
     }
     return info;
}

//插入数据库缓冲列表#/
void DataList_InsertRunLogTabel(logInfo rec)
{
    ST_sqldat sqldat;

    sqldat.time=rec.time;
    sqldat.stavetype=DE_RUNLOG_TYPE;

    sqldat.runLog.opration=rec.opration;
    sqldat.runLog.type=rec.type;
    sqldat.runLog.status=rec.status;
    sqldat.runLog.note=rec.note;

    ListSqlDat.append(sqldat);
}

bool DataSql_InsertRunLogTable(logInfo record)
{
    QSqlQuery database("",runLog_db);
    QString queryStr ;

    int totalItem;
    totalItem=DataSql_GetRunLogNum();//获取记录总条数#/

    if(totalItem>RUNLOG_MAXNUM)
    {
         //删除最先的，插入最新的，保持记录条数的数量#/
        queryStr.append("DELETE ")
                .append(" FROM ")
                .append(DBTABLE_RUNLOG)
                .append("where time=(select MIN(time) from ")
                .append(DBTABLE_RUNLOG)
                .append(")");

       database.exec(queryStr);
    }

    queryStr.clear();

    queryStr.append("INSERT INTO ")
            .append(DBTABLE_RUNLOG)
            .append("(Time,Opration,Type,Status,Note)")
            .append(" values('")
            .append(QString::number(record.time))
            .append("', '")
            .append(record.opration)
            .append("', '")
            .append(QString::number(record.type))
            .append("', '")
            .append(QString::number(record.status))
            .append("', '")
            .append(record.note)
            .append("')");

    return database.exec(queryStr);
}


QMap< int,logInfo> DataSql_GetRunLog(int offset, int count)
{
    QMap<int , logInfo> map;
    logInfo info;
    int i=0;
    map.clear();
    QSqlQuery database("", runLog_db);
    QString queryStr;
    queryStr.append("SELECT ")
            .append("Time,Opration,Type,Status,Note")
            .append(" FROM ")
            .append(DBTABLE_RUNLOG)
            .append(" ORDER BY Time DESC")
            .append(" LIMIT '")
            .append(QString::number(count))
            .append("' OFFSET '")
            .append(QString::number(offset))
            .append("'");

    database.exec(queryStr);

    while(database.next()) {
        info.time=database.value(0).toInt();
        info.opration=database.value(1).toString();
        info.type=database.value(2).toInt();
        info.status=database.value(3).toInt();
        info.note=database.value(4).toString();
        map.insert(i,info);
        i++;
    }

    return map;
}


//get all record num
int DataSql_GetRunLogNum()
{
    int num;
    QSqlQuery database("", runLog_db);
    QString queryStr;
    queryStr.append("SELECT ")
            .append(" COUNT() AS rownum")
            .append(" FROM ")
            .append(DBTABLE_RUNLOG);

    database.exec(queryStr);
    database.next();
    num=database.value(0).toInt();

    return num;

}

//修改
bool DataSql_UpdateRunLog(logInfo info)
{
    QSqlQuery database("", runLog_db);
    QString queryStr;

    queryStr.append("UPDATE ")
            .append(DBTABLE_RUNLOG)
            .append(" SET ")
            .append("Time=?")
            .append(",Opration=?")
            .append(",Type=?")
            .append(",Status=?")
            .append(",Note=?")
            .append(" WHERE Time=?");

    database.prepare(queryStr);
    database.addBindValue(QString::number(info.time));
    database.addBindValue(info.opration);
    database.addBindValue(QString::number(info.type));
    database.addBindValue(QString::number(info.status));
    database.addBindValue(info.note);
    database.addBindValue(QString::number(info.time));
    return  database.exec();

}



//查询治疗数据表中，共有几个治疗，即查询typeid有几个值#/
QList<int> DataSql_GetCureSumID()
{
    QList<int> list;
    list.clear();

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("select distinct typeid from cureDate order by time");

    database.exec(queryStr);

    while(database.next()){
        list.append(database.value(0).toInt());
    }

    return list;
}


//根据治疗次数的ID号typeid将该次治疗的所有数据读取出来#/
/*
QMap<int,CureProfileDate> DataSql_GetCureDataById(int id)
{
    QMap<int,CureProfileDate> mapval;
    CureProfileDate info;

    int i=0;
    mapval.clear();

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("select * from cureDate where typeid='")
            .append(QString::number(id))
            .append("' order by time");

    database.exec(queryStr);

    while(database.next()) {
        info.time=database.value(0).toInt();
        info.c701=database.value(2).toInt();
        info.c702=database.value(3).toInt();
        info.c704=database.value(4).toInt();
        info.c709=database.value(5).toInt();
        info.t201=database.value(6).toInt();
        info.t204=database.value(7).toInt();
        info.t205=database.value(8).toInt();
        info.t203=database.value(9).toInt();
        mapval.insert(i,info);
        i++;
    }

    return mapval;
}
*/


//删除某一次治疗的全部数据内容#/
bool DataSql_DeleteCureDataById(int id)
{

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("delete from cureDate where typeid='")
            .append(QString::number(id))
            .append("'");

    return database.exec(queryStr);
}

void DataList_InsertcureTable(int time,int cureid,int c701,int c702,int c704,int c709,int t201,int t204, int t205, int t203)
{
    ST_sqldat sqldat;

    sqldat.time=time;
    sqldat.stavetype=DE_CUREDATE_TYPE;

    sqldat.cureDate.tpid=cureid;
    sqldat.cureDate.c701=c701;
    sqldat.cureDate.c702=c702;
    sqldat.cureDate.c704=c704;
    sqldat.cureDate.c709=c709;
    sqldat.cureDate.t201=t201;
    sqldat.cureDate.t203=t203;
    sqldat.cureDate.t204=t204;
    sqldat.cureDate.t205=t205;

    ListSqlDat.append(sqldat);
}

//往治疗记录数据表里面插入一条数据#/
bool DataSql_InsertcureTable(int time,int cureid,int c701,int c702,int c704,int c709,int t201,int t204, int t205, int t203)
{
    QSqlQuery query("",db);
    QString queryStr ;
    queryStr.append("insert into cureDate (time,typeid,c701,c702,c704,c709,t201,t204,t205,t203) values('")
            .append(QString::number(time))
            .append("', '")
            .append(QString::number(cureid))
            .append("', '")
            .append(QString::number(c701))
            .append("', '")
            .append(QString::number(c702))
            .append("', '")
            .append(QString::number(c704))
            .append("', '")
            .append(QString::number(c709))

            .append("', '")
            .append(QString::number(t201))
            .append("', '")
            .append(QString::number(t204))
            .append("', '")
            .append(QString::number(t205))
            .append("', '")
            .append(QString::number(t203))
            .append("')");
    return query.exec(queryStr);
}


//数据库缓冲列表处理 num:每次处理条数#/
bool DataSql_ListHandle(int num)
{
    int i=0;
    bool ret=0;
    ST_sqldat sqldat;
    logInfo record;
    i=num;
    while(i>0)
    {
        if(ListSqlDat.size()>0)
        {
            sqldat=ListSqlDat.first();
            switch(sqldat.stavetype)
            {
            case DE_RUNLOG_TYPE:
                record.time=sqldat.time;
                record.opration=sqldat.runLog.opration;
                record.type=sqldat.runLog.type;
                record.status=sqldat.runLog.status;
                record.note=sqldat.runLog.note;
                ret=DataSql_InsertRunLogTable(record);
                break;
            case DE_CUREDATE_TYPE:
                ret=DataSql_InsertcureTable(sqldat.time,sqldat.cureDate.tpid,sqldat.cureDate.c701,\
                                            sqldat.cureDate.c702,sqldat.cureDate.c704,sqldat.cureDate.c709,\
                                            sqldat.cureDate.t201,sqldat.cureDate.t204,sqldat.cureDate.t205,sqldat.cureDate.t203);
                break;
            case DE_WARNTABLE_TYPE:
                ret=DataSql_InsertwarnTable(sqldat.time,sqldat.warnTable.who,sqldat.warnTable.code,sqldat.warnTable.priority,\
                                            sqldat.warnTable.alarmid,sqldat.warnTable.dispose);
                break;
            case DE_PATIENTINFOR_TYPE://病人信息记录表#/
                ret=DataSql_Insertpatientinfor(sqldat.patientinfor);
                break;
            case DE_CUREDATAINFOR_TYPE: //病人治疗数据记录表#/
                ret=DataSql_Insertcuredatainfor(sqldat.curedatainfor);
                break;
            }

            ListSqlDat.removeFirst();
        }// if(ListSqlDat.size()>0)
        else
            break;

        i--;
    }//    while(i>0)

    return ret;
}

//--------------------------------------------------------------------patientinfor 病人信息相关#/

//往病人信息数据表里面插入一条数据  插入数据库缓冲列表#/
bool DataList_Insertpatientinfor(stpatientinfor infor)
{
    ST_sqldat sqldat;

    sqldat.time=infor.timesec;
    sqldat.stavetype=DE_PATIENTINFOR_TYPE;

    sqldat.patientinfor=infor;

    ListSqlDat.append(sqldat);

    return true;
}


//往病人信息数据表里面插入一条数据  直接操作数据库#/
bool DataSql_Insertpatientinfor(stpatientinfor infor)
{
    QSqlQuery query("",db);
    QString queryStr ;
    queryStr.append("insert into patientinfor (idnum,curenum,timesec,name,gender,age,depart,weight,diagno,\
                     time,temp,na,hco3,ufgoal,bldflow,subvol,heprate,treatmode,profilemode) values('")
            .append(QString::number(infor.idnum))
            .append("', '")
            .append(QString::number(infor.curenum))
            .append("', '")
            .append(QString::number(infor.timesec))
            .append("', '")
            .append(infor.name)
            .append("', '")
            .append(QString::number(infor.gender))
            .append("', '")
            .append(QString::number(infor.age))

            .append("', '")
            .append(QString::number(infor.depart))
            .append("', '")
            .append(QString::number(infor.weight))
            .append("', '")
            .append(QString::number(infor.diagno))
            .append("', '")
            .append(QString::number(infor.time))
            .append("', '")
            .append(QString::number(infor.temp))
            .append("', '")
            .append(QString::number(infor.na))
            .append("', '")
            .append(QString::number(infor.hco3))
            .append("', '")
            .append(QString::number(infor.ufgoal))
            .append("', '")
            .append(QString::number(infor.bldflow))
            .append("', '")
            .append(QString::number(infor.subvol))
            .append("', '")
            .append(QString::number(infor.heprate))
            .append("', '")
            .append(QString::number(infor.treatmode))
            .append("', '")
            .append(QString::number(infor.profilemode))
            .append("')");
    return query.exec(queryStr);
}

//从病人信息数据表里面读取一条数据#/
stpatientinfor DataSql_Readpatientinfor(int idnum,int curenum)
{
    stpatientinfor infor;

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("select * from patientinfor where idnum='")
            .append(QString::number(idnum))
            .append("' and curenum='")
            .append(QString::number(curenum))
            .append("'");

    database.exec(queryStr);

    while(database.next()) {
        infor.idnum=database.value(0).toInt();
        infor.curenum=database.value(1).toInt();
        infor.timesec=database.value(2).toUInt();
        infor.name=database.value(3).toString();
        infor.gender=database.value(4).toInt();
        infor.age=database.value(5).toInt();
        infor.depart=database.value(6).toInt();
        infor.weight=database.value(7).toInt();
        infor.diagno=database.value(8).toInt();
        infor.time=database.value(9).toInt();
        infor.temp=database.value(10).toInt();
        infor.na=database.value(11).toInt();
        infor.hco3=database.value(12).toInt();
        infor.ufgoal=database.value(13).toInt();
        infor.bldflow=database.value(14).toInt();
        infor.subvol=database.value(15).toInt();
        infor.heprate=database.value(16).toInt();
        infor.treatmode=database.value(17).toInt();
        infor.profilemode=database.value(18).toInt();
    }

    return infor;

}

//根据ID删除某一次治疗的病人信息数据内容#/
bool DataSql_DeletepatientinforByIdnum(int idnum,int curenum)
{

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("delete from patientinfor where idnum='")
                    .append(QString::number(idnum))
                    .append("' and curenum='")
                    .append(QString::number(curenum))
                    .append("'");

    return database.exec(queryStr);
}

//根据ID号从病人信息数据表里面读取出来该病人所有治疗信息#/
QMap<int,stpatientinfor> DataSql_GetpatientinforByIdnum(int idnum)
{
    QMap<int,stpatientinfor> mapval;
    stpatientinfor infor;

    int i=0;
    mapval.clear();

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("select * from patientinfor where idnum='")
            .append(QString::number(idnum))
            .append("' order by timesec");

    database.exec(queryStr);

    while(database.next()) {
        infor.idnum=database.value(0).toInt();
        infor.curenum=database.value(1).toInt();
        infor.timesec=database.value(2).toUInt();
        infor.name=database.value(3).toString();
        infor.gender=database.value(4).toInt();
        infor.age=database.value(5).toInt();
        infor.depart=database.value(6).toInt();
        infor.weight=database.value(7).toInt();
        infor.diagno=database.value(8).toInt();
        infor.time=database.value(9).toInt();
        infor.temp=database.value(10).toInt();
        infor.na=database.value(11).toInt();
        infor.hco3=database.value(12).toInt();
        infor.ufgoal=database.value(13).toInt();
        infor.bldflow=database.value(14).toInt();
        infor.subvol=database.value(15).toInt();
        infor.heprate=database.value(16).toInt();
        infor.treatmode=database.value(17).toInt();
        infor.profilemode=database.value(18).toInt();

        mapval.insert(i,infor);
        i++;
    }

    return mapval;
}

//查询病人信息数据表中，某个ID号共有几次治疗,返回治疗次号值#/
QList<int> DataSql_patientinforGetNumByIdnum(int idnum)
{
    QList<int> list;
    list.clear();

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("select curenum from patientinfor where idnum='")
            .append(QString::number(idnum))
            .append("' order by curenum");

    database.exec(queryStr);

    while(database.next()){
        list.append(database.value(0).toInt());
    }

    return list;
}

//--------------------------------------------------------------------curedatainfor 病人治疗数据记录相关#/
//往病人信息数据表里面插入一条数据  插入数据库缓冲列表#/
bool DataList_Insertcuredatainfor(stcuredatainfor infor)
{
    ST_sqldat sqldat;

    sqldat.time=infor.timesec;
    sqldat.stavetype=DE_CUREDATAINFOR_TYPE;

    sqldat.curedatainfor=infor;

    ListSqlDat.append(sqldat);

    return true;

}

//往病人信息数据表里面插入一条数据  直接操作数据库#/
bool DataSql_Insertcuredatainfor(stcuredatainfor infor)
{
    QSqlQuery query("",db);
    QString queryStr ;
    queryStr.append("insert into curedatainfor (idnum,curenum,timesec,c701,c702,c703,c704,t201,t204,\
                     t205,t203,ap,vp,tmp,pulse,dia,sys) values('")
            .append(QString::number(infor.idnum))
            .append("', '")
            .append(QString::number(infor.curenum))
            .append("', '")
            .append(QString::number(infor.timesec))
            .append("', '")
            .append(QString::number(infor.c701))
            .append("', '")
            .append(QString::number(infor.c702))
            .append("', '")
            .append(QString::number(infor.c709))

            .append("', '")
            .append(QString::number(infor.c704))
            .append("', '")
            .append(QString::number(infor.t201))
            .append("', '")
            .append(QString::number(infor.t204))
            .append("', '")
            .append(QString::number(infor.t205))

            .append("', '")
            .append(QString::number(infor.t203))
            .append("', '")
            .append(QString::number(infor.ap))
            .append("', '")
            .append(QString::number(infor.vp))
            .append("', '")
            .append(QString::number(infor.tmp))
            .append("', '")
            .append(QString::number(infor.pulse))
            .append("', '")
            .append(QString::number(infor.dia))
            .append("', '")
            .append(QString::number(infor.sys))
            .append("')");

    return query.exec(queryStr);

}

//根据ID号和治疗次号将该次治疗的所有数据从病人信息数据表读取出来#/
QMap<int,stcuredatainfor> DataSql_GetcuredatainforByIdnum(int idnum,int curenum)
{
    QMap<int,stcuredatainfor> mapval;
    stcuredatainfor infor;

    int i=0;
    mapval.clear();

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("select * from curedatainfor where idnum='")
            .append(QString::number(idnum))
            .append("' and curenum='")
            .append(QString::number(curenum))
            .append("' order by timesec");

    database.exec(queryStr);

    while(database.next()) {
        infor.idnum=database.value(0).toInt();
        infor.curenum=database.value(1).toInt();
        infor.timesec=database.value(2).toUInt();
        infor.c701=database.value(3).toInt();
        infor.c702=database.value(4).toInt();
        infor.c709=database.value(5).toInt();
        infor.c704=database.value(6).toInt();
        infor.t201=database.value(7).toInt();
        infor.t204=database.value(8).toInt();
        infor.t205=database.value(9).toInt();
        infor.t203=database.value(10).toInt();
        infor.ap=database.value(11).toInt();
        infor.vp=database.value(12).toInt();
        infor.tmp=database.value(13).toInt();
        infor.pulse=database.value(14).toInt();
        infor.dia=database.value(15).toInt();
        infor.sys=database.value(16).toInt();

        mapval.insert(i,infor);
        i++;
    }

    return mapval;
}

//根据ID删除某一次治疗的病人数据#/
bool DataSql_DeletecuredatainforByIdnum(int idnum,int curenum)
{

    QSqlQuery database("", db);
    QString queryStr;
    queryStr.append("delete from curedatainfor where idnum='")
                    .append(QString::number(idnum))
                    .append("' and curenum='")
                    .append(QString::number(curenum))
                    .append("'");

    return database.exec(queryStr);
}
