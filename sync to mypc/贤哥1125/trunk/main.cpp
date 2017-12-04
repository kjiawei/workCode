#include <QApplication>
#ifdef Q_WS_QWS
#include <QWSServer>
#endif
#include <signal.h>
#include "config_ui.h"
#include "data_manage.h"
#include "window_boot.h"
#include "window_main.h"
#include "widget_wlan.h"
#include "Bronze_Style.h"
#include "widget_common.h"
#include "xml_config.h"
#include "common_func.h"
#include "state.h"
#include "global_var.h"
#include "Widget_UsbStorage_Signal.h"
#include "engineering_mode.h"
#include "buttons.h"
#include "middle.h"

#include "./src/httpserver/httpserver.h"
#include <QTextCodec>
#include <QtCore/QCoreApplication>

#include <ctime>
#include <sys/time.h>
#include <stdio.h>
#include <syslog.h>
#include "Daemon.h"

int main(int argc, char *argv[])
{
    argv[1]=(char*)"-qws";
 //   argc=2;
#if 0 //just test
    time_t timep;

    time(&timep);
    printf("%s\n", asctime(gmtime(&timep)));

    printf("TEST: V for male (50 years old, 176cm height, 75kg weight) is %f\n", V_MALE(50, 176, 75));
    printf("TEST: V for female (165cm height, 50kg weight) is %f\n\n", V_FEMALE(165, 50));
#endif
    //syslog(LOG_INFO, "test-%m\n");
    ///无线网卡插拔监控#/
    ///通过signal将信号与处理函数绑定#/
    ///添加和移除无线网卡设备时，分别发出ADD和REMOVE两个信号#/
//    APP_NetLink_Init();

    //U盘插拔监控#/
    ///同样通过signal方式实现#/
//    APP_UsbStorage_Signal_Init();

    //setCodecForTr 为Tr函数设置编码格式，Tr函数是QT为了实现字符串多国语言快速转换功能。#/
    //Tr包含的内容可以通过lupdate生成Ts文件，然后通过QT Linguist工具翻译Ts文件，可以将字符串hello转换为你好之类汉字#/
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    //定义一些控件在整个应用程序中的样式#/
    QApplication::setStyle(new BronzeStyle);

    //判断重生标志#/
    if(argc>=2&&(argv[2]!=NULL))
    {
        printf("argv[2]==%s \n", argv[2]);

        if(strcmp("restart",argv[2])==0)
        {
            printf("restart==OK \n");

            G_rebirth_flag=true; //20151029重生模块--重生标志#/
        }
    }

    PID_Process_Handle();  //实现监测守护子进程#/

    //创建一个QT应用程序类#/
    //其后所有的界面程序均在此基础上运行，这个类包含了很多QT的运行机制#/
    QApplication app(argc, argv);

    //通过事件过滤器实现了应用程序在特定事件处理指定事件#/
    //这里实现了外接键盘时，按F11可以进行触摸屏校正#/
    FunctionsKeyEater FunctionsKeyFilter;
    app.installEventFilter(&FunctionsKeyFilter);

#ifdef Q_WS_QWS
    QWSServer::setCursorVisible(false);
#endif
    //初始化资源文件#/
    Q_INIT_RESOURCE(hemodialysisviewer);

    ic = new InputPanelContext;

    //设置应用程序的输入上下文，主要是重写Qinputcontext类#/
    app.setInputContext(ic);

    //告警声音处理类，继承于QThread
    //为了实时性考虑，告警声音的处理在一个线程循环任务中进行#/

    audio = new audioserver();
    //LED显示处理类，继承于QTHread#/
    //通过一个线程实现处理#/
    lamp = new lamptask();

    //按键中断处理，使用SIGIO 信号驱动方式#/
    //自定义了一个buttonsirq类，当有按键发生时，通过该类以信号形式发出#/
    //需要使用按键的类，绑定该信号便可#/
    ButtonsInit();

    //一个定时器管理类，将定时器统一管理#/
    //该类通过队列容器（QMap）实现定时器的创建，销毁等管理#/
    //最终通过QTimer实现一个10ms间隔的定时器，然后通过该定时器不断#/
    //轮询队列中准备好的定时器，实现多定时器管理功能#/
    timer = new data_timer();

    //创建所有相应的数据包管理类，管理类包含具体数据包，#/
    //并通过QHash，将数据包管理类和自定义的字符串名称关联起来#/
    //并对其中涉及底层通信的数据包添加具体的操作处理#/
#if HJX_COMMUNICATION_SWITCH
    dataCanSend = new DataCanSend();
    dataManage = new DataManage();
#else
    dataManage = new DataManage();
#endif

    //开启状态处理线程#/
    //这里将每个总状态作为一个任务类，共4个，准备治疗、治疗、结束治疗、消毒#/
    //可以通过调用 StateSwitch函数，设置当前运行的状态#/
    //并循环进行处理#/
    StateManageStart();

    //收到终止信号后，退出该QT程序#/
    signal(SIGINT, MainQuit);
    signal(SIGTERM,MainQuit);

#if SYSTEM_DEBUG_FLAG
    Debug_init();
#endif

    //只做父窗口，没有任何操作#/
    window_parent = new WindowBottom();
    window_parent->resize(CONFIG_SCREEN_WIDTH, CONFIG_SCREEN_HIGH);

#ifdef HW_ARM
    //设置无边框，并且不能移动和重新改变大小
    window_parent->setWindowFlags(Qt::FramelessWindowHint );
#endif

    //运行日志记录和数据库处理类#/
    LogRun=new LogRunModel();

    //工程模式页面#/
    enginerDialog = new Dialog(window_parent);
    enginerDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint );

    //自定义的一个提示框，用于主界面和工作模式界面#/
//    msgBox= new MsgBox();
    msgBox=new QMessageBox();

    //创建数据曲线控件#/
#if  Widget_Data_Curve_Switch
//    curveWidget=new Widget_Data_Curve(window_parent);
#endif
     curveWidget=new Widget_Data_Curve(window_parent);

    //开机后，自检是否跳过窗口，进行一些自检前的操作处理#/
    WindowBoot windowBoot(window_parent);
    //windowBoot(window_parent);

    //主页面，所有页面的实现
    WindowMain windowMain(window_parent);

    //点击设置按钮，弹出二级菜单后，再点其他地方，能重新隐藏二级菜单#/
    windowMain.connect(&FunctionsKeyFilter, SIGNAL(mouseEvent(int)), &windowMain.widgetCross, \
                           SLOT(mouseClicked(int)));

    //水路状态图和表格#/
    WindowStatusMap windowStatusMap(window_parent);

    //工程页面
    Dialog windowEngineer(window_parent);

    //显示父窗口
    window_parent->show();
    //MarkDisinfectFinish(true);//delete 标记消毒完成#/
    if(DisinfectFinishLastTime()||(G_rebirth_flag==true))//
    {
//        PageShow("Boot");     //显示启动页面#/
        Disinfection_No_completed=false;
    }
    else{
        Disinfection_No_completed=true;
//        PageShow("Boot");
//        PageShow("Disinfect");
    }//消毒没有完成，直接进入消毒界面，且在消毒完成之前不能且换到其他界面，#/

    PageShow("Boot");     //显示启动页面#/

    //报警处理模块#/
    alarmmodule=new AlarmModule();
    //调整输入面板的位置#/
    InputPanelContext::setPosition(580, 70);

    //测试无线网卡是否正常工作，并根据结果显示图标#/
//    APP_NetLink_Qiut();
    //判断U盘是否存在，并设置标志变量#/
//    APP_UsbStorage_Signal_Quit();

    //根据xml文件更新参数#/
    ReUpdateXmlConfig();

    //统一排列方向#/
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");

    qRegisterMetaType<QAbstractItemDelegate::EndEditHint>("QAbstractItemDelegate::EndEditHint");

#if SYSTEM_DEBUG_FLAG
    Debug_setuptrap();   //调试函数
#endif

    struct stat st;
    if(G_rebirth_flag==true)
    {
        G_P_REBIRTH=new STR_Rebirth();

        int fd,len;
        bool success_flag=false;  //true 可成功初始化，false 不可成功初始化#/

        memset(&st,0,sizeof(st));
        if(!stat("./param.dat",&st)){//如果log_url不存在，stat就会返回非０
            if(st.st_size > 0){//加了一层保证

                if((fd=open("./param.dat",O_RDONLY|O_NONBLOCK,0666))<0)
                {
                     success_flag=false;
                     printf("open param.dat fail \n");
                }
                else
                {
                     printf("st.st_size=%d \n",(int)st.st_size);
                     printf("STR_Rebirth=%d \n",sizeof(STR_Rebirth));

                    if(st.st_size==sizeof(STR_Rebirth))
                    {
                        len=read(fd,G_P_REBIRTH,sizeof(STR_Rebirth));

                        close(fd);

                        success_flag=true;

                        printf("read size=%d \n",len);
                        printf("success_flagt==true \n");
 //                       printf("FLuid01==%d \n",G_P_REBIRTH->Fluid.Wspeed);
 //                       printf("SubBPump01==%d \n", G_P_REBIRTH->SubBPump.N10_set_speed);
                        printf("UFAim01==%d \n", G_P_REBIRTH->packetUF.ufVolAim);
                    }
                    else
                    {
                        success_flag=false;
                        printf("read param.dat fail \n");
                    }

                    unlink("./param.dat");//删除恢复文件#/
                }
            }
        }

        if(success_flag==true)
        {
            dietimes=G_P_REBIRTH->dietimes;         //死机次数#/
            rebirthtimes=G_P_REBIRTH->rebirthtimes;     //恢复次数#/
            dietimes +=1;
            rebirthtimes +=1;
            //写XML文件特定节点内容#/
            Write_XML_Node(QString("/Config/Engineer_Factoryset/Rebirth"),QString("Bir"),QString::number(rebirthtimes));
            Write_XML_Node(QString("/Config/Engineer_Factoryset/Rebirth"),QString("Die"),QString::number(dietimes));
        }

        if(success_flag==false)  //恢复失败#/
        {
            windowBoot.Selfcheck_Pass();  //跳过自检，直接进入主界面#/
            windowBoot.startRecive();
        }
        else if(success_flag==true&&(G_P_REBIRTH->selfcheck_enable==false)) //无自检，则进入重生恢复#/
        {
            windowBoot.Selfcheck_Pass();  //跳过自检，直接进入主界面#/
            G_WORKMODE_VAL=G_P_REBIRTH->G_WORKMODE_VAL;  //工作模式#/
            dataNodeUpdate(PACKET_NAME_WORKMODE,&G_WORKMODE_VAL);
            workstate=G_P_REBIRTH->workstate;   //工作状态设置#/
            SetWorkStateManage(&workstate);
            dataNodeUpdate(PACKET_REBIRTH_HANDLE,&fd);  //通知各页面进行重生操作#/
            dataNodeUpdate(PACKET_REBIRTH_START,&fd); //重生启动开始#/
            windowBoot.startRecive();
        }//无论恢复成功或失败有自检，则重头开始自检#/

        G_rebirth_flag=false;

        printf("FLuid03==%d \n",G_P_REBIRTH->Fluid.Wspeed);
        printf("SubBPump03==%d \n", G_P_REBIRTH->SubBPump.N10_set_speed);
        printf("UFAim03==%d \n", G_P_REBIRTH->packetUF.ufVolAim);

        delete G_P_REBIRTH; //释放内存#/
    }
    else
    {
        if(!stat("./param.dat",&st)){//如果log_url不存在，stat就会返回非０
            dietimes +=1;
            Write_XML_Node(QString("/Config/Engineer_Factoryset/Rebirth"),QString("Die"),QString::number(dietimes));
            unlink("./param.dat");//删除恢复文件#/
        }
    }

    HttpServer server;

    if(!server.listen(8000))  //监听端口号
    {
        qDebug()<<"http listen failed !!";
        exit(-1);
    }
    else
        qDebug()<<"http listen ok !!";


    return app.exec();//QT循环处理任务#/
}
