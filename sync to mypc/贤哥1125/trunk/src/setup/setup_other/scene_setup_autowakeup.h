#ifndef SCENE_SETUP_AUTOWAKEUP_H
#define SCENE_SETUP_AUTOWAKEUP_H

#include <QtGui>
#include "lang_switch.h"
#include "page_info.h"
#include "data_custom.h"
#include "packet_can.h"
#include "scene_setup_layout.h"

#define SCENE_SETUP_CREATE_EDIT_TIME(x,y)   \
do {\
    x.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT,\
                                  SCENE_SETUP_ROW_##y, 130,\
                                  SCENE_SETUP_WIDGET_HIGH));\
    x.setDisplayFormat("h:m ");\
    x.setTime(QTime(7,30));\
    x.setStyleSheet("QDateTimeEdit {"\
                              "color:#cdcdcd;"\
                              "background: #123f75;"\
                              "}");\
    x.setAlignment( Qt::AlignVCenter | Qt::AlignRight);\
    x.setFont(QFont(QString(CONFIG_FONT_STYLE),\
                              CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));\
}while(0)


struct PowerCalibDate
{
    unsigned char checkFlag;
    unsigned char year;
    unsigned char month;
    unsigned char day;
};

struct PowerCalibWeek
{
    unsigned char checkFlag;
    unsigned char weekNum;
    unsigned char unused;
    unsigned char unused1;
};

struct PowerWakeupCtrl
{
    unsigned char checkFlag;
    unsigned char onOffCtrl;
    unsigned char loopCtrl;
    unsigned char unused;
};

struct PowerWakeupTime
{
    unsigned char checkFlag;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};



struct PacketPowerWakeup
{

    struct CaliDateTime
    {
        PowerCalibDate  date;
        PowerWakeupTime time;
        PowerCalibWeek week;
    }caliDateTime;

    PowerWakeupCtrl powerWakeupCtrl;
    PowerWakeupTime wakeupTime[7];

};


class SceneSetupAutoWakeUp : public QWidget, public PieceVisible, public LangSetText ,public ActionCustom
{
    Q_OBJECT

public:
    SceneSetupAutoWakeUp(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi(void);
    void ActionHandle(const void* dataPtr=NULL, int name=0);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void switchAutoWakeUpEnable(int checked);
    void setWakeupTime(void);
    void calibPowerTime(void);
    void getPowerTime(void);
    void getPowerDate(void);

private:
    QLabel labelMon;
    QLabel labelTue;
    QLabel labelWed;
    QLabel labelThu;
    QLabel labelFri;
    QLabel labelSat;
    QLabel labelSun;
    QLabel powerTime;
    QLineEdit disPowerTime;



    QTimeEdit editTimeweek[7];


    QDateEdit calibDateEdit;

    QCheckBox enableFlag;
    QPushButton setWakeupButton;
    QPushButton calibTimeButton;
    QPushButton getPowerDateButton;


    PacketPowerWakeup  packetPowerWakeup;


};




#endif // SCENE_SETUP_AUTOWAKEUP_H
