#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_maintain_project_update.h"
#include "scene_maintain.h"

#include "Widget_UsbStorage_Signal.h"
#ifdef HW_ARM

#include <sys/stat.h>
#include <signal.h>
#include <wait.h>

#endif

int SceneProjectModeUpdate::updateData = 0 ;

inline void SceneProjectModeUpdate::setUi()
{

    if(updateMap.size() ==0 ) {
        updateMap.insert(UPDATE_BOOTSTRAP, "Updating file bootstrap.bin ");
        updateMap.insert(UPDATE_UBOOT, "Updating file uboot.bin ");
        updateMap.insert(UPDATE_PARA1, "Updating param1 ");
        updateMap.insert(UPDATE_PARA2, "Updating param2 ");
        updateMap.insert(UPDATE_LOGO, "Updating file logo.bin ");
        updateMap.insert(UPDATE_KERNEL, "Updating file kernel.bin ");
        updateMap.insert(UPDATE_ROOTFS, "Updating file rootfs.bin ");
        updateMap.insert(UPDATE_APPFS, "Updating file appfs.bin and  will reboot later");
        updateMap.insert(UNPACKFILE, "Starting unpack file ,please wait ... ");
        updateMap.insert(UNPACK_BOOTSTRAP, "Unpacking file bootstrap.bin ");
        updateMap.insert(UNPACK_UBOOT, "Unpacking file uboot.bin ");
        updateMap.insert(UNPACK_KERNEL, "Unpacking file kernel.bin ");
        updateMap.insert(UNPACK_ROOTFS ,"Unpacking file rootfs.bin ");
        updateMap.insert(UNPACK_APPFS, "Unpacking file appfs.bin ");
        updateMap.insert(UNPACK_DATA, "UNPACK ....UNPACK_DATA");
        updateMap.insert(UNPACKFINISH, "Unpack finish ,Please wait... ");
        updateMap.insert(UPDATEFINISH, "Updating finish and reboot now ...");
        updateMap.insert(UNPACKCHECKPKG, "Checking file update.pkg ...");
    }

    if(errorMap.size() == 0) {
        errorMap.insert(FILENOTFIND, "Cannot find the file update.pkg ...");
        errorMap.insert(FILEISNOTPKG, "The file is not a pkg file ...");
        errorMap.insert(MTDISBUSY, "/dev/mtd is busy ,update fail ");
        errorMap.insert(READWRITEERROR, "error to read or write file");
        errorMap.insert(INPUTPARAMERROR, "app input param error");
        errorMap.insert(UNPACK_ERROR, "unpack error");
        errorMap.insert(FILEPTAGNOTFIT, "the machine tag is not correct");
        errorMap.insert(UNPACKFILENOTFIND, "unpack file not find");
        errorMap.insert(CREATETMPFILEERROR, "error to create tmp file");
        errorMap.insert(UPDATEERROR, "update error");
        errorMap.insert(INPARMERROR, "Input Param error");
    }

    fontlabelProgress.setFamily("smallsmooth");
    fontlabelProgress.setWeight(QFont::Normal);
    fontlabelProgress.setPointSize(20);

    labelProgress.setStyleSheet("QLabel {border-style:none;background-color:transparent;color:#ffffff;}");
    labelProgress.setAlignment(Qt::AlignTop);
    labelProgress.setGeometry(30,70,810,680);
    labelProgress.setFont(fontlabelProgress);

//    scrollArea.setWidget(&labelProgress);
//    scrollArea.setGeometry(30,70,810,680);
//    scrollArea.setWidgetResizable(true);
//    scrollArea.setStyleSheet("QScrollArea {border-style:none;background-color:transparent;}");

    time_update =new QTimer(this);
    connect(time_update,SIGNAL(timeout()),this,SLOT(updateProgressTimer()));

    saveCurrentUpdateWhat = 0;
    updateData = 0;
}

SceneProjectModeUpdate::SceneProjectModeUpdate(QWidget *parent) :
    QObject(parent),

    labelProgress(parent)
{
    setUi();
    PageAppendPiece("Maintain Update", this);
}


bool SceneProjectModeUpdate::openSystemUpdate()
{
    int rev;
    labelProgress.clear();
    isUpdateing  = true;
    saveCurrentUpdateWhat = 0;
    saveCurrentUpdateData = 0;

    time_update->start(1);
    setText(0, 0, 0, "starting,please wait ... ");
    //qDebug() << "send updatesignal";
    rev=system("sendUpdateSignal &");

    return false;
}


void SceneProjectModeUpdate::updateProgressTimer()
{
    int msg_what = -1;
    if(updateData!=0 && saveCurrentUpdateData != updateData && isUpdateing == true){
        saveCurrentUpdateData = updateData;
        msg_what = (saveCurrentUpdateData & 0xff00)>>8;
        int msg_progress = saveCurrentUpdateData & 0x00ff;
        setText(msg_what, msg_progress, 0, "");
    }
    if(SceneMaintain::isUsbStroageExist == false && isUpdateing == true){
        isUpdateing = false;
        labelProgress.clear();
        QMessageBox::information(NULL, "Error", "U disk is accidentally pulled out,update failed", QMessageBox::Yes , QMessageBox::Yes);
        time_update->stop();
        PageShow("Maintain");
        bool ret = true;
        dataNodeUpdate(PACKET_MAINTAIN_RETURN, &ret);
    }
}

void SceneProjectModeUpdate::setText(int updateWhat, int updateProgress, int updateStatus, QString reverst)
{
    QString currentStr = labelProgress.text();
    if(saveCurrentUpdateWhat != updateWhat && updateWhat!=0){
        saveCurrentUpdateWhat = updateWhat;

    }else if(saveCurrentUpdateWhat == updateWhat){
        if(updateProgress == 100) {
           saveCurrentUpdateWhat = 0;
        }
        currentStr.remove(currentStr.length()-2, currentStr.length());
        currentStr.remove(currentStr.lastIndexOf("\r\n"), currentStr.length());
        currentStr.append("\r\n");
    }
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    QString currentDate = dt.toString("[hh:mm:ss]  ");
    currentStr.append(currentDate);
    if(updateWhat == 0) {
        currentStr.append(reverst).append("\r\n");
        labelProgress.setText(currentStr);
        return;
    }
    if(updateStatus != 0x00) {
        currentStr.append("update error,code is: ").append(updateStatus).append("\r\n");
        labelProgress.setText(currentStr);
        return;
    }
    if(updateMap.contains(updateWhat)){
        QMap<int,QString>::iterator it = updateMap.find(updateWhat);
        currentStr.append(it.value()).append("  ");
    } else {
        //qDebug()<< updateWhat;
        if(errorMap.contains(updateWhat)){
           QMap<int,QString>::iterator it_error = errorMap.find(updateWhat);
           currentStr.append(it_error.value()).append("  ");
        }else{
            currentStr.append("unknow error code : ").append(QString::number(updateWhat,10)).append("please check ... \r\n");
        }
        currentStr.append("\r\n update fail \r\n");
        labelProgress.setText(currentStr);
        saveCurrentUpdateWhat = 0;
        saveCurrentUpdateData = 0;
        isUpdateing = false;
        QMessageBox::information(NULL, "Error", "Update Fail ", QMessageBox::Ok , QMessageBox::Ok);
        time_update->stop();
        PageShow("Maintain");
        bool ret = true;
        dataNodeUpdate(PACKET_MAINTAIN_RETURN, &ret);
        return;
    }
    if(updateProgress!=-1 && updateWhat != UNPACKFILE && updateWhat != UNPACKFINISH
            && updateWhat != UPDATE_APPFS && updateWhat != UNPACKCHECKPKG
            && updateWhat != UPDATEFINISH ) {
        currentStr.append(QString::number(updateProgress,10)).append("\%").append("  ");
    }
    currentStr.append(reverst);
    currentStr.append("\r\n");
    labelProgress.setText(currentStr);
}

void SceneProjectModeUpdate::setCusVisible(bool vis, char* para)
{
    para = para;
    labelProgress.setVisible(vis);
}
