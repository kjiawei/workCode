#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_isouf.h"
#include "common_tr_string.h"
#include "common_func.h"

inline void SceneSetupIsoUF::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelIsoufTimeHour, 1);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditIsoufTimeHour, 1);
    lineEditIsoufTimeHour.setGeometry(QRect(395,76,50,38));
    SCENE_SETUP_CREATE_UNIT(labelIsoufTimeHourUnit, 1);
    labelIsoufTimeHourUnit.setText("");

    //SCENE_SETUP_CREATE_TABLE(labelIsoufTimeMin, 2);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditIsoufTimeMin, 1);
    SCENE_SETUP_CREATE_UNIT(labelIsoufTimeMinUnit, 1);
    labelIsoufTimeMinUnit.setText(":");
    labelIsoufTimeMinUnit.setAlignment(Qt::AlignCenter);
    labelIsoufTimeMinUnit.setGeometry(QRect(445,76,10,38));
    lineEditIsoufTimeMin.setGeometry(QRect(455,76,50,38));

    SCENE_SETUP_CREATE_TABLE(labelIsoufTotal, 2);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditIsoufTotal, 2);
    SCENE_SETUP_CREATE_UNIT(labelIsoufTotalUnit, 2);
    labelIsoufTotalUnit.setText("mL");

    SCENE_SETUP_CREATE_TABLE(labelActualAmountIsoUf, 3);
    SCENE_SETUP_CREATE_VALUE(labelActualAmountIsoUfValue, 3);
    SCENE_SETUP_CREATE_UNIT(labelActualAmountIsoUfunit, 3);
    labelActualAmountIsoUfunit.setText("mL");



    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    connect(&buttonSave,SIGNAL(clicked()),this,SLOT(Save()));
#if HJX_COMMUNICATION_SWITCH
    connect(this,SIGNAL(signalSDO(int)),dataCanSend,SLOT(sendSDOData(int)),DE_CONNECTION_TYPE);
#endif
    CustomSetText();
}

SceneSetupIsoUF::SceneSetupIsoUF(QWidget *parent) :
    QObject(parent),
    labelIsoufTimeHour(parent),
    lineEditIsoufTimeHour(parent),
    labelIsoufTimeHourUnit(parent),

    //labelIsoufTimeMin(parent),
    lineEditIsoufTimeMin(parent),
    labelIsoufTimeMinUnit(parent),


    labelIsoufTotal(parent),
    lineEditIsoufTotal(parent),
    labelIsoufTotalUnit(parent),

    labelActualAmountIsoUf(parent),
    labelActualAmountIsoUfValue(parent),
    labelActualAmountIsoUfunit(parent),

    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    PageAppendPiece("Setup ISO UF", this);
    lineEdit_init();
    setParentName("SceneSetupIsoUF");
    dataNodeAddObject(PACKET_MASTERCTRL_UFVALUE, this);
    dataNodeAddObject(PACKET_NAME_WORKMODE,this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
}
void SceneSetupIsoUF::lineEdit_init()
{

    lineEditIsoufTimeHour.setObjectName("UF Hour");
    lineEditIsoufTimeMin.setObjectName("UF Min");
    lineEditIsoufTotal.setObjectName("UF Total");

    ic->appendWidget(&lineEditIsoufTimeHour,NumInputMethod, LineEdit, this,
                     1,
                     1,
                     1,
                     tr("ISO UF Hour:"),"ISO UF Hour");
    ic->appendWidget(&lineEditIsoufTimeMin,NumInputMethod, LineEdit, this,
                     1,
                     1,
                     1,
                     tr("ISO UF Min:"),"ISO UF Min");
    ic->appendWidget(&lineEditIsoufTotal,NumInputMethod, LineEdit, this,
                     1,
                     1,
                     1,
                     tr("ISO UF Goal:"),"ISO UF Goal");
}

void SceneSetupIsoUF::lineEdit_start(const char *name, float *max, float *min)
{
    int iso_TimeHour ,iso_TimeMin;
    if(strcmp(name, "ISO UF Hour") == 0) {
        iso_TimeHour=residue_secCount /3600;
        iso_TimeMin=(residue_secCount-iso_TimeHour*3600)/60;
        *max  = (float)iso_TimeHour;
        if(((lineEditIsoufTimeMin.text().toInt())>iso_TimeMin)&&(*max>0)){
            *max -=1;
        }
        *min = 0 ;

    } else if(strcmp(name, "ISO UF Min") == 0) {
        iso_TimeHour=residue_secCount /3600;
        iso_TimeMin=(residue_secCount-iso_TimeHour*3600)/60;
        *max  = 59;
        if(lineEditIsoufTimeHour.text().toInt()==iso_TimeHour)*max=(float)iso_TimeMin;
        *min = 0 ;
    }else if(strcmp(name, "ISO UF Goal") == 0) {
        PacketUF packetUF;
        dataRead(PACKET_NAME_UF, &packetUF);
        if(packetUF.ufVolAim > packetUF.ufVolNow)*max  = packetUF.ufVolAim - G_UF_CUREFINISH;
        else{*max = 0;}

        *max +=ISO_finishvolume;
        *min=ISO_finishvolume;
        /*
        if(*max>ISO_finishvolume)
            *min=ISO_finishvolume;
        else
            *min  = 0;
            * */
    }
}
void SceneSetupIsoUF::lineEdit_data_set(const char *name, const char *data)
{
    name = name;
    data = data;
}
void SceneSetupIsoUF::Save(void)
{
    int totalVolumeTMP = int(lineEditIsoufTotal.text().toInt());
    int ISOTimeTMP = (int)(lineEditIsoufTimeHour.text().toInt());
    int timemin=(int)(lineEditIsoufTimeMin.text().toInt());
    ISOTimeTMP=ISOTimeTMP*3600+timemin*60;
   /*
    qDebug()<<totalVolumeTMP;
    if(totalVolumeTMP>G_UF_CURERESIDUE || ISOTimeTMP > residue_secCount){
   //     qDebug()<<"ISO setting error";
        showYesDialog(DE_TR_PROMPT,DE_TR_TREATMENTMODELNOTCHANGED);
        return;
    }
    */

    if(totalVolumeTMP>G_UF_CURERESIDUE)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_UFVOLUMECANNOTEXCEED);
        return;
    }

    if(totalVolumeTMP<=ISO_finishvolume)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_UFVOLUMECANNOTLESS);
        return;
    }


    if(ISOTimeTMP > residue_secCount)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_THETIMECANNOTEXCEED);
        return;
    }

    /*
    if(ISOTimeTMP<=ISO_residuesec)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_THETIMECANNOTLESS);
        return;
    }
    */

//    PacketWorkMode workModeTMP;
    dataRead(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);

    if(G_WORKMODE_VAL.treatmode.target != ISOUF_SNSP&&G_WORKMODE_VAL.treatmode.target != ISOUF_SNDP&&\
            G_WORKMODE_VAL.treatmode.target != ISOUF_DNDP)return;//没有改变单超模式，
/*
    PacketUfValue packetUfValue;
    dataRead(PACKET_MASTERCTRL_UFVALUE,&packetUfValue);
*/
    if(G_WORKMODE_VAL.treatmode.target==G_WORKMODE_VAL.treatmode.current)//ISO已经开始，修改参数#/
    {
        ISO_totalsec=ISOTimeTMP;
//        ISO_residuesec=residue_secCount;
        ISO_totalvolume=totalVolumeTMP;
        G_UF_FINISHVOL_FLAG=false;
        G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
        G_UF_ENDTIME=G_SYS_CURTIME+ISO_totalsec;  //超滤的总时间 单位秒#/
        //剩余量/剩余时间=设置超滤速度
        G_UF_SETRATE=((ISO_totalvolume-ISO_finishvolume)/(ISO_totalsec-ISO_residuesec-residue_secCount))*3600.0;//修改ISO参数时#/

        dataNodeUpdate(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
        PageShow("Main");
//        ISO_finishvolume=0;
//        UFPump_Now_volume=packetUfValue.N3_UF_amount;
//        showYesDialog(DE_TR_PROMPT,DE_TR_THEPARAMETERSAFRESH);
    }
    else   //开始一次新的ISO#/
    {
//        msgBox->setText(DE_TR_PROMPT);
        msgBox->setInformativeText(DE_TR_WHETHERTOSTARTISO);
        int ret=msgBox->exec();
        if(ret==QMessageBox::Yes)
         {
            ISO_totalsec=ISOTimeTMP;
            ISO_residuesec=residue_secCount; //ISO 计时开始点#/
            ISO_totalvolume=totalVolumeTMP;
            ISO_finishvolume=0;
            UFPump_Now_volume=G_UF_CUREFINISH;//ISO开始时，开始计算的超滤量#/
            G_UF_FINISHVOL_FLAG=false;
            G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
            G_UF_ENDTIME=G_SYS_CURTIME+ISO_totalsec;  //超滤的总时间 单位秒#/
            G_UF_SETRATE=(ISO_totalvolume/ISO_totalsec)*3600.0; //开启ISO时#/
            //改变超滤泵的速度#/
            CHANGED_UF_SPEED(true,G_UF_SETRATE);
            //工作模式切换时，曲线治疗停止#/
            WorkState  stateTMP=GetWorkStateManage();
            if((G_NA_PROFILE_FLAG==true||G_UF_PROFILE_FLAG==true)&&(stateTMP.tipState.current==TREAT_RUNNING))
            {
                G_NA_PROFILE_FLAG=false;
                G_NA_PROFILE_NUM=0;
                G_NA_SELECTCURVE_NUM=-1;

                G_UF_PROFILE_FLAG=false;
                G_UF_PROFILE_NUM=0;
                G_UF_SELECTCURVE_NUM=-1;

                showYesDialog(DE_TR_PROMPT,DE_TR_WORKMODECHANGEDPROFILECANCEL);
            }
            G_NA_PROFILE_FLAG=false;     //NA曲线工作模式切换时，曲线治疗停止#/
            G_UF_PROFILE_FLAG=false;     //UF曲线工作模式切换时，曲线治疗停止#/

            G_WORKMODE_VAL.treatmode.before = G_WORKMODE_VAL.treatmode.current ;
            G_WORKMODE_VAL.treatmode.current = G_WORKMODE_VAL.treatmode.target ;
            G_WORKMODE_VAL.fluidmode.current = G_WORKMODE_VAL.fluidmode.target ;
            G_WORKMODE_VAL.flag=setok_state;
            dataNodeUpdate(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
            PageShow("Main");
          }
        else if(ret==QMessageBox::No)
         {
            G_WORKMODE_VAL.treatmode.target=G_WORKMODE_VAL.treatmode.current;
            G_WORKMODE_VAL.fluidmode.target=G_WORKMODE_VAL.fluidmode.current;
            G_WORKMODE_VAL.flag=runing_state;
            dataNodeUpdate(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
            PageShow("Setup Treatment time");
            PageShow("SetupWorkmode");
         }
    }
}

void SceneSetupIsoUF::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupIsoUF-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_MASTERCTRL_UFVALUE) {
//        PacketWorkMode workModeTMP;

        if(G_UF_PROFILE_FLAG==true)  //UF曲线运行时，由曲线处理目标量到的问题#/
        {
            labelActualAmountIsoUfValue.setText(QString::number(ISO_finishvolume));
            return;
        }

        dataRead(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);

        if(G_WORKMODE_VAL.treatmode.current == ISOUF_SNSP||G_WORKMODE_VAL.treatmode.current == ISOUF_SNDP||\
                G_WORKMODE_VAL.treatmode.current == ISOUF_DNDP)
        {
            if(ISO_finishvolume>=ISO_totalvolume)
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

                    dataNodeUpdate(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
#if HJX_COMMUNICATION_SWITCH
                    emit signalSDO(DE_CMD_Treat_start);
#else
                    MasterCtrol_CMD_Treat_start(G_WORKMODE_VAL.treatmode.current);
#endif
                    showYesDialog(DE_TR_PROMPT,str);

                    G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
                    G_UF_ENDTIME=G_SYS_CURTIME+residue_secCount;  //超滤的总时间 单位秒#/
                    ISO_finishvolume=ISO_totalvolume;
                    G_UF_SETRATE=((G_UF_CURETARGET-G_UF_CUREFINISH)/residue_secCount)*3600.0; //ISO运行结束时#/
                }
            }
            labelActualAmountIsoUfValue.setText(QString::number(ISO_finishvolume));
        }
    }
    else if(name==PACKET_NAME_WORKMODE){
       PacketWorkMode workModeData = *(PacketWorkMode *)dataPtr;
       if((workModeData.treatmode.current == ISOUF_SNSP||workModeData.treatmode.current == ISOUF_SNDP||\
               workModeData.treatmode.current == ISOUF_DNDP)&&workModeData.flag==setok_state&&G_UF_PROFILE_FLAG==true)
       {
           lineEditIsoufTotal.setText(QString::number(ISO_totalvolume));
           lineEditIsoufTimeHour.setText(QString::number(ISO_totalsec/3600));
           lineEditIsoufTimeMin.setText(QString::number((ISO_totalsec%3600)/60));
           labelActualAmountIsoUfValue.setText(QString::number(ISO_finishvolume));
       }
    }
    else if(name==PACKET_REBIRTH_HANDLE)
    {
        ISO_totalsec=G_P_REBIRTH->ISO_totalsec;     //单超总时间#/
        ISO_residuesec=G_P_REBIRTH->ISO_residuesec;   //单超剩余时间#/
        ISO_totalvolume=G_P_REBIRTH->ISO_totalvolume;  //单超总目标量#/
        ISO_finishvolume=G_P_REBIRTH->ISO_finishvolume; //单超已完成量#/
        UFPump_Now_volume=G_P_REBIRTH->UFPump_Now_volume; //开始ISO时，已超滤量#/

        lineEditIsoufTotal.setText(QString::number(ISO_totalvolume));
        lineEditIsoufTimeHour.setText(QString::number(ISO_totalsec/3600));
        lineEditIsoufTimeMin.setText(QString::number((ISO_totalsec%3600)/60));
        labelActualAmountIsoUfValue.setText(QString::number(ISO_finishvolume));
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupIsoUF-ActionHandle-out");
#endif
}

void SceneSetupIsoUF::CustomSetText()
{
    labelIsoufTimeHour.setText(tr("ISO UF Time:"));
    //labelIsoufTimeMin.setText(tr("ISO UF Min:"));
    labelIsoufTotal.setText(tr("ISO UF Goal:"));
    labelActualAmountIsoUf.setText(tr("ISO UF Volume:"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
}

void SceneSetupIsoUF::setCusVisible(bool vis, char* para)
{
#if SYSTEM_DEBUG_FLAG
//    Debug_record("setup_isouf-setCusVisible-in");
    Debug_insert("setup_isouf-setCusVisible-in");
#endif

    para = para;
/*
    char *tp=NULL;
    if(vis==true)
    {
       strcpy(tp,"hjx---test!");
    }
*/

    labelIsoufTimeHour.setVisible(vis);
    //labelIsoufTimeMin.setVisible(false);
    lineEditIsoufTimeHour.setVisible(vis);
    labelIsoufTimeHourUnit.setVisible(vis);
    lineEditIsoufTimeMin.setVisible(vis);
    labelIsoufTimeMinUnit.setVisible(vis);

    labelIsoufTotal.setVisible(vis);
    lineEditIsoufTotal.setVisible(vis);
    labelIsoufTotalUnit.setVisible(vis);

    labelActualAmountIsoUf.setVisible(vis);
    labelActualAmountIsoUfValue.setVisible(vis);
    labelActualAmountIsoUfunit.setVisible(vis);

    int hour=0,min=0;
    if(vis==true)
    {
        hour=(unsigned int)(ISO_totalsec/3600);
        min=(unsigned int)((ISO_totalsec-hour*3600)/60);
        lineEditIsoufTimeHour.setText(QString::number(hour));
        lineEditIsoufTimeMin.setText(QString::number(min));
        lineEditIsoufTotal.setText(QString::number(ISO_totalvolume));
        labelActualAmountIsoUfValue.setText(QString::number(ISO_finishvolume));
    }

    buttonSave.setVisible(vis);
    buttonCancel.setVisible(false);

#if SYSTEM_DEBUG_FLAG
    Debug_insert("setup_isouf-setCusVisible-out");
#endif
}
