#include <QtGui>
#include "config_ui.h"
#include "page_info.h"
#include "widget_time.h"

#include "global_var.h"

WidgetTime::WidgetTime(QWidget *parent) :
    QWidget(parent),
    timeLeave(0, 0, 0, 0),
//    timer(this),
    labelTime(this),
    widgetButtonTime(this)
{
    labelTime.setGeometry(QRect(WIDGETTIME_LABEL_X,
                WIDGETTIME_LABEL_Y,
                WIDGETTIME_WIDTH,
                WIDGETTIME_LABEL_HIGH));
    labelTime.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter);
    labelTime.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_3, CONFIG_FONT_NORMAL));
    labelTime.setStyleSheet(CONFIG_COLOR_BLUE_CHARS);

    widgetButtonTime.setStyleNormal(":/resource/images/time.png",
            QRect(WIDGETTIME_BUTTON_X,
                WIDGETTIME_BUTTON_Y,
                WIDGETTIME_BUTTON_WIDTH,
                WIDGETTIME_BUTTON_HIGH));

    widgetButtonTime.setTextFontPos(CONFIG_FONT_POINTSIZE_SMALL2, CONFIG_FONT_NORMAL,
            CONFIG_COLOR_BLUE, QRect(42, 0, 82, 36));
    pixmapShadow = QPixmap(":/resource/images/button_shadow.png");

    connect(&widgetButtonTime, SIGNAL(clicked()), this, SLOT(buttonTimeSlot()));
//  connect(&timer, SIGNAL(timeout()), this, SLOT(timerDone()));


    dataNodeAddObject(TREAT_TIMER, this);
    //Timer_state=-1;//TimeIdle;
    //dataNodeUpdate(TREAT_TIMER, &Timer_state);

    //setCountDown(60*60*4);
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
    CustomSetText();
    memset(&ownNote,0,sizeof(ownNote));
}

bool WidgetTime::setCountDown(const int sec)
{
    if (sec < 1) {
        residue_secCount=0;
        return false;
    }
//    total_secCount = sec;
    timePoint = QTime::currentTime();
//    timer.start(1000);
    timer->createTimer(this, 1000);

    widgetButtonTime.setText((timeLeave.addSecs(sec)).toString(QString("hh:mm:ss")));
    emit timeSignal(widgetButtonTime.text());

    residue_secCount=sec;
    return true;
}

void WidgetTime::timer_handler(const int timeOut)
{

    Q_UNUSED(timeOut);
    static bool timeOver=false;
    static bool flag=true;
    static int treatOverTwoMin = 120;
    int secPass=timePoint.elapsed()/1000;  //距离上一次调用currentTime函数过去的秒数#/
    int tmp = total_secCount - secPass;

    /*
    if((tmp<=heparinPreStop_sec) && !heparinPreStop)
    {
       heparinPreStop =true;
       ownNote.note.note1=0x01;
       dataNodeUpdate(OWNNOTE, &ownNote);
    }
    */

    if(!timeOver){
        if(tmp <= 0){
            timeOver = true;
            timePoint = QTime::currentTime();
            tmp = 0;

            struct PacketUF packetTmp;
            dataRead(PACKET_NAME_UF,&packetTmp);
            packetTmp.ufRate = 50;//??????????????????????需要把治疗完成的血泵速度放到工程界面#/
            G_UF_SETRATE=packetTmp.ufRate;//治疗时间到#/
            dataNodeUpdate(PACKET_NAME_UF, &packetTmp);                 //修改下发的超滤速度#/

         //   if(G_UF_FINISHVOL_FLAG==true)   //超滤量到，并且时间到，才会弹出结束治疗框#/
         //   {
                Timer_state =Timestop;
                dataNodeUpdate(TREAT_TIMER, &Timer_state);//弹出是否结束治疗提示#/
         //   }

            treatOverTwoMin = 120;
        }
  //      widgetButtonTime.setText((timeLeave.addSecs(tmp)).toString(QString("hh:mm:ss")));
        residue_secCount = tmp;
        if(tmp<=60)   //最后一分钟显示#/
            tmp=61;
        if(flag)
            widgetButtonTime.setText((timeLeave.addSecs(tmp)).toString(QString("hh:mm")));
        else
            widgetButtonTime.setText((timeLeave.addSecs(tmp)).toString(QString("hh mm")));
        flag=!flag;
        emit timeSignal(widgetButtonTime.text());

    }
    else{

        tmp = timePoint.elapsed()/1000;

        if(tmp>treatOverTwoMin&&G_UF_FINISHVOL_FLAG==true){
            Timer_state =Timestop;//超过治疗时间，定时两分钟弹出是否结束治疗提示#/
            dataNodeUpdate(TREAT_TIMER, &Timer_state);
            treatOverTwoMin = tmp + 120;
        }
        else if(G_UF_FINISHVOL_FLAG==true&&(Timer_state!=Timestop))
        {
            Timer_state =Timestop;//超过治疗时间，定时两分钟弹出是否结束治疗提示#/
            dataNodeUpdate(TREAT_TIMER, &Timer_state);
        }

  //      widgetButtonTime.setText("-"+(timeLeave.addSecs(tmp)).toString(QString("hh:mm:ss")));
        if(flag)
            widgetButtonTime.setText("-"+(timeLeave.addSecs(tmp)).toString(QString("hh:mm")));
        else
            widgetButtonTime.setText("-"+(timeLeave.addSecs(tmp)).toString(QString("hh mm")));
        flag=!flag;
        emit timeSignal(widgetButtonTime.text());
        residue_secCount = 0;
    }

    WorkState stateTMP = GetWorkStateManage();
    if(TREAT_RUNNING != stateTMP.tipState.current){
        if(timeOver==true)
            widgetButtonTime.setText("-"+(timeLeave.addSecs(tmp)).toString(QString("hh:mm")));
        else
            widgetButtonTime.setText((timeLeave.addSecs(tmp)).toString(QString("hh:mm")));

        timer->stopTimer(this);//治疗停止，时间停止计算#/
        timeOver=false;
    }
   /* if (tmp < 0) {
        timer->stopTimer(this);
        tmp=0;
        widgetButtonTime.setText((timeLeave.addSecs(tmp)).toString(QString("hh:mm:ss")));
        Timer_state =Timestop;
        //dataNodeUpdate(TREAT_TIMER, &Timer_state);
    } */

    update();
}

void WidgetTime::buttonTimeSlot()
{
    PageShow("Setup Treatment time");
}

void WidgetTime::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);
    if (!pixmapShadow.isNull()) {
        painter.drawPixmap(WIDGETTIME_SHADOW_X, WIDGETTIME_SHADOW_Y, pixmapShadow);
    }
}

void WidgetTime::CustomSetText()
{
    labelTime.setText(tr("Remaining time"));
}

void WidgetTime::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetTime-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==TREAT_TIMER){
        Timer_state = *((int*) dataPtr);

        if(G_rebirth_flag==true)  //重生操作时取消处理#/
            return;

        if(Timer_state==Timestart){
            setCountDown(total_secCount);
            residue_secCount = total_secCount;
        }

        if(Timer_state==Timechange){
            WorkState stateTMP = GetWorkStateManage();
            if(TREAT_RUNNING == stateTMP.tipState.current){
                setCountDown(total_secCount);
            }
            else
            {
                residue_secCount = total_secCount;
        //        widgetButtonTime.setText((timeLeave.addSecs(total_secCount)).toString(QString("hh:mm:ss")));
                widgetButtonTime.setText((timeLeave.addSecs(total_secCount)).toString(QString("hh:mm")));
            }
        }
    }

    if(name==PACKET_MAINTAIN_PARAMS) {
        params = (Params*)dataPtr;
        int sectmp = (params->Params_TreatTime.Hour.attribute.default_value)*3600 +
                (params->Params_TreatTime.min.attribute.default_value)*60;
   //     widgetButtonTime.setText((timeLeave.addSecs(sectmp)).toString(QString("hh:mm:ss")));
         widgetButtonTime.setText((timeLeave.addSecs(sectmp)).toString(QString("hh:mm")));
        widgetButtonTime.update();
        emit timeSignal(widgetButtonTime.text());
    }
    else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        total_secCount=G_P_REBIRTH->total_secCount;
        residue_secCount=G_P_REBIRTH->residue_secCount;

        widgetButtonTime.setText((timeLeave.addSecs(residue_secCount)).toString(QString("hh:mm")));
        emit timeSignal(widgetButtonTime.text());

        if(workstate.tipState.current==TREAT_RUNNING)
        {
            timePoint = QTime::currentTime();
            timePoint=timePoint.addSecs(-(total_secCount-residue_secCount));//恢复原始起点#/
            timer->createTimer(this, 1000);
        }
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetTime-ActionHandle-out");
#endif
}
