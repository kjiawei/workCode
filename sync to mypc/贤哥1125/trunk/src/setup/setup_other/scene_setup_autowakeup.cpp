#include "scene_setup_autowakeup.h"
#include "canopen_shell.h"
#include "common_func.h"

void SceneSetupAutoWakeUp::setUi(void)
{

    SCENE_SETUP_CREATE_TABLE(powerTime,1);
    LABEL_REMIND_TITLE(powerTime);

    SCENE_SETUP_CREATE_LINEEDIT(disPowerTime,1);
    disPowerTime.setGeometry(SCENE_SETUP_COL_LINEEDIT, SCENE_SETUP_ROW_1,\
                              200,SCENE_SETUP_WIDGET_HIGH);
    disPowerTime.setReadOnly(true);

    BUTTON_SIMPLE_STYLE(calibTimeButton);
    calibTimeButton.setGeometry(QRect(350, SCENE_SETUP_ROW_2, 110, 39));

    BUTTON_SIMPLE_STYLE(getPowerDateButton);
    getPowerDateButton.setGeometry(QRect(510, SCENE_SETUP_ROW_2, 110, 39));


    enableFlag.setStyleSheet("QCheckBox {color:#dce6f0;}"     \
                "QCheckBox::indicator {width: 29px;height: 31px;} "  \
                "QCheckBox::indicator::checked {image: url(:/resource/images/radio_button_check.png);} "  \
                "QCheckBox::indicator::unchecked {image: url(:/resource/images/radio_button_uncheck.png);}");

    enableFlag.setGeometry(180, SCENE_SETUP_ROW_4, 300, 35);
    enableFlag.setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    SCENE_SETUP_CREATE_TABLE(labelMon,5);
    LABEL_REMIND_TITLE(labelMon);

    SCENE_SETUP_CREATE_TABLE(labelTue,6);
    LABEL_REMIND_TITLE(labelTue);

    SCENE_SETUP_CREATE_TABLE(labelWed,7);
    LABEL_REMIND_TITLE(labelWed);

    SCENE_SETUP_CREATE_TABLE(labelThu,8);
    LABEL_REMIND_TITLE(labelThu);

    SCENE_SETUP_CREATE_TABLE(labelFri,9);
    LABEL_REMIND_TITLE(labelFri);

    SCENE_SETUP_CREATE_TABLE(labelSat,10);
    LABEL_REMIND_TITLE(labelSat);

    SCENE_SETUP_CREATE_TABLE(labelSun,11);
    LABEL_REMIND_TITLE(labelSun);

    BUTTON_SIMPLE_STYLE(setWakeupButton);
    setWakeupButton.setGeometry(QRect(560, SCENE_SETUP_ROW_12, 85, 39));


    SCENE_SETUP_CREATE_EDIT_TIME(editTimeweek[0], 5);
    SCENE_SETUP_CREATE_EDIT_TIME(editTimeweek[1], 6);
    SCENE_SETUP_CREATE_EDIT_TIME(editTimeweek[2], 7);
    SCENE_SETUP_CREATE_EDIT_TIME(editTimeweek[3], 8);
    SCENE_SETUP_CREATE_EDIT_TIME(editTimeweek[4], 9);
    SCENE_SETUP_CREATE_EDIT_TIME(editTimeweek[5], 10);
    SCENE_SETUP_CREATE_EDIT_TIME(editTimeweek[6], 11);
}

SceneSetupAutoWakeUp::SceneSetupAutoWakeUp(QWidget *parent) :
    QWidget(parent),
    labelMon(parent),
    labelTue(parent),
    labelWed(parent),
    labelThu(parent),
    labelFri(parent),
    labelSat(parent),
    labelSun(parent),
    powerTime(parent),
    disPowerTime(parent),
    enableFlag(parent),
    setWakeupButton(parent),
    calibTimeButton(parent),
    getPowerDateButton(parent)
{
    for (int i=0;i<7;i++)
    {
        (editTimeweek[i]).setParent(parent);
    }

    setUi();
    PageAppendPiece("Setup autoWakeUp", this);

    connect(&enableFlag, SIGNAL(stateChanged(int)), this, SLOT(switchAutoWakeUpEnable(int)));
    connect(&setWakeupButton,SIGNAL(clicked()),this,SLOT(setWakeupTime()));
    connect(&calibTimeButton,SIGNAL(clicked()),this,SLOT(calibPowerTime()));
    connect(&getPowerDateButton,SIGNAL(clicked()),this,SLOT(getPowerTime()));
    connect(&getPowerDateButton,SIGNAL(clicked()),this,SLOT(getPowerDate()));

    dataNodeAppend(PACKET_NAEME_POWER_WAKEUP, NULL, sizeof(Message));
    dataNodeAddObject(PACKET_NAEME_POWER_WAKEUP, this);

    getPowerTime();
    getPowerDate();

}

void SceneSetupAutoWakeUp::ActionHandle(const void *dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupAutoWakeUp-ActionHandle-in");
#endif
    unsigned char *message = ((Message *)dataPtr)->data;

    name = name;

    switch (message[3]) {
    case 1:
        break;
    case 2:
        packetPowerWakeup.caliDateTime.date.year = message[5];
        packetPowerWakeup.caliDateTime.date.month = message[6];
        packetPowerWakeup.caliDateTime.date.day = message[7];
        break;
    case 3:
        packetPowerWakeup.caliDateTime.time.hour = message[5];
        packetPowerWakeup.caliDateTime.time.minute = message[6];
        packetPowerWakeup.caliDateTime.time.second = message[7];
        break;
    case 4:
        packetPowerWakeup.caliDateTime.week.weekNum = message[5];
        break;
    case 5:
        packetPowerWakeup.powerWakeupCtrl.onOffCtrl = message[5];
        if (message[5])
            enableFlag.setChecked(true);

        packetPowerWakeup.powerWakeupCtrl.loopCtrl = message[6];
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        packetPowerWakeup.wakeupTime[message[3] - 6].hour = message[5];
        packetPowerWakeup.wakeupTime[message[3] - 6].minute = message[6];
        packetPowerWakeup.wakeupTime[message[3] - 6].second = message[7];
        break;
    default:
        break;

    }
    update();
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupAutoWakeUp-ActionHandle-out");
#endif
}

void SceneSetupAutoWakeUp::paintEvent(QPaintEvent *event)
{
    event = event;
    QString dataStr,dataTmp;

    dataStr.setNum(packetPowerWakeup.caliDateTime.date.year + 2000);
    dataStr.append(".");
    dataTmp.setNum(packetPowerWakeup.caliDateTime.date.month);
    dataTmp.append(".");
    dataStr.append(dataTmp);

    dataTmp.setNum(packetPowerWakeup.caliDateTime.date.day);
    dataTmp.append("  ");
    dataStr.append(dataTmp);

    dataTmp.setNum(packetPowerWakeup.caliDateTime.time.hour);
    dataTmp.append(":");
    dataStr.append(dataTmp);
    dataTmp.setNum(packetPowerWakeup.caliDateTime.time.minute);
    dataTmp.append(":");
    dataStr.append(dataTmp);
    dataTmp.setNum(packetPowerWakeup.caliDateTime.time.second);
    dataStr.append(dataTmp);
    disPowerTime.setText(dataStr);

    for (int i = 0;i < 7; i++)
        editTimeweek[i].setTime(QTime(packetPowerWakeup.wakeupTime[i].hour,\
                                      packetPowerWakeup.wakeupTime[i].minute,\
                                      packetPowerWakeup.wakeupTime[i].second));
}


void SceneSetupAutoWakeUp::getPowerTime(void)
{
    unsigned char i;

    for (i = 5; i <= 12; i++) {
        ReadDeviceSdoEntry(7, 0x2000, i);
        us_sleep(10000);
    }
}

void SceneSetupAutoWakeUp::getPowerDate(void)
{
    unsigned char i;

    for (i = 2; i <= 4; i++) {
        ReadDeviceSdoEntry(7, 0x2000, i);
        us_sleep(10000);
    }
}

void SceneSetupAutoWakeUp::calibPowerTime(void)
{
    PowerCalibDate calibDateTmp;
    PowerCalibWeek calibWeekTmp;
    PowerWakeupTime calibTimeTmp;

    calibDateTmp.checkFlag = 2;
    calibDateTmp.year = QDateTime::currentDateTime().date().year() - 2000;
    calibDateTmp.month = QDateTime::currentDateTime().date().month();
    calibDateTmp.day = QDateTime::currentDateTime().date().day();


    WriteDeviceSdoEntry(7, 0x2000, 2,4, (void *)&calibDateTmp);
    us_sleep(10000);

    calibTimeTmp.checkFlag = 3;
    calibTimeTmp.hour = QDateTime::currentDateTime().time().hour();
    calibTimeTmp.minute = QDateTime::currentDateTime().time().minute();
    calibTimeTmp.second = QDateTime::currentDateTime().time().second();
    WriteDeviceSdoEntry(7, 0x2000, 3,4, (void *)&calibTimeTmp);
    us_sleep(10000);

    calibWeekTmp.checkFlag = 4;
    calibWeekTmp.weekNum = QDateTime::currentDateTime().date().dayOfWeek();
    //monday is 1, sunday is 7

    calibWeekTmp.unused = 0;
    calibWeekTmp.unused1 = 0;

    WriteDeviceSdoEntry(0x07, 0x2000, 4, 4, (void *)&calibWeekTmp);
    us_sleep(10000);

    getPowerDate();
}

void SceneSetupAutoWakeUp::setWakeupTime(void)
{
    unsigned char i;

    packetPowerWakeup.powerWakeupCtrl.checkFlag = 5;
    if(enableFlag.isChecked())
        packetPowerWakeup.powerWakeupCtrl.onOffCtrl = 1;//enable auto wakeup function
    else
        packetPowerWakeup.powerWakeupCtrl.onOffCtrl = 0;//disable auto wakeup function

    packetPowerWakeup.powerWakeupCtrl.loopCtrl = 0;//temp use
    packetPowerWakeup.powerWakeupCtrl.unused = 0;

    WriteDeviceSdoEntry(0x07, 0x2000, 5, 4, (void *)&packetPowerWakeup.powerWakeupCtrl);
    us_sleep(10000);

   for (i = 0; i < 7; i++) {
       packetPowerWakeup.wakeupTime[i].checkFlag = i + 6;
       packetPowerWakeup.wakeupTime[i].hour = (unsigned char)editTimeweek[i].time().hour();
       packetPowerWakeup.wakeupTime[i].minute = (unsigned char)editTimeweek[i].time().minute();
       packetPowerWakeup.wakeupTime[i].second = (unsigned char)editTimeweek[i].time().second();

       WriteDeviceSdoEntry(7, 0x2000, i + 6, 4, (void *)&packetPowerWakeup.wakeupTime[i]);
       us_sleep(10000);
   }

   getPowerTime();
}



void SceneSetupAutoWakeUp::switchAutoWakeUpEnable(int checked)
{
    PowerWakeupCtrl ctrltmp;
    if (strcmp(PageCurrent(), "Setup autoWakeUp") == 0) {
        if (Qt::Unchecked == checked) {
            ctrltmp.checkFlag = 5;//when close the switch of autowackup,send SDO to power
            ctrltmp.onOffCtrl = 0;
            ctrltmp.loopCtrl = 0;
            ctrltmp.unused = 0;
            WriteDeviceSdoEntry(7, 0x2000, 5, 4, (void *)&ctrltmp);
            us_sleep(10000);

            labelMon.setVisible(false);
            labelTue.setVisible(false);
            labelWed.setVisible(false);
            labelThu.setVisible(false);
            labelFri.setVisible(false);
            labelSat.setVisible(false);
            labelSun.setVisible(false);

            for (int i = 0; i < 7; i++)
                editTimeweek[i].setVisible(false);

            setWakeupButton.setVisible(false);

        } else {
            labelMon.setVisible(true);
            labelTue.setVisible(true);
            labelWed.setVisible(true);
            labelThu.setVisible(true);
            labelFri.setVisible(true);
            labelSat.setVisible(true);
            labelSun.setVisible(true);

            for (int i = 0; i < 7; i++)
                editTimeweek[i].setVisible(true);

            setWakeupButton.setVisible(true);

        }
    }
}

void SceneSetupAutoWakeUp::CustomSetText(void)
{
    labelMon.setText(tr("Monday"));
    labelTue.setText(tr("Tuesday"));
    labelWed.setText(tr("Wednesday"));
    labelThu.setText(tr("Thursday"));
    labelFri.setText(tr("Friday"));
    labelSat.setText(tr("Saturday"));
    labelSun.setText(tr("Sunday"));
    setWakeupButton.setText(tr("OK"));
    enableFlag.setText(tr("Enable Auto-wake-up"));
    powerTime.setText(tr("Power Time: "));
    disPowerTime.setText(tr("2000.0.0 0:0:0"));
    setWakeupButton.setText(tr("Set Wakeup Time"));
    calibTimeButton.setText(tr("Time Adjust"));
    getPowerDateButton.setText(tr("Time Copy"));
}

void SceneSetupAutoWakeUp::setCusVisible(bool vis, char* para)
{
    para = para;

    enableFlag.setVisible(vis);
    powerTime.setVisible(vis);
    disPowerTime.setVisible(vis);
    calibTimeButton.setVisible(vis);
    getPowerDateButton.setVisible(vis);

    if (true == vis) {
        if (true == enableFlag.isChecked()) {
            labelMon.setVisible(true);
            labelTue.setVisible(true);
            labelWed.setVisible(true);
            labelThu.setVisible(true);
            labelFri.setVisible(true);
            labelSat.setVisible(true);
            labelSun.setVisible(true);

            for (int i = 0; i < 7; i++)
                editTimeweek[i].setVisible(true);

            setWakeupButton.setVisible(true);
        } else {
            labelMon.setVisible(false);
            labelTue.setVisible(false);
            labelWed.setVisible(false);
            labelThu.setVisible(false);
            labelFri.setVisible(false);
            labelSat.setVisible(false);
            labelSun.setVisible(false);

            for (int i = 0; i < 7; i++)
                editTimeweek[i].setVisible(false);

            setWakeupButton.setVisible(false);

        }

    } else {
        labelMon.setVisible(vis);
        labelTue.setVisible(vis);
        labelWed.setVisible(vis);
        labelThu.setVisible(vis);
        labelFri.setVisible(vis);
        labelSat.setVisible(vis);
        labelSun.setVisible(vis);

        for (int i = 0; i < 7; i++)
            editTimeweek[i].setVisible(vis);

        setWakeupButton.setVisible(vis);
    }

}











































