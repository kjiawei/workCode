#include "dbConnection.h"

/*//初始化数据库
int initDataBase()
{
	
}*/

//打开数据库
int openDataBase()
{
	for(int i = 0;i<dbName[].length();++i){
		sqliteDB[i] = QSqlDatabase::addDatabase("QSQLITE", dbName[i]);
		sqliteDB[i].setDatabaseName(dbName[i]);
		if (!sqliteDB[i].open())
		{
			QMessageBox::critical(0, qApp->tr("Cannot open database"),
			qApp->tr("Unable to establish a database connection."), QMessageBox::Cancel);
			return false;
		}
	}
    ListSqlDat.clear();
    return 0;
}

int openDataBase(const char* dbName)
{
	db = QSqlDatabase::addDatabase("QSQLITE", dbName);//整机软件
    db.setDatabaseName(dbName);
    if (!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
           qApp->tr("Unable to establish a database connection."), QMessageBox::Cancel);
        return false;
    }
    //ListSqlDat.clear();
    return 0;
}

//关闭数据库
int closeDataBase()
{
	for (int i=0;i<dbName[].length();++i)
		if (sqliteDB[i].isOpen()) sqliteDB[i].close();
    return 0;
}

int closeDataBase(const char* dbName)
{
	if (dbName.isOpen()) dbName.close();
    return 0;
}

//创建数据表
bool createTable()
{
	//索引 事务
	QString queryStr[];
	for(int i=0;i<dbName[].length();++i)
	{
		QSqlQuery query("", sqliteDB[i]);
		queryStr[i].append("create table ")
        .append(dbName[i])
        .append("(")
        .append("_id INTEGER PRIMARY KEY AUTOINCREMENT, ")
        .append("time varchar(20), ")
        .append("address varchar(10), ")
        .append("code varchar(20), ")
        .append("priority varchar(5), ")
        .append("note varchar(512), ")
        //.append("help varchar(1024),")
        .append("dispose varchar(5))");
		return query.exec(queryStr[i]);
	}
}

//指定数据库 表名创建表
bool createTable(const char* dbName,const char* tableName)
{
	QSqlQuery query("", dbName);
    QString queryStr;
    queryStr.append("create table ")
        .append(tableName)
        .append("(")//字段不一样
        .append("_id INTEGER PRIMARY KEY AUTOINCREMENT, ")
        .append("time varchar(20), ")
        .append("address varchar(10), ")
        .append("code varchar(20), ")
        .append("priority varchar(5), ")
        .append("note varchar(512), ")
        //.append("help varchar(1024),")
        .append("dispose varchar(5))");
    return query.exec(queryStr);
}

//指定字段创建表
bool createTable(const char* dbName,const char* tableName,const char* field1,
		const char* field2,const char* field3,const char* field4,
	const char* field5,const char* field6,const char* field7)
{
	QSqlQuery query("", dbName);
    QString queryStr;
    queryStr.append("create table ")
        .append(tableName)
        .append("(")//字段不一样
        .append(field1)
        .append(field2)
        .append(field3)
        .append(field4)
        .append(field5)
        .append(field6)
        //.append("help varchar(1024),")
        .append(field7);
    return query.exec(queryStr);
}

//查询所有库 所有表
bool searchDatabase()
{
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
    return database.exec(queryStr);
}

//指定数据库   DataSql_GetrownumwarnTable  DataSql_GetinforwarnTable  
//DataSql_GetinfoDisinfect  DataSql_GetTimesFilter  DataSql_GetFilterTimes
//DataSql_GetRunLog DataSql_GetRunLogNum DataSql_GetCureSumID 
//DataSql_Readpatientinfor DataSql_GetpatientinforByIdnum DataSql_patientinforGetNumByIdnum
//DataSql_GetcuredatainforByIdnum 
//bool替换为泛型 https://www.cnblogs.com/wangduo/p/5559260.html
bool searchDatabase(const char* dbName)
{
	QSqlQuery database("", dbName);
    QString queryStr;
    queryStr.append("SELECT * ")
        .append("FROM ")
        .append(DBTABLE_HISTORY)
        .append(" WHERE time='")
        .append(QString::number(time))
        .append("' AND alarmid='")
        .append(QString::number(alarmid))
        .append("'");
    return database.exec(queryStr);
}

//指定数据表查询数据
bool searchDatabase(const char* dbName,const char* tableName)
{
	QSqlQuery database("", dbName);
    QString queryStr;
    queryStr.append("SELECT * ")
        .append("FROM ")
        .append(tableName)
        .append(" WHERE time='")//where等条件设置为参数或用变量控制
        .append(QString::number(time))
        .append("' AND alarmid='")
        .append(QString::number(alarmid))
        .append("'");
    return database.exec(queryStr);
}

//插入数据 与业务解耦
bool insertValue(const char* dbName,const char* tableName)
{
	QSqlQuery query("", dbName);
    QString queryStr;
    queryStr.append("INSERT INTO ")
        .append(tableName)//下面参数用变量或数组控制
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

//将数据插入到数据库缓存队列
void insertCache()
{
	//泛型对应的结构体
	//赋值
	//databaseCache.append(泛型对应的结构体);
}

//删除数据
bool deleteValue(const char* dbName,const char* tableName)
{
	QSqlQuery database("", dbName);
    QString queryStr;
	queryStr.append("DELETE ")
            .append(" FROM ")
            .append(tableName)
            .append("where time=(select MIN(time) from ")
            .append(tableName)
            .append(")");
    return database.exec(queryStr);
}

//更新数据
bool updateValue(const char* dbName,const char* tableName)
{
	QSqlQuery database("", dbName);
    QString queryStr;
    queryStr.append("UPDATE ")
        .append(tableName)
        .append(" SET ")
        .append("Time=?")
        .append(",Opration=?")
        .append(",Type=?")
        .append(",Status=?")
        .append(",Note=?")
        .append(" WHERE Time=?");
    database.prepare(queryStr);
    /*database.addBindValue(QString::number(info.time));
    database.addBindValue(info.opration);
    database.addBindValue(QString::number(info.type));
    database.addBindValue(QString::number(info.status));
    database.addBindValue(info.note);
    database.addBindValue(QString::number(info.time));*/
    return database.exec();
}