#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include "public.h"
#include <sys/types.h>
#include <sys/wait.h>

extern int updateWhat;

void MainWindow::setUi(void)
{
    fontlabelProgress.setFamily("Droid Sans Mono");
    fontlabelProgress.setWeight(QFont::Normal);
    fontlabelProgress.setPointSize(16);

    for (int i = 0; i < 10; i++) {
        dis_updating[i].setStyleSheet("QLabel {border-style:none;background-color:transparent;color:#ffffff;}");
        dis_updating[i].setAlignment(Qt::AlignTop);
        dis_updating[i].setGeometry(20,100 + i * 32,1001, 61);
        dis_updating[i].setFont(fontlabelProgress);
    }

}

void MainWindow::setdistext(QString disStr, int labelNum)
{
    dis_updating[labelNum].setText(disStr);
    update();
}

int MainWindow::unPackZip(const char * path)
{
    QString zipPath(path);
    QString cmd;

    setdistext("unZip the update packet ...", 0);
    eventHandleDelay();

    zipPath = zipPath.mid(0, zipPath.lastIndexOf("/"));
    cmd.append("rm -rf ").append(zipPath).append("/update");
    system(cmd.toLatin1().data());
    qDebug()<<"rm cmd "<<cmd;

    cmd.clear();
    cmd.append("unzip ").append(path).append(" -d ").append(zipPath);
    qDebug()<<"unzip cmd "<<cmd;
    system(cmd.toLatin1().data());

    return 0;
}

void MainWindow::updateSystem(const char* path)
{
    QStringList pathList;
    QString pkgPath(path);

//    pkgPath = pkgPath.mid(0, pkgPath.lastIndexOf("/"));
//    pkgPath.append("/update");
//    qDebug()<<"inupdate sys"<<pkgPath;

    QDir dir(pkgPath);

    if(!dir.exists())
    {
        return ;
    }

    QStringList filters;
    filters << "*.hex" << "*.pkg";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Type);

    pathList = dir.entryList();
    QString tempStr;
    tempStr.append("will update these packets:");
    setdistext(tempStr, 0);

    tempStr.clear();
    tempStr.append(pathList.join(" "));
    setdistext(tempStr, 1);

    int file_count = pathList.count();
    if( file_count<= 0)
    {
        setdistext(tempStr, 0);
        setdistext(" ", 1);
        return ;
    }

    for (int i = 0; i < file_count; i++) {
        if (pathList.at(i).mid(pathList.at(i).lastIndexOf(".")) == ".hex") {
            QString cmd("can-update ");

            cmd.append(pkgPath).append("/");
            cmd.append(pathList.at(i));
            qDebug()<<"cmd 1  is"<<cmd;
            tempStr.clear();
            tempStr.append("Start updating ").append(pathList.at(i)).append(" ...");
            setdistext(tempStr, i + 2);
            eventHandleDelay();

            updateWhat = 0;

            system(cmd.toLatin1().data());

            tempStr.clear();
            tempStr.append("update ").append(pathList.at(i));
            if(updateWhat == 100)
                tempStr.append(" success");
            else
                tempStr.append(" failure");

            setdistext(tempStr, i + 2);

        } else if (pathList.at(i).mid(pathList.at(i).lastIndexOf(".")) == ".pkg") {
            QString cmd("arm-update -t JH5058  ");
            cmd.append(pkgPath).append(" ").append(pkgPath);
            qDebug()<<"cmd 2 is"<<cmd;
            setdistext("Start updating ARM ...", i + 2);
            eventHandleDelay();

            system(cmd.toLatin1().data());

            tempStr.clear();

            if ((updateWhat >> 8) == UPDATEFINISH)
                tempStr.append("update finish, system reboot now!");
            else
                tempStr.append("update ARM failure!");
            setdistext(tempStr, i + 2);
        }

    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    for (int i = 0; i <10; i++) {
        dis_updating[i].setParent(this);
    }
    ui->setupUi(this);
    setUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::eventHandleDelay(void)
{
    QElapsedTimer waitTimer;
    waitTimer.start();
    while(waitTimer.elapsed()<500)
        QCoreApplication::processEvents();
}
