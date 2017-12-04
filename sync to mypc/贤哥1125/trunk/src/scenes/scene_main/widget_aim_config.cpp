#include <QtGui>
#include "config_ui.h"
#include "page_info.h"
#include "widget_aim_config.h"
#include "data_set.h"
#include "global_var.h"
#include "common_tr_string.h"

#define IS_FROM_MASTERCTRL_OUTLET  0   //1：从主控板接收数据   0：从监控板接收数据#/

inline void WidgetAimConfig::setUi()
{
    labelHemoPump.setGeometry(QRect(0, 55, 282, 100));
    labelHemoPump.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelHemoPump.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_1, CONFIG_FONT_NORMAL));
    labelHemoPump.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    labelHemoPump.setNum(0);

    labelDialysateTmp.setGeometry(QRect(282, 55, 287, 100));
    labelDialysateTmp.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter);
    labelDialysateTmp.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_1, CONFIG_FONT_NORMAL));
    labelDialysateTmp.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    labelDialysateTmp.setNum(dialysisLiquid);

    labelUltrafiltrationTarget.setGeometry(QRect(569, 55, 281, 100));
    labelUltrafiltrationTarget.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter);
    labelUltrafiltrationTarget.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_1, CONFIG_FONT_NORMAL));
    labelUltrafiltrationTarget.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelSubsSpeed.setGeometry(QRect(213, 55, 213, 100));
    labelSubsSpeed.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter);
    labelSubsSpeed.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_1, CONFIG_FONT_NORMAL));
    labelSubsSpeed.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    labelSubsSpeed.setNum(23);

  //  labelISOText.setGeometry(QRect(228, 130, 113, 20));
    labelISOText.setGeometry(QRect(218, 130, 113, 20));
    labelISOText.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter);
    labelISOText.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelISOText.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    //labelISOText.setText(tr("单超剩余时间:"));
    //labelISOText.setText(tr("ISO Time:"));

 //   labelISOData.setGeometry(QRect(341, 130, 100, 20));
    labelISOData.setGeometry(QRect(333, 130, 100, 20));
    labelISOData.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelISOData.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelISOData.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    labelISOData.setText("00:00:00");

    widgetButtonBloodPump.setStyleNormal(":/resource/images/aim_config_hemo_pump.png",
            QRect(0, 1, 282, 63));
    widgetButtonBloodPump.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
            CONFIG_COLOR_BLUE, QRect(0, 0, 282, 43));

    widgetButtonDialysateTemp.setStyleNormal(":/resource/images/aim_config_dialysis_liquid.png",
            QRect(282, 1, 287, 63));
    widgetButtonDialysateTemp.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
            CONFIG_COLOR_BLUE, QRect(0, 0, 287, 43));

    widgetButtonUFTarget.setStyleNormal(":/resource/images/aim_config_dialysis_aim.png",
            QRect(569, 1, 281, 63));
    widgetButtonUFTarget.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
            CONFIG_COLOR_BLUE, QRect(0, 0, 281, 43));

    widgetButtonSubsSpeed.setStyleNormal(":/resource/images/aim_config_online_middle.png",
            QRect(213, 1, 213, 63));
    widgetButtonSubsSpeed.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
            CONFIG_COLOR_BLUE, QRect(0, 0, 213, 43));

    CustomSetText();
}

WidgetAimConfig::WidgetAimConfig(QWidget *parent) :
    QWidget(parent),
    labelISOText(this),
    labelISOData(this),
    labelHemoPump(this),
    labelSubsSpeed(this),
    labelDialysateTmp(this),
    labelUltrafiltrationTarget(this),
    widgetButtonBloodPump(this),
    widgetButtonSubsSpeed(this),
    widgetButtonDialysateTemp(this),
    widgetButtonUFTarget(this)
{
    dialysisLiquid = 37.1;

    setUi();

    connect(&widgetButtonBloodPump, SIGNAL(clicked()), this, SLOT(configHemoPump()));
    connect(&widgetButtonSubsSpeed, SIGNAL(clicked()), this, SLOT(configSubsPump()));
    connect(&widgetButtonDialysateTemp, SIGNAL(clicked()), this, SLOT(configTemp()));
    connect(&widgetButtonUFTarget, SIGNAL(clicked()), this, SLOT(configUfTarget()));
#if HJX_COMMUNICATION_SWITCH
    connect(this,SIGNAL(signalSDO(int)),dataCanSend,SLOT(sendSDOData(int)),DE_CONNECTION_TYPE);
#endif
    dataNodeAddObject(PACKET_NAME_WORKMODE, this);
    dataNodeAddObject(PACKET_SDO_BLOOD, this);
    dataNodeAddObject(PACKET_PARAMS_UF, this);
    dataNodeAddObject(PACKET_MASTERCTRL_OUTLET, this);
    dataNodeAddObject(PACKET_PDO_SUBBLOODPUMP, this);
//   dataNodeAddObject(PACKET_MASTERCTRL_UFVALUE, this);

    pixmapAimConfig = QPixmap(":/resource/images/aim_config_lable.bmp");
    pixmapAimConfigOnline = QPixmap(":/resource/images/aim_config_lable_online.bmp");
    pixmapBack = &pixmapAimConfig;
}


void WidgetAimConfig::configHemoPump()
{
    PageShow("Setup Hemopump");
}

void WidgetAimConfig::configSubsPump()
{
    PacketWorkMode G_WORKMODE_VAL;
    dataRead(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
    if(G_WORKMODE_VAL.treatmode.current == ISOUF_SNSP||G_WORKMODE_VAL.treatmode.current == ISOUF_SNDP||\
            G_WORKMODE_VAL.treatmode.current == ISOUF_DNDP)
    {
        PageShow("Setup ISO UF");
    }
    else
    {
        PageShow("Setup Subspump");
    }
}

void WidgetAimConfig::configTemp()
{
    PageShow("Setup Dialysate");
}

void WidgetAimConfig::configUfTarget()
{
    PageShow("Setup UF");
}

bool WidgetAimConfig::setDialysisLiquid(const float value)
{
    dialysisLiquid = value;
    labelDialysateTmp.setNum(dialysisLiquid);

    return true;
}

void WidgetAimConfig::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if (!pixmapBack->isNull()) {
        QPainter painter(this);
        painter.drawPixmap(0, 55, *pixmapBack);
    }
}

inline void WidgetAimConfig::WorkModeSwitch(const void* dataPtr)
{
    PacketWorkMode *packetWorkMode = (PacketWorkMode *)dataPtr;

    if (packetWorkMode->treatmode.current==HD_double||packetWorkMode->treatmode.current==HD_SNDP||packetWorkMode->treatmode.current==HD_SNSP||\
        packetWorkMode->treatmode.current == PE||packetWorkMode->treatmode.current == MODELNUM) {
        pixmapBack = &pixmapAimConfig;
        labelHemoPump.setGeometry(QRect(0, 55, 282, 100));
        labelDialysateTmp.setGeometry(QRect(282, 55, 287, 100));
        labelUltrafiltrationTarget.setGeometry(QRect(569, 55, 281, 100));

        widgetButtonBloodPump.setStyleNormal(":/resource/images/aim_config_hemo_pump.png",
                QRect(0, 1, 282, 63));
        widgetButtonBloodPump.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
                CONFIG_COLOR_BLUE, QRect(0, 0, 282, 43));

        widgetButtonDialysateTemp.setStyleNormal(":/resource/images/aim_config_dialysis_liquid.png",
                QRect(282, 1, 287, 63));
        widgetButtonDialysateTemp.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
                CONFIG_COLOR_BLUE, QRect(0, 0, 287, 43));

        widgetButtonUFTarget.setStyleNormal(":/resource/images/aim_config_dialysis_aim.png",
                QRect(569, 1, 281, 63));
        widgetButtonUFTarget.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
                CONFIG_COLOR_BLUE, QRect(0, 0, 281, 43));

        labelSubsSpeed.hide();
        widgetButtonSubsSpeed.hide();
        labelISOText.hide();
        labelISOData.hide();
        timer->stopTimer(this);//时间停止计算#/

    } else if(packetWorkMode->treatmode.current != PE&&packetWorkMode->treatmode.current != MODELNUM)
    {

        labelHemoPump.setGeometry(QRect(0, 55, 213, 100));
        labelDialysateTmp.setGeometry(QRect(426, 55, 213, 100));
        labelUltrafiltrationTarget.setGeometry(QRect(629, 55, 213, 100));

        widgetButtonBloodPump.setStyleNormal(":/resource/images/aim_config_online_left.png",
                QRect(0, 1, 213, 63));
        widgetButtonBloodPump.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
                CONFIG_COLOR_BLUE, QRect(0, 0, 213, 43));

        widgetButtonDialysateTemp.setStyleNormal(":/resource/images/aim_config_online_middle.png",
                QRect(426, 1, 213, 63));
        widgetButtonDialysateTemp.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
                CONFIG_COLOR_BLUE, QRect(0, 0, 213, 43));

        widgetButtonUFTarget.setStyleNormal(":/resource/images/aim_config_online_right.png",
                QRect(639, 1, 213, 63));
        widgetButtonUFTarget.setTextFontPos(CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL,
                CONFIG_COLOR_BLUE, QRect(0, 0, 213, 43));

        if(packetWorkMode->treatmode.current==ISOUF_DNDP||packetWorkMode->treatmode.current==ISOUF_SNDP||packetWorkMode->treatmode.current==ISOUF_SNSP)
        {
            pixmapAimConfigOnline = QPixmap(":/resource/images/aim_config_lable_iso.bmp");
            pixmapBack = &pixmapAimConfigOnline;
            QTime timeLeave;
            labelISOText.show();
            labelISOData.show();
            widgetButtonSubsSpeed.setText(tr("ISO Goal"));

            labelSubsSpeed.setNum(ISO_totalvolume);
            labelSubsSpeed.show();
            labelISOData.setText((timeLeave.addSecs(ISO_totalsec)).toString(QString("hh:mm:ss")));
            widgetButtonSubsSpeed.show();
            timer->createTimer(this, 1000);
        }
        else if(packetWorkMode->treatmode.current==HDF_pre_dilute||packetWorkMode->treatmode.current==HDF_post_dilute||\
                packetWorkMode->treatmode.current==HF_pre_dilute||packetWorkMode->treatmode.current==HF_post_dilute)
        {
            pixmapAimConfigOnline = QPixmap(":/resource/images/aim_config_lable_online.bmp");
            pixmapBack = &pixmapAimConfigOnline;
            labelISOText.hide();
            labelISOData.hide();
            widgetButtonSubsSpeed.setText(tr("Sub. Pump"));

            labelSubsSpeed.setNum(SubBPump.N10_set_speed);
            labelSubsSpeed.show();
            widgetButtonSubsSpeed.show();
            timer->stopTimer(this);
        }


     /*
        if(strcmp(packetWorkMode->modeNow, "ISOUF") == 0){
            labelISOText.show();
            labelISOData.show();
            widgetButtonSubsSpeed.setText(tr("ISO Goal"));
            labelSubsSpeed.show();
            widgetButtonSubsSpeed.show();
        }
        else{
            labelISOText.hide();
            labelISOData.hide();
            widgetButtonSubsSpeed.setText(tr("Sub. Pump"));
            labelSubsSpeed.show();
            widgetButtonSubsSpeed.show();
        }
        */
    }
}

void WidgetAimConfig::timer_handler(const int timeOut)
{
    Q_UNUSED(timeOut);
    int value=0;
    dataRead(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
    if(G_WORKMODE_VAL.treatmode.current == ISOUF_SNSP||G_WORKMODE_VAL.treatmode.current == ISOUF_SNDP||\
            G_WORKMODE_VAL.treatmode.current == ISOUF_DNDP)
    {
        value=ISO_residuesec-residue_secCount;
        if(value>=ISO_totalsec)
        {
            if(total_secCount>(ISO_totalsec+30))  //即证明不是单一的ISO治疗#/
            {
                QString str;
                G_WORKMODE_VAL.treatmode.target = G_WORKMODE_VAL.treatmode.before;
                G_WORKMODE_VAL.treatmode.before = G_WORKMODE_VAL.treatmode.current;
                G_WORKMODE_VAL.treatmode.current = G_WORKMODE_VAL.treatmode.target;
                G_WORKMODE_VAL.flag=setok_state;
                str=DE_TR_ISOTREATMENTEND;
                switch(G_WORKMODE_VAL.treatmode.current)
                {
                    case HD_double:
                        str += "HD_double";
                        break;
                    case HD_SNDP:
                        str += "HD_SNDP";
                        break;
                    case HD_SNSP:
                        str += "HD_SNSP";
                        break;
                    case HDF_pre_dilute:
                        str += "HDF_pre_dilute";
                        break;
                    case HDF_post_dilute:
                        str += "HDF_post_dilute";
                        break;
                    case HF_pre_dilute:
                        str += "HF_pre_dilute";
                        break;
                    case HF_post_dilute:
                        str += "HF_post_dilute";
                        break;
                    case PE:
                        str += "PE";
                        break;
                }

                timer->stopTimer(this);
            //    WorkModeSwitch(&G_WORKMODE_VAL);
                if(G_UF_PROFILE_FLAG==false)   //UF曲线不运行时，由ISO处理切换工作模式#/
                {
                    dataNodeUpdate(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
    #if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Treat_start);
    #else
                    MasterCtrol_CMD_Treat_start(G_WORKMODE_VAL.treatmode.current);
    #endif
                    G_UF_FINISHVOL_FLAG=false;
                    G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
                    G_UF_ENDTIME=G_SYS_CURTIME+residue_secCount;  //超滤的总时间 单位秒#/

                    G_UF_SETRATE=((G_UF_CURETARGET-G_UF_CUREFINISH)/residue_secCount)*3600.0; //超滤曲线运行结束时，重新计算UF速度#/

                    showYesDialog(DE_TR_PROMPT,str);
                }
            }
            ISO_residuesec=residue_secCount;
            value=ISO_totalsec;
        }
        QTime timeLeave;
//        labelSubsSpeed.setNum(ISO_totalvolume);
        labelISOData.setText((timeLeave.addSecs(ISO_totalsec-value)).toString(QString("hh:mm:ss")));
    }
}

void WidgetAimConfig::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("aim_config-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_NAME_WORKMODE) {
        WorkModeSwitch(dataPtr);
        return;
    } else if (name==PACKET_SDO_BLOOD) {
        struct PacketSdoBlood *packet = (struct PacketSdoBlood*)dataPtr;
        if (sdoBlood.N3_set_BPspeed != packet->N3_set_BPspeed) {
            sdoBlood.N3_set_BPspeed = packet->N3_set_BPspeed;
            labelHemoPump.setNum(sdoBlood.N3_set_BPspeed);
        }
    } else if (name==PACKET_PARAMS_UF) {
        int* uf_goal = (int *)dataPtr;
        labelUltrafiltrationTarget.setNum(*uf_goal);

    } else if (name==PACKET_MASTERCTRL_OUTLET) {
        PacketOutlet *outlet = (PacketOutlet*)dataPtr;
        labelDialysateTmp.setNum((double(outlet->N3_out_temp))/10);
    }
    else if (name==PACKET_PDO_SUBBLOODPUMP) {
             PacketSubBloodPump *packet = ( PacketSubBloodPump*)dataPtr;
            if (SubBPump.N10_set_speed != packet->N10_set_speed) {
                SubBPump.N10_set_speed = packet->N10_set_speed;
                labelSubsSpeed.setNum(SubBPump.N10_set_speed);
            }
    }else if(name==PACKET_MASTERCTRL_UFVALUE)
    {

    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("aim_config-ActionHandle-out");
#endif
}

void WidgetAimConfig::CustomSetText()
{
    widgetButtonBloodPump.setText(tr("Blood Pump"));
    widgetButtonDialysateTemp.setText(tr("Temperature"));
    widgetButtonUFTarget.setText(tr("UF Goal"));
    widgetButtonSubsSpeed.setText(tr("Sub. Pump"));
//    labelISOText.setText(tr("ISO Remaining time:"));
    labelISOText.setText(tr("ISO time:"));
}
