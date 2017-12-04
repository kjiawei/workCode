#include <QtGui>
#include "config_ui.h"
#include "global_var.h"
#include "scene_setup_EC.h"
#include "scene_setup_layout.h"

SceneSetupEC::SceneSetupEC(QWidget *parent) :
    QObject(parent),
    labelNaC(parent),
    labelNaCValue(parent),
    labelNaCUnit(parent),
    labelEC(parent),
    labelECValue(parent),
    labelECUnit(parent)
{
    SCENE_SETUP_CREATE_TABLE(labelNaC, 1);
    SCENE_SETUP_CREATE_VALUE(labelNaCValue, 1);
    SCENE_SETUP_CREATE_UNIT(labelNaCUnit, 1);
    labelNaC.setText(tr("Na+:"));
    labelNaCValue.setNum(0);
    labelNaCUnit.setText("mmol/L");

    SCENE_SETUP_CREATE_TABLE(labelEC, 2);
    SCENE_SETUP_CREATE_VALUE(labelECValue, 2);
    SCENE_SETUP_CREATE_UNIT(labelECUnit, 2);
    labelEC.setText(tr("Cond:"));
    labelECValue.setNum(0);
    labelECUnit.setText("mS/cm");

    CustomSetText();
    PageAppendPiece("Setup Cond", this);
#if  Widget_Data_Curve_Switch
    //位置和大小#/
    curveWidget->setParent(parent);

#endif

}

void SceneSetupEC::CustomSetText()
{
    labelEC.setText(tr("Cond:"));
    labelNaC.setText(tr("Na+:"));
}

void SceneSetupEC::curve_Handler()  //曲线刷新处理#/
{
    /*----------------------数据曲线示例----------------------------------*/
        QList<QPointF> point;
        QString unitx,unity1,unity2;
        unitx.append("H");
        unity1.append("cm");
        unity2.append("g");
        //位置和大小#/
        curveWidget->resetSize(170,220,660,360);
        curveWidget->setMargin(50,20);
        curveWidget->enableYaxis02(true);
        curveWidget->setFontWidth(15,10);
        curveWidget->setAxisTickNum(5,6,7);
        curveWidget->delAllCurveLine();  //删除原所有曲线#/
        curveWidget->setXunitName(unitx);//设置X轴单位名称#/
        curveWidget->setY1unitName(unity1);//设置Y1轴单位名称#/
        curveWidget->setY2unitName(unity2);//设置Y2轴单位名称#/


        //离散点数据#/
        point.append(QPointF(1,10));
        point.append(QPointF(2,20));
        point.append(QPointF(3,30));
        point.append(QPointF(3.5,30));
        point.append(QPointF(4,60));
        point.append(QPointF(4.2,35));
        point.append(QPointF(4.6,40));
        point.append(QPointF(5,28));
        point.append(QPointF(5.2,36));
        point.append(QPointF(6,35));
        point.append(QPointF(6.6,40));
        point.append(QPointF(7,28));
        point.append(QPointF(7.5,36));
        //新添加一条曲线，曲线类型为离散点#/
        curveWidget->addCurveLine(0,Pointtype,QString("point"),QColor(Qt::yellow));
        curveWidget->setCurveData(0, point);

        point.clear();
        //阶梯线数据#/
        point.append(QPointF(0,15));
        point.append(QPointF(1,15));
        point.append(QPointF(1,20));
        point.append(QPointF(2,20));
        curveWidget->addCurveLine(1,Laddertype,QString("Ladder"),QColor(Qt::red));
        curveWidget->setCurveData(1,point);

        point.clear();
        //高低柱状线#/
        point.append(QPointF(1,25));
        point.append(QPointF(1,30));
        point.append(QPointF(1.5,40));
        point.append(QPointF(1.5,50));
        point.append(QPointF(2,40));
        point.append(QPointF(2,50));
        point.append(QPointF(3,45));
        point.append(QPointF(3,55));
        point.append(QPointF(4.2,55));
        point.append(QPointF(4.2,65));
        point.append(QPointF(5,60));
        point.append(QPointF(5,65));
        point.append(QPointF(6,48));
        point.append(QPointF(6,58));
        curveWidget->addCurveLine(2,Columntype,QString("Column"),QColor(Qt::blue));
        curveWidget->setCurveData(2,point);
        curveWidget->setCurveYtype(2,Y2_AXIS_ID);//设置选中的曲线归属那个Y轴#/

        //点连线类型#/
        point.clear();
        point.append(QPointF(0,15));
        point.append(QPointF(1,20));
        point.append(QPointF(2,30));
        point.append(QPointF(3,25));
        point.append(QPointF(3.5,20));
        point.append(QPointF(4,30));
        point.append(QPointF(4.1,45));
        point.append(QPointF(4.2,20));
        point.append(QPointF(4.6,50));
        point.append(QPointF(4.9,25));
        point.append(QPointF(5.1,35));
        point.append(QPointF(5.6,20));
        point.append(QPointF(6,30));
        point.append(QPointF(6.5,25));
        curveWidget->addCurveLine(3,Linetype,QString("Line"),QColor(Qt::green));
        curveWidget->setCurveData(3,point);

        curveWidget->refresh(); //刷新并开始曲线绘制#/
}

void SceneSetupEC::setCusVisible(bool vis, char* para)
{
    para = para;
    labelNaC.setVisible(vis);
    labelNaCValue.setVisible(vis);
    labelNaCUnit.setVisible(vis);
    labelEC.setVisible(vis);
    labelECValue.setVisible(vis);
    labelECUnit.setVisible(vis);
#if  Widget_Data_Curve_Switch
    /*
    curveWidget->setVisible(vis);
    if(vis==true)
    {
        this->curve_Handler();
    }
    */
#endif
}
