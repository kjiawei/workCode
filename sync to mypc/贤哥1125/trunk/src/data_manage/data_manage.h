#ifndef DATA_MANAGE_H
#define DATA_MANAGE_H

#include <QObject>
#include "data_sqllite.h"
#include "data_manage_can.h"
#include "data_can_send.h"
#include "data_set.h"

#if HJX_COMMUNICATION_SWITCH
class DataManage : public QObject
{
    Q_OBJECT
public:
    DataManage(QObject *parent=0);
    ~DataManage();
    void CanopenOpen();
    void CanopenClose();
    DataCan dataCan;  //CAN数据操作管理类 各类数据包结构，各设备的can接收回调处理#/
};
#else

/*!
 * \brief The DataManage class
 */
class DataManage : public QObject
{
    Q_OBJECT
public:
    DataManage(QObject *parent=0);
    ~DataManage();
    void CanopenOpen();
    void CanopenClose();
    DataCan dataCan;  //CAN数据操作管理类 各类数据包结构，各设备的收发回调处理#/
private:
    DataCanSend dataCanSend;//canopen发送类#/
};

#endif

#endif
