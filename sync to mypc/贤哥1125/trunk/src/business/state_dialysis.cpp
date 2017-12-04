#include "state_dialysis.h"
#include "deal.h"
#include "interfix_os.h"
#include <stdlib.h>
#include <unistd.h>
#include "global_var.h"
#include "config_ui.h"
#include "common_func.h"

StateDialysis::StateDialysis(char *name) :
    StateUnit(name)
{
}


void StateDialysis::Init()
{
    apspan=250;
    vpspan=200;
    tmpspan=200;
    Adjlimit=true;
    secCount=0;//两分钟#/
    sample_count=0;
    memset(apBuff,0,sizeof(apBuff));
    memset(vpBuff,0,sizeof(vpBuff));
    memset(tmpBuff,0,sizeof(tmpBuff));
//    timer->createTimer(this, 1000);
    dataNodeAddObject(PACKET_NAME_WORKMODE, this);
    dataNodeAddObject(PACKET_NAME_AP, this);
    dataNodeAddObject(PACKET_NAME_VP, this);
    dataNodeAddObject(PACKET_NAME_TMP, this);
    dataNodeAddObject(PACKET_NAME_WORKSTATE, this);
    dataNodeAddObject(PACKET_NAME_ADJPRESSLIMIT, this);

}

void StateDialysis::ActionEnter(void)
{

    Init();

    while( workstate.current == HD_double_needle_run) {
       if( secCount<(60*2) && secCount>(60*1))
        {
        AdjPresslimit();//自动卡标#/
        interfix_sleep(2);
         }

       //timer->stopTimer(this);
       if(secCount>(60*2) && Adjlimit==true){

        int j,sum=0;
        for(j=0;j<sample_count;j++){
            sum +=apBuff[j];
          }
        target_apPiont=sum/sample_count;
        dataRead(PACKET_NAME_AP,&packetAP);
        packetAP.apUp=target_apPiont+apspan/2;
        packetAP.apL = target_apPiont-apspan/2;
        dataNodeUpdate(PACKET_NAME_AP, &packetAP);

        sum=0;
        for(j=0;j<sample_count;j++){
            sum +=vpBuff[j];
          }
        target_vpPiont=sum/sample_count;
        dataRead(PACKET_NAME_VP,&packetVP);
        packetVP.vpUp=target_vpPiont+vpspan/2;
        packetVP.vpL = target_vpPiont-vpspan/2;
        dataNodeUpdate(PACKET_NAME_VP, &packetVP);

        sum=0;
        for(j=0;j<sample_count;j++){
            sum +=tmpBuff[j];
          }
        target_tmpPiont=sum/sample_count;
        dataRead(PACKET_NAME_TMP,&packetTMP);
        packetTMP.tmpUp=target_tmpPiont+tmpspan/2;
        packetTMP.tmpL = target_tmpPiont-tmpspan/2;
        dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);

        Adjlimit=false;
        qDebug()<<"AdjPresslimit complete !";
       }

    }

    interfix_sleep(1);
    StateSwitch(NULL);
}

void StateDialysis::ActionExit(void)
{
    secCount=0;
    sample_count=0;
    memset(apBuff,0,sizeof(apBuff));
    memset(vpBuff,0,sizeof(vpBuff));
    memset(tmpBuff,0,sizeof(tmpBuff));

    timer->stopTimer(this);

    dataNodeDelObject(PACKET_NAME_WORKMODE, this);
    dataNodeDelObject(PACKET_NAME_AP, this);
    dataNodeDelObject(PACKET_NAME_VP, this);
    dataNodeDelObject(PACKET_NAME_TMP, this);

}

void StateDialysis::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("state_dialysis-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)){
        return;
    }

//    if (name==PACKET_NAME_AP) {
//        struct PacketAP *pData = (struct PacketAP*)dataPtr;
//        packetAP.ap=pData->ap;
//    }

    if (name==PACKET_NAME_WORKMODE) {


    }

//    if (strcmp(name,PACKET_NAME_ADJPRESSLIMIT) == 0){
//        Adjlimit =*((int*) dataPtr);
//        if(Adjlimit==true){
//            secCount=0;
//            sample_count=0;
//            memset(apBuff,0,sizeof(apBuff));
//            memset(vpBuff,0,sizeof(vpBuff));
//            memset(tmpBuff,0,sizeof(tmpBuff));
//        }
//    }

    if (name==PACKET_NAME_WORKSTATE){
        workstate  = *((WorkState*) dataPtr);
    }
    if (name==SelfCheck_Timer){
        //bool isSelfcheck = *((int*) dataPtr);
//        timer->createTimer(this, 1000);
        printf("\r\n start monitor... \r\n");

    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("state_dialysis-ActionHandle-out");
#endif
}

void StateDialysis::AdjPresslimit()
{

    dataRead(PACKET_NAME_AP,&packetAP);
    apBuff[sample_count]=packetAP.ap;
    packetAP.apUp=packetAP.ap+apspan/2;
    packetAP.apL = packetAP.ap-apspan/2;
    dataNodeUpdate(PACKET_NAME_AP, &packetAP);

    dataRead(PACKET_NAME_VP,&packetVP);
    vpBuff[sample_count]=packetVP.vp;
    packetVP.vpUp=packetVP.vp+vpspan/2;
    packetVP.vpL = packetVP.vp-vpspan/2;
    dataNodeUpdate(PACKET_NAME_VP, &packetVP);

    dataRead(PACKET_NAME_TMP,&packetTMP);
    vpBuff[sample_count]=packetTMP.tmp;
    packetTMP.tmpUp=packetTMP.tmp+tmpspan/2;
    packetTMP.tmpL = packetTMP.tmp-tmpspan/2;
    dataNodeUpdate(PACKET_NAME_TMP, &packetTMP);

    sample_count++;
    if(sample_count>99)
        sample_count=99;

    qDebug()<<"AdjPresslimit!";
    }

void StateDialysis::timer_handler(const int timeOut)
{
#if DEBUG_TIMER_HANDLE
    Debug_insert("StateDialysis-timer-in");
#endif

    Q_UNUSED(timeOut);
    if(Adjlimit==true)
     secCount++;
}

