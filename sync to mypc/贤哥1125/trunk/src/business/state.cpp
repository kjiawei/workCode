#include "state.h"
#include "global_var.h"
#include "config_ui.h"
#include "common_func.h"
#include "state_test.h"
#include "state_prime.h"
#include "state_dialysis.h"
#include "state_disinfect.h"

StatePrime *prime; //最初的处理任务#/
StateTest *test; //测试任务#/
StateDialysis *dialysis; //透析任务#/
state_disinfect *disinfect; //消毒任务#/

StateManage::StateManage(QObject* parent) :
    QThread(parent)
{
    FatherThread = QThread::currentThread();
    running = true;
    NameLast.clear();
    NameCurrent.clear();

    prime = new StatePrime((char *)"Prime"); //最初的处理任务#/
    //test = new StateTest((char *)"Test"); //测试任务#/
    //dialysis = new StateDialysis((char *)"Dialysis"); //透析任务#/
    disinfect = new state_disinfect((char *)"Disinfect"); //消毒任务#/

    //将各个处理任务类，和关键字关联起来#/
    hashState.insert(QString("Prime"), prime);
//    hashState.insert(QString("Test"), test);
//    hashState.insert(QString("Dialysis"), dialysis);
    hashState.insert(QString("Disinfect"), disinfect);
    //每3秒调用一次，检测总状态和工作模式的变化，remove#/
//    QObject::connect(&monitor,SIGNAL(timeout()),this,SLOT(StateFeedback()));

    memset(&workstate,0, sizeof(WorkState) );
    workstate.tipState.current = TREAT_READY; //准备状态#/
}

StateManage::~StateManage()
{
    QHash<QString, StateUnit*>::const_iterator it;
    for (it= hashState.constBegin(); it != hashState.constEnd(); it++) {
        delete it.value();
    }
    hashState.clear();
    running = false;
}

bool StateManage::StateSwitch(QString name)
{
    StateUnit *business=NULL;
    if (name == NameCurrent) {
        return false;
    }

    business = hashState.value(NameLast);
    if (business != NULL) {
        business->ActionExit();
    }
    NameLast = NameCurrent;
    NameCurrent = name;
    return true;
}
//暂时没用#/
//定时进行状态和工作模式的检测#/
void StateManage::StateFeedback()
{
    /*
    enum Treatmodel
    {
        HD_double = 0,   //
        HD_SNDP = 1,  //
        HD_SNSP = 2,  //
        HDF_pre_dilute = 3,  //
        HDF_post_dilute = 4,  //
        HF_pre_dilute = 5,  //
        HF_post_dilute = 6,  //
        ISOUF_DNDP = 7,  //
        ISOUF_SNDP = 8,  //
        ISOUF_SNSP = 9,  //
        PE = 10,  //
        MODELNUM=11
    };

    const int code[11]={0x21,0x28,0x27,0x23,0x23,0x24,0x24,0x22,0x2a,0x29,0x26};
    //static bool fault=false;

    //当水路状态改变时#/
    if(destState.water_status!=feedbackState.status.water_status)
       {
            //fault=true;
            //secCount++;
            if(secpass>2)
            {
               //printf("\r\n Detect waterstatus fault... \r\n");
               //secCount=0;
               //MasterCtrol_CMD_Treat_start(destState.work_model);
             }

         }
    //else fault=false;

    //当治疗状态下，工作模式改变时#/
    if(destState.header_status==Cure_run)
    {
        if(code[destState.work_model]!=feedbackState.status.blood_status)
        {
         secpass++;
         if(secpass>2)
         {
            //printf("\r\n Detect bloodstatus fault... \r\n");
            secpass=0;
            //MasterCtrol_CMD_Treat_start(destState.work_model);
          }
        }
    }
    //printf("\r\n Mornitor Status... \r\n");
*/
}

void StateManage::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("state-ActionHandle-in");
#endif

    if (name==PACKET_NAME_WORKSTATE){
      workstate  = *((WorkState*) dataPtr);   
      //destState.header_status=workstate.tip;
      //destState.blood_status=workstate.tip;
      //destState.water_status=workstate.current;
    }
    if(name==PACKET_STATUS_NOTE){
      feedbackState = *(PacketStatusNote *) dataPtr;

      if(workstate.waterState.current!=feedbackState.status.water_status)
      {
          /*
          WorkState stateTMP = workstate;
          stateTMP.waterState.previous=stateTMP.waterState.current;
          stateTMP.waterState.current=feedbackState.status.water_status;  
          SetWorkStateManage(&stateTMP);

          workstate.waterState.previous=workstate.waterState.current;
          workstate.waterState.current=feedbackState.status.water_status;
          */
          if(workstate.tipState.current!=CLEANING)
          {
            workstate.waterState.current=feedbackState.status.water_status;
            PDOstate.state.N1_water_status =feedbackState.status.water_status;
          }
      }
    }
    if (name==PACKET_NAME_WORKMODE) {
        PacketWorkMode workModeData = *(PacketWorkMode *)dataPtr;
        WorkState stateTMP = GetWorkStateManage();
        stateTMP.workMode = workModeData.treatmode.current;//治疗状态改变，更新状态的治疗模式#/
        SetWorkStateManage(&stateTMP);
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("state-ActionHandle-out");
#endif
}

void StateManage::run(void)
{
    StateUnit *business=NULL;
    //更新本机的各项工作状态#/
    dataNodeAddObject(PACKET_NAME_WORKSTATE, this);
    //将本机状态赋值canopen的通信结构#/
    dataNodeAddObject(PACKET_STATUS_NOTE, this);
    //更新工作模式#/
    dataNodeAddObject(PACKET_NAME_WORKMODE, this);

    disinfect->OtherHandle();
 //   monitor.start(3000);//3秒定时器  remove#/

    //将每个总状态作为一个任务类，共4个#/
    //可以通过调用 StateSwitch函数，设置当前运行的状态#/
    //并循环进行处理#/
    while (running) {
#if DEBUG_OTHER_FLAG
    Debug_insert("StateManage::run_in");
#endif
        if (NameCurrent.isEmpty()) {
            sleep(1);
            continue;
        }

        business = hashState.value(NameCurrent);//哈希表查找到当前#/
        if (business != NULL) {
            business->ActionEnter();//具体每个总状态下，执行的操作处理#/
        } else {
            sleep(1);
        }
#if DEBUG_OTHER_FLAG
    Debug_insert("StateManage::run_out");
#endif
    }
}

void StateManage::upDatePDOstate(void){
    if((TREAT_READY == workstate.tipState.current)||(TREAT_RUNNING == workstate.tipState.current)\
            || (TREAT_FINISH == workstate.tipState.current)){//在状态下的N1_blood_status#/
        unsigned char N1status[]={0x11,0x18,0x17,0x13,0x13,0x14,0x14,0x12,0x1a,0x19,0x16};//对应治疗模式
        PDOstate.state.N1_blood_status=N1status[workstate.workMode];
    }
    else if(CLEANING == workstate.tipState.current){//消毒状态下的N1_blood_status#/
        PDOstate.state.N1_blood_status=workstate.bloodState.current;
    }
    else{//初始化状态下的N1_blood_status#/

    }
    PDOstate.state.N1_status = workstate.tipState.current;
    PDOstate.state.N1_sub_status = workstate.subState.current;
    PDOstate.state.N1_water_status = workstate.waterState.current;
}

StateManage stateManage;

void StateManageStart()
{
    stateManage.start();
}

void StateManageStop()
{
    if (stateManage.Stop() == true) {
        stateManage.quit();
        stateManage.wait();
    }
}

bool StateSwitch(const QString name)
{
    return stateManage.StateSwitch(name);
}

WorkState GetWorkStateManage(void){
    return stateManage.GetWorkState();//读取状态#/
}

bool SetWorkStateManage(WorkState * state){
    return stateManage.SetWorkState(state);//保存状态，有改变返回true，否则返回false#/
}
