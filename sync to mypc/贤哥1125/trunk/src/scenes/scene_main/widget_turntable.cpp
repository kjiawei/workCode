#include <QtGui>
#include "config_ui.h"
#include "widget_turntable.h"
#include "global_var.h"
#include "widget_time.h"
WidgetTurntable::WidgetTurntable(QWidget *parent) :
    QWidget(parent),
    labelValue(this),
    labelUnits(this),
    labelOverSpeed(this)
{

    dataNodeAddObject(PACKET_MASTERCTRL_UFVALUE, this);
    dataNodeAddObject(PACKET_NAME_WORKSTATE, this);//工作模式#/
    dataNodeAddObject(PACKET_NAME_UF, this);
    dataNodeAddObject(TREAT_TIMER,this);

    needleValue = 0;
    colorNeedle.setRgb(255, 0, 0);//转盘上指针红色#/
    scaleSet(0, 8000, 140);
    pixmapNeedle = QPixmap(":/resource/images/turntable.png");
    pixmapCap = QPixmap(":/resource/images/turntable_cap.png");

    labelValue.setGeometry(QRect(WIDGETTURNTABLE_LABEL_VALUE_X,
                WIDGETTURNTABLE_LABEL_VALUE_Y,
                WIDGETTURNTABLE_LABEL_VALUE_WIDTH,
                WIDGETTURNTABLE_LABEL_VALUE_HIGH));
    labelValue.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelValue.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_2+10, CONFIG_FONT_NORMAL));
    labelValue.setStyleSheet(CONFIG_COLOR_BLUE_CHARS);

    labelUnits.setGeometry(QRect(WIDGETTURNTABLE_LABEL_UNITS_X,
                WIDGETTURNTABLE_LABEL_UNITS_Y,
                WIDGETTURNTABLE_LABEL_UNITS_WIDTH,
                WIDGETTURNTABLE_LABEL_UNITS_HIGH));
    labelUnits.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    labelUnits.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL));
    labelUnits.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
    labelUnits.setText("ml");

    labelOverSpeed.setGeometry(QRect(WIDGETTURNTABLE_OVER_SPEED_X,
                WIDGETTURNTABLE_OVER_SPEED_Y,
                WIDGETTURNTABLE_OVER_SPEED_WIDTH,
                WIDGETTURNTABLE_OVER_SPEED_HIGH));  
    labelOverSpeed.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    labelOverSpeed.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_3-1, CONFIG_FONT_NORMAL));
    labelOverSpeed.setStyleSheet(CONFIG_COLOR_BLUE_CHARS);
    setOverSpeed(300);//转盘中间显示的速度    
}

void WidgetTurntable::scaleSet(const int min,
        const int max, const int degree)
{
    needleMin = min;
    needleMax = max;
    needleDegree = degree;
}

void WidgetTurntable::valueSet(const int value)
{
    if (value < needleMin) {
        needleValue = needleMin;
    } else if (value > needleMax) {
        needleValue = needleMax;
    } else {
        needleValue = value;
    }

    update();
}

void WidgetTurntable::setOverSpeed(const int value)
{
    overSpeed = value;
    labelOverSpeed.setNum(overSpeed);
}

void WidgetTurntable::keyPressEvent(QKeyEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *eventKey = static_cast<QKeyEvent*>(event);
        switch (eventKey->key()) {
            case Qt::Key_Down:
            case Qt::Key_Left:
                valueSet(needleValue - 50);
                break;
            case Qt::Key_Up:
            case Qt::Key_Right:
                valueSet(needleValue + 50);
                break;
            default:
                break;
        }
    }
}

void WidgetTurntable::paintEvent(QPaintEvent *event)
{
    event = event;
    static const QPoint needleHand[3] = {
        QPoint(6, 0),
        QPoint(-6, 0),
        QPoint(0, -85)
    };

    QPainter painter(this);
    double angle ;
    static bool colorFlag=false;//超滤量到了之后启动定时器，红色，黑色不同
    static bool colorSwitch=true;
    if(needleValue>=needleMax){
        angle = double(needleDegree)/2;
        if(colorFlag==false)
        {
            timer->createTimer(this, 500);
            colorFlag=true;
        }
        if(true== colorSwitch){
            colorNeedle.setRgb(0, 0, 255);//转盘上指针设置为蓝色
        }
        else {
            colorNeedle.setRgb(255, 0, 0);//转盘上指针设置为红色
        }
        colorSwitch = !colorSwitch;//指针的颜色交替变化闪烁
    }
    else{
        angle = (((double(needleValue)*needleDegree)/needleMax)-
                    double(needleDegree)/2);
        colorNeedle.setRgb(255, 0, 0);//转盘上指针红色
        if(colorFlag==true)//定时器停止
        {
            timer->stopTimer(this);
            colorFlag = false;//定时器停止标记
        }
    }

    if (!pixmapNeedle.isNull()) {
        painter.drawPixmap(WIDGETTURNTABLE_MAP_X,
                WIDGETTURNTABLE_MAP_Y, pixmapNeedle);
    }

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(WIDGETTURNTABLE_NEEDLE_CENTRE_X,
            WIDGETTURNTABLE_NEEDLE_CENTRE_Y);

    painter.save();
    painter.rotate(angle);
    painter.setPen(Qt::NoPen);
    painter.setBrush(colorNeedle);
    painter.drawConvexPolygon(needleHand, 3);
    painter.restore();

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(33, 115, 197), 6, Qt::SolidLine));
    painter.drawArc(-24, -24, 48, 48, int(angle+45)*(-16), -90*16);
    painter.drawArc(-24, -24, 48, 48, int(45-angle)*16, 90*16);

    if (!pixmapCap.isNull()) {
        painter.drawPixmap(-22, -23, pixmapCap);
    }
}

void WidgetTurntable::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetTurntable-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }
    if (name==PACKET_MASTERCTRL_UFVALUE) {//来自主控板的更新#/

        packetUfValue = (PacketUfValue*) dataPtr;
        static unsigned char updateOneTimeAgain=0;
        static unsigned short int PreflushBasicBpvol = 0;
        static int uf_volume=0;
        WorkState stateTMP = GetWorkStateManage();
        PacketUF packetTmp;
        dataRead(PACKET_NAME_UF,&packetTmp);

        PacketWorkMode workModeData;
        dataRead(PACKET_NAME_WORKMODE,&workModeData);

        if(stateTMP.tipState.current==TREAT_READY)//准备状态时，预冲量的显示#/
        {
            needleValue=G_UF_PREFINISH;   //当前超滤量
            needleMax=G_UF_PRETARGET;    //当前超滤目标量
         //   uf_volume=G_UF_PREFINISH+UFPump_Pre_Amount;
        //    labelValue.setNum(uf_volume); //更新主界面上的已超滤量#/

            uf_volume=packetUfValue->N3_UF_amount+UFPump_Pre_Amount;

            labelValue.setNum(uf_volume);

            if(packetTmp.chooseRate==false)
                setOverSpeed(packetTmp.preRate); //更新主界面显示的超滤速度#/
            else
                setOverSpeed(packetTmp.ufRate);

            if(PreflushBasicBpvol!=packetUfValue->N3_UF_amount)
            {
                PreflushBasicBpvol = packetUfValue->N3_UF_amount;//记录当前超滤读数#/
                update();
            }

            dataNodeUpdate(PACKET_NAME_NOW_UF_VOLUME,&uf_volume);
        }

        if(TREAT_RUNNING == stateTMP.tipState.current)updateOneTimeAgain=2;
        if(TREAT_RUNNING == stateTMP.tipState.current||updateOneTimeAgain>0){
            updateOneTimeAgain--;

            needleValue=G_UF_CUREFINISH;   //当前超滤量
            needleMax=G_UF_CURETARGET;    //当前超滤目标量

        //    uf_volume=G_UF_CUREFINISH+UFPump_Cure_Amount;
            uf_volume=G_UF_CUREFINISH;

            if(workModeData.treatmode.current == ISOUF_SNSP||workModeData.treatmode.current == ISOUF_SNDP||\
               workModeData.treatmode.current == ISOUF_DNDP)//单超模式#/
            {
                labelValue.setNum(ISO_finishvolume);
            }
            else
            {
                labelValue.setNum(uf_volume);
            }

            setOverSpeed(packetTmp.ufRate); //更新主界面显示的超滤速度#/

            dataNodeUpdate(PACKET_NAME_NOW_UF_VOLUME,&uf_volume);
            update();
        }
        else if(CLEANING == stateTMP.tipState.current && SUCK == stateTMP.waterState.current){

             labelValue.setNum(packetUfValue->N3_UF_amount); //消毒时，消毒液吸入量就是超滤量#/

     //       labelValue.setNum(packetUfValue->N3_UF_amount-PreflushBasicBpvol); //更新主界面上的已超滤量#/
    //        dataNodeUpdate(PACKET_NAME_NOW_UF_VOLUME,&UFPump_Cure_Amount);
    //       update();
        }
        else if(stateTMP.tipState.current==TREAT_FINISH)
        {
            PreflushBasicBpvol = packetUfValue->N3_UF_amount;
        }
        else if(CLEANING == stateTMP.tipState.current)
        {

        }
        return;
    }

    if (name==PACKET_NAME_WORKSTATE) {//改变工作状态#/
        WorkState workstate =*((WorkState*) dataPtr);

        if(workstate.tipState.previous!=CLEANING&&workstate.tipState.current==CLEANING)
        {
            needleValue=0;   //当前超滤量
            needleMax=100;    //当前超滤目标量
            labelValue.setNum(0);
            update();
        }
    }

    if (name==PACKET_NAME_UF) {
         struct PacketUF *tmp = (PacketUF*) dataPtr;

         WorkState stateTMP = GetWorkStateManage();
         if(stateTMP.tipState.current==TREAT_READY)//准备状态时，预冲量的显示#/
         {
             needleValue=G_UF_PREFINISH;   //当前超滤量
             needleMax=G_UF_PRETARGET;    //当前超滤目标量
             if(tmp->chooseRate==false)
                 setOverSpeed(tmp->preRate); //更新主界面显示的超滤速度#/
             else
                 setOverSpeed(tmp->ufRate);

             if(stateTMP.tipState.previous==TREAT_FINISH)
                labelValue.setNum(G_UF_PREFINISH+UFPump_Pre_Amount); //更新主界面上的已超滤量#/

             update();
         }
         else if(stateTMP.tipState.current==TREAT_RUNNING)
         {
             needleValue=G_UF_CUREFINISH;   //当前超滤量
             needleMax=G_UF_CURETARGET;    //当前超滤目标量

             setOverSpeed(tmp->ufRate);

          //   labelValue.setNum(G_UF_CUREFINISH+UFPump_Cure_Amount); //更新主界面上的已超滤量#/
             labelValue.setNum(G_UF_CUREFINISH); //更新主界面上的已超滤量#/
             update();
         }
    }

    if(name==TREAT_TIMER){
        int value = *((int*) dataPtr);
        WorkState stateTMP = GetWorkStateManage();
        if(value==Timestart)
        {
             if(TREAT_RUNNING == stateTMP.tipState.current)
             {
               labelValue.setNum(G_UF_CUREFINISH+UFPump_Cure_Amount);
             }
        }
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetTurntable-ActionHandle-out");
#endif
}

void WidgetTurntable::timer_handler(const int timeOut)
{
#if DEBUG_TIMER_HANDLE
    Debug_insert("WidgetTurntable-timer-in");
#endif
    Q_UNUSED(timeOut);

    update();
}
