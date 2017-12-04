#include "mainwindow.h"
#include <QApplication>
#include <sys/types.h>
#include <signal.h>
#include <QtGui>

#include <unistd.h>

int updateWhat;//0 updating 100 update success 101 update faile

void SignalRecv(int signum,siginfo_t *info,void*myact)
{
    updateWhat = info->si_value.sival_int;
}



int main(int argc, char *argv[])
{
    pid_t app_pid;
    QApplication a(argc, argv);
    MainWindow w;

    struct sigaction sa;
    sa.sa_sigaction = SignalRecv;
    sa.sa_flags |= SA_SIGINFO;
    sigemptyset(&sa.sa_mask);   // 清空阻塞信号
    sa.sa_flags=SA_SIGINFO;     // 设置SA_SIGINFO 表示传递附加信息到触发函数
    sigaction(SIGRTMIN,&sa,NULL);


//    app_pid = QString(argv[2]).toInt();
//    if (app_pid)
//        kill(app_pid,SIGKILL);
    chdir("/");

   w.setGeometry(0, 0, 1024, 768);
   w.show();
   w.eventHandleDelay();

//  w.unPackZip(argv[1]);
   w.updateSystem(argv[1]);

   return a.exec();
}
