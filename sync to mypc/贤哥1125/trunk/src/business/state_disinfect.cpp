#include "state_disinfect.h"
#include "deal.h"
#include "interfix_os.h"
#include <stdlib.h>
#include <unistd.h>
#include "global_var.h"
#include "config_ui.h"
#include "common_func.h"
#include "widget_time.h"
#include "common_tr_string.h"

state_disinfect::state_disinfect(char *name):
    StateUnit(name)
{

}

void state_disinfect::ActionEnter(void)
{
    Init();
    qDebug()<<"workstate.current"<<workstate.current;

    while(
          GetWorkStateManage().subState.current == CLEANING_RUN
          //||
//          GetWorkStateManage().subState.current == hot_rinse_run  ||
//          GetWorkStateManage().subState.current== chemical_Disinfect_run  ||
//          GetWorkStateManage().subState.current == hot_chemical_Disinfect_run
          //workstate.tip==Disinfect_run
          )
    {

        //if(secCount<(60*1) )
        {
         interfix_sleep(1);//睡眠1S#/
         }

        //冷冲洗
        if(GetWorkStateManage().bloodState.current ==COOL_RINSE)   //--------冷冲洗
        {
            if( (secCount >= (Disinfect.loop_Time) *60 ))
            {
               //workstate.current=CLEANING_STOP;
               cleanState=GetWorkStateManage();
               cleanState.subState.current=CLEANING_FINISHED;
               SetWorkStateManage(&cleanState);
               //workstate.tip=Disinfect_finish;
               //dataNodeUpdate(PACKET_NAME_WORKSTATE,&workstate);//
               break;
            }
        }
        //热冲洗
       // else if( workstate.current == HOT_RINSE)//hot_rinse_run
       else if( cleanState.bloodState.current == HOT_RINSE)  //------------热冲洗
        {
            if(( secCount >= (Disinfect.preFlush_Time) *60 || iSHeatTmpok==true) && Disinfect.step==hotrinse1 && iSPreCondok==false)
            {
             MasterCtrol_CMD_Disinfect_start(hotrinse2);//热冲洗阶段2
             Disinfect.step=hotrinse2;
             cleanState=GetWorkStateManage();
             cleanState.waterState.current=HOTRINSE2;
             SetWorkStateManage(&cleanState);
             interfix_sleep(1);
             dataRead(Disinfect_Work_Model,&model);
             interfix_sleep(1);
             dataNodeUpdate(Disinfect_Work_Model,&model);//更新加热腔目标温度
             }

              if(secCount >= (Disinfect.loop_Time) *60&& Disinfect.step==hotrinse2)
            {
             MasterCtrol_CMD_Disinfect_start(hotrinse3);//热冲洗阶段3
             Disinfect.step=hotrinse3;
             cleanState=GetWorkStateManage();
             cleanState.waterState.current=HOTRINSE3;
             SetWorkStateManage(&cleanState);
             interfix_sleep(1);
             dataRead(Disinfect_Work_Model,&model);
             interfix_sleep(1);
             dataNodeUpdate(Disinfect_Work_Model,&model);//更新加热腔目标温度
             }

            if( (secCount >= (Disinfect.loop_Time+Disinfect.backFlush_Time) *60 ))
            {
               //workstate.current=Disinfect_stop;
               //workstate.tip=Disinfect_finish;
               cleanState=GetWorkStateManage();
               cleanState.subState.current=CLEANING_FINISHED;
               SetWorkStateManage(&cleanState);
               //dataNodeUpdate(PACKET_NAME_WORKSTATE,&workstate);//
               break;
            }
        }
        //化学消毒
        //else if(workstate.current ==chemical_Disinfect_run || workstate.current == hot_chemical_Disinfect_run)
        else   //----------------------化学消毒
        {
            cleanState=GetWorkStateManage();
            if(cleanState.bloodState.current ==CHEMICAL_CITRICACID
                    || cleanState.bloodState.current == CHEMICAL_PERACETIC
                    || cleanState.bloodState.current == CHEMICAL3
                    || cleanState.bloodState.current == HOT_CHEMICAL)
        {
          if((secCount >= (Disinfect.preFlush_Time) *60 && Disinfect.step==preflush)||(Disinfect.step==preflush && iSPreCondok==true))
   //         if(Disinfect.step==preflush&&iSPreCondok==true)
            {
             MasterCtrol_CMD_Disinfect_start(suck);//吸取消毒液
             Disinfect.step=suck;
             cleanState=GetWorkStateManage();
             cleanState.waterState.current=SUCK;
             SetWorkStateManage(&cleanState);
             interfix_sleep(1);
             dataRead(Disinfect_Work_Model,&model);
             dataNodeUpdate(Disinfect_Work_Model,&model);//更新目标温度
             }

            if(suck_state ==0x04 && Disinfect.step==suck && iSCondok==true)//吸液完成
            {
             MasterCtrol_CMD_Disinfect_start(hold);//启动滞留
             Disinfect.step=hold;
             cleanState=GetWorkStateManage();
             cleanState.waterState.current=HOLD;
             SetWorkStateManage(&cleanState);
             interfix_sleep(1);
             dataRead(Disinfect_Work_Model,&model);
             dataNodeUpdate(Disinfect_Work_Model,&model);//更新目标温度
             }

            if(secCount >= (Disinfect.preFlush_Time+Disinfect.hold_Time) *60
                    && Disinfect.step==hold
                    //&& iSHeatTmpok==true  //先进入循环状态，再判断温度
                    )
            {
             MasterCtrol_CMD_Disinfect_start(loop);//启动循环消毒
             Disinfect.step=loop;
             cleanState=GetWorkStateManage();
             cleanState.waterState.current=LOOP;
             SetWorkStateManage(&cleanState);
             interfix_sleep(1);
             dataRead(Disinfect_Work_Model,&model);
             interfix_sleep(1);
             dataNodeUpdate(Disinfect_Work_Model,&model);//更新流速
             }
            cleanState=GetWorkStateManage();
            if(secCount >= (Disinfect.preFlush_Time+Disinfect.hold_Time+Disinfect.loop_Time) *60 && Disinfect.step==loop)
    //        if(secCount >= (Disinfect.preFlush_Time+Disinfect.hold_Time+Disinfect.loop_Time) *60 && cleanState.waterState.current==LOOP)
            {
             MasterCtrol_CMD_Disinfect_start(backflush);//启动后冲洗
             Disinfect.step=backflush;
             cleanState=GetWorkStateManage();
             cleanState.waterState.current=AFTERRINSE;
             SetWorkStateManage(&cleanState);
             interfix_sleep(1);
             dataRead(Disinfect_Work_Model,&model);
             interfix_sleep(1);
             dataNodeUpdate(Disinfect_Work_Model,&model);//更新目标温度
             }

            if( (secCount >= (Disinfect.preFlush_Time+Disinfect.hold_Time+Disinfect.loop_Time+Disinfect.backFlush_Time) *60 ))
            {
               //workstate.current=Disinfect_stop;
               //workstate.tip=Disinfect_finish;
               cleanState=GetWorkStateManage();
               cleanState.subState.current=CLEANING_FINISHED;
               SetWorkStateManage(&cleanState);
               //dataNodeUpdate(PACKET_NAME_WORKSTATE,&workstate);//
               break;
            }
         }//
         else if(cleanState.bloodState.current ==HOT)
            {
                cleanState=GetWorkStateManage();
                if(secCount >= (Disinfect.preFlush_Time) *60 && Disinfect.step==preflush )//&& iSCondok==true
                {
                 MasterCtrol_CMD_Disinfect_start(loop);
                 Disinfect.step=loop;
                 cleanState=GetWorkStateManage();
                 cleanState.waterState.current=LOOP;
                 SetWorkStateManage(&cleanState);
                 interfix_sleep(1);
                 dataRead(Disinfect_Work_Model,&model);
                 dataNodeUpdate(Disinfect_Work_Model,&model);//更新目标温度
                 }
                //
                if(secCount >= (Disinfect.preFlush_Time+Disinfect.loop_Time) *60 && cleanState.waterState.current==LOOP)
                {
                 MasterCtrol_CMD_Disinfect_start(backflush);//启动后冲洗
                 Disinfect.step=backflush;
                 cleanState=GetWorkStateManage();
                 cleanState.waterState.current=AFTERRINSE;
                 SetWorkStateManage(&cleanState);
                 interfix_sleep(1);
                 dataRead(Disinfect_Work_Model,&model);
                 interfix_sleep(1);
                 dataNodeUpdate(Disinfect_Work_Model,&model);//更新目标温度
                 }
                //
                if( (secCount >= (Disinfect.preFlush_Time+Disinfect.loop_Time+Disinfect.backFlush_Time) *60 ))
                {
                   //workstate.current=Disinfect_stop;
                   //workstate.tip=Disinfect_finish;
                   cleanState=GetWorkStateManage();
                   cleanState.subState.current=CLEANING_FINISHED;
                   SetWorkStateManage(&cleanState);
                   //dataNodeUpdate(PACKET_NAME_WORKSTATE,&workstate);//
                   break;
                }
            }
        }//else

        //判断是否达到冲洗目标电导值，即冲洗将电导降低#/
        if(iSPreCondok==false&& GetWorkStateManage().bloodState.current !=COOL_RINSE&& GetWorkStateManage().bloodState.current !=HOT_RINSE )
        {
            if(packetMix.N3_Mix_cond<=disinfectCond.Mix_cond  && packetFluidB.N3_B_cond<=disinfectCond.B_cond && packetOutlet.N3_out_cond<=disinfectCond.out_cond)
            {
                iSPreCondok=true;
                secpass=secCount;//保存时间
                timePoint = QTime::currentTime();
            }

        }

        if(MasterStateOk==false&&(errorCount>=20))
        {
            showYesDialog(DE_TR_PROMPT,DE_TR_THESTATUSOFMASTERABNORMAL);
            cleanState=GetWorkStateManage();
            cleanState.subState.current=CLEANING_STOP;
            SetWorkStateManage(&cleanState);
            break;
        }

//    qDebug()<<"Now cleaning!";
     }

    interfix_sleep(1);

    secCount=0;
    timer->stopTimer(this);

    StateSwitch(NULL);
 }

void state_disinfect::ActionExit(void)
{
     //dataNodeDelObject(PACKET_NAME_WORKSTATE, this);
     dataNodeDelObject(PACKET_STATUS_NOTE, this);
//     dataNodeDelObject(Disinfect_Work_Model, this);
     dataNodeDelObject(PACKET_STATUS_MASTALARM, this);
     dataNodeDelObject(PACKET_MASTERCTRL_HEAT, this);
//     dataNodeDelObject(PACKET_STATUS_NOTE, this);
     dataNodeDelObject(PACKET_MASTERCTRL_MIX, this);
     dataNodeDelObject(PACKET_MASTERCTRL_OUTLET, this);
     dataNodeDelObject(PACKET_MASTERCTRL_FLUIDB, this);

 }

void state_disinfect::OtherHandle(void)
{
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
}

void state_disinfect::Init()
{
    if(G_RebirthDisinfect_flag==false)
    {
        secCount=0;
        secpass=0;
        timePoint = QTime::currentTime();
        MasterStateOk=false;   //主控板状态正确标志#/
        errorCount=0;      //发生错误计数器#/
        Disinfect_secCount=0;
    }
    else
    {
        MasterStateOk=true;   //主控板状态正确标志#/
    }

    suck_state=0x0;
    iSHeatTmpok=false;
    iSPreCondok=false;//
    iSCondok=true;
    iSPipeok=true;

    pHeatTmp.dataType=0;
    pHeatTmp.N3_heat_Power=0;
    pHeatTmp.N3_heat_Target=0;
    pHeatTmp.N3_heat_temp=0;

    timer->createTimer(this, 1000);

    //dataNodeAddObject(PACKET_NAME_WORKSTATE, this);
    dataNodeAddObject(PACKET_STATUS_NOTE, this);
//    dataNodeAddObject(Disinfect_Work_Model, this);
    dataNodeAddObject(PACKET_STATUS_MASTALARM, this);
    dataNodeAddObject(PACKET_MASTERCTRL_HEAT, this);
//    dataNodeAddObject(PACKET_STATUS_NOTE, this);//A、B管放在哪

    dataNodeAddObject(PACKET_MASTERCTRL_MIX, this);
    dataNodeAddObject(PACKET_MASTERCTRL_OUTLET, this);
    dataNodeAddObject(PACKET_MASTERCTRL_FLUIDB, this);
}


void state_disinfect::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("state_disinfect-ActionHandle-in");
#endif

    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

//    if (name==PACKET_NAME_WORKSTATE){
//        workstate  = *((WorkState*) dataPtr);
//    }

    if(name==PACKET_STATUS_NOTE){   //获取主控板的工作状态#/
        PacketStatusNote *p = (PacketStatusNote *) dataPtr;
        suck_state=(p->note.note4)&0x04;   //消毒液吸入完成#/
        //if(suck_state==0x04 && Disinfect.step==suck)

        if(suck_state==0x04 && cleanState.waterState.current==SUCK)
        {
            secpass=secCount;//保存时间
            timePoint = QTime::currentTime();
         }

//        if(iSPreCondok==false)
//        {
//            if(((p->note.note4)&0x02) == 0x02)
//            {
//             iSPreCondok=true;
//             secpass=secCount;//保存时间
//             timePoint = QTime::currentTime();
//             }
//        }
    }
    else if (name==PACKET_STATUS_MASTALARM){
        PacketStatusAlarm *p = (PacketStatusAlarm *) dataPtr;
        N3_errorstate=((unsigned char *)((char *)(&(p->alarm.errorstate1))));
    /*
        if(cleanState.waterState.current==SUCK && suck_state==0x04 && iSCondok==false)
        {                                //吸完后判断消毒液是否合格
             if((N3_errorstate[0]&0x18) == 0)//无电导上下限错误#/
             {
              iSCondok=true;
              secpass=secCount;
              timePoint = QTime::currentTime();
              }
        }
        */
    }
    else if (name==PACKET_MASTERCTRL_HEAT){  //加热腔温度#/
        pHeatTmp = *(PacketHeat *) dataPtr;
    }
    else if (name==PACKET_MASTERCTRL_MIX){
        packetMix = *(PacketMix *) dataPtr;
    }
    else if (name==PACKET_MASTERCTRL_OUTLET){
        packetOutlet = *(PacketOutlet *) dataPtr;
    }
    else if (name==PACKET_MASTERCTRL_FLUIDB){
        packetFluidB = *(PacketFluidB *) dataPtr;
    }
    else if (name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        Disinfect_secCount=G_P_REBIRTH->Disinfect_secCount;           //消毒剩余时间#/
        iSPipeok=G_P_REBIRTH->iSPipeok;  //消毒时管路状态#/
        if(workstate.tipState.current==CLEANING&&workstate.subState.current==CLEANING_RUN)
        {
            timePoint = QTime::currentTime().addSecs(0-Disinfect_secCount);//恢复开始点#/
            secCount=Disinfect_secCount;
        }
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("state_disinfect-ActionHandle-out");
#endif
}

void state_disinfect::timer_handler(const int timeOut)
{
#if DEBUG_TIMER_HANDLE
    Debug_insert("state_disinfect-timer-in");
#endif
    Q_UNUSED(timeOut);
    cleanState=GetWorkStateManage();

    PacketStatusNote masterstate;

    if(MasterStateOk==false)
    {
        dataRead(PACKET_STATUS_NOTE,&masterstate);//获取主控板的工作状态#/
        if(masterstate.status.header_status==0x04)
        {
            MasterStateOk=true;
            errorCount=0;
            timePoint = QTime::currentTime();//重新定开始点
        }
        else
        {
            errorCount++;
        }

        if(errorCount%5==0)
        {
            us_sleep(5000);
            MasterCtrol_CMD_Disinfect_start(preflush);
            us_sleep(5000);
        }

        return;
    }

    if(cleanState.bloodState.current ==HOT_RINSE && iSHeatTmpok==false && cleanState.waterState.current==HOTRINSE1 ) //热冲洗和冷冲洗判断是否达到目标温度#/
    {
        if(pHeatTmp.N3_heat_temp >= Disinfect.runTemp || Disinfect.targetTemp==0)//达到指定的温度
          {
            iSHeatTmpok=true;
            secpass=secCount;
            timePoint = QTime::currentTime();//
            }
     }
    //else if ((workstate.current !=cool_rinse_run) && (workstate.current !=hot_rinse_run)){
    else if ((cleanState.bloodState.current !=COOL_RINSE) && (cleanState.bloodState.current !=HOT_RINSE)) //其他消毒模式判断是否达到目标温度#/
    {

      if(secCount >= (Disinfect.preFlush_Time+Disinfect.hold_Time) *60 && iSHeatTmpok==false)
           {
              if(pHeatTmp.N3_heat_temp >= Disinfect.runTemp)//达到指定的温度  Disinfect.targetTemp || Disinfect.targetTemp==0
                {
                  iSHeatTmpok=true;
                  secpass=secCount;
                  timePoint = QTime::currentTime();//重新定开始点
                  //qDebug()<<"N3_heat_temp:"<<pHeatTmp.N3_heat_temp;
                  }
           }
    }
    //=====================

    //冷冲洗模式的计时#/
    //if(iSPreCondok==true && (workstate.current ==cool_rinse_run )
    if((cleanState.bloodState.current ==COOL_RINSE )&& iSPipeok==true)
        secCount=timePoint.elapsed()/1000+secpass;
    //热冲洗模式的计时#/
    //else if( workstate.current ==hot_rinse_run && iSPipeok==true)
    else if( cleanState.bloodState.current ==HOT_RINSE && iSPipeok==true)
       {
        //if(iSPreCondok==true &&iSHeatTmpok==true &&Disinfect.step==hotrinse1)
        if(iSHeatTmpok==true && cleanState.waterState.current==HOTRINSE1)
            secCount=timePoint.elapsed()/1000+secpass;
        //else if( (Disinfect.step==hotrinse2||Disinfect.step==hotrinse3))
        else if( (cleanState.waterState.current==HOTRINSE2||cleanState.waterState.current==HOTRINSE3))
            secCount=timePoint.elapsed()/1000+secpass;
    }
     //除了冷冲洗和热冲洗外的消毒模式的计时#/
    //else if(workstate.current !=cool_rinse_run && workstate.current !=hot_rinse_run)
    else if ((cleanState.bloodState.current !=COOL_RINSE) && (cleanState.bloodState.current !=HOT_RINSE))
    {
        //  if(iSPreCondok==true && (cleanState.waterState.current==PRERINSE || cleanState.waterState.current==HOLD))
            //预冲和滞留阶段#/
            if(iSPreCondok==true&&(Disinfect.step==preflush||Disinfect.step==hold))
             {
                /*
              if(secCount <= (Disinfect.preFlush_Time) *60)
                secCount=timePoint.elapsed()/1000+secpass;
              else if(secCount > (Disinfect.preFlush_Time) *60 && suck_state ==0x04 )
                secCount=timePoint.elapsed()/1000+secpass;
                */
                secCount=timePoint.elapsed()/1000+secpass;
              }
            else{//消毒阶段#/
                //if(workstate.current ==chemical_Disinfect_run)
                cleanState=GetWorkStateManage();
                if(cleanState.bloodState.current ==CHEMICAL_CITRICACID
                    || cleanState.bloodState.current == CHEMICAL_PERACETIC
                    || cleanState.bloodState.current == CHEMICAL3
                    )
                {
                 if(iSCondok==true && suck_state ==0x04)
                   secCount=timePoint.elapsed()/1000+secpass;
                }
                //else if(workstate.current == hot_chemical_Disinfect_run)
                else if( cleanState.bloodState.current == HOT_CHEMICAL)//热化学消毒#/
                {
                    if(iSHeatTmpok==true && iSCondok==true && suck_state ==0x04)
                      secCount=timePoint.elapsed()/1000+secpass;
                 }
                else if( cleanState.bloodState.current == HOT)//纯热消毒#/
                {
                    if(iSHeatTmpok==true && (cleanState.waterState.current==LOOP || cleanState.waterState.current==AFTERRINSE))
                      secCount=timePoint.elapsed()/1000+secpass;
                 }
           }
     }

    Disinfect_secCount=secCount;  //将计时值传递到消毒页面#/
    G_RebirthDisinfect_flag=false;  //消毒重生标志#/
#if DEBUG_TIMER_HANDLE
    Debug_insert("state_disinfect-timer-out");
#endif
}
