#include "lamp_task.h"
#include <signal.h>
#include "lamp_control.h"
#include "config_ui.h"
#include "common_func.h"

extern "C" {
    void sigroutine_lamp(int dunno);
}

lamptask::lamptask(void)
    :lamp_sem(0)
{

#ifdef OS_LINUX
    signal(SIGKILL, sigroutine_lamp);

    if (Lampinit()< 0)
    {
        qDebug()<<"open of /dev/lamp failed";
    }
    else
    {
        start(QThread::TimeCriticalPriority);
    }
#endif
     onoff_flag=true;
//    lamp_sem=0;  //信号量初始化#/
}
lamptask::~lamptask()
{
#ifdef OS_LINUX
    close(fd_lamp);
#endif
}

void sigroutine_lamp(int dunno)
{
    dunno = dunno;
#ifdef OS_LINUX
    close(fd_lamp);
#endif
}

void lamptask::run()
{
#ifdef OS_LINUX
bool greenflag=false;
    stop(2);
    while(1){
#if DEBUG_OTHER_FLAG
    Debug_insert("lamptask::run_in");
#endif
    if(onoff_flag==true)
    {
        if(alarmBit==true)
        {
          if(flag==1)//红
           {
              LampControl(1);
              msleep(delayms);
              LampControl(2);
              msleep(delayms);
           }
          else if(flag==2)//黄
           {
              LampControl(3);
              msleep(delayms);
              LampControl(4);
              msleep(delayms);
           }
          else if(flag==3)//绿
           {
              LampControl(5);
              msleep(delayms);
              LampControl(6);
              msleep(delayms);
           }
          greenflag=false;
        }
        else
        {
            lamp_sem.acquire();
            if(greenflag==false)
            {
                if(flag==1)//红
                 {
                    LampControl(1);
                 }
                else if(flag==2)//黄
                 {
                    LampControl(3);
                 }
                else if(flag==3)//绿
                 {
                    LampControl(5);
                 }
             greenflag=true;
            }
            msleep(500);
        }
    }
    else
    {
        lamp_sem.acquire();
        LampControl(2);
        msleep(500);
    }

#if DEBUG_OTHER_FLAG
    Debug_insert("lamptask::run_out");
#endif
    }
    close(fd_lamp);
#endif
}


void lamptask::flash(int model, int interval)
{
    if(flag==model&&delayms==interval&&alarmBit==true)
        return;

    flag=model;
    alarmBit = true;
    lamp_state=0;
    delayms=interval;

    lamp_sem.release();
}

void lamptask::stop(int model)
{
    if(flag==model&&alarmBit==false)
        return;

    alarmBit = false;
    flag=model;

    lamp_sem.release();
}

void lamptask::ledoff()  //关闭LED#/
{
    LampControl(2);
    onoff_flag=false;
}

void lamptask::ledon() //开启LED#/
{
    onoff_flag=true;
    lamp_sem.release();
}
