
#include <QtGui>
#include "config_ui.h"
#include "scene_bloodvolume.h"
#include "widget_common.h"
#include "scene_setup_layout.h"
#include "widget_switch.h"
void SceneBloodVolume::setUi(void)
{
    /*labelWeight.setGeometry(300,100,100,38);
    labelWeight.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelWeight.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelWeight.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelKg.setGeometry(450,100,50,38);
    labelKg.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelKg.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelKg.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);*/

    SCENE_SETUP_CREATE_TABLE(labelWeight, 2);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditWeight, 2);
    SCENE_SETUP_CREATE_UNIT(labelKg, 2);

    SCENE_SETUP_CREATE_TABLE(labelBloodVolume, 3);
    SCENE_SETUP_CREATE_LINEEDIT(lineEditBloodVolume, 3);

    //lineEditWeight.setGeometry(400,100,50,38);
    ic->appendWidget(&lineEditWeight,NumInputMethod);

    buttonApply.setGeometry(QRect(620, 500, 64, 39));
    BUTTON_SIMPLE_STYLE(buttonApply);

    CustomSetText();


}

SceneBloodVolume::SceneBloodVolume(QWidget *parent) :
    QWidget(parent),
    labelWeight(parent),
    lineEditWeight(parent),
    labelKg(parent),
    labelBloodVolume(parent),
    lineEditBloodVolume(parent),    
    buttonApply(parent)
{
    setUi();
    PageAppendPiece("BloodVolume", this);
    setMouseTracking(true);
    connect(&buttonApply,SIGNAL(clicked()),this,SLOT(on_buttonApply_clicked()));
    dataNodeAddObject(PACKET_NAME_BLOODVOLUME, this);
}

void SceneBloodVolume::on_buttonApply_clicked()
{

}

void SceneBloodVolume::lineEdit_init()
{

}

void SceneBloodVolume::CustomSetText()
{
    labelWeight.setText(tr("Dry weight:"));
    labelKg.setText(tr("Kg"));
    lineEditWeight.setText("");
    labelBloodVolume.setText(tr("BloodVolume"));
    lineEditBloodVolume.setText("");
    buttonApply.setText(tr("Apply"));
}

void SceneBloodVolume::setCusVisible(bool vis, char* para)
{
    para = para;

    labelWeight.setVisible(vis);
    labelKg.setVisible(vis);
    labelBloodVolume.setVisible(vis);
    lineEditBloodVolume.setVisible(vis);
    lineEditWeight.setVisible(vis);
    buttonApply.setVisible(vis);
}

void SceneBloodVolume::mouseMoveEvent (QMouseEvent * event)
{
    qDebug()<<"current point is "<<event->pos().x()<<","<<event->pos().y();
}

void SceneBloodVolume::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("bloodvolume-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }
    if (name==PACKET_NAME_BLOODVOLUME) {
        int *bloodVolumeTemp = (int*)dataPtr;
        float temp = 0;
        int weight = lineEditWeight.text().toInt();

        if(weight == 0)return;
        if(*bloodVolumeTemp == BLOODVOLUME_START){
            bloodVolumeTime = 0;
            thirtyMinUf = 0;
            thirtyMinBloodV = 0;
        }
        else if(*bloodVolumeTemp == BLOODVOLUME_RUNING){
            if(bloodVolumeTime<=THIRTY_MIN){//定时器设置为1分钟，小于等于30分钟用以下算法，并记录30分钟时的超滤量和血容值
                temp = FULLB_LOOD_PARA*weight*1000;
                if(temp>0){
                    temp = 1-(1-BEFORE_THIRTY_MIN_PUR)*G_UF_CUREFINISH/temp;
                    temp = (temp*100);
                }
                else{temp = 0;}
                if(THIRTY_MIN == bloodVolumeTime){
                    thirtyMinUf = G_UF_CUREFINISH;
                    thirtyMinBloodV = temp;
                }
            }
            else{
                temp = FULLB_LOOD_PARA*weight*1000;
                if(temp>0){
                    int i=G_UF_CUREFINISH-thirtyMinUf;
                    temp = (100*(1-AFTER_THIRTY_MIN_PUR)*i/temp);
                    temp = thirtyMinBloodV-temp;
                }
                else{temp = 0;}
            }
        }
        else if(*bloodVolumeTemp == BLOODVOLUME_STOP){
            qDebug("Stop");
        }
        QString str;
        lineEditBloodVolume.setText((str.setNum((int)temp))+"%");//更新显示的血容值
        bloodVolumeTime++;
    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("bloodvolume-ActionHandle-out");
#endif
}

