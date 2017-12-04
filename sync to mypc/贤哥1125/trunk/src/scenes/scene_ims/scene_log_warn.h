#ifndef SCENE_LOG_WARN_H
#define SCENE_LOG_WARN_H

#include <QtGui>
#include <QLabel>
#include "lang_switch.h"
#include "page_info.h"
#include "Alarm_module.h"

class SceneLogWarn : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneLogWarn(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();
    //将一条报警内容插入历史记录报警表
    void insertHistory(int row,int id,AlarmInfor alarm);
    //更新历史记录报警表
    void updateHistoryTable();
    //清空释放表格
    void clearTableContents(QTableWidget *table);

private:
    QTableWidget * tableWidgetHistory;  //历史记录表对象
//    QPushButton buttonSave;
//    QPushButton buttonCancel;
};

#endif
