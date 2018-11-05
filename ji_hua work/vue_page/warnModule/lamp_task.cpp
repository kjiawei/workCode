#include <signal.h>

#include "lamp_task.h"
#include "commonVar.h"
#include "commonUtil.h"

extern "C" {
    void sigroutine_lamp(int dunno);
}

lampTask::lampTask(void):lamp_sem(0)
{
#ifdef OS_LINUX
    signal(SIGKILL, sigroutine_lamp);
    if (lampInit()< 0)
        qDebug()<<"open of /dev/lamp failed";
    else
        start(QThread::TimeCriticalPriority);
#endif
     isLampOn = true;//lamp_sem=0;  //信号量初始化#/
}

lampTask::~lampTask()
{
#ifdef OS_LINUX
    close(lamp_fd);
#endif
}

void sigroutine_lamp(int dunno)
{
    dunno = dunno;
#ifdef OS_LINUX
    close(lamp_fd);
#endif
}

void lampTask::run()
{
#ifdef OS_LINUX
bool isGreen = false;
    stop(2);
    while(1){
#if DEBUG_OTHER_lampState
    Debug_insert("lampTask::run_in");
#endif
    if(isLampOn == true)
    {
        if(isAlarm == true)
        {
          if(lampState == 1)//红
           {
              lampControl(1);
              msleep(delayMs);
              lampControl(2);
              msleep(delayMs);
           }
          else if(lampState == 2)//黄
           {
              lampControl(3);
              msleep(delayMs);
              lampControl(4);
              msleep(delayMs);
           }
          else if(lampState == 3)//绿
           {
              lampControl(5);
              msleep(delayMs);
              lampControl(6);
              msleep(delayMs);
           }
          isGreen = false;
        }
        else
        {
            lamp_sem.acquire();
            if(isGreen == false)
            {
                if(lampState == 1)//红
                 {
                    lampControl(1);
                 }
                else if(lampState == 2)//黄
                 {
                    lampControl(3);
                 }
                else if(lampState == 3)//绿
                 {
                    lampControl(5);
                 }
             isGreen = true;
            }
            msleep(500);
        }
    }
    else
    {
        lamp_sem.acquire();
        lampControl(2);
        msleep(500);
    }

#if DEBUG_OTHER_lampState
    Debug_insert("lampTask::run_out");
#endif
    }
    close(lamp_fd);
#endif
}

void lampTask::flash(int model, int interval)
{
    if(lampState == model && delayMs == interval && isAlarm == true)
        return;

    lampState = model;
    isAlarm = true;
    lampState = 0;
    delayMs = interval;
    lamp_sem.release();
}

void lampTask::stop(int model)
{
    if(lampState == model && isAlarm == false)
        return;

    isAlarm = false;
    lampState = model;
    lamp_sem.release();
}

void lampTask::ledon() //开启LED
{
    isLampOn = true;
    lamp_sem.release();
}

void lampTask::ledoff()  //关闭LED
{
    lampControl(2);
    isLampOn = false;
}
