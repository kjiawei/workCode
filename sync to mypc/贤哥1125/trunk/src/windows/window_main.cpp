#include <QFile>
#include <QPainter>
#include <QApplication>
#include "window_main.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

WindowMain::WindowMain(QWidget * parent):
    QWidget(parent),
    c_Debug_Mode(parent),
    sceneMain(this),
    sceneDisinfect(this),
    sceneHistory(this),
    sceneSummary(this),
    sceneSetupWorkmode(this),
    widgetIms(this),
    widgetSetupCure(this),
    widgetSetupAdvanced(this),
    widgetSetupOther(this),
    widgetSetupExt(this),
    widgetWarn(this),
    widgetNotice(this),
    widgetTime(this),
    widgetClock(this),
    widgetHeartbeat(this),
    widgetTurntable(this),
    sceneMaintain(this),
    widgetCross(this)
{
    resize(CONFIG_SCREEN_WIDTH, CONFIG_SCREEN_HIGH);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/images/background.bmp")));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    widgetClock.setGeometry(910, 705, 110, 48);

    widgetTime.setGeometry(QRect(WIDGET_TIME_X, WIDGET_TIME_Y,
                WIDGETTIME_WIDTH, WIDGETTIME_HIGH));

    widgetTurntable.setGeometry(QRect(WIDGET_TURNTABLE_X, WIDGET_TURNTABLE_Y,
                WIDGETTURNTABLE_WIDTH, WIDGETTURNTABLE_HIGH));

    widgetCross.setGeometry(QRect(827, 515,
                WIDGETCROSS_WIDTH, WIDGETCROSS_HIGH));

    widgetHeartbeat.setGeometry(QRect(WIDGET_HEARTBEAT_X, WIDGET_HEARTBEAT_Y,
                WIDGETHEARTBEAT_WIDTH, WIDGETHEARTBEAT_HIGH));

    widgetWarn.setGeometry(QRect(WIDGET_WARN_X, WIDGET_WARN_Y,
                WIDGETWARN_WIDTH, WIDGETWARN_HIGH));

    setFocusPolicy(Qt::StrongFocus);

    nodeSceneSw = dataNodeFind(PACKET_NAME_SCENESWITCH);

    /*-------------下面将子页面的刷新绑定本页面，这样就可以子页面刷新前，先刷新父页面------*/
    PageAppendPiece("Main", this);
    PageAppendPiece("Disinfect", this);
    PageAppendPiece("Warn History", this);
    PageAppendPiece("Summary", this);
    PageAppendPiece("SetupWorkmode", this);

    PageAppendPiece("IMS", this);
    PageAppendPiece("LogTreat", this);
    PageAppendPiece("LogWarn", this);
    PageAppendPiece("LogEmr", this);
    PageAppendPiece("LogRun", this);
    PageAppendPiece("RecordCurve",this);

    PageAppendPiece("Setup Cure", this);
    PageAppendPiece("Setup Pressure", this);
    PageAppendPiece("Setup Treatment time", this);
    PageAppendPiece("Setup UF", this);
    PageAppendPiece("Setup ISO UF", this);
    PageAppendPiece("Setup Heparin", this);
    PageAppendPiece("Setup Hemopump", this);
    PageAppendPiece("Setup Dialysate", this);
    PageAppendPiece("Setup Subspump", this);
    PageAppendPiece("SceneSetupOherOper", this);

    PageAppendPiece("SetupAdvanced", this);
    PageAppendPiece("SetupAdvanced", this);
    PageAppendPiece("SetupExtersion", this);

    PageAppendPiece("Setup Cond", this);
    PageAppendPiece("Setup NaProfile", this);
    PageAppendPiece("Setup Unit", this);
    PageAppendPiece("Setup CurveNa", this);
    PageAppendPiece("Setup CurveUF", this);
    PageAppendPiece("Setup SpO2", this);
    PageAppendPiece("Setup BloodTemp", this);
    PageAppendPiece("Setup Sequential", this);
    PageAppendPiece("Setup Bicarbonate", this);
    PageAppendPiece("Setup BPM", this);
    PageAppendPiece("Setup XmlConfig", this);
    PageAppendPiece("Setup HeparinCurve", this);
    PageAppendPiece("Setup DiaFlowCurve",this);
    PageAppendPiece("Setup CondCurve",this);
    PageAppendPiece("Setup TempCurve",this);
    PageAppendPiece("Setup Hco3Curve",this);
    PageAppendPiece("Setup Preflush",this);
    PageAppendPiece("Setup Timer", this);
    PageAppendPiece("KtV", this);
    PageAppendPiece("KtV2", this);
    PageAppendPiece("BloodVolume", this);

    PageAppendPiece("SetupOther", this);
    PageAppendPiece("Setup Lang", this);
    PageAppendPiece("Setup Network", this);
    PageAppendPiece("Setup System Info", this);
    PageAppendPiece("Setup System Time", this);

    PageAppendPiece("Maintain", this);
    PageAppendPiece("Maintain Formula", this);
    PageAppendPiece("Maintain Curve", this);

    PageAppendPiece("Maintain Language", this);
    PageAppendPiece("RFCard", this);
    PageAppendPiece("Setup autoWakeUp",this);

    connect(&widgetHeartbeat, SIGNAL(bmpTest()), &widgetSetupExt.sceneSetupBPM, SLOT(bmpTest()));
    connect(&widgetTime, SIGNAL(timeSignal(QString)), &sceneSummary, SLOT(timeRemain(QString)));
    connect(&widgetCross, SIGNAL(clickEvent(int)), &sceneDisinfect, SLOT(clickEvent(int)));

}
