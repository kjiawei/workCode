#ifndef SCENE_SETUP_HCO3CURVE_H
#define SCENE_SETUP_HCO3CURVE_H

#include <QtGui>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "data_custom.h"
#include "config_ui.h"
#include "lang_switch.h"
#include "page_info.h"
#include "common_customwidget.h"


class SceneSetupHco3Curve;



/*-------------------------柱状图类-------------------------------*/
//每个治疗时段的柱状图类#/
class ColumHco3Curve: public QLabel
{
    Q_OBJECT

public:
    ColumHco3Curve(QWidget *parent,SceneSetupHco3Curve *p,int x,int y,int order);
    void setShow(bool vis);

protected:
    void mousePressEvent(QMouseEvent *ev); //点击事件#/
    void mouseMoveEvent(QMouseEvent *ev); //鼠标移动事件#/
    void mouseReleaseEvent(QMouseEvent *ev);//鼠标释放事件#/
    void focusInEvent(QFocusEvent *ev);//焦点选中#/
    void focusOutEvent(QFocusEvent *ev);//失去焦点#/
    void paintEvent(QPaintEvent *);//重绘事件#/

private:
    inline void setUi();

public:
    int orderNum;//本柱状图编号#/
    float naValue;//本治疗时段的钠浓度数值 单位 ms/cm#/
    float periodTime;//本治疗时段对应的时间长度 单位min#/
    char selectMark; //是否选择本柱状图#/

private:
    int xAxis; //本柱状图的x坐标#/
    int yAxis;//y坐标#/
    bool passMark; //点击标记#/
    SceneSetupHco3Curve *topscene; //为了操控上层页面的控件而定义#/
    QLabel labeltop;  //柱状图顶部显示数据 超滤率=平均超滤量*百分比 [ml/h]#/
};

/*-------------------------钠曲线显示编辑页面类-------------------------------*/
//钠曲线显示编辑页面类#/
class SceneSetupHco3Curve:public QObject, public ActionCustom,public PieceVisible,public LangSetText
{
    Q_OBJECT
public:
    SceneSetupHco3Curve(QWidget *parent=0); //构造函数#/
    ~SceneSetupHco3Curve();
    void CustomSetText(); //设置相应的显示文字#/
    void setCusVisible(bool vis=false,char* para=NULL);//显示调用函数#/
    void ActionHandle(const void* dataPtr=NULL, int name=0);

private:
    inline void setUi();

public:
    //相应的变量#/
    int nowSelectCurve; //当前选择的曲线号#/
    int nowSelectNum;   //当前选择的治疗时段#/
    ColumHco3Curve *curveArray[10]; //指向柱状图的指针，默认数量是10个#/
    //各种文本标签#/
    QLabel labelTitleTop;
    QLabel labelPrompt;
    QLabel labelYaxisUnit;
    QLabel labelLeftTop;
    QLabel labelLeftMiddle;
    QLabel labelRightTop;
    QLabel labelRightMiddle;
    QLabel labelCurveNum;
    QLabel labelSumUF;
    QLabel labelSumTime;
    QLabel labelPeriodTime;
    QLabel labelCurveTime;
    //y坐标轴标签#/
    QLabel label160;
    QLabel label151;
    QLabel label142;
    QLabel label134;
    QLabel label125;
    //每个治疗时段数值编辑框#/
    MyLineEdit lineEditSetNum;
    //页面相应的按钮#/
    QPushButton buttonAllAdd;//全加#/
    QPushButton buttonAllSub;//全减#/
    QPushButton buttonSelectAdd;//曲线选择+#/
    QPushButton buttonSelectSub;//曲线选择-#/
    QPushButton buttonSave; //保存#/
    QPushButton buttonOk;//应用按钮#/
    QPushButton buttonCancel;//返回按钮#/

public slots:
    void selectedCurve(int num);

protected slots:
    void lineEditInFocus(); //编辑框选中时#/
    void allAddClicked();//全加按钮点击时#/
    void allSubClicked();//全减按钮点击#/
    void selectAddClicked();
    void selectSubClicked();
    void buttonSaveClicked();//保存按钮#/
    void buttonOkClicked();//应用按钮点击#/
    void buttonCancelClicked();
};




#endif
