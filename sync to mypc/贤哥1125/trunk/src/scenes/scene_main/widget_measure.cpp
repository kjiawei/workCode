#include <QtGui>
#include <sstream>

#include "config_ui.h"
#include "page_info.h"
#include "widget_measure.h"
#include "global_var.h"
#include "scene_main.h"

inline void WidgetMeasure::setUi()
{
    resize(_WidgetMeasure_WIDTH, _WidgetMeasure_HIGH);

    pixmapCursorRed = QPixmap(":/resource/images/cursor_red.png");
    pixmapCursorGreen = QPixmap(":/resource/images/cursor_green.png");
    pixmapVertical = QPixmap(":/resource/images/vertical.png");

    pushButtonRise.setStyleNormal(":/resource/images/measure_button_rise.png",
            QRect(_widgetMeasure_PushButton_X,
                _widgetMeasure_PushButton_Rise_Y, 41, 63));
    pushButtonFall.setStyleNormal(":/resource/images/measure_button_fall.png",
            QRect(_widgetMeasure_PushButton_X,
                _widgetMeasure_PushButton_Fall_Y, 41, 63));

    QPalette palette;
    widgetCursorLower.setGeometry(QRect(_widgetMeasure_CursorLimen_X,
                _widgetMeasure_CursorLimen_Lower_Y-4, 29, 11));
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/images/cursor_lower.png")));
    widgetCursorLower.setAutoFillBackground(true);
    widgetCursorLower.setPalette(palette);

    widgetCursorUpper.setGeometry(QRect(_widgetMeasure_CursorLimen_X,
                _widgetMeasure_CursorLimen_Upper_Y-14, 28, 12));
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/resource/images/cursor_upper.png")));
    widgetCursorUpper.setAutoFillBackground(true);
    widgetCursorUpper.setPalette(palette);

    widgetCursor.setFixedSize(102, 38);
    widgetCursor.setAutoFillBackground(true);

    labelLower.setGeometry(QRect(_widgetMeasure_CursorLimen_X-52,
                _widgetMeasure_CursorLimen_Lower_Y-17, 50, 30));
    labelLower.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL));
    labelLower.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    labelLower.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelUpper.setGeometry(QRect(_widgetMeasure_CursorLimen_X-52,
                _widgetMeasure_CursorLimen_Upper_Y-20, 50, 30));
    labelUpper.setAlignment( Qt::AlignVCenter | Qt::AlignRight);
    labelUpper.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL));
    labelUpper.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelValue.setGeometry(QRect(30, 1, 62, 32));
    labelValue.setAlignment(Qt::AlignCenter);
    labelValue.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_2, CONFIG_FONT_NORMAL));
    labelValue.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelUnit.setGeometry(QRect(_widgetMeasure_Label_X,
                _widgetMeasure_Label_Y, 70, 30));
    labelUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FIRST_3, CONFIG_FONT_NORMAL));
    labelUnit.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    labelUnit.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
}

WidgetMeasure::WidgetMeasure(QWidget *parent) :
    QWidget(parent),
    widgetCursor(this),
    widgetCursorLower(this),
    widgetCursorUpper(this),
    labelLower(this),
    labelUpper(this),
    labelValue(&widgetCursor),
    labelUnit(this),
    //pushButtonMark(this),
    pushButtonRise(this),
    pushButtonFall(this)
{
    setUi();
    //connect(&buttonsirqApp, SIGNAL(BtnSignal(int)),this, SLOT(ResetLock(int)));

    //connect(&widgetSwitch, SIGNAL(BtnSignal(int)),
    //&, SLOT(ResetLock(int)));
    AlarmLock = false;
}

#if HJX_NEW_SWITCH==0
bool WidgetMeasure::setValue(const float value)
{
    QPalette palette;

     if(this->AlarmLock&&(currentValue!=value)){
         saveNewValue = value;
         return false;
      }
    if ((currentValue <= lower) || (currentValue >= upper))
    {
        if ((value > lower) && (value < upper))
        {
            palette.setBrush(QPalette::Background, QBrush(pixmapCursorGreen));
            widgetCursor.setPalette(palette);
        }
        else if(PDOstate.state.N1_status==0x03){ //治疗状态，卡标不动#/
            this->AlarmLock=true;
        }
    }
    else if ((value <= lower) || (value >= upper))
    {
        if(PDOstate.state.N1_status==0x03)
        {
         this->AlarmLock=true;
        }
        palette.setBrush(QPalette::Background, QBrush(pixmapCursorRed));
        widgetCursor.setPalette(palette);
    }

    currentValue = value;
    saveNewValue = value;

    currentValuePos = _widgetMeasure_CursorLimen_Lower_Y - (currentValue-lower)*k;

    if (currentValuePos>365) currentValuePos=365;
    if (currentValuePos<164) currentValuePos=164;
    labelValue.setNum(currentValue);
    widgetCursor.move(_widgetMeasure_Cursor_X, currentValuePos-21);
    this->update();

    return true;
}
#else
bool WidgetMeasure::setValue(const float value)
{
    QPalette palette;

    if ((currentValue <= lower) || (currentValue >= upper))
    {
        if ((value > lower) && (value < upper))
        {
            palette.setBrush(QPalette::Background, QBrush(pixmapCursorGreen));
            widgetCursor.setPalette(palette);
        }
        else if(PDOstate.state.N1_status==0x03){ //治疗状态，卡标不动#/
            this->AlarmLock=true;
        }
    }
    else if ((value <= lower) || (value >= upper))
    {
        if(PDOstate.state.N1_status==0x03)
        {
         this->AlarmLock=true;
        }
        palette.setBrush(QPalette::Background, QBrush(pixmapCursorRed));
        widgetCursor.setPalette(palette);
    }

    currentValue = value;
    saveNewValue = value;

    currentValuePos = _widgetMeasure_CursorLimen_Lower_Y - (currentValue-lower)*k;

    if (currentValuePos>365) currentValuePos=365;
    if (currentValuePos<164) currentValuePos=164;
    labelValue.setNum(currentValue);
//    labelValue.setText(QString::number(value,'f',1));
    widgetCursor.move(_widgetMeasure_Cursor_X, currentValuePos-21);
    this->update();

    return true;
}
#endif

float WidgetMeasure::autoSetRange(void)
{
    return currentValue;
}

bool WidgetMeasure::setRange(const float low, const float up,bool flag)
{
    QPalette palette;

    if(flag){//只有按reset键清报警标志及更新最新卡标值#/
        AlarmLock = false;
        currentValue = saveNewValue;
    }

    lower = low;
    upper = up;
    labelLower.setNum(lower);
    labelUpper.setNum(upper);

    //qDebug()<<"PDOstate.state.N1_status:"<<PDOstate.state.N1_status;

     if ((currentValue <= lower) || (currentValue >= upper))
    {
        palette.setBrush(QPalette::Background, QBrush(pixmapCursorRed));
    }
    else
    {
        palette.setBrush(QPalette::Background, QBrush(pixmapCursorGreen));
    }
    widgetCursor.setPalette(palette);

    k = (_widgetMeasure_CursorLimen_Lower_Y-
            _widgetMeasure_CursorLimen_Upper_Y)/(upper-lower);
    setValue(currentValue);
    this->update();

    return true;
}
/*
bool WidgetMeasure::setTextMark(const QString text)
{
    pushButtonMark.setText(text);
    return true;
}
*/
bool WidgetMeasure::setTestUnit(const QString text)
{
    labelUnit.setText(text);
    return true;
}

void WidgetMeasure::paintEvent(QPaintEvent *event)
{
    event = event;

    QPainter painter(this);

    if (!pixmapVertical.isNull()) {
        painter.drawPixmap(_widgetMeasure_Monial_Map_X,
                _widgetMeasure_Monial_Map_Y, pixmapVertical);
    }

    if ((currentValue <= lower) || (currentValue >= upper)) {
        painter.setPen(QPen(QColor(CONFIG_COLOR_RED), 8, Qt::SolidLine));
    } else {
        painter.setPen(QPen(QColor(CONFIG_COLOR_GREEN), 8, Qt::SolidLine));
    }
    painter.drawLine(_widgetMeasure_Vertival_X2, _widgetMeasure_Vertival_Y2,
            _widgetMeasure_Vertival_X1, currentValuePos);
}

// AP
WidgetMeasureAP::WidgetMeasureAP(QWidget *parent) :
    WidgetMeasure(parent)
{
    setTestUnit("mmHg");
    dataNodeAddObject(PACKET_NAME_AP, this);
    dataNodeAddObject(PACKET_NAME_SCALE_AP, this);
    dataNodeAddObject(PACKET_NAME_UPDATE_AP, this);
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
    //dataNodeAddObject(PACKET_MONITOR_PRESS, this);
    //connect(&pushButtonMark, SIGNAL(clicked()), this, SLOT(switchScene()));
    connect(&pushButtonFall, SIGNAL(clicked()), this, SLOT(limitFall()));
    connect(&pushButtonRise, SIGNAL(clicked()), this, SLOT(limitRise()));
    ConfirmDlgAP = new Window_Confirm(0, QObject::tr("Confirm AP"), ":/resource/test.png");
    ConfirmDlgAP->raise();
    connect(ConfirmDlgAP, SIGNAL(NoteSelSignal(int)), this, SLOT(autoScale(int)),Qt::DirectConnection);
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(confirmAutoScale()));
    step = 5;
    treatmentStateAP = TREATMENT_IS_READY;
}
void WidgetMeasureAP::autoScale(int)  //自动卡标AP
{
//    if(this->AlarmLock==true)return;  //有报警的时候，不卡标#/
    float tmp = autoSetRange();
    if(TREATMENT_IS_READY == treatmentStateAP){
            packetAP.apL = tmp + AP_scale_range.readySafeScaleLow;
            packetAP.apUp = tmp + AP_scale_range.readySafeScaleUp;
        }
    else if(TREATMENT_IS_RUNNING == treatmentStateAP){
        packetAP.apL = tmp + AP_scale_range.runAutoScaleLow;
        packetAP.apUp = tmp + AP_scale_range.runAutoScaleUp;
    }
    else if(TREATMENT_IS_STOP == treatmentStateAP){
        packetAP.apL = tmp + AP_scale_range.stopSafeScaleLow;
        packetAP.apUp = tmp + AP_scale_range.stopSafeScaleUp;
    }
    confirmSettingrangeAP();
    setRange(packetAP.apL, packetAP.apUp,true);
    dataNodeUpdate(PACKET_NAME_AP, &packetAP);
}
void WidgetMeasureAP::confirmAutoScale(void)
{
    timer.stop();
    autoScale(0);
 //   showNoteDialog("Confirm AP");
}

#if HJX_NEW_SWITCH==0
void WidgetMeasureAP::limitFall(void)
{
    if(treatmentStateAP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((APAutoScaleMIN ==packetAP.apL)  && (APAutoScaleMAX == packetAP.apUp) )return;
    packetAP.apUp -= step;
    packetAP.apL -= step;
    confirmSettingrangeAP();
    setRange(packetAP.apL, packetAP.apUp,false);
    dataNodeUpdate(PACKET_NAME_AP, &packetAP);
    //ResetLock(1);//?>?
}
#else
void WidgetMeasureAP::limitFall(void)
{
//    if(treatmentStateAP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((APAutoScaleMIN ==packetAP.apL)  && (APAutoScaleMAX == packetAP.apUp) )return;
//    packetAP.apUp -= step;
    packetAP.apL -= step;
    confirmSettingrangeAP();
    setRange(packetAP.apL, packetAP.apUp,false);
    dataNodeUpdate(PACKET_NAME_AP, &packetAP);
    //ResetLock(1);//?>?
}
#endif

#if HJX_NEW_SWITCH==0
void WidgetMeasureAP::limitRise(void)
{
    if(treatmentStateAP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((APAutoScaleMIN ==packetAP.apL)  && (APAutoScaleMAX == packetAP.apUp) )return;
    packetAP.apUp += step;
    packetAP.apL += step;
    confirmSettingrangeAP();
    setRange(packetAP.apL, packetAP.apUp,false);
    dataNodeUpdate(PACKET_NAME_AP, &packetAP);
}
#else
void WidgetMeasureAP::limitRise(void)
{
//    if(treatmentStateAP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((APAutoScaleMIN ==packetAP.apL)  && (APAutoScaleMAX == packetAP.apUp) )return;
    packetAP.apUp += step;
//    packetAP.apL += step;
    confirmSettingrangeAP();
    setRange(packetAP.apL, packetAP.apUp,false);
    dataNodeUpdate(PACKET_NAME_AP, &packetAP);
}
#endif

#if HJX_NEW_SWITCH==0
void WidgetMeasureAP::confirmSettingrangeAP(void)
{
    if(packetAP.apUp >APAutoScaleMAX){
        packetAP.apL -= (packetAP.apUp - APAutoScaleMAX );
        packetAP.apUp = APAutoScaleMAX;
        if(packetAP.apL<APAutoScaleMIN)packetAP.apL=APAutoScaleMIN;
    }
    else if(packetAP.apL <APAutoScaleMIN){
        packetAP.apUp += (APAutoScaleMIN - packetAP.apL );
        packetAP.apL = APAutoScaleMIN;
        if(packetAP.apUp>APAutoScaleMAX)packetAP.apUp=APAutoScaleMAX;
    }
}
#else
void WidgetMeasureAP::confirmSettingrangeAP(void)
{
    if(packetAP.apUp >APAutoScaleMAX){
        packetAP.apUp = APAutoScaleMAX;
    }

    if(packetAP.apL <APAutoScaleMIN){
        packetAP.apL = APAutoScaleMIN;
    }
}
#endif

void WidgetMeasureAP::switchScene(void)
{
    PageShow("Setup Pressure");
}


void WidgetMeasureAP::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureAP-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)){
        return;
    }
    if (name==PACKET_MAINTAIN_PARAMS){
        struct Params *params;
        params = (Params*)dataPtr;
        APAutoScaleMAX = params->auto_scale.ap_span.attribute.max_value-700;//工程界面设置的值-700为下发的值
        APAutoScaleMIN = params->auto_scale.ap_span.attribute.min_value-700;
        APAutoScaleDefault = params->auto_scale.ap_span.attribute.default_value-700;

        AP_scale_range.runAutoScaleLow = params->auto_scale.ap_run_auto_scale_low.attribute.default_value-700;
        AP_scale_range.runAutoScaleUp = params->auto_scale.ap_run_auto_scale_up.attribute.default_value-700;
        AP_scale_range.stopSafeScaleUp = params->auto_scale.ap_stop_safe_scale_up.attribute.default_value-700;
        AP_scale_range.stopSafeScaleLow = params->auto_scale.ap_stop_safe_scale_low.attribute.default_value-700;
        AP_scale_range.readySafeScaleUp = params->auto_scale.ap_ready_safe_scale_up.attribute.default_value-700;
        AP_scale_range.readySafeScaleLow = params->auto_scale.ap_ready_safe_scale_low.attribute.default_value-700;
        AP_scale_range.runSafeScaleUp = params->auto_scale.ap_run_safe_scale_up.attribute.default_value-700;
        AP_scale_range.runSafeScaleLow = params->auto_scale.ap_run_safe_scale_low.attribute.default_value-700;
        ResetLock(TREATMENT_CHANGE_STATE);
    }
    else if ( (name==PACKET_NAME_AP))
    {
        APData = *(struct PacketAP*)dataPtr;
        if(treatmentStateAP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中栓塞，不能操作
        if ((packetAP.apUp != APData.apUp) || (packetAP.apL != APData.apL)) {
            packetAP.apL = APData.apL;
            packetAP.apUp = APData.apUp;
            confirmSettingrangeAP();
            setRange(packetAP.apL, packetAP.apUp,true);
        }

        if (packetAP.ap != APData.ap)
        {
            packetAP.ap = APData.ap;
            setValue(packetAP.ap);
        }
    }
    else if (name==PACKET_NAME_UPDATE_AP){
        int AP_value = *(int *)dataPtr;
#if HJX_NEW_SWITCH==0
        if(treatmentStateAP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作
#else
        if(treatmentStateAP==TREATMENT_IS_RUNNING&&(alarmmodule->Is_CurrentAlarm(ALARM_APUP)||alarmmodule->Is_CurrentAlarm(ALARM_APDOWN)))
//        if(treatmentStateAP==TREATMENT_IS_RUNNING&&AlarmLock==true)  //治疗中卡标，不能操作
        {
            if(AlarmLock_AP==false)
            {
                packetAP.ap = AP_value;
                setValue(packetAP.ap);
                AlarmLock_AP=true;
            }
            return;
        }
#endif
        if (packetAP.ap != AP_value)
        {//更新动脉压显示值#/
            packetAP.ap = AP_value;
            setValue(packetAP.ap);
        }
    }
    else if (name==PACKET_NAME_SCALE_AP){//改变卡标方式#/
        int scaleType = *(int *)dataPtr;
        if(treatment_start_to_scale == scaleType)treatmentStateAP = TREATMENT_IS_RUNNING;
        else if(treatment_stop_to_scale == scaleType){treatmentStateAP = TREATMENT_IS_STOP;timer.stop();}
        ResetLock(TREATMENT_CHANGE_STATE);
    }
    else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        if(workstate.tipState.current==TREAT_RUNNING)
           treatmentStateAP = TREATMENT_IS_RUNNING;
        else if(workstate.tipState.current==TREAT_FINISH)
           treatmentStateAP = TREATMENT_IS_STOP;

        packetAP=G_P_REBIRTH->packetAP;
        setRange(packetAP.apL, packetAP.apUp,true);
        dataNodeUpdate(PACKET_NAME_AP, &packetAP);
    }


//    if (strcmp(name, PACKET_MONITOR_PRESS) == 0)
//    {
//            PacketPress *pre = (PacketPress *)dataPtr;
//            APData.ap=pre->N5_AP-700;
//            if (packetAP.ap != APData.ap)
//            {
//                packetAP.ap = APData.ap;
//                qDebug()<<"packetAP.ap:"<<packetAP.ap;
//                setValue(packetAP.ap);
//            }
//    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureAP-ActionHandle-out");
#endif

}


void WidgetMeasureAP::ResetLock(int key)
{
     if(key==1)
     {         
         if(TREATMENT_IS_RUNNING == treatmentStateAP){
//             timer.start(DELAY_TO_AUTO_SCALE);//启动定时30秒等待自动卡标#/
             packetAP.apL = AP_scale_range.runSafeScaleLow;//s1
             packetAP.apUp = AP_scale_range.runSafeScaleUp;
             confirmSettingrangeAP();
             setRange(packetAP.apL, packetAP.apUp,true);
             dataNodeUpdate(PACKET_NAME_AP, &packetAP);//报警后按reset键将卡标设为最大范围#/
         }
         else{
             autoScale(1);// 该输入参数，没有意义#/
         }
     }
     else if(TREATMENT_CHANGE_STATE == key)
     {
         AlarmLock_AP=false;
         if(TREATMENT_IS_RUNNING == treatmentStateAP){
             timer.start(DELAY_TO_AUTO_SCALE);//启动定时30秒等待自动卡标#/
             packetAP.apL = AP_scale_range.runSafeScaleLow;//s1
             packetAP.apUp = AP_scale_range.runSafeScaleUp;
             confirmSettingrangeAP();
             setRange(packetAP.apL, packetAP.apUp,true);
             dataNodeUpdate(PACKET_NAME_AP, &packetAP);//报警后按reset键将卡标设为最大范围#/
         }
         else if(TREATMENT_IS_STOP == treatmentStateAP){
             packetAP.apL = AP_scale_range.stopSafeScaleLow;//s2
             packetAP.apUp = AP_scale_range.stopSafeScaleUp;
             confirmSettingrangeAP();
             setRange(packetAP.apL, packetAP.apUp,true);
             dataNodeUpdate(PACKET_NAME_AP, &packetAP);//报警后按reset键将卡标设为最大范围#/
         }
         else if(TREATMENT_IS_READY == treatmentStateAP){
             packetAP.apL = AP_scale_range.readySafeScaleLow;//s1
             packetAP.apUp = AP_scale_range.readySafeScaleUp;
             confirmSettingrangeAP();
             setRange(packetAP.apL, packetAP.apUp,true);
             dataNodeUpdate(PACKET_NAME_AP, &packetAP);//报警后按reset键将卡标设为最大范围#/
         }
         dataRead(PACKET_NAME_UPDATE_AP,&packetAP.ap);
         setValue(packetAP.ap);
     }

}
void WidgetMeasureVP::ResetLock(int key)
{

     if(key==1)
     {
         if(TREATMENT_IS_RUNNING == treatmentStateVP){
//             timer.start(DELAY_TO_AUTO_SCALE);//启动定时30秒等待自动卡标#/
             packetVP.vpL = VP_scale_range.runSafeScaleLow;//s1
             packetVP.vpUp = VP_scale_range.runSafeScaleUp;
             confirmSettingrangeVP();
             setRange(packetVP.vpL, packetVP.vpUp,true);
             dataNodeUpdate(PACKET_NAME_VP, &packetVP);//报警后按reset键将卡标设为最大范围#/
         }
         else{
             autoScale(1);// 该输入参数，没有意义#/
         }
          //APData.ap=0;
     }
     else if(TREATMENT_CHANGE_STATE == key){
         AlarmLock_VP=false;
         autolock_flag=false;
         if(TREATMENT_IS_RUNNING == treatmentStateVP){
             timer.start(DELAY_TO_AUTO_SCALE);//启动定时30秒等待自动卡标#/
             packetVP.vpL = VP_scale_range.runSafeScaleLow;//s1
             packetVP.vpUp = VP_scale_range.runSafeScaleUp;
             confirmSettingrangeVP();
             setRange(packetVP.vpL, packetVP.vpUp,true);
             dataNodeUpdate(PACKET_NAME_VP, &packetVP);//报警后按reset键将卡标设为最大范围#/
         }
         else if(TREATMENT_IS_STOP == treatmentStateVP){
             packetVP.vpL = VP_scale_range.stopSafeScaleLow;//s1
             packetVP.vpUp = VP_scale_range.stopSafeScaleUp;
             confirmSettingrangeVP();
             setRange(packetVP.vpL, packetVP.vpUp,true);
             dataNodeUpdate(PACKET_NAME_VP, &packetVP);//报警后按reset键将卡标设为最大范围#/
         }
         else if(TREATMENT_IS_READY == treatmentStateVP){
             packetVP.vpL = VP_scale_range.readySafeScaleLow;//s1
             packetVP.vpUp = VP_scale_range.readySafeScaleUp;
             confirmSettingrangeVP();
             setRange(packetVP.vpL, packetVP.vpUp,true);
             dataNodeUpdate(PACKET_NAME_VP, &packetVP);//报警后按reset键将卡标设为最大范围#/
         }
         dataRead(PACKET_NAME_UPDATE_VP,&packetVP.vp);
         setValue(packetVP.vp);
     }
}
void WidgetMeasureTMP::ResetLock(int key)
{

    if(key==1)  //复位键按下时#/
     {
        if(TREATMENT_IS_RUNNING == treatmentStateTMP){
             timer.start(DELAY_TO_AUTO_SCALE);//启动定时30秒等待自动卡标#/
             packetTMP.tmpL = TMP_scale_range.runSafeScaleLow;//治疗状态按reset键，用安全卡标1卡标30秒后自动卡标#/
             packetTMP.tmpUp = TMP_scale_range.runSafeScaleUp;
             confirmSettingrangeTMP();
             setRange(packetTMP.tmpL, packetTMP.tmpUp,true);
             dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
        }
        else {
            autoScale(1);// 该输入参数，没有意义，非治疗状态按reset键，立刻把卡标范围设为相应值#/
        }
         //APData.ap=0;
     }
   else if(TREATMENT_CHANGE_STATE == key){
        AlarmLock_TMP=false;
        if(TREATMENT_IS_RUNNING == treatmentStateTMP){
             timer.start(DELAY_TO_AUTO_SCALE);//启动定时30秒等待自动卡标#/
             packetTMP.tmpL = TMP_scale_range.runSafeScaleLow;//启动治疗，用安全卡标1卡标30秒后自动卡标#/
             packetTMP.tmpUp = TMP_scale_range.runSafeScaleUp;
             confirmSettingrangeTMP();
             setRange(packetTMP.tmpL, packetTMP.tmpUp,true);
             dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
        }
        else if(TREATMENT_IS_STOP == treatmentStateTMP) {
            packetTMP.tmpL = TMP_scale_range.stopSafeScaleLow;//停止治疗，用安全卡标2卡标#/
            packetTMP.tmpUp = TMP_scale_range.stopSafeScaleUp;
            confirmSettingrangeTMP();
            setRange(packetTMP.tmpL, packetTMP.tmpUp,true);
            dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
        }
        else if(TREATMENT_IS_READY == treatmentStateTMP){
            packetTMP.tmpL = TMP_scale_range.readySafeScaleLow;//准备状态，用安全卡标1卡标#/
            packetTMP.tmpUp = TMP_scale_range.readySafeScaleUp;
            confirmSettingrangeTMP();
            setRange(packetTMP.tmpL, packetTMP.tmpUp,true);
            dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
        }
        dataRead(PACKET_NAME_UPDATE_TMP,&packetTMP.tmp);
        setValue(packetTMP.tmp);
    }
}

// VP
WidgetMeasureVP::WidgetMeasureVP(QWidget *parent) :
    WidgetMeasure(parent)
{
    setTestUnit("mmHg");
    dataNodeAddObject(PACKET_NAME_VP, this);
    dataNodeAddObject(PACKET_NAME_SCALE_VP, this);
    dataNodeAddObject(PACKET_NAME_UPDATE_VP, this);
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/

    //connect(&pushButtonMark, SIGNAL(clicked()), this, SLOT(switchScene()));
    connect(&pushButtonFall, SIGNAL(clicked()), this, SLOT(limitFall()));
    connect(&pushButtonRise, SIGNAL(clicked()), this, SLOT(limitRise()));
    ConfirmDlgVP = new Window_Confirm(0, QObject::tr("Confirm VP"), ":/resource/test.png");
    ConfirmDlgVP->raise();
    connect(ConfirmDlgVP, SIGNAL(NoteSelSignal(int)), this, SLOT(autoScale(int)),Qt::DirectConnection);
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(confirmAutoScale()));

    step = 5;
    autolock_flag=false;
    treatmentStateVP = TREATMENT_IS_READY;
}
void WidgetMeasureVP::autoScale(int)
{
//    if(this->AlarmLock==true)return;  //有报警的时候，不卡标#/
    float tmp = autoSetRange();
    autolock_flag=true;
    if(TREATMENT_IS_READY == treatmentStateVP){
            packetVP.vpL = tmp + VP_scale_range.readySafeScaleLow;
            packetVP.vpUp = tmp + VP_scale_range.readySafeScaleUp;
        }
    else if(TREATMENT_IS_RUNNING == treatmentStateVP){
        packetVP.vpL = tmp + VP_scale_range.runAutoScaleLow;
        packetVP.vpUp = tmp + VP_scale_range.runAutoScaleUp;
    }
    else if(TREATMENT_IS_STOP == treatmentStateVP){
        packetVP.vpL = tmp + VP_scale_range.stopSafeScaleLow;
        packetVP.vpUp = tmp + VP_scale_range.stopSafeScaleUp;
    }
    confirmSettingrangeVP();
    setRange(packetVP.vpL, packetVP.vpUp,true);
    dataNodeUpdate(PACKET_NAME_VP, &packetVP);
}
void WidgetMeasureVP::confirmAutoScale(void)
{
    timer.stop();
    autoScale(0);
//    showNoteDialog("Confirm VP");
}

#if HJX_NEW_SWITCH==0
void WidgetMeasureVP::limitFall(void)
{
    if(treatmentStateVP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    //if((VPAutoScaleMIN ==packetVP.vpL)  && (VPAutoScaleMAX == packetVP.vpUp) )return;
    packetVP.vpUp -= step;
    packetVP.vpL -= step;
    confirmSettingrangeVP();
    setRange(packetVP.vpL, packetVP.vpUp,false);
    dataNodeUpdate(PACKET_NAME_VP, &packetVP);
}
#else
void WidgetMeasureVP::limitFall(void)
{
//    if(treatmentStateVP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((VPAutoScaleMIN ==packetVP.vpL)  && (VPAutoScaleMAX == packetVP.vpUp) )return;
//    packetVP.vpUp -= step;
    packetVP.vpL -= step;
    confirmSettingrangeVP();
    setRange(packetVP.vpL, packetVP.vpUp,false);
    dataNodeUpdate(PACKET_NAME_VP, &packetVP);
}
#endif

#if HJX_NEW_SWITCH==0
void WidgetMeasureVP::limitRise(void)
{
    if(treatmentStateVP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    packetVP.vpUp += step;
    packetVP.vpL += step;
    confirmSettingrangeVP();
    setRange(packetVP.vpL, packetVP.vpUp,false);
    dataNodeUpdate(PACKET_NAME_VP, &packetVP);
}
#else
void WidgetMeasureVP::limitRise(void)
{
//    if(treatmentStateVP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((VPAutoScaleMIN ==packetVP.vpL)  && (VPAutoScaleMAX == packetVP.vpUp) )return;
    packetVP.vpUp += step;
//    packetVP.vpL += step;
    confirmSettingrangeVP();
    setRange(packetVP.vpL, packetVP.vpUp,false);
    dataNodeUpdate(PACKET_NAME_VP, &packetVP);
}
#endif

#if HJX_NEW_SWITCH==0
void WidgetMeasureVP::confirmSettingrangeVP(void)
{
    if(packetVP.vpUp >VPAutoScaleMAX){
        packetVP.vpL -= (packetVP.vpUp - VPAutoScaleMAX );
        packetVP.vpUp = VPAutoScaleMAX;
        if(packetVP.vpL<VPAutoScaleMIN)packetVP.vpL=VPAutoScaleMIN;
    }
    else if(packetVP.vpL <VPAutoScaleMIN){
        packetVP.vpUp += (VPAutoScaleMIN - packetVP.vpL );
        packetVP.vpL = VPAutoScaleMIN;
        if(packetVP.vpL>VPAutoScaleMAX)packetVP.vpL=VPAutoScaleMAX;
    }
}
#else
void WidgetMeasureVP::confirmSettingrangeVP(void)
{
    if(packetVP.vpUp >VPAutoScaleMAX){
        packetVP.vpUp = VPAutoScaleMAX;
    }
    if(packetVP.vpL <VPAutoScaleMIN){
        packetVP.vpL = VPAutoScaleMIN;
    }

    if(autolock_flag==true&&treatmentStateVP==TREATMENT_IS_RUNNING)
    {
        if(packetVP.vpL <10){
            packetVP.vpL = 10;
        }
    }
}
#endif

void WidgetMeasureVP::switchScene(void)
{
    PageShow("Setup Pressure");
}


void WidgetMeasureVP::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureVP-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_MAINTAIN_PARAMS){
        struct Params *params;
        params = (Params*)dataPtr;
        VPAutoScaleMAX = params->auto_scale.vp_span.attribute.max_value-700;//工程界面设置的值减700后下发的值
        VPAutoScaleMIN = params->auto_scale.vp_span.attribute.min_value-700;
        VPAutoScaleDefault = params->auto_scale.vp_span.attribute.default_value-700;

        VP_scale_range.runAutoScaleLow = params->auto_scale.vp_run_auto_scale_low.attribute.default_value-700;
        VP_scale_range.runAutoScaleUp = params->auto_scale.vp_run_auto_scale_up.attribute.default_value-700;
        VP_scale_range.stopSafeScaleUp = params->auto_scale.vp_stop_safe_scale_up.attribute.default_value-700;
        VP_scale_range.stopSafeScaleLow = params->auto_scale.vp_stop_safe_scale_low.attribute.default_value-700;
        VP_scale_range.readySafeScaleUp = params->auto_scale.vp_ready_safe_scale_up.attribute.default_value-700;
        VP_scale_range.readySafeScaleLow = params->auto_scale.vp_ready_safe_scale_low.attribute.default_value-700;
        VP_scale_range.runSafeScaleUp = params->auto_scale.vp_run_safe_scale_up.attribute.default_value-700;//治疗安全卡标#/
        VP_scale_range.runSafeScaleLow = params->auto_scale.vp_run_safe_scale_low.attribute.default_value-700;
        //qDebug()<<"VP value= "<<VP_scale_range.readySafeScaleUp<<VP_scale_range.readySafeScaleLow
                  //<<VP_scale_range.stopSafeScaleUp<<VP_scale_range.stopSafeScaleLow<<VP_scale_range.runAutoScaleUp
                    //<<VP_scale_range.runAutoScaleLow<<VP_scale_range.stopSafeScaleUp<<VP_scale_range.stopSafeScaleLow;
        ResetLock(TREATMENT_CHANGE_STATE);
    }
    if ( (name==PACKET_NAME_VP)){
        VPData = *(struct PacketVP*)dataPtr;
        if(treatmentStateVP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作
        if ((packetVP.vpUp != VPData.vpUp) || (packetVP.vpL != VPData.vpL)) {
            packetVP.vpL = VPData.vpL;
            packetVP.vpUp = VPData.vpUp;
            confirmSettingrangeVP();
            setRange(packetVP.vpL, packetVP.vpUp,true);
        }

        if (packetVP.vp != VPData.vp) {
            packetVP.vp = VPData.vp;
            setValue(packetVP.vp);
        }
    }
    else if (name==PACKET_NAME_UPDATE_VP){
        int VP_value = *(int *)dataPtr;
#if HJX_NEW_SWITCH==0
        if(treatmentStateVP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作
#else
        if(treatmentStateVP==TREATMENT_IS_RUNNING&&(alarmmodule->Is_CurrentAlarm(ALARM_VPUP)||alarmmodule->Is_CurrentAlarm(ALARM_VPDOWN)))
//        if(treatmentStateVP==TREATMENT_IS_RUNNING&&AlarmLock==true)  //治疗中卡标，不能操作
        {
            if(AlarmLock_VP==false)
            {
                packetVP.vp = VP_value;
                setValue(packetVP.vp);
                AlarmLock_VP=true;
            }
            return;
        }
#endif
        if (packetVP.vp != VP_value) {
            packetVP.vp = VP_value;
            setValue(packetVP.vp);
        }
    }
    else if (name==PACKET_NAME_SCALE_VP){                        //根据不同状态改变卡标方式
        int scaleType = *(int *)dataPtr;
        if(treatment_start_to_scale == scaleType)treatmentStateVP = TREATMENT_IS_RUNNING;//标记治疗状态
        else if(treatment_stop_to_scale == scaleType){
            treatmentStateVP =   TREATMENT_IS_STOP;
            timer.stop();
        }
        ResetLock(TREATMENT_CHANGE_STATE);
    }
    else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        if(workstate.tipState.current==TREAT_RUNNING)
           treatmentStateVP = TREATMENT_IS_RUNNING;
        else if(workstate.tipState.current==TREAT_FINISH)
           treatmentStateVP = TREATMENT_IS_STOP;

        packetVP=G_P_REBIRTH->packetVP;
        setRange(packetVP.vpL, packetVP.vpUp,true);
        dataNodeUpdate(PACKET_NAME_VP, &packetVP);
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureVP-ActionHandle-out");
#endif
}

// TMP
WidgetMeasureTMP::WidgetMeasureTMP(QWidget *parent) :
    WidgetMeasure(parent)
{
    setTestUnit("mmHg");
    dataNodeAddObject(PACKET_NAME_TMP, this);
    dataNodeAddObject(PACKET_NAME_SCALE_TMP, this);
    dataNodeAddObject(PACKET_NAME_UPDATE_TMP, this);
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
    //connect(&pushButtonMark, SIGNAL(clicked()), this, SLOT(switchScene()));
    connect(&pushButtonFall, SIGNAL(clicked()), this, SLOT(limitFall()));
    connect(&pushButtonRise, SIGNAL(clicked()), this, SLOT(limitRise()));
    ConfirmDlgTMP = new Window_Confirm(0, QObject::tr("Confirm TMP"), ":/resource/test.png");
    ConfirmDlgTMP->raise();
    connect(ConfirmDlgTMP, SIGNAL(NoteSelSignal(int)), this, SLOT(autoScale(int)),Qt::DirectConnection);
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(confirmAutoScale()));
    step = 5;
    treatmentStateTMP = TREATMENT_IS_READY;
}
void WidgetMeasureTMP::autoScale(int)
{
//    if(this->AlarmLock==true)return;  //有报警的时候，不卡标#/
    float tmp = autoSetRange();//获取当前显示值作为自动卡标基准#/
    if(TREATMENT_IS_READY == treatmentStateTMP){
            packetTMP.tmpL = tmp + TMP_scale_range.readySafeScaleLow;
            packetTMP.tmpUp = tmp + TMP_scale_range.readySafeScaleUp;
        }
    else if(TREATMENT_IS_RUNNING == treatmentStateTMP){
        packetTMP.tmpL = tmp + TMP_scale_range.runAutoScaleLow;
        packetTMP.tmpUp = tmp + TMP_scale_range.runAutoScaleUp;
    }
    else if(TREATMENT_IS_STOP == treatmentStateTMP){
        packetTMP.tmpL = tmp + TMP_scale_range.stopSafeScaleLow;
        packetTMP.tmpUp = tmp + TMP_scale_range.stopSafeScaleUp;
    }
    confirmSettingrangeTMP();
    setRange(packetTMP.tmpL, packetTMP.tmpUp,true);
    dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
}
void WidgetMeasureTMP::confirmAutoScale(void)
{
    timer.stop();
    autoScale(0);
//    showNoteDialog("Confirm TMP");
}

#if HJX_NEW_SWITCH==0
void WidgetMeasureTMP::limitFall(void)
{
    if(treatmentStateTMP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((TMPAutoScaleMIN ==packetTMP.tmpL)  && (TMPAutoScaleMAX == packetTMP.tmpUp) )return;
    packetTMP.tmpUp -= step;
    packetTMP.tmpL -= step;
    confirmSettingrangeTMP();
    setRange(packetTMP.tmpL, packetTMP.tmpUp,false);
    dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
}
#else
void WidgetMeasureTMP::limitFall(void)
{
//    if(treatmentStateTMP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((TMPAutoScaleMIN ==packetTMP.tmpL)  && (TMPAutoScaleMAX == packetTMP.tmpUp))return;
//    packetTMP.tmpUp -= step;
    packetTMP.tmpL -= step;
    confirmSettingrangeTMP();
    setRange(packetTMP.tmpL, packetTMP.tmpUp,false);
    dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
}
#endif

#if HJX_NEW_SWITCH==0
void WidgetMeasureTMP::limitRise(void)
{
    if(treatmentStateTMP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((TMPAutoScaleMIN ==packetTMP.tmpL)  && (TMPAutoScaleMAX == packetTMP.tmpUp) )return;
    packetTMP.tmpUp += step;
    packetTMP.tmpL += step;
    confirmSettingrangeTMP();
    setRange(packetTMP.tmpL, packetTMP.tmpUp,false);
    dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
}
#else
void WidgetMeasureTMP::limitRise(void)
{
//    if(treatmentStateTMP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作#/
    if((TMPAutoScaleMIN ==packetTMP.tmpL)  && (TMPAutoScaleMAX == packetTMP.tmpUp) )return;
    packetTMP.tmpUp += step;
//    packetTMP.tmpL += step;
    confirmSettingrangeTMP();
    setRange(packetTMP.tmpL, packetTMP.tmpUp,false);
    dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
}
#endif

#if HJX_NEW_SWITCH==0
void WidgetMeasureTMP::confirmSettingrangeTMP(void)
{
    if(packetTMP.tmpUp >TMPAutoScaleMAX){
        packetTMP.tmpL -= (packetTMP.tmpUp - TMPAutoScaleMAX );
        packetTMP.tmpUp = TMPAutoScaleMAX;
        if(packetTMP.tmpL<TMPAutoScaleMIN)packetTMP.tmpL=TMPAutoScaleMIN;
    }
    else if(packetTMP.tmpL <TMPAutoScaleMIN){
        packetTMP.tmpUp += (TMPAutoScaleMIN - packetTMP.tmpL );
        packetTMP.tmpL = TMPAutoScaleMIN;
        if(packetTMP.tmpUp>TMPAutoScaleMAX)packetTMP.tmpUp=TMPAutoScaleMAX;
    }
}
#else
void WidgetMeasureTMP::confirmSettingrangeTMP(void)
{
    if(packetTMP.tmpUp >TMPAutoScaleMAX){
        packetTMP.tmpUp = TMPAutoScaleMAX;
    }
    if(packetTMP.tmpL <TMPAutoScaleMIN){
        packetTMP.tmpL = TMPAutoScaleMIN;
    }
}
#endif

void WidgetMeasureTMP::switchScene(void)
{
    PageShow("Setup Pressure");
}


void WidgetMeasureTMP::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureTMP-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0) ) {
        return;
    }
    if ( name==PACKET_MAINTAIN_PARAMS){
        struct Params *params;
        params = (Params*)dataPtr;
        TMPAutoScaleMAX = params->auto_scale.temp_span.attribute.max_value-700;//工程界面设置的值-700为下发的值
        TMPAutoScaleMIN = params->auto_scale.temp_span.attribute.min_value-700;
        TMPAutoScaleDefault = params->auto_scale.temp_span.attribute.default_value-700;

        TMP_scale_range.runAutoScaleLow = params->auto_scale.tmp_run_auto_scale_low.attribute.default_value-700;
        TMP_scale_range.runAutoScaleUp = params->auto_scale.tmp_run_auto_scale_up.attribute.default_value-700;
        TMP_scale_range.stopSafeScaleUp = params->auto_scale.tmp_stop_safe_scale_up.attribute.default_value-700;
        TMP_scale_range.stopSafeScaleLow = params->auto_scale.tmp_stop_safe_scale_low.attribute.default_value-700;
        TMP_scale_range.readySafeScaleUp = params->auto_scale.tmp_ready_safe_scale_up.attribute.default_value-700;
        TMP_scale_range.readySafeScaleLow = params->auto_scale.tmp_ready_safe_scale_low.attribute.default_value-700;
        TMP_scale_range.runSafeScaleUp = params->auto_scale.tmp_run_safe_scale_up.attribute.default_value-700;
        TMP_scale_range.runSafeScaleLow = params->auto_scale.tmp_run_safe_scale_low.attribute.default_value-700;
        ResetLock(TREATMENT_CHANGE_STATE);
    }
    if ( (name==PACKET_NAME_TMP)){    //改变上下限范围，并发送PDO#/
        TMPData= *(struct PacketTMP*)dataPtr;
        if(treatmentStateTMP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作
        if ((packetTMP.tmpUp != TMPData.tmpUp) || (packetTMP.tmpL != TMPData.tmpL)) {
            packetTMP.tmpL = TMPData.tmpL;
            packetTMP.tmpUp = TMPData.tmpUp;
            confirmSettingrangeTMP();
            setRange(packetTMP.tmpL, packetTMP.tmpUp,true);
        }
        if (packetTMP.tmp !=TMPData.tmp) {
            packetTMP.tmp = TMPData.tmp;
            setValue(packetTMP.tmp);
        }
    }
    else if (name==PACKET_NAME_UPDATE_TMP){//接收到的跨膜压值#/
        int TMP_value = *(int *)dataPtr;

#if HJX_NEW_SWITCH==0
        if(treatmentStateTMP==TREATMENT_IS_RUNNING&&AlarmLock==true)return;  //治疗中卡标，不能操作
#else
        if(treatmentStateTMP==TREATMENT_IS_RUNNING&&(alarmmodule->Is_CurrentAlarm(ALARM_TMPUP)||alarmmodule->Is_CurrentAlarm(ALARM_TMPDOWN)))
//        if(treatmentStateTMP==TREATMENT_IS_RUNNING&&AlarmLock==true)  //治疗中卡标，不能操作#/
        {
            /*
            if(AlarmLock_TMP==false)
            {
                packetTMP.tmp = TMP_value;
                setValue(packetTMP.tmp);
                AlarmLock_TMP=true;
            }
            */
            return;
        }
#endif
        if (packetTMP.tmp !=TMP_value) {
            packetTMP.tmp = TMP_value;
            setValue(packetTMP.tmp);
        }
    }
    else if (name==PACKET_NAME_SCALE_TMP){
        int scaleType = *(int *)dataPtr;
        qDebug()<<"scaleType=="<<scaleType;
        if(treatment_start_to_scale == scaleType)treatmentStateTMP = TREATMENT_IS_RUNNING;
        else if(treatment_stop_to_scale == scaleType){treatmentStateTMP = TREATMENT_IS_STOP;timer.stop();}
        ResetLock(TREATMENT_CHANGE_STATE);
    }
    else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        if(workstate.tipState.current==TREAT_RUNNING)
           treatmentStateTMP = TREATMENT_IS_RUNNING;
        else if(workstate.tipState.current==TREAT_FINISH)
           treatmentStateTMP = TREATMENT_IS_STOP;

        packetTMP=G_P_REBIRTH->packetTMP;
        setRange(packetTMP.tmpL, packetTMP.tmpUp,true);
        dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureTMP-ActionHandle-out");
#endif
}

// Cond
WidgetMeasureCond::WidgetMeasureCond(QWidget *parent) :
    WidgetMeasure(parent)
{
    setTestUnit("mS/cm");
    dataNodeAddObject(PACKET_NAME_COND, this);
//    dataNodeAddObject(PACKET_MASTERCTRL_OUTLET,this);
    dataNodeAddObject(PACKET_MONITOR_WATER, this);
    dataNodeAddObject(PACKET_FORMULA_TARGET, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
    //connect(&pushButtonMark, SIGNAL(clicked()), this, SLOT(switchScene()));
    connect(&pushButtonFall, SIGNAL(clicked()), this, SLOT(limitFall()));
    connect(&pushButtonRise, SIGNAL(clicked()), this, SLOT(limitRise()));

    step = 0.1;
    struct PacketCond tmpCond;
    tmpCond.condUp = 14.3;
    tmpCond.condL = 13.7;
    tmpCond.cond = 14;
    dataNodeUpdate(PACKET_NAME_COND, &tmpCond);
}

void WidgetMeasureCond::limitFall(void)
{
    if(packetCond.condL<= 11.3)//
        return;
    packetCond.condUp -= step;
    packetCond.condL -= step;
    setRange(packetCond.condL, packetCond.condUp,false);
#if DEBUG_OTHER_FLAG
    Debug_insert("NAME_COND_limitFall-in");
#endif
    dataNodeUpdate(PACKET_NAME_COND, &packetCond);
#if DEBUG_OTHER_FLAG
    Debug_insert("NAME_COND_limitFall-out");
#endif
}

void WidgetMeasureCond::limitRise(void)
{
    if(packetCond.condUp>=16.4)
        return;
    packetCond.condUp += step;
    packetCond.condL += step;
    setRange(packetCond.condL, packetCond.condUp,false);
#if DEBUG_OTHER_FLAG
    Debug_insert("NAME_COND_limitRise-in");
#endif
    dataNodeUpdate(PACKET_NAME_COND, &packetCond);
#if DEBUG_OTHER_FLAG
    Debug_insert("NAME_COND_limitRise-out");
#endif
}

void WidgetMeasureCond::switchScene(void)
{
    PageShow("Setup Cond");
}

void WidgetMeasureCond::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureCond-ActionHandle-in00");
#endif
    if ((dataPtr == NULL) || (name == 0) ) {
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureCond-ActionHandle-in01");
#endif
        return;
    }

   if(name==PACKET_NAME_COND)   //设置更新电导上下限
   {
    CondData = *(struct PacketCond*)dataPtr;
#if DEBUG_OTHER_FLAG
    Debug_insert("measureNAME_COND-in01");
#endif
    if ((packetCond.condUp != CondData.condUp) || (packetCond.condL != CondData.condL)) {
#if DEBUG_OTHER_FLAG
    Debug_insert("measureNAME_COND-in02");
#endif
        packetCond.condL = CondData.condL;
        packetCond.condUp = CondData.condUp;
        setRange(packetCond.condL, packetCond.condUp,true);         
    }

    if (packetCond.cond != CondData.cond) {
#if DEBUG_OTHER_FLAG
    Debug_insert("measureNAME_COND-in03");
#endif
        packetCond.cond = CondData.cond;
        setValue(packetCond.cond);         
    }
#if DEBUG_OTHER_FLAG
    Debug_insert("measureNAME_COND-out");
#endif
    }
    else if(name==PACKET_MONITOR_WATER)  //根据监控板上传值更新显示#/
    {
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureCond-ActionHandle-in02");
#endif

       PacketWaterOut *Outlet = (struct PacketWaterOut*)dataPtr;
       float cond=0;
//       cond=((float)((int)(Outlet->N3_out_cond/10)))/10;
//       cond=Outlet->N3_out_cond/100;
       cond=QString::number(((float)Outlet->N5_out_cond/100),'f',1).toFloat();
       if (packetCond.cond != cond) {
           packetCond.cond = cond;
           setValue(packetCond.cond);
       }
    }else if(name==PACKET_FORMULA_TARGET) {
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureCond-ActionHandle-in03");
#endif
       FormulaSet *formulaSet = (struct FormulaSet*)dataPtr;
       PacketCond  CondTmp;
       float cond=0;

       if(G_rebirth_flag==true)  //重生操作时取消处理#/
           return;


       cond=formulaSet->outTarget.target_mix+5;
  //     cond=QString::number(((float)formulaSet->outTarget.target_mix/100.0),'f',1).toFloat();
       cond=QString::number(cond/100,'f',1).toFloat();
       CondTmp.condL=cond-0.5;
       CondTmp.condUp=cond+0.5;

       /*
       CondTmp.condL=(float)formulaSet->outTarget.target_mix/100.0-0.3;
       CondTmp.condUp=(float)formulaSet->outTarget.target_mix/100.0+0.3;
       */

       if(CondTmp.condL>15.8)
       {
           CondTmp.condL=15.8;
           CondTmp.condUp=16.8;
       }

       if(CondTmp.condL<11.3)
       {
           CondTmp.condL=11.3;
           CondTmp.condUp=12.3;
       }

       if(CondTmp.condUp>16.8)
       {
           CondTmp.condUp=16.8;
           CondTmp.condL=15.8;
       }

       if(CondTmp.condUp<12.3)
       {
           CondTmp.condUp=12.3;
           CondTmp.condL=11.3;
       }
       int value=0;
       QString strL,strUp;
       CondTmp.condL=strL.setNum(CondTmp.condL, 'f', 1).toFloat();
       CondTmp.condUp=strUp.setNum(CondTmp.condUp, 'f', 1).toFloat();
       if ((packetCond.condUp != CondTmp.condUp) || (packetCond.condL != CondTmp.condL)) {
           value=(CondTmp.condL+0.05)*10;
           packetCond.condL = value/10.0;
           value=(CondTmp.condUp+0.05)*10;
           packetCond.condUp = value/10.0;
           setRange(packetCond.condL, packetCond.condUp,false);//true
           dataNodeUpdate(PACKET_NAME_COND,&packetCond);
       }
   }
   else if(name==PACKET_REBIRTH_HANDLE)
   {
        packetCond=G_P_REBIRTH->packetCond;
        setRange(packetCond.condL, packetCond.condUp,true);
        dataNodeUpdate(PACKET_NAME_COND,&packetCond);
   }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureCond-ActionHandle-out");
#endif
}

//btmark
///*
WidgetPButtontMark::WidgetPButtontMark(QWidget *parent) :
    QWidget(parent),
    pushButtonMark(this)
{
    pushButtonMark.setStyleNormal(":/resource/images/measure_button_mark.png",
            QRect(WIDGETHEASURE_PUSHBUTTON_MARK_X-40,
                WIDGETHEASURE_PUSHBUTTON_MARK_Y-8, 107, 42)); //中间四个按键#/

    pushButtonMark.setTextFontPos(CFG_FONT_SIZE_FIRST_2,
            CONFIG_FONT_NORMAL, CONFIG_COLOR_BLUE);
}

bool WidgetPButtontMark::setTextMark(const QString text)
{
    pushButtonMark.setText(text);
    return true;
}

WidgetPButtontMarkAP::WidgetPButtontMarkAP(QWidget *parent) :
    WidgetPButtontMark(parent)
{
   connect(&pushButtonMark, SIGNAL(clicked()), this, SLOT(switchScene()));
}

void WidgetPButtontMarkAP::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureAP-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0) ||
            (name==PACKET_NAME_COND)) {
        return;
    }
}

void WidgetPButtontMarkAP::switchScene(void)
{
    PageShow("Setup Pressure");
}


WidgetPButtontMarkVP::WidgetPButtontMarkVP(QWidget *parent) :
    WidgetPButtontMark(parent)
{
   connect(&pushButtonMark, SIGNAL(clicked()), this, SLOT(switchScene()));
}

void WidgetPButtontMarkVP::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("WidgetMeasureVP-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0) ||
            (name==PACKET_NAME_COND)) {
        return;
    }
}

void WidgetPButtontMarkVP::switchScene(void)
{
    PageShow("Setup Pressure");//Setup Cond
}

WidgetPButtontMarkTMP::WidgetPButtontMarkTMP(QWidget *parent) :
    WidgetPButtontMark(parent)
{
   connect(&pushButtonMark, SIGNAL(clicked()), this, SLOT(switchScene()));
}

void WidgetPButtontMarkTMP::ActionHandle(const void* dataPtr, int name)
{
    if ((dataPtr == NULL) || (name == 0) ||
            (name==PACKET_NAME_COND)) {
        return;
    }
}

void WidgetPButtontMarkTMP::switchScene(void)
{
    PageShow("Setup Pressure");
}

WidgetPButtontMarkCond::WidgetPButtontMarkCond(QWidget *parent) :
    WidgetPButtontMark(parent)
{
   connect(&pushButtonMark, SIGNAL(clicked()), this, SLOT(switchScene()));
}

void WidgetPButtontMarkCond::ActionHandle(const void* dataPtr, int name)
{
    if ((dataPtr == NULL) || (name == 0) ||
            (name==PACKET_NAME_COND)) {
        return;
    }
}

void WidgetPButtontMarkCond::switchScene(void)
{
    PageShow("Setup Cond");
}
