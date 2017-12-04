#include <QtGui>
#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_ultrafiltration.h"
#include "data_set.h"
#include "global_var.h"
#include "common_tr_string.h"

inline void SceneSetupUf::setUi()
{
    //Ultrafiltration target
//    SCENE_SETUP_CREATE_TABLE(labelUfTarget, 1);
    labelUfTarget.setGeometry(QRect(SCENE_SETUP_COL_LABEL-50,
                                    SCENE_SETUP_ROW_1,
                                    350,
                                    SCENE_SETUP_WIDGET_HIGH));
    labelUfTarget.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    labelUfTarget.setFont(QFont(QString(CONFIG_FONT_STYLE), CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUfTarget.setStyleSheet(CONFIG_COLOR_LABEL);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditUfAim, 1);
    SCENE_SETUP_CREATE_UNIT(labelUfTargetUnit, 1);
    labelUfTargetUnit.setText("ml");

    //Ultrafiltration rate
 //   SCENE_SETUP_CREATE_TABLE(labelUfRate, 2);
    labelUfRate.setGeometry(QRect(SCENE_SETUP_COL_LABEL-50,
                                    SCENE_SETUP_ROW_2,
                                    350,
                                    SCENE_SETUP_WIDGET_HIGH));
    labelUfRate.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    labelUfRate.setFont(QFont(QString(CONFIG_FONT_STYLE), CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelUfRate.setStyleSheet(CONFIG_COLOR_LABEL);
//    SCENE_SETUP_CREATE_LINEEDIT(lineEditUfRate, 2);
    SCENE_SETUP_CREATE_SHOWSTYLE(lineEditUfRate, 2);
    SCENE_SETUP_CREATE_UNIT(labelUfRateUnit, 2);
    labelUfRateUnit.setText("ml/h");

    lineEditUfRate.setEnabled(false);

    SCENE_SETUP_CREATE_TABLE(labelNowUfRateText, 3);
    SCENE_SETUP_CREATE_VALUE(labelNowUfRateValue, 3);
    SCENE_SETUP_CREATE_UNIT(labelNowUfRateUnit, 3);
    labelNowUfRateUnit.setText("ml/h");

    //The actual amount of ultrafiltration
    SCENE_SETUP_CREATE_TABLE(labelActualAmountUf, 4);
    SCENE_SETUP_CREATE_VALUE(labelActualAmountUfValue, 4);
    SCENE_SETUP_CREATE_UNIT(labelActualAmountUfunit, 4);
    labelActualAmountUfunit.setText("ml");

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));
    buttonCurve.setGeometry(QRect(620, 121, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);
    BUTTON_SIMPLE_STYLE(buttonCurve);

    labelActualAmountUfValue.setNum(0);
    CustomSetText();
}

SceneSetupUf::SceneSetupUf(QWidget *parent) :
    QObject(parent),
    labelUfTarget(parent),
    lineEditUfAim(parent),
    labelUfTargetUnit(parent),

    labelUfRate(parent),
    lineEditUfRate(parent),
    labelUfRateUnit(parent),

    labelNowUfRateText(parent),
    labelNowUfRateValue(parent),
    labelNowUfRateUnit(parent),

    labelActualAmountUf(parent),
    labelActualAmountUfValue(parent),
    labelActualAmountUfunit(parent),

    buttonCurve(parent),
    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    connect(&buttonSave, SIGNAL(clicked()), this, SLOT(SlotParameterSave()));
    connect(&buttonCancel, SIGNAL(clicked()), this, SLOT(SlotParameterCancel()));
    connect(&buttonCurve, SIGNAL(clicked()), this, SLOT(on_buttonCurve_clicked()));
    dataNodeAddObject(TREAT_TIMER, this);   //治疗时间的状态标志#/
    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(PACKET_NAME_NOW_UF_VOLUME, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/

    PageAppendPiece("Setup UF", this);

    isKeyInit = false;
    setParentName("SceneSetupUf");
}

void SceneSetupUf::on_buttonCurve_clicked()
{
    PageShow("Setup CurveUF");
}

void SceneSetupUf::SlotParameterSave()
{
    dataRead(PACKET_NAME_UF, &packetUF);
    if((packetUF.ufVolAim!=lineEditUfAim.text().toInt())||(packetUF.ufRate != lineEditUfRate.text().toInt()))
    {
        packetUF.ufVolAim = lineEditUfAim.text().toInt();
        packetUF.ufRate = (int)(lineEditUfRate.text().toFloat()); //* 60

        if(packetUF.chooseRate==true)
            G_UF_SETRATE=packetUF.ufRate;   //超滤页面参数设置时#/
        dataNodeUpdate(PACKET_NAME_UF, &packetUF);

        G_UF_FINISHVOL_FLAG=false;
        G_UF_CURETARGET=packetUF.ufVolAim;
        dataNodeUpdate(PACKET_PARAMS_UF, &G_UF_CURETARGET);

        WorkState  stateTMP=GetWorkStateManage();
        if((G_UF_PROFILE_FLAG==true)&&(stateTMP.tipState.current==TREAT_RUNNING))
        {
            G_UF_PROFILE_FLAG=false;
            G_UF_PROFILE_NUM=0;
            G_UF_SELECTCURVE_NUM=-1;

            showYesDialog(DE_TR_PROMPT,DE_TR_UFAIMVOLCHANGEDPROFILECANCEL);
        }
    }

    if(packetUF.chooseRate==true)
    {
        labelNowUfRateValue.setText(QString::number(packetUF.ufRate));
    }
    else
    {
        labelNowUfRateValue.setText(QString::number(packetUF.preRate));
    }
}

void SceneSetupUf::SlotParameterCancel()
{
    QString str;
    if (packetUF.ufVolAim != lineEditUfAim.text().toInt()) {
        lineEditUfAim.setText(str.setNum(packetUF.ufVolAim));
    }
}

//键盘上set按键返回#/
void SceneSetupUf::lineEdit_data_set(const char *name, const char *data)
{
    name = name;
    data = data;
    LableUpdate(name);
    SlotParameterSave();
}

//弹出键盘时#/
void SceneSetupUf::lineEdit_start(const char *name, float *max, float *min)
{
    if(name == NULL)
        return;

    QString num;
    float uf_Volume=0;
    WorkState  stateTMP=GetWorkStateManage();
    if(stateTMP.tipState.current==TREAT_RUNNING)
    {
        uf_Volume = labelActualAmountUfValue.text().toFloat();//已超滤量#/
    }

    if(strcmp(name, "UF Goal") == 0) {

        *max = (unsigned int)(num.setNum(uf_Volume+params->Params_UF.UF_Rate.attribute.max_value * residue_secCount/3600 , 'f', 2).toFloat());
         if(*max  >= params->Params_UF.UF_Goal.attribute.max_value)*max=params->Params_UF.UF_Goal.attribute.max_value;
        *min = uf_Volume ;

    } else if(strcmp(name, "UF rate") == 0) {
        *max=0;
        if(residue_secCount>0)*max = (unsigned int)(params->Params_UF.UF_Goal.attribute.max_value*3600/residue_secCount);
        else{*max = 0;}
        if(*max > params->Params_UF.UF_Rate.attribute.max_value){
            *max = params->Params_UF.UF_Rate.attribute.max_value;
            }
        *min = 0;
    }
}

//关闭键盘时#/
void SceneSetupUf::lineEdit_close(const char *name){
   name = name;
}

void SceneSetupUf::LableUpdate(const char *name)
{
    QString num;
    float uf_goal = lineEditUfAim.text().toFloat();
    float uf_rate = lineEditUfRate.text().toFloat();
    float uf_time;
    if(strcmp(name, "UF Goal") == 0) {  //治疗超滤目标#/
        //变速度#/
        uf_time=(float)residue_secCount;//剩余治疗时间#/
        dataRead(PACKET_NAME_UF, &packetUF);
        G_UF_CURERESIDUE = uf_goal-G_UF_CUREFINISH;//剩余治疗超滤目标量#/
        if(uf_time>0){
            lineEditUfRate.setText(num.setNum((1.0 * G_UF_CURERESIDUE*3600) / uf_time, 'f', 0));
        }
        else{
            lineEditUfAim.setText(num.setNum(packetUF.ufVolNow));
            lineEditUfRate.setText(num.setNum(0));
        }

    } else if(strcmp(name, "UF rate") == 0) {

        lineEditUfAim.setText(num.setNum(1.0 * uf_rate*residue_secCount/3600+packetUF.ufVolNow, 'f', 0));
    }
}

void SceneSetupUf::lineEdit_init()
{
    lineEditUfAim.setObjectName(tr("UF Goal:"));
    lineEditUfRate.setObjectName(tr("UF rate:"));

    ic->appendWidget(&lineEditUfAim, NumInputMethod, LineEdit, this,
                     params->Params_UF.UF_Goal.attribute.min_value,
                     params->Params_UF.UF_Goal.attribute.max_value,
                     params->Params_UF.UF_Goal.attribute.step_value,
                     tr("UF Goal:"),"UF Goal");

    ic->appendWidget(&lineEditUfRate, NumInputMethod, LineEdit, this,
                     params->Params_UF.UF_Rate.attribute.min_value,
                     params->Params_UF.UF_Rate.attribute.max_value,
                     params->Params_UF.UF_Rate.attribute.step_value,
                     tr("UF rate:"),"UF rate");

}

void SceneSetupUf::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupUf-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name ==0) ) {
        return;
    }
    if(name==PACKET_NAME_NOW_UF_VOLUME){
        QString num;
        int uf_volume=0;
        int temp = *((int*)dataPtr);

        dataRead(PACKET_NAME_UF,&packetUF);
        packetUF.ufVolNow = G_UF_CUREFINISH;
        G_UF_CURERESIDUE = packetUF.ufVolAim-G_UF_CUREFINISH;
        /*
        labelActualAmountUfValue.setText(num.setNum(packetUF.ufVolNow));//实时更新已超滤量#/
        */

        WorkState  stateTMP=GetWorkStateManage();
        if(stateTMP.tipState.current==TREAT_READY)
        {
       //     uf_volume=G_UF_PREFINISH+UFPump_Pre_Amount;
       //     labelActualAmountUfValue.setText(num.setNum(uf_volume));//实时更新已超滤量#/
            labelActualAmountUfValue.setText(num.setNum(temp));//实时更新已超滤量#/
            labelNowUfRateValue.setText(QString::number(packetUF.preRate));
        }
        else if(stateTMP.tipState.current==TREAT_RUNNING)
        {
            uf_volume=G_UF_CUREFINISH;
            labelActualAmountUfValue.setText(num.setNum(uf_volume));
            labelNowUfRateValue.setText(QString::number(packetUF.ufRate));
        }
        else
        {
            uf_volume=UFPump_Cure_Amount;
            labelActualAmountUfValue.setText(num.setNum(uf_volume));
            labelNowUfRateValue.setText(QString::number(packetUF.ufRate));
        }
        return;
    }

    if(name==PACKET_MAINTAIN_PARAMS){
        params = (Params*)dataPtr;
        dataRead(PACKET_NAME_UF, &packetUFinit);
        lineEditUfAim.setText(QString::number(params->Params_UF.UF_Goal.attribute.default_value));
        float rate=1;
        if(params->Params_UF.UF_Time.attribute.default_value>0)
             rate = params->Params_UF.UF_Goal.attribute.default_value / params->Params_UF.UF_Time.attribute.default_value;
        QString num;
        lineEditUfRate.setText(num.setNum(rate, 'f', 0));

        G_UF_EARLIER_TIME=params->Params_UF.Earlier_Time.attribute.default_value; //UF量提前完成的时间#/

        G_UF_autochangelimit=params->Params_UF.AutoChange_Limit.attribute.default_value;
        G_UF_autochangelimit=G_UF_autochangelimit/100;

        G_UF_CURETARGET = params->Params_UF.UF_Goal.attribute.default_value;
        G_UF_CURERESIDUE = G_UF_CURETARGET;
        dataNodeUpdate(PACKET_PARAMS_UF, &G_UF_CURETARGET);

        packetUFinit.ufVolAim = params->Params_UF.UF_Goal.attribute.default_value;
        G_UF_CUREFINISH = packetUFinit.ufVolNow = 0;
        packetUFinit.ufT = 240;
        packetUFinit.ufRate = rate;
        packetUFinit.chooseRate=false;
        dataNodeUpdate(PACKET_NAME_UF, &packetUFinit);
      }

    if (name==TREAT_TIMER)//治疗时间改变时#/
    {
         int Timer_state = *((int*) dataPtr);
//         WorkState  stateTMP=GetWorkStateManage();
         if(Timer_state==Timechange)
         {
             QString num;
             float uf_goal = lineEditUfAim.text().toFloat();
             float uf_rate;
             float uf_time;

             uf_time=(float)residue_secCount;//剩余治疗时间#/

             G_UF_CURERESIDUE = uf_goal-G_UF_CUREFINISH;//剩余治疗超滤目标量#/
                 if(uf_time>0){

                     uf_rate=(1.0 * G_UF_CURERESIDUE*3600) / uf_time;

                     G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
                     G_UF_ENDTIME=G_UF_STARTTIME+residue_secCount;  //超滤的总时间 单位秒#/

                     if(uf_rate>params->Params_UF.UF_Rate.attribute.max_value)
                         uf_rate=params->Params_UF.UF_Rate.attribute.max_value;
                     else if(uf_rate<params->Params_UF.UF_Rate.attribute.min_value)
                         uf_rate=params->Params_UF.UF_Rate.attribute.min_value;

                     G_UF_SETRATE=uf_rate;   //治疗时间改变时#/
                     lineEditUfRate.setText(num.setNum(uf_rate, 'f', 0));

                        dataRead(PACKET_NAME_UF, &packetUF);
                        packetUF.ufT = uf_time/60;
                        packetUF.ufRate = G_UF_SETRATE;
                        dataNodeUpdate(PACKET_NAME_UF, &packetUF);
                 }
                 else{
                     lineEditUfAim.setText(num.setNum(packetUF.ufVolNow));
                     lineEditUfRate.setText(num.setNum(0));
                 }
         }
         return;
    }else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        G_UF_PRETARGET=G_P_REBIRTH->G_UF_PRETARGET; //预冲阶段超滤目标量#/
        G_UF_CURERESIDUE=G_P_REBIRTH->G_UF_CURERESIDUE;  //治疗剩余的超滤量#/
        G_UF_CURETARGET=G_P_REBIRTH->G_UF_CURETARGET;  //治疗阶段目标超滤量#/
        G_UF_STARTTIME=G_P_REBIRTH->G_UF_STARTTIME;    //超滤开启时间 单位秒#/
        G_UF_ENDTIME=G_P_REBIRTH->G_UF_ENDTIME;        //超滤的结束时间#/
        G_UF_FINISHVOL_FLAG=G_P_REBIRTH->G_UF_FINISHVOL_FLAG;    //超滤泵已完成本次超滤目标的标志#/
        G_UF_PREFINISH=G_P_REBIRTH->G_UF_PREFINISH;   //实时已预冲超滤量#/
        G_UF_CUREFINISH=G_P_REBIRTH->G_UF_CUREFINISH;  //治疗阶段已完成超滤量#/
        UFPump_Pre_Amount=G_P_REBIRTH->UFPump_Pre_Amount;   //超滤泵预冲累计量#/
        UFPump_Cure_Amount=G_P_REBIRTH->UFPump_Cure_Amount;   //超滤泵治疗累计量#/
        packetUF=G_P_REBIRTH->packetUF;//超滤泵参数#/

        lineEditUfAim.setText(QString::number(packetUF.ufVolAim));
        lineEditUfRate.setText(QString::number(packetUF.ufRate, 'f', 0));

        dataNodeUpdate(PACKET_PARAMS_UF, &G_UF_CURETARGET);

        printf("UFAim02==%d \n", G_P_REBIRTH->packetUF.ufVolAim);
        printf("packetUF==%d \n", packetUF.ufVolAim);

        if(packetUF.chooseRate==true)
        {
            labelNowUfRateValue.setText(QString::number(packetUF.ufRate));
        }
        else
        {
            labelNowUfRateValue.setText(QString::number(packetUF.preRate));
        }

        dataNodeUpdate(PACKET_NAME_UF, &packetUF);
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupUf-ActionHandle-out");
#endif
}


void SceneSetupUf::CustomSetText()
{
#if EEDA_VERSION_OPEN
    labelUfTarget.setText(tr("Treatment UF Goal:"));
    labelUfRate.setText(tr("Treatment UF Rate:"));
    labelNowUfRateText.setText(tr("Now Rate:"));
    labelActualAmountUf.setText(tr("Acc UF Volume:"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    buttonCurve.setText(tr("Profile"));
#else
    labelUfTarget.setText(tr("Cure UF Goal:"));
    labelUfRate.setText(tr("Cure UF rate:"));
    labelNowUfRateText.setText(tr("Now rate:"));
    labelActualAmountUf.setText(tr("Acc UF Volume:"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    buttonCurve.setText(tr("Profile"));
#endif
}

void SceneSetupUf::setCusVisible(bool vis, char* para)
{
    para = para;

    if(vis && !isKeyInit) {
        isKeyInit = true;
        lineEdit_init();
    }

    /*
    if(G_UF_CUREFINISH != packetUF.ufVolNow){
        packetUF.ufVolNow = G_UF_CUREFINISH;
        dataNodeUpdate(PACKET_NAME_UF,&packetUF);
    }
    */
    dataRead(PACKET_NAME_UF, &packetUF);

    if(packetUF.chooseRate==true)
    {
        labelNowUfRateValue.setText(QString::number(packetUF.ufRate));
    }
    else
    {
        labelNowUfRateValue.setText(QString::number(packetUF.preRate));
    }



    /*
    QString num;
    WorkState  stateTMP=GetWorkStateManage();
    if(stateTMP.tipState.current<=TREAT_READY)
    {
        PacketPreflush Preflush;
        dataRead(PACKET_PREFLUSH, &Preflush);

        if(packetUF.chooseRate==false)
        {
            lineEditUfRate.setText(num.setNum(packetUF.preRate));
            lineEditUfAim.setText(num.setNum(Preflush.PreflushUfVolume));
        }
        else
        {
            lineEditUfRate.setText(num.setNum(packetUF.ufRate));
            lineEditUfAim.setText(num.setNum(packetUF.ufVolAim));
        }
    }
    else
    {
        lineEditUfAim.setText(num.setNum(packetUF.ufVolAim));
    //    labelActualAmountUfValue.setText(num.setNum(packetUF.ufVolNow));//切换到超滤界面时，显示最新的超滤量
        lineEditUfRate.setText(num.setNum(packetUF.ufRate));
    }
    */

    labelUfTarget.setVisible(vis);
    lineEditUfAim.setVisible(vis);
    labelUfTargetUnit.setVisible(vis);

    labelUfRate.setVisible(vis);
    lineEditUfRate.setVisible(vis);
    labelUfRateUnit.setVisible(vis);

#if EEDA_VERSION_OPEN==1
    labelNowUfRateText.setVisible(vis);
    labelNowUfRateValue.setVisible(vis);
    labelNowUfRateUnit.setVisible(vis);

#else
    labelNowUfRateText.setVisible(false);
    labelNowUfRateValue.setVisible(false);
    labelNowUfRateUnit.setVisible(false);
#endif

    labelActualAmountUf.setVisible(vis);
    labelActualAmountUfValue.setVisible(vis);
    labelActualAmountUfunit.setVisible(vis);
    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);
    buttonCurve.setVisible(vis);
}
