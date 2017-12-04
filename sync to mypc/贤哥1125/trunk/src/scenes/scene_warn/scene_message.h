/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-1-12
** 修改人：Huangjunxian
** 修改时间：2014-1-12
** 描述：
**    5058报警提示框 《头文件》
*****************************************************/
#ifndef SCENE_MESSAGE_H
#define SCENE_MESSAGE_H

#include <QtGui>
#include "config_ui.h"
#include "lang_switch.h"
#include "page_info.h"
#include "widget_button.h"
#include "data_set.h"
class Window_Message : public QDialog , public LangSetText
{
    Q_OBJECT
public:
    explicit Window_Message(QWidget *parent = 0, const QString title = "Messages",
                            const QString brushPath = "");
     ~Window_Message();
    void CustomSetText();
    void removeAllRow();
    void removeOneAlarm(int alarmid);
    void showItem(QMap<int, AlarmInfor> map);
    void setOnTop();
    bool isCanShow();
    void showDialog(bool isShow);
    bool isInfoNull();

public:
    bool isMainShow;        //判断主界面是否已经启动#/
    bool cleanmap;          //清空内容标志#/
signals:
    void showDialogSignal(bool isshow);

private:
    inline void setUi(const QString title, const QString brushBk);

    void insertRow(QString warnResume, QString warnInfo);

    void showMessText();

public slots:
    void btnCloseDialog();
    void btnConfirmClick();
    void btnLastClick();
    void btnNextClick();
    void btnReleaseClick();
    void timeHandle();
private:
    QWidget widget;
    QLabel labeltitle; //标题栏#/
    QLabel labelInfo;  //内容栏#/
    QLabel labelPage; //页码栏#/
    QPushButton btnconfirm; //确定#/
    QPushButton btnLast;  //上一页#/
    QPushButton btnNext;  //下一页#/
    QPushButton btnClose; //关闭

private:
    QString messageName;
    QString title;   //消息框类型字符串#/
    QHash<int, AlarmInfor> mapValue;
    QString btnUpStyle;
    QString btnClickStyle;
    int index;
    QTimer timer;
    int secpass;
    int alarmConfirm;
    QHBoxLayout *layout;
 //   QBitmap bmp;
};

#endif
