#include <unistd.h>
#include <cstddef>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QKeyEvent>
#include "common_func.h"
#include <QDebug>
#include "global_var.h"

#ifdef OS_LINUX
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include "xml_parse.h"
#include "Daemon.h"

/*---------------------------调试专用--------------------------------*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<execinfo.h>

#define DEBUG_BUF_MAX 1024
#define DEBUG_CACHE_MAX 128

static char Debug_buf[DEBUG_BUF_MAX];
static char Debug_cache[DEBUG_CACHE_MAX];

static int debug_buf_index=0;
static int debug_cache_index=0;

void Debug_init()
{
    memset(Debug_buf,0,sizeof(Debug_buf));
    memset(Debug_cache,0,sizeof(Debug_cache));
    debug_buf_index=0;
    debug_cache_index=0;
}

void Debug_insert(const char *str)
{
    static char i=0;
    int len=0;
    len=strlen(str);
    if(len<=0)
    {
        return;
    }
    else if((len+debug_buf_index+5)>=DEBUG_BUF_MAX)
    {
        if(len<=(DEBUG_BUF_MAX-5))
        {
            memcpy(Debug_buf,str,len);
            debug_buf_index=len;
        }
        else
        {
            memcpy(Debug_buf,str,DEBUG_BUF_MAX-5);
            debug_buf_index=0;
        }
    }
    else
    {
        memcpy(&Debug_buf[debug_buf_index],str,len);
        debug_buf_index +=len;
    }

    Debug_buf[debug_buf_index++]='-';
    Debug_buf[debug_buf_index++]=0x30+i;
    Debug_buf[debug_buf_index++]=';';
    Debug_buf[debug_buf_index++]=0x0d;
    Debug_buf[debug_buf_index++]=0x0a;
    i++;
    if(i>9)
        i=0;

}

void Debug_record(const char *str)
{
    int len=0;
    len=strlen(str);
    memset(Debug_cache,0,sizeof(Debug_cache));
    if(len>=(DEBUG_CACHE_MAX-2))
        memcpy(Debug_cache,str,len);
    else
        memcpy(Debug_cache,str,DEBUG_CACHE_MAX-2);
}

void Debug_output()
{
    int fd;
    int len=0;
    Debug_insert(Debug_cache);
    Debug_buf[debug_buf_index++]='e';
    Debug_buf[debug_buf_index++]='n';
    Debug_buf[debug_buf_index++]='d';
    Debug_buf[debug_buf_index++]=0x0d;
    Debug_buf[debug_buf_index++]=0x0a;
//    Debug_buf[debug_buf_index++]=0;
    len=strlen(Debug_buf);
    if((fd=open("./debug.txt",O_CREAT|O_TRUNC|O_WRONLY,0666))<0)
    {
        return;
    }
  //   printf("debug-output--1\n");
 //   printf("%s\n",Debug_buf);
    len=write(fd,Debug_buf,len);
    close(fd);
}

#define DEBUG_SIZE 1000
void *buffer[DEBUG_SIZE];

void fault_trap(int n,siginfo_t *siginfo,void *myact)
{
    n=n;
    siginfo=siginfo;
    myact=myact;

        printf("fault_in\n");
        Rebirth_Handle();
        printf("Rebirth_out\n");

        Debug_output();
        printf("Debug_out\n");
        //死机后重生处理函数---保存参数#/

        //写XML文件特定节点内容#/
 //       Write_XML_Node(QString("/Config/Engineer_Factoryset/Rebirth"),QString("Die"),QString::number(dietimes));//放在这里写，XML文件可能出问题#/

        if(G_cpid>0)
        {
            kill(G_cpid,SIG_APPQUIT);
            printf("KILL_SIG_APPQUIT\n");
        }
/*
 *      int i, num;
        char **calls;

        printf("Fault address:%X\n",siginfo->si_errno);
//        printf("Fault address:%X\n",siginfo->si_addr);
        num = backtrace(buffer, DEBUG_SIZE);
        calls = backtrace_symbols(buffer, num);
        for (i = 0; i < num; i++)
                printf("%s\n", calls[i]);
*/

        exit(1);
}

void Debug_setuptrap()
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags=SA_SIGINFO;
    act.sa_sigaction=fault_trap;
    sigaction(SIGSEGV,&act,NULL);
    sigaction(SIGILL,&act,NULL);
}

/*---------------------------调试专用--------------------------------*/

//HJX实现非阻塞性的互斥锁
void Try_Map_mutex(void)
{
    static char count=0;
    while(count<6)
    {
        if(Map_mutex.tryLock()==true)
        {
            count=0;
            break;
        }
        else
        {
            count++;
            us_sleep(5000);
        }
    }
    if(count>=6)
    {
        Map_mutex.unlock();
        count=0;
        Map_mutex.tryLock();
    }
}

//死机后重生处理函数---保存参数#/
void Rebirth_Handle(void)
{
    struct STR_Rebirth T_val;
    PacketPDOHeparinPump heparinPDO;  //肝素泵参数#/
    PacketSdoBlood sdoBlood;//血泵1参数#/
    PacketSubBloodPump SubBPump;  //置换泵参数#/
    PacketUF packetUF;  //超滤泵参数#/
    PacketWorkMode workModeData;  //工作模式#/
    FormulaSet formulaSet;//配方参数表#/
    PacketFluid Fluid;  //透析液流速和温度#/
    PacketPreflush Preflush;         //预冲参数#/

    PacketAP packetAP; // AP  动脉压
    PacketVP packetVP; // VP
    PacketTMP packetTMP; // TMP
    PacketCond packetCond;

    T_val.selfcheck_enable=selfcheck_enable;
    T_val.G_SAVECUREPARAM_FLAG=G_SAVECUREPARAM_FLAG;
    T_val.total_secCount=total_secCount;
    T_val.residue_secCount=residue_secCount;
    T_val.heparinPreStop_sec=heparinPreStop_sec;
    T_val.heparinPreStop=heparinPreStop;
    T_val.HeparinPumpFirstflag=HeparinPumpFirstflag;

    dataRead(PACKET_PDO_HEPARINPUMP,&heparinPDO);
    T_val.heparinPDO=heparinPDO;

    T_val.G_UF_PRETARGET=G_UF_PRETARGET;
    T_val.G_UF_CURERESIDUE=G_UF_CURERESIDUE;
    T_val.G_UF_CURETARGET=G_UF_CURETARGET;

    dietimes += 1;
    T_val.dietimes=dietimes;
    T_val.rebirthtimes=rebirthtimes;
    T_val.BloodPump_Volume=BloodPump_Volume;
    T_val.BloodPump_Pre_Amount=BloodPump_Pre_Amount;
    T_val.BloodPump_Cure_Amount=BloodPump_Cure_Amount;
    T_val.static_BP_amount=static_BP_amount;
    T_val.G_BPRATE_TREAT=G_BPRATE_TREAT;
    T_val.BloodPump_link_flag=BloodPump_link_flag;
    T_val.G_SET_BPSPEED=G_SET_BPSPEED;

    dataRead(PACKET_SDO_BLOOD, &sdoBlood);
    T_val.sdoBlood=sdoBlood;

    T_val.SubsPump_totalvolume=SubsPump_totalvolume;

    T_val.SubsAddnum=SubsAddnum;

    dataRead(PACKET_PDO_SUBBLOODPUMP, &SubBPump);
    T_val.SubBPump=SubBPump;

    T_val.G_UF_STARTTIME=G_UF_STARTTIME;
    T_val.G_UF_ENDTIME=G_UF_ENDTIME;
    T_val.G_UF_FINISHVOL_FLAG=G_UF_FINISHVOL_FLAG;
    T_val.G_UF_PREFINISH=G_UF_PREFINISH;
    T_val.G_UF_CUREFINISH=G_UF_CUREFINISH;
    T_val.UFPump_Pre_Amount=UFPump_Pre_Amount;
    T_val.UFPump_Cure_Amount=UFPump_Cure_Amount;
    T_val.static_UF_amount=static_UF_amount;
    T_val.G_UF_SETRATE=G_UF_SETRATE;        //UF的设置速度#/

    dataRead(PACKET_NAME_UF, &packetUF);
    T_val.packetUF=packetUF;

    dataRead(PACKET_FORMULA_TARGET, &formulaSet);
    T_val.formulaSet=formulaSet;
    dataRead(PACKET_NAME_DIALYSATE,&Fluid);
    T_val.Fluid=Fluid;

    T_val.Blood_online_lead=Blood_online_lead;
    T_val.Blood_online_return=Blood_online_return;
    T_val.Startpreflush_mode=Startpreflush_mode;
    T_val.Startreturnblood_mode=Startreturnblood_mode;
    dataRead(PACKET_PREFLUSH, &Preflush);
    T_val.Preflush=Preflush;         //预冲参数#/

    T_val.G_BYPASS_BUT_FLAG=G_BYPASS_BUT_FLAG;
    T_val.G_CURE_STARTTIME=G_CURE_STARTTIME;
    T_val.G_CURE_ENDTIME=G_CURE_ENDTIME;

    T_val.prestatusUFPump=prestatusUFPump;  //上一次超滤泵状态#/
    T_val.prestatusHeparinPump=prestatusHeparinPump;
    T_val.prestatusSideWay=prestatusSideWay;

    T_val.ISO_totalsec=ISO_totalsec;
    T_val.ISO_residuesec=ISO_residuesec;
    T_val.ISO_totalvolume=ISO_totalvolume;
    T_val.ISO_finishvolume=ISO_finishvolume;
    T_val.UFPump_Now_volume=UFPump_Now_volume;

    T_val.G_NA_PROFILE_FLAG=G_NA_PROFILE_FLAG;
    T_val.G_NA_PROFILE_NUM=G_NA_PROFILE_NUM;
    T_val.G_NA_PROFILE_START=G_NA_PROFILE_START;
    T_val.G_NA_PROFILE_END=G_NA_PROFILE_END;
    memcpy(T_val.A_NA_PROFILE,A_NA_PROFILE,sizeof(NA_Profile_Array)*10);
    T_val.G_NA_SELECTCURVE_NUM=G_NA_SELECTCURVE_NUM;
    T_val.G_PRE_NA_VALUE=G_PRE_NA_VALUE;

    T_val.G_UF_PROFILE_FLAG=G_UF_PROFILE_FLAG;
    T_val.G_UF_PROFILE_NUM=G_UF_PROFILE_NUM;
    T_val.G_UF_PROFILE_START=G_UF_PROFILE_START;
    T_val.G_UF_PROFILE_END=G_UF_PROFILE_END;
    memcpy(T_val.A_UF_PROFILE,A_UF_PROFILE,sizeof(UF_Profile_Array)*10);
    T_val.G_UF_PROFILE_STARTVOL=G_UF_PROFILE_STARTVOL;
    T_val.G_UF_PROFILE_PERTOTALVOL=G_UF_PROFILE_PERTOTALVOL;
    T_val.G_UF_SELECTCURVE_NUM=G_UF_SELECTCURVE_NUM;

    T_val.language=language;

    T_val.workstate=GetWorkStateManage();   //工作状态设置#/

    dataRead(PACKET_NAME_WORKMODE,&workModeData);
    T_val.G_WORKMODE_VAL=workModeData;  //工作模式#/

    T_val.Disinfect_secCount=Disinfect_secCount;           //消毒剩余时间#/
    T_val.Disinfect=Disinfect;  //消毒参数全局变量#/
    T_val.disinfectCond=disinfectCond;   //消毒电导#、
    T_val.iSPipeok=iSPipeok;  //消毒时管路状态#/

    dataRead(PACKET_NAME_AP, &packetAP);
    T_val.packetAP=packetAP; // AP  动脉压
    dataRead(PACKET_NAME_VP, &packetVP);//报警后按reset键将卡标设为最大范围#/
    T_val.packetVP=packetVP; // VP
    dataRead(PACKET_NAME_TMP, &packetTMP);
    T_val.packetTMP=packetTMP;
    dataRead(PACKET_NAME_COND,&packetCond);
    T_val.packetCond=packetCond;

    T_val.packetBPMparam=packetBPMparam;

    int fd,len;

    if((fd=open("./param.dat",O_CREAT|O_TRUNC|O_WRONLY,0666))<0)
    {
        return;
    }

    len=write(fd,&T_val,sizeof(STR_Rebirth));

    close(fd);

    if(len==sizeof(STR_Rebirth))
    {
       printf("write OK len=%d\n",len);
    }
    else
    {
        if((fd=open("./param.dat",O_CREAT|O_TRUNC|O_WRONLY,0666))<0)
        {
            return;
        }

        len=write(fd,&T_val,sizeof(STR_Rebirth));

        close(fd);
    }
}


//系统调用，非并发。。。把父进程的mask传给系统调用的mask。#/
int SystemShellCmd(const char *file, char **arg)
{
#ifdef OS_LINUX
    pid_t pid;
    sigset_t savemask;

    if (sigprocmask(SIG_BLOCK, NULL, &savemask) < 0) {
        return -1;
    }

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid == 0) {
        sigprocmask(SIG_SETMASK, &savemask, NULL);
        execvp(file, arg);
        _exit(127);
        _exit(127);
    } else {
        while(waitpid(pid, NULL, 0) < 0) ;
    }
#else
    if (file == NULL || arg == NULL)
        return -1;
#endif
    return 0;
}

//休眠#/
void us_sleep(const int us)
{
#ifdef OS_LINUX
    struct timeval delay;
    delay.tv_sec = 0;
    delay.tv_usec = us;
    select(0, NULL, NULL, NULL, &delay);
#else

#endif
}

//屏幕载屏#/
void GetScreenShots(void)
{
    QString FilePath, FileName, CMDPara;
    QDir MakePath;

    FilePath = "/usr/local/app/data/screenshots/";
    MakePath.mkpath(FilePath);

    FileName = QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss.zzz");

    FileName.append(".png");
    FilePath.append(FileName);

    CMDPara = "/usr/local/bin/gsnap  ";
    CMDPara.append(FilePath);
    CMDPara.append(" /dev/fb0  ");

    system(CMDPara.toLatin1().data());
}

//触摸屏校正#/
void CallTsCalibrate(void)
{
    FILE *TsCalibrateSH;

    TsCalibrateSH = fopen("/usr/local/sbin/call_ts_calibrate.sh" , "w+");
    if (TsCalibrateSH == NULL) {
        qDebug()<<"Can not open /usr/local/sbin/call_ts_calibrate.sh\n";
        return;
    }

    fprintf(TsCalibrateSH, "%s","#!/bin/bash\n");

    fprintf(TsCalibrateSH, "%s","killall hemodialysis\n");

    //给5s时间给系统回收资源#/
    fprintf(TsCalibrateSH, "%s","sleep 1s\n");

    fprintf(TsCalibrateSH, "%s","/usr/local/bin/ts_calibrate\n");

    fprintf(TsCalibrateSH, "%s","cd /usr/local/app\n");
    fprintf(TsCalibrateSH, "%s","./hemodialysis -qws & \n");

    fclose(TsCalibrateSH);

    chmod("/usr/local/sbin/call_ts_calibrate.sh",0777);
    system("/usr/local/sbin/call_ts_calibrate.sh");
}

//过滤器。。。#/
//按f12这个键，载屏#/
//按f11这个键，触摸屏校正#/
bool FunctionsKeyEater::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //qDebug("Ate key press %d", keyEvent->key());

        switch (keyEvent->key()) {
            case Qt::Key_F12:
                GetScreenShots();
                break;

            case Qt::Key_F11:
/*
                printf("CallTsCalibrate \n");
                CallTsCalibrate();
*/
                if(G_cpid>0)
                {
                    kill(G_cpid,SIGKILL);
                }


                pid_t res;

                res=fork();

                if(res==-1)
                {
                    perror("fork");
                }
                else if(res==0)  //子进程----处理#/
                {
                   sleep(1);
                   CallTsCalibrate();
                }
                else if(res>0)  //父进程---处理#/
                {
                    exit(0);
                }

                break;

            default:
                return QObject::eventFilter(obj, event);
                break;
        }

        return true;
    }

    /*
     if(IsNoteDialogShow()){
         if (event->type() == QEvent::MouseButtonPress ||
             event->type() == QEvent::MouseButtonRelease||
             event->type() == QEvent::MouseButtonDblClick)
         {
              if(obj->parent())
              {
                  if(obj->parent()->parent())
                 {
                   if(obj->parent()->parent()->objectName().compare("NoteDialogParent")==0)
                   {
                    qDebug() << "\r\n Get Press: !"<<obj->objectName();
                    return QObject::eventFilter(obj, event);
                   }
                   else return true;
                 }
                else return true;

             }
             else {
                 qDebug() << "\r\n Forbit !";
                 return true;
             }

           }
         else  return true;

     }
    */
    ///*
    if(IsNoteDialogShow()){
         if (event->type() == QEvent::MouseButtonPress ||
             event->type() == QEvent::MouseButtonRelease||
             event->type() == QEvent::MouseButtonDblClick)
        {

            if(obj->objectName().compare("btnClose")==0 ||
               obj->objectName().compare("btnLast")==0 ||
               obj->objectName().compare("btnNext")==0 ||
               obj->objectName().compare("Notice_btnClose")==0 ||
               obj->objectName().compare("Notice_btnYes")==0 ||
               obj->objectName().compare("Notice_btnNo")==0      )
            {
              //qDebug() << "\r\n Get Press: !"<<obj->objectName();
              return QObject::eventFilter(obj, event);
            }
            else {
                //qDebug() << "\r\n Forbit !";
                return true;
            }
        }
        else return QObject::eventFilter(obj, event);
    }
    //*/
    else {
        if (event->type() == QEvent::MouseButtonRelease)
            {
                if(obj->objectName().compare("pushButtonSetup")!=0){
                    //鼠标事件#/
                    emit mouseEvent(1);
                }
            }
        return QObject::eventFilter(obj, event);
    }

}



void Write_XML_Node(QString path,QString strmode,QString strnum)
{
    QMap<QString, QString> mapV;
    QDomDocument doc;

    xml_DocReadOpen(&doc, DEFAULT_XML_FILE);
    mapV.clear();

    mapV.insert(strmode, strnum);
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);
    waiteToFile(&doc, DEFAULT_XML_FILE);
}

QMap<QString, QString>  Read_XML_Node(QString path)
{
    QMap<QString, QString> mapV;
    QDomDocument doc;

    xml_DocReadOpen(&doc, DEFAULT_XML_FILE);
    mapV.clear();

    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //肝素泵使用选择#/
    return mapV;
}



/**************************/
/*
 return true 上一次关机完成消毒
 */
/**************************/
bool GetDisinfectState(void)
{
    QFile f("conf/disinfect_state");
    if(!f.open(QIODevice::ReadWrite | QIODevice::Text )){
        qDebug("Open disinfect file failed.") ;
        return false;
    }
    QString str(f.readAll());
    if("disinfect is not complete"==str){
        qDebug("disinfect is not complete");
        return false;
    }
    else{
        qDebug("disinfect Ok");
        return true;
    }
}
/******/
/*
true 表示消毒完成
false表示消毒没有完成
*/
/******/
bool MarkDisinfectFinish(bool disinfectFinish)
{
    QFile f("conf/disinfect_state");
    if(!f.open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug("Open disinfect file failed.") ;
        return false;
    }
    QString str(f.readAll());
    f.close();
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text )){
        qDebug("Open disinfect file failed.") ;
        return false;
    }
    if(disinfectFinish){
        if("disinfect Ok"==str){
            qDebug("disinfect is not need to change");
        }
        else if("disinfect is not complete"==str){
            qDebug("disinfect is not complete change to disinfect Ok");
            str = "disinfect Ok";
        }
        else{
            qDebug("disinfect is not sure change to disinfect Ok");
            str = "disinfect Ok";
        }
    }
    else{
        if("disinfect Ok"==str){
            qDebug("disinfect Ok change to disinfect is not complete");
            str = "disinfect is not complete";
        }
        else if("disinfect is not complete"==str){
            qDebug("disinfect is not need to change");
        }
        else{
            qDebug("disinfect is not sure change to disinfect Ok");
            str = "disinfect is not complete";
        }
    }
    f.write(str.toUtf8());
    f.close();
    return true;
}



//改变血泵的速度#/
void CHANGED_BP_SPEED(unsigned short int bpspeed)
{
    static PacketSdoBlood sdoBlood;
    dataRead(PACKET_SDO_BLOOD,&sdoBlood);
    sdoBlood.N3_set_BPspeed = bpspeed;

    G_SET_BPSPEED=sdoBlood.N3_set_BPspeed;
    dataNodeUpdate(PACKET_SDO_BLOOD, &sdoBlood);
}

//改变超滤泵的速度#/
void CHANGED_UF_SPEED(bool curerate,int ufspeed)
{
    static PacketUF packetTmp;
    dataRead(PACKET_NAME_UF,&packetTmp);
    packetTmp.chooseRate=curerate;
    if(packetTmp.chooseRate==true)
        packetTmp.ufRate=ufspeed;    //ml/h
    else
        packetTmp.preRate=ufspeed;

    dataNodeUpdate(PACKET_NAME_UF, &packetTmp);                 //修改下发的超滤速度#/
}


//超滤泵和时间同步处理函数#/
void UF_SYNTIME_HANDLE(unsigned int starttime,unsigned int curtime,unsigned int endtime,unsigned int finishuf,unsigned int totaluf,unsigned int ufsetrate)
{
    static char count=0;
    static PacketUF packetTmp;
    PacketStatusNote MasterStatus;
//    float ufrate=0;
    float ratio=0,adjrate=0,lastsec=0;
    float timerate=0,volrate=0;

    count++;

    if(count<=5)    //防止一直修改超滤速度#/
        return;

    count=0;

    dataRead(PACKET_STATUS_NOTE,&MasterStatus);
    dataRead(PACKET_NAME_UF,&packetTmp);
/*
    if(packetTmp.chooseRate==true)
        ufrate=packetTmp.ufRate;    //ml/h
    else
        ufrate=packetTmp.preRate;
*/

    if(MasterStatus.note.note2&0x80)  //超滤泵正在运行#/
    {

        lastsec=endtime-curtime-G_UF_EARLIER_TIME+5;   //剩余治疗时间#/
        if(lastsec<=0)
        {
            lastsec=1;
            if(finishuf>=totaluf)
            {
                adjrate=0;
            }
            else
            {
                adjrate=(1.0*totaluf-finishuf)/lastsec;   //实际超滤速度  ml/s #/
                adjrate=adjrate*3600;  //ml/h
            }
        }
        else
        {
            timerate=(curtime-starttime*1.0)/(endtime-starttime-G_UF_EARLIER_TIME+5);
            volrate=finishuf*1.0/totaluf;
            ratio=timerate/volrate;
            adjrate=ufsetrate*ratio;  //计算出调整超滤速度#/

            if(timerate>=0&&timerate<0.15)  //当时间过去10%的时候才开始速度计算#/
            {
                if(adjrate>ufsetrate*1.15)
                    adjrate=ufsetrate*1.15;
                else if(adjrate<ufsetrate*0.9)
                    adjrate=ufsetrate*0.9;
            }

            /*
            if(timerate>=0.1&&timerate<0.3)  //当时间过去10%的时候才开始速度计算#/
            {
                if(adjrate>ufsetrate*1.15)
                    adjrate=ufsetrate*1.15;
                else if(adjrate<ufsetrate*0.9)
                    adjrate=ufsetrate*0.9;
            }
            else if(timerate>=0.3&&timerate<0.8)
            {
                if(adjrate>ufsetrate*1.2)
                    adjrate=ufsetrate*1.2;
                else if(adjrate<ufsetrate*0.85)
                    adjrate=ufsetrate*0.85;
            }
            else if(timerate>=0.8)
            {
            */
                /*
                if(ratio<1)
                {
                    adjrate=(1.0*totaluf-finishuf)/lastsec;   //实际超滤速度  ml/s #/
                    adjrate=adjrate*3600;  //ml/h
                    adjrate=ufsetrate*ratio;  //计算出调整超滤速度#/
                }
                */
    /*
                if(adjrate>ufsetrate*1.15)
                    adjrate=ufsetrate*1.15;
                else if(adjrate<ufsetrate*0.9)
                    adjrate=ufsetrate*0.9;
            }
            else
                adjrate=ufsetrate;
    */

        }

        if(adjrate > usr_params.Params_UF.UF_Rate.attribute.max_value){
            adjrate = usr_params.Params_UF.UF_Rate.attribute.max_value;
            }

        if(adjrate < usr_params.Params_UF.UF_Rate.attribute.min_value){
            adjrate = usr_params.Params_UF.UF_Rate.attribute.min_value;
            }


        if(packetTmp.chooseRate==true)
        {
            packetTmp.ufRate=(unsigned int)adjrate;
        }
        else
        {
            packetTmp.preRate=(unsigned int)adjrate;
        }

        dataNodeUpdate(PACKET_NAME_UF, &packetTmp);                 //修改下发的超滤速度#/
    }
}

//NA曲线处理函数#/
void NA_PROFILE_HANDLE(unsigned int starttime,unsigned int curtime,unsigned int endtime)
{
    static FormulaSet formula;

    float persec=0;
    unsigned char s=0;
    persec=(endtime-starttime)/(10-G_NA_PROFILE_NUM);  //开始时间间隔/剩余的治疗时段=每个将进行的治疗时段的时间

    s=(curtime-starttime)/persec;

    s=s+G_NA_PROFILE_NUM;

    if(s>9)
        s=9;

    if(A_NA_PROFILE[s].flag==false)   //该时段治疗尚未开始#/
    {
        dataRead(PACKET_FORMULA_TARGET,&formula);
        formula.tempBuffer.na=A_NA_PROFILE[s].navalue;
        updateFaisonFormulaCond(&formula);
        A_NA_PROFILE[s].flag=true;
    }

}

//UF曲线处理函数#/
void UF_PROFILE_HANDLE(unsigned int starttime,unsigned int curtime,unsigned int endtime)
{
    float persec=0;
    float sumtmp=0,valtmp=0;
    unsigned char s=0,i=0;
    persec=(endtime-starttime)/(10-G_UF_PROFILE_NUM);//每一段的时间#/
//    perufvol=(G_UF_CURETARGET-G_UF_PROFILE_STARTVOL)/10.0; //每段平均超滤量#/
    s=(curtime-starttime)/persec;

    s=s+G_UF_PROFILE_NUM;

    if(s>9)
        s=9;

    if(A_UF_PROFILE[s].flag==false)   //该时段治疗尚未开始#/
    {
        if(s==0)  //开始的第一段#/
        {
            G_UF_SETRATE=(A_UF_PROFILE[s].ufvol/persec)*3600; //计算出该时段的超滤速度 ml/H #/
        }
        else if(s>0)
        {
            for(i=s;i<10;i++)  //计算出所有剩余时段的超滤量#/
            {
                sumtmp +=A_UF_PROFILE[i].ufvol;
            }
            valtmp=G_UF_CURETARGET-G_UF_PROFILE_STARTVOL-sumtmp;  //理论已超滤量#/
            valtmp=valtmp-(G_UF_CUREFINISH-G_UF_PROFILE_STARTVOL);  //总误差超滤量#/
            valtmp=valtmp/(10.0-s);  //每段误差#/
            //进行每段校正#/
            for(i=s;i<10;i++)
            {
                A_UF_PROFILE[i].ufvol += valtmp;
            }

            G_UF_SETRATE=(A_UF_PROFILE[s].ufvol/persec)*3600; //计算出该时段的超滤速度 ml/H #/
        }

        if(A_UF_PROFILE[s].seqmode==true)  //切换到单超模式#/
        {
            ISO_totalsec=persec;
            ISO_residuesec=residue_secCount; //ISO 计时开始点#/
            ISO_totalvolume=A_UF_PROFILE[s].ufvol;
            ISO_finishvolume=0;
            UFPump_Now_volume=G_UF_CUREFINISH;//ISO开始时，开始计算的超滤量#/
            G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
            G_UF_ENDTIME=G_SYS_CURTIME+ISO_totalsec;  //超滤的总时间 单位秒#/

            dataRead(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);

            if(G_WORKMODE_VAL.treatmode.current==HD_SNDP)
                G_WORKMODE_VAL.treatmode.target =  ISOUF_SNDP;
            else if(G_WORKMODE_VAL.treatmode.current==HD_SNSP)
                G_WORKMODE_VAL.treatmode.target =  ISOUF_SNSP;
            else
                G_WORKMODE_VAL.treatmode.target =  ISOUF_DNDP;

            G_WORKMODE_VAL.treatmode.before = G_WORKMODE_VAL.treatmode.current;
            G_WORKMODE_VAL.treatmode.current = G_WORKMODE_VAL.treatmode.target;
            G_WORKMODE_VAL.flag=setok_state;
            dataNodeUpdate(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
        }
        else if(s>0)  //如果已切换到ISO则恢复为原模式#/
        {
            if(A_UF_PROFILE[s-1].seqmode==true)
            {
                dataRead(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);

                G_WORKMODE_VAL.treatmode.target = G_WORKMODE_VAL.treatmode.before;
                G_WORKMODE_VAL.treatmode.before = G_WORKMODE_VAL.treatmode.current;
                G_WORKMODE_VAL.treatmode.current = G_WORKMODE_VAL.treatmode.target;
                G_WORKMODE_VAL.flag=setok_state;

                if(G_WORKMODE_VAL.treatmode.current == ISOUF_SNSP||G_WORKMODE_VAL.treatmode.current == ISOUF_SNDP||\
                        G_WORKMODE_VAL.treatmode.current == ISOUF_DNDP)
                {
                    ISO_totalsec=persec;
                    ISO_residuesec=residue_secCount; //ISO 计时开始点#/
                    ISO_totalvolume=A_UF_PROFILE[s].ufvol;
                    ISO_finishvolume=0;
                    UFPump_Now_volume=G_UF_CUREFINISH;//ISO开始时，开始计算的超滤量#/
                    G_UF_STARTTIME=G_SYS_CURTIME;      //超滤开启时间 单位秒#/
                    G_UF_ENDTIME=G_SYS_CURTIME+ISO_totalsec;  //超滤的总时间 单位秒#/
                }

                dataNodeUpdate(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
            }
        }

        G_UF_FINISHVOL_FLAG=false;          //超滤目标量未到#/
        G_UF_PROFILE_PERTOTALVOL=G_UF_CUREFINISH+A_UF_PROFILE[s].ufvol;  //本时段的超滤目标量#/
        A_UF_PROFILE[s].flag=true;
        //改变超滤泵的速度#/
        CHANGED_UF_SPEED(true,G_UF_SETRATE);
    }

    //超滤速度和时间同步#/
//    UF_SYNTIME_HANDLE(G_UF_PROFILE_START,G_SYS_CURTIME,G_UF_PROFILE_END,G_UF_CUREFINISH-G_UF_PROFILE_STARTVOL,G_UF_PROFILE_PERTOTALVOL,G_UF_SETRATE);
}

//透析液曲线处理函数#/
void DIA_PROFILE_HANDLE()
{
    int pertimesec=0;   //每个曲线时间段时间#/
    int passtimesec=0;   //已经治疗过去的时间#/
    unsigned char num=0;  //当前运行到第几段#/

    pertimesec=total_secCount/10;

    passtimesec=G_SYS_CURTIME-G_CURE_STARTTIME;

    num=passtimesec/pertimesec;

    if(num>9)   //超过曲线段数
    {
        G_DIA_PROFILE_FLAG=false;
        return;
    }

    if(num!=G_DIA_PROFILE_NUM)
    {

        G_DIA_PROFILE_NUM=num;

        PacketFluid packet;

        dataRead(PACKET_NAME_DIALYSATE,&packet);
        packet.Wspeed = A_DIA_PROFILE[num].curevalue;
        dataNodeUpdate(PACKET_NAME_DIALYSATE,&packet);

    }
}
//肝素曲线处理函数#/
void HEP_PROFILE_HANDLE()
{
    int pertimesec=0;   //每个曲线时间段时间#/
    int passtimesec=0;   //已经治疗过去的时间#/
    unsigned char num=0;  //当前运行到第几段#/

    pertimesec=total_secCount/10;

    passtimesec=G_SYS_CURTIME-G_CURE_STARTTIME;

    num=passtimesec/pertimesec;

    if(num>9)   //超过曲线段数
    {
        G_HEP_PROFILE_FLAG=false;
        return;
    }

    if(num!=G_HEP_PROFILE_NUM)
    {

        G_HEP_PROFILE_NUM=num;

        PacketPDOHeparinPump heparinPDO;

        dataRead(PACKET_PDO_HEPARINPUMP,&heparinPDO);
        heparinPDO.speed = A_HEP_PROFILE[num].curevalue*10;
        dataNodeUpdate(PACKET_PDO_HEPARINPUMP,&heparinPDO);

    }

}
//电导曲线处理函数#/
void COND_PROFILE_HANDLE()
{
    int pertimesec=0;   //每个曲线时间段时间#/
    int passtimesec=0;   //已经治疗过去的时间#/
    unsigned char num=0;  //当前运行到第几段#/

    pertimesec=total_secCount/10;

    passtimesec=G_SYS_CURTIME-G_CURE_STARTTIME;

    num=passtimesec/pertimesec;

    if(num>9)   //超过曲线段数
    {
        G_COND_PROFILE_FLAG=false;
        return;
    }

    if(num!=G_COND_PROFILE_NUM)
    {

        G_COND_PROFILE_NUM=num;

        uiPDO04.N1_set_mix = A_COND_PROFILE[num].curevalue*100;
    }

}
//温度曲线处理函数#/
void TEMP_PROFILE_HANDLE()
{
    int pertimesec=0;   //每个曲线时间段时间#/
    int passtimesec=0;   //已经治疗过去的时间#/
    unsigned char num=0;  //当前运行到第几段#/

    pertimesec=total_secCount/10;

    passtimesec=G_SYS_CURTIME-G_CURE_STARTTIME;

    num=passtimesec/pertimesec;

    if(num>9)   //超过曲线段数
    {
        G_TEMP_PROFILE_FLAG=false;
        return;
    }

    if(num!=G_TEMP_PROFILE_NUM)
    {

        G_TEMP_PROFILE_NUM=num;

        PacketFluid packet;

        dataRead(PACKET_NAME_DIALYSATE,&packet);
        packet.tempTarget = A_TEMP_PROFILE[num].curevalue*10;
        dataNodeUpdate(PACKET_NAME_DIALYSATE,&packet);

    }

}
//HCO3曲线处理函数#/
void HCO3_PROFILE_HANDLE()
{
    int pertimesec=0;   //每个曲线时间段时间#/
    int passtimesec=0;   //已经治疗过去的时间#/
    unsigned char num=0;  //当前运行到第几段#/

    pertimesec=total_secCount/10;

    passtimesec=G_SYS_CURTIME-G_CURE_STARTTIME;

    num=passtimesec/pertimesec;

    if(num>9)   //超过曲线段数
    {
        G_HCO3_PROFILE_FLAG=false;
        return;
    }

    if(num!=G_HCO3_PROFILE_NUM)
    {

        G_HCO3_PROFILE_NUM=num;

        FormulaSet formula;
        dataRead(PACKET_FORMULA_TARGET,&formula);
        formula.tempBuffer.hco3 = A_HCO3_PROFILE[num].curevalue*10;
        dataNodeUpdate(PACKET_FORMULA_TARGET,&formula);
    }
}


