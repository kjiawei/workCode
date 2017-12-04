#include "config_ui.h"
#include "global_var.h"
#include "scene_setup_layout.h"
#include "scene_setup_cure_time.h"
#include <QtGui>
#include "common_tr_string.h"

inline void SceneSetupCureTime::setUi()
{
    SCENE_SETUP_CREATE_UNIT(testlabel, 7);
    testlabel.setGeometry(QRect(340, 550, 340, 80));

  //  SCENE_SETUP_CREATE_TABLE(labelTime, 2);
    labelTime.setGeometry(QRect(SCENE_SETUP_COL_LABEL-50,
                                    SCENE_SETUP_ROW_2,
                                    350,
                                    SCENE_SETUP_WIDGET_HIGH));
    labelTime.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    labelTime.setFont(QFont(QString(CONFIG_FONT_STYLE), CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelTime.setStyleSheet(CONFIG_COLOR_LABEL);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditTime, 2);

    //最大范围值#/
    SCENE_SETUP_CREATE_TABLE(labelmaxtext, 3);
    SCENE_SETUP_CREATE_VALUE(labelmax, 3);
    SCENE_SETUP_CREATE_UNIT(labelmaxunit, 3);

    SCENE_SETUP_CREATE_TABLE(labelmintext, 4);
    SCENE_SETUP_CREATE_VALUE(labelmin, 4);
    SCENE_SETUP_CREATE_UNIT(labelminunit, 4);

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    CustomSetText();
}

SceneSetupCureTime::SceneSetupCureTime(QWidget *parent) :
    QObject(parent),
    testlabel(parent),
    labelTime(parent),
    lineEditTime(parent),
    labelmintext(parent),
    labelmin(parent),
    labelminunit(parent),
    labelmaxtext(parent),
    labelmax(parent),
    labelmaxunit(parent),
    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();

    connect(&buttonSave,SIGNAL(clicked()),this,SLOT(save()));

    PageAppendPiece("Setup Treatment time", this);

    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(DE_STR_SAVECUREPARAM,this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/

    isKeyInit = false;
    min_curetime=0;
    max_curetime=0;
    setParentName("SceneSetupCureTime");

    lineEditTime.setObjectName("Treatment time");
}

void SceneSetupCureTime::lineEdit_close(const char *name)
{
    name = name;
}

void SceneSetupCureTime::lineEdit_data_set(const char *name, const char *data)
{
    float max,min;
    name = name;
    data = data;

    int li_val=0;
    //获取小数，分钟部分，判断是否超过60#/
    li_val=(lineEditTime.text().toFloat()+0.001)*100;
    li_val=li_val%100;
    if(li_val>=60)
    {
        lineEditTime.setText(timestr);
        return;
    }

    lineEdit_start(name, &max, &min);

    if(lineEditTime.text().toFloat()<min_curetime)//设置值比最小值还小时，退出
        return;
    if(lineEditTime.text().toFloat()>max_curetime)
        return;

    save();

    /*
    struct PacketUF packetTmp; //根据治疗时间，重新计算超滤速度#/
    dataRead(PACKET_NAME_UF, &packetTmp);
    if(residue_secCount>0)packetTmp.ufRate=G_UF_CURERESIDUE*3600/residue_secCount;
    dataNodeUpdate(PACKET_NAME_UF, &packetTmp);
    */
}

/*
void SceneSetupCureTime::lineEdit_start(const char *name, float *max, float *min)
{
    float uf_timeHour = lineEditHour.text().toFloat();
    float uf_timeMin = lineEditMin.text().toFloat();
    float temp=900,temp1;
    int n;
    struct PacketUF packetTmp;
    dataRead(PACKET_NAME_UF, &packetTmp);
    G_UF_CURERESIDUE =  packetTmp.ufVolAim-G_UF_CUREFINISH;
    if(G_UF_CURERESIDUE<0)G_UF_CURERESIDUE = 0;

    if(strcmp(name, "Hour") == 0) {
        if(params.Params_UF.UF_Rate.attribute.max_value>0)temp = G_UF_CURERESIDUE/params.Params_UF.UF_Rate.attribute.max_value;
        *min=(unsigned short )temp;
        n = (int)temp;
        temp = (temp-n)*60;
        if(temp>uf_timeMin)*min+=1;
        *max=99;
    }else if(strcmp(name, "Minute") == 0) {
        if(params.Params_UF.UF_Rate.attribute.max_value>0)temp = G_UF_CURERESIDUE/params.Params_UF.UF_Rate.attribute.max_value;
        temp1=(unsigned short )temp;//hour
        n = (int)temp;
        temp = (temp-n)*60;
        if(uf_timeHour>temp1)*min=0;
        else {
            *min=(int)temp;
            if(*min<59&&(temp-(int)temp>0))*min+=1;
        }

        *max=59;
    }
}
*/

void SceneSetupCureTime::lineEdit_start(const char *name, float *max, float *minx)
{
    name=name;
    float temp=90;
    int n;
    int hou=0,min=0;

    struct PacketUF packetTmp;
    dataRead(PACKET_NAME_UF, &packetTmp);
    G_UF_CURERESIDUE =  packetTmp.ufVolAim-G_UF_CUREFINISH;
    if(G_UF_CURERESIDUE<0)G_UF_CURERESIDUE = 0;

     //根据默认最大的超滤速度计算最小治疗时间，单位小时#/
      if(params.Params_UF.UF_Rate.attribute.max_value>0)
          temp = G_UF_CURERESIDUE/params.Params_UF.UF_Rate.attribute.max_value;
      else
          temp=0;
      /*
      n=temp*100;
      min_curetime=(float)n/100;//
      */
      n=temp*60;   //计算总分钟数#/
      hou=n/60;
      min=n%60;
      min_curetime=hou+((float)min)/100;
      *minx=min_curetime;

      if(params.Params_UF.UF_Rate.attribute.min_value>0)
          temp = G_UF_CURERESIDUE/params.Params_UF.UF_Rate.attribute.min_value;
      else
          temp=99.99;
    /*
      n=temp*100;
      max_curetime=(float)n/100;//只要小数点后两位#/
      */
      n=temp*60;   //计算总分钟数#/
      hou=n/60;
      min=n%60;
      max_curetime=hou+((float)min)/100;
      *max=max_curetime;
}
/*

void SceneSetupCureTime::lineEdit_init()
{
    ic->appendWidget(&lineEditHour, NumInputMethod, LineEdit, this,
                     params.Params_TreatTime.Hour.attribute.min_value,
                     params.Params_TreatTime.Hour.attribute.max_value,
                     params.Params_TreatTime.Hour.attribute.step_value,
                     tr("Hour:"),"Hour");

    ic->appendWidget(&lineEditMin, NumInputMethod, LineEdit, this,
                     params.Params_TreatTime.min.attribute.min_value,
                     params.Params_TreatTime.min.attribute.max_value,
                     params.Params_TreatTime.min.attribute.step_value,
                     tr("Minute:"),"Minute");
}
*/

void SceneSetupCureTime::lineEdit_init()
{
    float temp=90;
    int n;
    int hou=0,min=0;

    //根据默认最大的超滤速度计算最小治疗时间，单位小时#/
     if(params.Params_UF.UF_Rate.attribute.max_value>0)
         temp = G_UF_CURERESIDUE/params.Params_UF.UF_Rate.attribute.max_value;
     else
         temp=0;

     n=temp*60;   //计算总分钟数#/

     hou=n/60;
     min=n%60;
     min_curetime=hou+((float)min)/100;

     /*
     n=temp*100;
     min_curetime=(float)n/100;//只要小数点后两位#/
     */

     if(params.Params_UF.UF_Rate.attribute.min_value>0)
         temp = G_UF_CURERESIDUE/params.Params_UF.UF_Rate.attribute.min_value;
     else
         temp=99.99;

     /*
     n=temp*100;
     max_curetime=(float)n/100;//只要小数点后两位#/
     */

     n=temp*60;   //计算总分钟数#/

     hou=n/60;
     min=n%60;
     max_curetime=hou+((float)min)/100;

    ic->appendWidget(&lineEditTime, NumInputMethod, LineEdit, this,
                     min_curetime,
                     max_curetime,
                     params.Params_TreatTime.Hour.attribute.step_value,
                     tr("Time:"),"Time");
}

/*
void SceneSetupCureTime::save()
{
    usr_params.Params_TreatTime.Hour.attribute.default_value = lineEditHour.text().toUShort();
    usr_params.Params_TreatTime.min.attribute.default_value = lineEditMin.text().toUShort();
    total_secCount=usr_params.Params_TreatTime.Hour.attribute.default_value*3600+
                   usr_params.Params_TreatTime.min.attribute.default_value*60;
    residue_secCount=total_secCount;
    Timer_state=Timechange;
    dataNodeUpdate(TREAT_TIMER, &Timer_state);
}
*/

void SceneSetupCureTime::save()
{
    int li_val=0;
    int li_min=0;
    QString temstr;

    WorkState  stateTMP=GetWorkStateManage();

    li_val=(int)(lineEditTime.text().toFloat());  //整数部分#/
    li_min=(lineEditTime.text().toFloat()+0.001)*100;  //获取小数部分#/
    li_min=li_min%100;

    usr_params.Params_TreatTime.Hour.attribute.default_value = li_val;
    usr_params.Params_TreatTime.min.attribute.default_value=li_min;
//    usr_params.Params_TreatTime.min.attribute.default_value = (int)(lf_val*60);  //分钟#/
    total_secCount=usr_params.Params_TreatTime.Hour.attribute.default_value*3600+
                   usr_params.Params_TreatTime.min.attribute.default_value*60;
    residue_secCount=total_secCount;

    temstr=QString::number(li_val);
    temstr.append(" : ");
//    temstr.append(QString::number((int)(lf_val*60)));
    if(li_min>=10)
    {
        temstr.append(QString::number(li_min));
    }
    else
    {
        temstr.append("0");
        temstr.append(QString::number(li_min));
    }
    lineEditTime.setText(temstr);
    timestr=lineEditTime.text();

    /*
    //NA profile
    if(G_NA_PROFILE_FLAG==true)   //NA profile runing
    {
        //曲线应用处理#/
        for(li_val=0;li_val<10;li_val++)
        {
            if(A_NA_PROFILE[li_val].flag==false)
            {
                G_NA_PROFILE_NUM=li_val;//NA曲线从第几段开始#/
                break;
            }
        }
        G_NA_PROFILE_START=G_SYS_CURTIME;  //NA曲线开始时间#/
        G_NA_PROFILE_END=G_SYS_CURTIME+residue_secCount;    //NA曲线结束时间#/
    }

    //UF profile
    if(G_UF_PROFILE_FLAG==true)   //UF profile runing
    {
        //曲线应用处理#/
        for(li_val=0;li_val<10;li_val++)
        {
            if(A_UF_PROFILE[li_val].flag==false)
            {
                G_UF_PROFILE_NUM=li_val;//NA曲线从第几段开始#/
                break;
            }
        }
        G_UF_PROFILE_START=G_SYS_CURTIME;  //NA曲线开始时间#/
        G_UF_PROFILE_END=G_SYS_CURTIME+residue_secCount;    //NA曲线结束时间#/
    }
    */

    //修改治疗时间，已选择的曲线治疗取消#/
    if((G_NA_PROFILE_FLAG==true||G_UF_PROFILE_FLAG==true)&&(stateTMP.tipState.current==TREAT_RUNNING))
    {
        G_NA_PROFILE_FLAG=false;
        G_NA_PROFILE_NUM=0;
        G_NA_SELECTCURVE_NUM=-1;

        G_UF_PROFILE_FLAG=false;
        G_UF_PROFILE_NUM=0;
        G_UF_SELECTCURVE_NUM=-1;

        showYesDialog(DE_TR_PROMPT,DE_TR_CURECHANGEDPROFILECANCEL);
    }

    if(stateTMP.tipState.current==TREAT_RUNNING)
    {
        G_CURE_ENDTIME=G_CURE_STARTTIME+residue_secCount;    //治疗结束时间#/
    }

    Timer_state=Timechange;
    dataNodeUpdate(TREAT_TIMER, &Timer_state);

    if(residue_secCount<=heparinPreStop_sec)  //剩余治疗时间比肝素预停止时间小，肝素不起作用#/
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_RESIDUELESSHEPARINPRESTOP);
    }
}

void SceneSetupCureTime::ActionHandle(const void *dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupCureTime-ActionHandle-in");
#endif

    if(name==PACKET_MAINTAIN_PARAMS){

        params = *(Params*)dataPtr;
    //    lineEditHour.setText(QString::number(params.Params_TreatTime.Hour.attribute.default_value));
    //    lineEditMin.setText(QString::number(params.Params_TreatTime.min.attribute.default_value));
        QString temstr;
        temstr=QString::number(params.Params_TreatTime.Hour.attribute.default_value);
        temstr.append(" : ");
        if(params.Params_TreatTime.min.attribute.default_value>=10)
        {
            temstr.append(QString::number(params.Params_TreatTime.min.attribute.default_value));
        }
        else
        {
            temstr.append("0");
            temstr.append(QString::number(params.Params_TreatTime.min.attribute.default_value));
        }
        lineEditTime.setText(temstr);
        total_secCount=params.Params_TreatTime.Hour.attribute.default_value*3600+\
                params.Params_TreatTime.min.attribute.default_value*60;
        residue_secCount=total_secCount;

        timestr=lineEditTime.text();
    }
    else if(name==DE_STR_SAVECUREPARAM)
    {
       if(G_SAVECUREPARAM_FLAG==1)
       {

           int hour=0;
           float tmp=0;

           hour=residue_secCount/3600;

           tmp=(residue_secCount%3600)/60.0;

           tmp=hour+(tmp/100.0);
           lineEditTime.setText(QString::number(tmp,'f',2));

           this->save();
       }
    }
    else  if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        QString temstr;
        int min;

        total_secCount=G_P_REBIRTH->total_secCount;

        temstr=QString::number(total_secCount/3600);
        min=(total_secCount%3600)/60.0;

        temstr.append(" : ");
        if(min>=10)
        {
            temstr.append(QString::number(min));
        }
        else
        {
            temstr.append("0");
            temstr.append(QString::number(min));
        }
        lineEditTime.setText(temstr);
    }
}


void SceneSetupCureTime::CustomSetText()
{
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    labelTime.setText(tr("Treatment time:"));
    labelmintext.setText(tr("Min time"));
    labelminunit.setText(tr("/H:m"));
    labelmaxtext.setText(tr("Max time"));
    labelmaxunit.setText(tr("/H:m"));
}

void SceneSetupCureTime::setCusVisible(bool vis, char* para)
{
    para = para;
    if(vis && !isKeyInit) {
        isKeyInit = true;
        lineEdit_init();
    }

    if(vis==true)
    {
        float temp=90;
        int n;
        int hou=0,min=0;
        QString str;
        //根据默认最大的超滤速度计算最小治疗时间，单位小时#/
         if(params.Params_UF.UF_Rate.attribute.max_value>0)
             temp = G_UF_CURERESIDUE/params.Params_UF.UF_Rate.attribute.max_value;
         else
             temp=0;

         n=temp*60;   //计算总分钟数#/

         hou=n/60;
         min=n%60;
         min_curetime=hou+((float)min)/100;

         /*
         n=temp*100;
         min_curetime=(float)n/100;//只要小数点后两位#/
         */
         str=QString::number(hou);
         str.append(" : ");
         if(min>=10)
         {
             str.append(QString::number(min));
         }
         else
         {
             str.append("0");
             str.append(QString::number(min));
         }

         labelmin.setText(str);

         if(params.Params_UF.UF_Rate.attribute.min_value>0)
             temp = G_UF_CURERESIDUE/params.Params_UF.UF_Rate.attribute.min_value;
         else
             temp=99.99;

         n=temp*60;   //计算总分钟数#/

         hou=n/60;
         min=n%60;
         max_curetime=hou+((float)min)/100;

         /*
         n=temp*100;
         max_curetime=(float)n/100;//只要小数点后两位#/
         */
         str=QString::number(hou);
         str.append(" : ");
         str.append(QString::number(min));
         labelmax.setText(str);
    }

    labelTime.setVisible(vis);
    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);

#if EEDA_VERSION_OPEN
    testlabel.setVisible(vis);
    lineEditTime.setVisible(vis);
    labelmintext.setVisible(false);
    labelmin.setVisible(false);
    labelminunit.setVisible(false);
    labelmaxtext.setVisible(false);
    labelmax.setVisible(false);
    labelmaxunit.setVisible(false);

    /*
    WorkState stateTMP = GetWorkStateManage();
    testlabel.setText("main state: 0x"+QString::number(stateTMP.tipState.current,16)\
                         +"\nsub   state: 0x"+QString::number(stateTMP.subState.current,16));
    */

#else
    testlabel.setVisible(vis);
    lineEditTime.setVisible(vis);
    labelmintext.setVisible(vis);
    labelmin.setVisible(vis);
    labelminunit.setVisible(vis);
    labelmaxtext.setVisible(vis);
    labelmax.setVisible(vis);
    labelmaxunit.setVisible(vis);

    WorkState stateTMP = GetWorkStateManage();
    testlabel.setText("main state: 0x"+QString::number(stateTMP.tipState.current,16)\
                         +"\nsub   state: 0x"+QString::number(stateTMP.subState.current,16));
#endif
}
