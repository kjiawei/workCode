#ifndef _DB_CONNECTION_H_
#define _DB_CONNECTION_H_

//数据库操作 
//使用参考 http://www.runoob.com/sqlite/sqlite-c-cpp.html

#include <QtSql>
#include <QString>

#define SQLDB_NAME "db/hemodialysis.db"
#define DBTABLE_DATA "warnDataTable"//报警信息
#define DBTABLE_STATUSDATA "statusDataTable"//状态表
#define DBTABLE_HISTORY "warnTable"//历史记录

#define SQLDB_DISINFECT "db/disinfect.db"
#define DBTABLE_FILTER "filter"//超净滤器
#define DBTABLE_RECORD "record"//记录

#define SQLDB_LOGDATA "db/logData.db"
#define DBTABLE_RUNLOG "RunLog"//运行日志(记录)

#define RUNLOG_MAXNUM 500   //最大记录数目
#define RUNLOG_PAGEITEMS 10 //16 运行日志每页最大记录条数

#define DE_RUNLOG_TYPE 1        //日志记录表
#define DE_CUREDATE_TYPE 2      //数据曲线记录表
#define DE_WARNTABLE_TYPE 3     //报警记录表
#define DE_PATIENTINFOR_TYPE 4  //病人信息记录表
#define DE_CUREDATAINFOR_TYPE 5 //病人治疗数据记录表

//如何保证安全性
QString dbName[] = {"db/hemodialysis.db","db/disinfect.db","db/logData.db"};
QString tableName1[] = {"warnDataTable","statusDataTable","warnTable"};
QString fieldTable1[] = {};
QString tableName2[] = {"filter","record"};
QString fieldTable1[] = {};
QString tableName3[] = {"RunLog"};
QString fieldTable1[] = {};
QSqlDatabase sqliteDB[];//sqlite数据库
QList<ST_sqldat> databaseCache; //数据库缓冲队列 ST_sqldat改为泛型

//int initDataBase();//初始化数据库
int openDataBase();//打开数据库
int openDataBase(const char* dbName);
int closeDataBase();//关闭数据库
int closeDataBase(const char* dbName);
bool createTable();//创建数据表
bool createTable(const char* dbName,const char* tableName);
//传入指定字段的建表函数
bool createTable(const char* dbName,const char* tableName,const char* field1,
		const char* field2,const char* field3,const char* field4,
	const char* field5,const char* field6,const char* field7);

//查询数据
bool searchDatabase();//
bool searchDatabase(const char* dbName);//
bool searchDatabase(const char* dbName,const char* tableName);//

//插入数据
void insertCache();//将数据插入到数据库缓存队列
bool insertValue(const char* dbName,const char* tableName);


//删除数据
bool deleteValue(const char* dbName,const char* tableName);


//修改数据
bool updateValue(const char* dbName,const char* tableName);

//事务 .transaction();  //同时也在测时 https://www.cnblogs.com/findumars/p/5308857.html
//BEGIN TRANSACTION; COMMIT; END TRANSACTION;ROLLBACK;

//业务数据库操作
//showBenchmark() //返回数据库性能指标
#endif