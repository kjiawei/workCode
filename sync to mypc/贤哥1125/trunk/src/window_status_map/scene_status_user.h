#ifndef SCENE_STATUS_USER_H
#define SCENE_STATUS_USER_H

#include <QtGui>
#include <QHash>
#include "page_info.h"
#include "lang_switch.h"
#include "data_custom.h"


#define MASTER_PDO1_FLAG  1   //主控板报警PDO1#/
#define MONIT_PDO1_FLAG  2     //监控板报警PDO1#/



class SceneStatusUser : public QTableView, public LangSetText,
    public PieceVisible, public ActionCustom
{
    Q_OBJECT
public:
    SceneStatusUser(QWidget *parent=0);
    void CustomSetColRowCount(int row=0, int col=0);
    bool SetCusContent(const char *name=NULL, const unsigned value=0, int row=0);
    void SetTableContent(int flag);

private:
    void CustomSetText();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void setCusVisible(bool vis=false, char* para=NULL);


private:
    inline void setUi();

private:
    QStandardItemModel model;

    int colNum;
    int rowNum;
    int itemCount;

    QHash<unsigned int,PacketStatusAlarm> masterHash;
    QHash<unsigned int,PacketStatusAlarm> monitHash;
    PacketStatusAlarm masterAlarmData;
    PacketStatusAlarm monitAlarmData;

    char itemName[8];
    QString itemValue;
};

#endif
