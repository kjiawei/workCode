#include <QtGui>
#include "config_ui.h"
#include "page_info.h"
#include "widget_heartbeat.h"
#include "global_var.h"
#include "widget_setup_ext.h"
#include "common_tr_string.h"

WidgetHeartbeat::WidgetHeartbeat(QWidget *parent) :
    QWidget(parent),
    widgetButtonHeartbeat(this),
    labelTime(this),
    labelResult(this),
    labelPResult(this)
{
    widgetButtonHeartbeat.setStyleNormal(":/resource/images/heartbeat.png",
            QRect(WIDGET_HEARTBEAT_HEARTBEAT_X,
                WIDGET_HEARTBEAT_HEARTBEAT_Y,
                WIDGET_HEARTBEAT_HEARTBEAT_WIDTH,
                WIDGET_HEARTBEAT_HEARTBEAT_HIGH));
    pixmapShadow = QPixmap(":/resource/images/button_shadow.png");

    labelTime.setGeometry(QRect(5, 38,139,25));
    labelTime.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter);
    labelTime.setFont(QFont(QString(CONFIG_FONT_STYLE),
                13, CONFIG_FONT_NORMAL));
    labelTime.setStyleSheet(CONFIG_COLOR_BLUE_CHARS);

    connect(&widgetButtonHeartbeat, SIGNAL(clicked()), this, SLOT(SignalPageSwitch()));

    //labelTime.setText("2014/11/7 10:38:30");

    widgetButtonHeartbeat.setTextFontPos(CONFIG_FONT_POINTSIZE_SMALL2, CONFIG_FONT_NORMAL,
               CONFIG_COLOR_BLUE, QRect(42, 0, 82, 36));
    widgetButtonHeartbeat.setText("---/--- \n --");

    dataNodeAddObject(PACKET_BMPRESULT, this);
    dataNodeAddObject(PACKET_MONITOR_BPMST, this);
}

void WidgetHeartbeat::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);
    if (!pixmapShadow.isNull()) {
        painter.drawPixmap(WIDGET_HEARTBEAT_SHADOW_X, WIDGET_HEARTBEAT_SHADOW_X, pixmapShadow);
    }
}

void WidgetHeartbeat::SignalPageSwitch()
{
     //MasterCtrol_CMD_BMP_Start();
     emit bmpTest();
    //PageShow("Setup BPM");
}

void WidgetHeartbeat::showResult(bpmResult bpm)
{
//    bpm=bpm;
    //labelTime.setText("2014/11/7 10:38:30");
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    QString timestr=date.toString(Qt::ISODate)+" "+time.toString(Qt::TextDate);
    labelTime.setText(timestr);
    //widgetButtonHeartbeat.setText("120/110 \n 65");
    /*
    widgetButtonHeartbeat.setText(QString::number(bpm.diastolic)+"/"+QString::number(bpm.systolic)
                                  +" \n "+QString::number(bpm.pulse));
    */
    widgetButtonHeartbeat.setText(QString::number(bpm.systolic)+"/"+QString::number(bpm.diastolic)
                                  +" \n "+QString::number(bpm.pulse));
}

void WidgetHeartbeat::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("widget_heartbeat-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    static char show_flag=0;

    if (name==PACKET_BMPRESULT){
       bpmResult bmptmp  = *(bpmResult *) dataPtr;
       showResult(bmptmp);
       show_flag=2;
    }


    if (name==PACKET_MONITOR_BPMST) {  //监控板血压计PDO#/

       PacketBPMState packetBPMState = *(struct PacketBPMState*)dataPtr;

        if((packetBPMState.N5_BPM_status==1)&&(show_flag==0||show_flag==2))
        {
            widgetButtonHeartbeat.setText(DE_TR_BPMTESTING);
            show_flag=1;
        }
        else if((packetBPMState.N5_BPM_status==0)&&(show_flag==1))
        {
            widgetButtonHeartbeat.setText("---/--- \n --");
            show_flag=0;
        }
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("widget_heartbeat-ActionHandle-out");
#endif
}
