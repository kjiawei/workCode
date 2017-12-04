#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_condcurve.h"
#include "common_tr_string.h"
#include "curve_datahandle.h"
#include "global_var.h"
#include "ScreenSnaper.h"
#include "data_can_send.h"

//曲线绘制开始坐标#/
#define SCENE_SETUP_CURVE_START_X              205
#define SCENE_SETUP_CURVE_START_Y              185
#define SCENE_SETUP_CURVE_STRIP_WIDTH          40
#define SCENE_SETUP_CURVE_STRIP_HIGH           280



//#define CONFIG_STYLE_CHARS  "border:2px groove gray;color:black;background-color:#1A2131"


/*-------------------------柱状图类----------------------------------*/
//界面设置函数#/
inline void ColumCondCurve::setUi()
{
    this->setGeometry(xAxis,yAxis,
                      SCENE_SETUP_CURVE_STRIP_WIDTH,SCENE_SETUP_CURVE_STRIP_HIGH);
    this->setStyleSheet("border-width: 3px; border-style: outset; \
                         border-color: gray; background-color: #237d3a");

    labeltop.setGeometry(xAxis-3,yAxis-25,50,20);
    labeltop.setAlignment(Qt::AlignHCenter | Qt::AlignHCenter);
    labeltop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                           CONFIG_FONT_NORMAL));
    labeltop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
}

//构造函数#/
ColumCondCurve::ColumCondCurve(QWidget *parent,SceneSetupCondCurve *p,int x,int y,int order):
QLabel(parent),orderNum(order),xAxis(x),yAxis(y),topscene(p),labeltop(parent)
{
    setUi();
    setFocusPolicy(Qt::ClickFocus); //设置该控件有点击焦点，才能触发focusEvent#/
    selectMark=0;
    passMark=false;
    naValue=0;
    periodTime=0;
}

//鼠标点击处理事件，标准触摸屏点击也是调用这个事件#/
void ColumCondCurve::mousePressEvent(QMouseEvent *ev)
{
    if(selectMark==0)
        return;
    if(selectMark==1)  //为了避免选中时会改变柱状图的值，特加多一个标记数#/
    {
        selectMark=2;
        return;
    }

    passMark=true;

    naValue=usr_params.Params_Dialyser.Cond.attribute.max_value-((ev->y()-3)*((usr_params.Params_Dialyser.Cond.attribute.max_value-usr_params.Params_Dialyser.Cond.attribute.min_value)/(SCENE_SETUP_CURVE_STRIP_HIGH-6.0)));

//    naValue=16-((ev->y()-3)/6.66)*0.1;
    update();
//  ev->ignore();   //忽略该事件#/
//  qDebug()<<"mousePressEvent";
}

//鼠标移动事件#/
void ColumCondCurve::mouseMoveEvent(QMouseEvent *ev)
{
    if(passMark==false)
        return;
    naValue=usr_params.Params_Dialyser.Cond.attribute.max_value-((ev->y()-3)*((usr_params.Params_Dialyser.Cond.attribute.max_value-usr_params.Params_Dialyser.Cond.attribute.min_value)/(SCENE_SETUP_CURVE_STRIP_HIGH-6.0)));
//    naValue=16-((ev->y()-3)/6.66)*0.1;
    update();
//  ev->ignore();   //忽略该事件#/
//  qDebug()<<"mouseMoveEvent";
}

//鼠标释放事件#/
void ColumCondCurve::mouseReleaseEvent(QMouseEvent *ev)
{
    passMark=false;
    ev->ignore();
//  qDebug()<<"mouseReleaseEvent";
}

//焦点进入#/
void ColumCondCurve::focusInEvent(QFocusEvent *ev)
{

    selectMark=1; //标记选中该柱状图#/
    this->setStyleSheet("border-width: 3px; border-style: inset; \
                           border-color: blue; background-color: #237d3a");
    QString str(DE_TR_SELECTING);
    str=str+" "+QString().setNum(orderNum+1,10)+"\n";
    str=str+QString().setNum(periodTime,'f',1)+"[min]";
    topscene->labelCurveTime.setText(str);
    topscene->labelCurveTime.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);
    topscene->nowSelectNum=orderNum;
    ev->ignore();
//  qDebug()<<"focusInEvent";
}

//焦点移出#/
void ColumCondCurve::focusOutEvent(QFocusEvent *ev)
{
    ev=ev;
    selectMark=0;
    this->setStyleSheet("border-width: 3px; border-style: outset; \
                        border-color: gray; background-color: #237d3a");
//  qDebug()<<"focusOUTEvent";
}

//重绘事件#/
void ColumCondCurve::paintEvent(QPaintEvent *)
{
    QPainter paint(this);

    naValue=naValue>usr_params.Params_Dialyser.Cond.attribute.max_value?usr_params.Params_Dialyser.Cond.attribute.max_value:naValue;
    naValue=naValue<usr_params.Params_Dialyser.Cond.attribute.min_value?usr_params.Params_Dialyser.Cond.attribute.min_value:naValue;
    float high=((usr_params.Params_Dialyser.Cond.attribute.max_value-naValue)*1.0/(usr_params.Params_Dialyser.Cond.attribute.max_value-usr_params.Params_Dialyser.Cond.attribute.min_value))*(SCENE_SETUP_CURVE_STRIP_HIGH-6.0);
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(QColor(166,166,166)));
    paint.drawRect(3,3,SCENE_SETUP_CURVE_STRIP_WIDTH-6,high);
    if(topscene->nowSelectNum==orderNum)
        topscene->lineEditSetNum.setText(QString().setNum(naValue,'f',1));

    labeltop.setText(QString().setNum(naValue,'f',1));

    /*
    naValue=naValue>16?16:naValue;
    naValue=naValue<12?12:naValue;
    float high=((16-naValue)/(16-12))*(SCENE_SETUP_CURVE_STRIP_HIGH-6);
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(QColor(166,166,166)));
    paint.drawRect(3,3,SCENE_SETUP_CURVE_STRIP_WIDTH-6,high);
    if(topscene->nowSelectNum==orderNum)
        topscene->lineEditSetNum.setText(QString().setNum(naValue,'f',2));
    */

//  qDebug()<<"paintEvent";
}

    //显示#/
void ColumCondCurve::setShow(bool vis)
{
    this->setVisible(vis);
    labeltop.setVisible(vis);
}

/*-------------------------曲线页面显示类-------------------------------*/
//界面设置函数#/
inline void SceneSetupCondCurve::setUi()
{

    /*
    labelTitleTop.setGeometry(255,70,350,40);
    labelTitleTop.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelTitleTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_SECOND,   //字体设置过大容易导致黑屏#/
                                CONFIG_FONT_WEIGHT));
//    labelTitleTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
 //                               CONFIG_FONT_WEIGHT));
    labelTitleTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelPrompt.setGeometry(180,105,500,35);
    labelPrompt.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelPrompt.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                                CONFIG_FONT_WEIGHT));
    labelPrompt.setStyleSheet(CONFIG_COLOR_RED_CHARS);

    labelYaxisUnit.setGeometry(115,140,70,40);
    labelYaxisUnit.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelYaxisUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                                CONFIG_FONT_WEIGHT));
    labelYaxisUnit.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelLeftTop.setGeometry(20,160,100,80);
    labelLeftTop.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelLeftTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_WEIGHT));
    labelLeftTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    lineEditSetNum.setGeometry(20,250,80,40);
    LINEEDIT_STYLE(lineEditSetNum);

    labelLeftMiddle.setGeometry(20,295,100,80);
    labelLeftMiddle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelLeftMiddle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                  CONFIG_FONT_WEIGHT));
    labelLeftMiddle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    buttonAllAdd.setGeometry(20,375,60,40);
    BUTTON_SIMPLE_STYLE(buttonAllAdd);

    buttonAllSub.setGeometry(20,435,60,40);
    BUTTON_SIMPLE_STYLE(buttonAllSub);

    label160.setGeometry(115,170,60,38);
    label160.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label160.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label160.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label151.setGeometry(115,230,60,38);
    label151.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label151.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label151.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label142.setGeometry(115,290,60,38);
    label142.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label142.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label142.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label134.setGeometry(115,350,60,38);
    label134.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label134.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label134.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label125.setGeometry(115,410,60,38);
    label125.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label125.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label125.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    labelSumUF.setGeometry(20,510,130,90);
    labelSumUF.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelSumUF.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_WEIGHT));
    labelSumUF.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelSumTime.setGeometry(180,510,160,90);
    labelSumTime.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelSumTime.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_WEIGHT));
    labelSumTime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelPeriodTime.setGeometry(370,510,150,90);
    labelPeriodTime.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelPeriodTime.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_WEIGHT));
    labelPeriodTime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelCurveTime.setGeometry(560,510,140,90);
    labelCurveTime.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelCurveTime.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_WEIGHT));
    labelCurveTime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelRightTop.setGeometry(710,155,120,80);
    labelRightTop.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelRightTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_WEIGHT));
    labelRightTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelCurveNum.setGeometry(710,245,130,40);
    labelCurveNum.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelCurveNum.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                               CONFIG_FONT_WEIGHT));
    labelCurveNum.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelRightMiddle.setGeometry(710,290,120,80);
    labelRightMiddle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelRightMiddle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_WEIGHT));
    labelRightMiddle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
*/

    labelTitleTop.setGeometry(255,70,350,40);
    labelTitleTop.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelTitleTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,   //字体设置过大容易导致黑屏#/
                                CONFIG_FONT_NORMAL));
    labelTitleTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelPrompt.setGeometry(180,105,500,35);
    labelPrompt.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelPrompt.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                                CONFIG_FONT_NORMAL));
    labelPrompt.setStyleSheet(CONFIG_COLOR_RED_CHARS);

    labelYaxisUnit.setGeometry(115,140,70,40);
    labelYaxisUnit.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelYaxisUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                                CONFIG_FONT_NORMAL));
    labelYaxisUnit.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelLeftTop.setGeometry(20,160,100,80);
    labelLeftTop.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelLeftTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_NORMAL));
    labelLeftTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    lineEditSetNum.setGeometry(20,250,80,40);
    LINEEDIT_STYLE(lineEditSetNum);

    labelLeftMiddle.setGeometry(20,295,100,80);
    labelLeftMiddle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelLeftMiddle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                  CONFIG_FONT_NORMAL));
    labelLeftMiddle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    buttonAllAdd.setGeometry(20,375,60,40);
    BUTTON_SIMPLE_STYLE(buttonAllAdd);

    buttonAllSub.setGeometry(20,435,60,40);
    BUTTON_SIMPLE_STYLE(buttonAllSub);

    label160.setGeometry(SCENE_SETUP_CURVE_START_X-60,170,60,38);
    label160.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label160.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label160.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label151.setGeometry(SCENE_SETUP_CURVE_START_X-60,230,60,38);
    label151.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label151.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label151.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label142.setGeometry(SCENE_SETUP_CURVE_START_X-60,290,60,38);
    label142.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label142.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label142.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label134.setGeometry(SCENE_SETUP_CURVE_START_X-60,350,60,38);
    label134.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label134.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label134.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label125.setGeometry(SCENE_SETUP_CURVE_START_X-60,410,60,38);
    label125.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label125.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label125.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    labelSumUF.setGeometry(20,510,130,90);
    labelSumUF.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelSumUF.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_NORMAL));
    labelSumUF.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelSumTime.setGeometry(180,510,160,90);
    labelSumTime.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelSumTime.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_NORMAL));
    labelSumTime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelPeriodTime.setGeometry(370,510,150,90);
    labelPeriodTime.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelPeriodTime.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_NORMAL));
    labelPeriodTime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelCurveTime.setGeometry(560,510,140,90);
    labelCurveTime.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelCurveTime.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                             CONFIG_FONT_NORMAL));
    labelCurveTime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelRightTop.setGeometry(710,155,120,80);
    labelRightTop.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelRightTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_NORMAL));
    labelRightTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelCurveNum.setGeometry(710,245,130,40);
    labelCurveNum.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelCurveNum.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                               CONFIG_FONT_NORMAL));
    labelCurveNum.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelRightMiddle.setGeometry(710,290,120,80);
    labelRightMiddle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelRightMiddle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_NORMAL));
    labelRightMiddle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    buttonSelectAdd.setGeometry(710,375,85,40);
    BUTTON_SIMPLE_STYLE(buttonSelectAdd);

    buttonSelectSub.setGeometry(710,435,85,40);
    BUTTON_SIMPLE_STYLE(buttonSelectSub);

    buttonSave.setGeometry(160,650,80,40);
    BUTTON_SIMPLE_STYLE(buttonSave);

    buttonOk.setGeometry(370,650,80,40);
    BUTTON_SIMPLE_STYLE(buttonOk);

    buttonCancel.setGeometry(600,650,80,40);
    BUTTON_SIMPLE_STYLE(buttonCancel);
}

//构造函数#/
SceneSetupCondCurve::SceneSetupCondCurve(QWidget *parent):
QObject(parent),
labelTitleTop(parent),
labelPrompt(parent),
labelYaxisUnit(parent),
labelLeftTop(parent),
labelLeftMiddle(parent),
labelRightTop(parent),
labelRightMiddle(parent),
labelCurveNum(parent),
labelSumUF(parent),
labelSumTime(parent),
labelPeriodTime(parent),
labelCurveTime(parent),
label160(parent),
label151(parent),
label142(parent),
label134(parent),
label125(parent),
lineEditSetNum(parent),
buttonAllAdd(parent),
buttonAllSub(parent),
buttonSelectAdd(parent),
buttonSelectSub(parent),
buttonSave(parent),
buttonOk(parent),
buttonCancel(parent)
{
    setUi();

    //将软键盘和要输入的控制框建立联系#/
//    ic->appendWidget(&lineEditSetNum,NumInputMethod,LineEdit,NULL,12,16,
//                      0.01,"Na Value:","Na",false,INT);

      ic->appendWidget(&lineEditSetNum,NumInputMethod,LineEdit,NULL,11,18,
                        0.1,"Cond Value:","Cond",false,INT);

    //编辑框数值改变时#/
    connect(&lineEditSetNum,SIGNAL(InFocus()),this,SLOT(lineEditInFocus()));
    connect(&lineEditSetNum,SIGNAL(editingFinished()),this,SLOT(lineEditInFocus()));

    connect(&buttonAllAdd,SIGNAL(clicked()),this,SLOT(allAddClicked()));
    connect(&buttonAllSub,SIGNAL(clicked()),this,SLOT(allSubClicked()));
    connect(&buttonSelectAdd,SIGNAL(clicked()),this,SLOT(selectAddClicked()));
    connect(&buttonSelectSub,SIGNAL(clicked()),this,SLOT(selectSubClicked()));
    connect(&buttonSave,SIGNAL(clicked()),this,SLOT(buttonSaveClicked()));
    connect(&buttonOk,SIGNAL(clicked()),this,SLOT(buttonOkClicked()));
    connect(&buttonCancel,SIGNAL(clicked()),this,SLOT(buttonCancelClicked()));

 //   nowSelectCurve=0;
    for(int i=0;i<10;i++)
    {
        curveArray[i]=new ColumCondCurve(parent,this,SCENE_SETUP_CURVE_START_X+52*i,
                                        SCENE_SETUP_CURVE_START_Y,i);
        curveArray[i]->naValue=14;
        curveArray[i]->periodTime=0;
        curveArray[i]->setVisible(false);
    }

    nowSelectNum=-1;
    nowSelectCurve=0;
    lineEditSetNum.setText("0.0");
 //   this->CustomSetText();
    PageAppendPiece("Setup CondCurve",this);
}

SceneSetupCondCurve::~SceneSetupCondCurve()
{
    for(int i=0;i<10;i++)
    {
        if(curveArray[i]!=NULL)
            delete curveArray[i];
    }
}

//界面文字显示设置#/
void SceneSetupCondCurve::CustomSetText()
{

    labelTitleTop.setText(DE_TR_CONDPROFILE);

    labelLeftTop.setText(DE_TR_THESELECT);
    labelLeftMiddle.setText(DE_TR_SETALL);
    labelRightTop.setText(DE_TR_CURRENTCURVE);
    labelRightMiddle.setText(DE_TR_BROWSECURVE);


    labelSumUF.setText(DE_TR_SUMUF);
    labelSumTime.setText(DE_TR_TREATMENT);
    labelPeriodTime.setText(DE_TR_EACHPERIOD);
    labelCurveTime.setText(DE_TR_NOTSELECT);

//    labelYaxisUnit.setText("[ms/cm]");
    labelYaxisUnit.setText("ms/cm");


    label160.setText(QString::number(usr_params.Params_Dialyser.Cond.attribute.max_value)+"-");
    label151.setText(QString::number(((15.1-12)/(16-12))*(usr_params.Params_Dialyser.Cond.attribute.max_value-usr_params.Params_Dialyser.Cond.attribute.min_value)+usr_params.Params_Dialyser.Cond.attribute.min_value,'f',0)+"-");
    label142.setText(QString::number(((14.2-12)/(16-12))*(usr_params.Params_Dialyser.Cond.attribute.max_value-usr_params.Params_Dialyser.Cond.attribute.min_value)+usr_params.Params_Dialyser.Cond.attribute.min_value,'f',0)+"-");
    label134.setText(QString::number(((13.4-12)/(16-12))*(usr_params.Params_Dialyser.Cond.attribute.max_value-usr_params.Params_Dialyser.Cond.attribute.min_value)+usr_params.Params_Dialyser.Cond.attribute.min_value,'f',0)+"-");
    label125.setText(QString::number(((12.5-12)/(16-12))*(usr_params.Params_Dialyser.Cond.attribute.max_value-usr_params.Params_Dialyser.Cond.attribute.min_value)+usr_params.Params_Dialyser.Cond.attribute.min_value,'f',0)+"-");

    /*
    label160.setText("16.0-");
    label151.setText("15.1-");
    label142.setText("14.2-");
    label134.setText("13.4-");
    label125.setText("12.5-");
    */
    QString str;
    float sumtime=0;

    //总超滤量#/
    str=DE_TR_SUMUF+"\n"+QString().setNum(G_UF_CURETARGET)+"[ml]";
    labelSumUF.setText(str);

    //剩余治疗时间#/
    sumtime=(float)residue_secCount/60;
    str=DE_TR_TREATMENT+"\n"+QString().setNum((int)(total_secCount/3600));
    str=str+":"+QString().setNum((int)(total_secCount%3600)/60)+"[h:min]";
    labelSumTime.setText(str);

    //每个时段时间#/
    sumtime=sumtime/10; //默认是10个时段#/
    str=DE_TR_EACHPERIOD+"\n"+QString().setNum(sumtime,'f',1)+"[min]";
    labelPeriodTime.setText(str);

    for(int i=0;i<10;i++)
    {
        if(curveArray[i]!=0)
        {
            curveArray[i]->periodTime=sumtime;
        }
    }

    buttonAllAdd.setText("+");
    buttonAllSub.setText("-");
    buttonSelectAdd.setText(DE_TR_PREVIOUS);
    buttonSelectSub.setText(DE_TR_NEXT);
    buttonSave.setText(DE_TR_SAVE);
    if(G_COND_PROFILE_FLAG==false)
    {
        buttonOk.setText(DE_TR_APPLY);
    }
    buttonCancel.setText(DE_TR_RETURN);
}

//页面显示调用函数--#/
void SceneSetupCondCurve::setCusVisible(bool vis, char *para)
{
    para=para;

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupCondCurve::setCusVisible-in");
#endif

    if(vis==true)
    {
        CustomSetText();
    }


    for(int i=0;i<10;i++)
    {
        if(curveArray[i]!=0)
        {
            curveArray[i]->setShow(vis);
        }
    }

//    bool bfvis=vis;
//    vis=false;
    //各种文本标签#/
    labelTitleTop.setVisible(vis);
    labelPrompt.setVisible(vis);
    labelYaxisUnit.setVisible(vis);
    labelLeftTop.setVisible(vis);
    labelLeftMiddle.setVisible(vis);
    labelRightTop.setVisible(false);
    labelRightMiddle.setVisible(false);
    labelCurveNum.setVisible(false);
    labelSumUF.setVisible(vis);
    labelSumTime.setVisible(vis);
    labelPeriodTime.setVisible(vis);
    labelCurveTime.setVisible(vis);
    //y坐标轴标签#/
    label160.setVisible(vis);
    label151.setVisible(vis);
    label142.setVisible(vis);
    label134.setVisible(vis);
    label125.setVisible(vis);

//    vis=bfvis;
    //每个治疗时段数值编辑框#/
    lineEditSetNum.setVisible(vis);
    //页面相应的按钮#/
    buttonAllAdd.setVisible(vis);//全加#/
    buttonAllSub.setVisible(vis);//全减#/
    buttonSelectAdd.setVisible(false);//曲线选择+#/
    buttonSelectSub.setVisible(false);//曲线选择-#/
    buttonSave.setVisible(false);
    buttonOk.setVisible(vis);//应用按钮#/
    buttonCancel.setVisible(vis);//返回按钮#/


#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupCondCurve::setCusVisible-out");
#endif
}

void SceneSetupCondCurve::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupCondCurve-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0) ) {
        return;
    }

    if(name==PACKET_REBIRTH_HANDLE) //重生恢复操作处理#/
    {

    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupCondCurve-ActionHandle-out");
#endif
}

//根据选择的曲线号，读取对应的数据文件显示曲线#/
void SceneSetupCondCurve::selectedCurve(int num)
{
    num=num;

}

//编辑框选择时#/
void SceneSetupCondCurve::lineEditInFocus()
{
    if(nowSelectNum<0||nowSelectNum>9)
        return;

    if(curveArray[nowSelectNum]->selectMark==0)  //选中当前编辑的时段#/
    {
        curveArray[nowSelectNum]->selectMark=1;
        curveArray[nowSelectNum]->setStyleSheet("border-width: 3px; border-style: inset; \
                                                 border-color: blue; background-color: #237d3a");
    }
    else
    {
        curveArray[nowSelectNum]->naValue=lineEditSetNum.text().toFloat();
        curveArray[nowSelectNum]->setFocus();//设置焦点进入#/
    }
}

//全加按钮点击时#/
void SceneSetupCondCurve::allAddClicked()
{

    for(int i=0;i<10;i++)
    {
        if(curveArray[i]!=0)
            curveArray[i]->naValue=curveArray[i]->naValue+1;
        curveArray[i]->update();
    }
}

//全减按钮点击#/
void SceneSetupCondCurve::allSubClicked()
{

    for(int i=0;i<10;i++)
    {
        if(curveArray[i]!=0)
            curveArray[i]->naValue=curveArray[i]->naValue-1;
        curveArray[i]->update();
    }
}

//曲线选择按钮---+#/
void SceneSetupCondCurve::selectAddClicked()
{
    nowSelectCurve++;
    if(nowSelectCurve>=16)
        nowSelectCurve=0;
    selectedCurve(nowSelectCurve);
}


void SceneSetupCondCurve::selectSubClicked()
{
    nowSelectCurve--;
    if(nowSelectCurve<0)
        nowSelectCurve=15;
    selectedCurve(nowSelectCurve);
}

void SceneSetupCondCurve::buttonSaveClicked()//保存按钮#/
{


}

//应用按钮点击，将曲线数据保存为数据文件，并且应用#/
void SceneSetupCondCurve::buttonOkClicked()
{
    QString str;
    int i=0;

    if((residue_secCount<3600)&&(G_COND_PROFILE_FLAG==false))//小于60分钟不给应用#/
    {
        str=DE_TR_CANNOTLESS;
        str += QString(" 60 ")+DE_TR_MINTUES;
        labelPrompt.setText(str);
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
        return;
    }


    if(G_COND_PROFILE_FLAG==true)  //该曲线正在应用,是否取消#/
    {
        //确定是否应用#/
        QMessageBox box(QMessageBox::Question,DE_TR_PROMPT,DE_TR_AREYOUSURECANCEL);
        QAbstractButton *bno =box.addButton(DE_TR_NO,QMessageBox::YesRole);
        box.addButton(DE_TR_YES,QMessageBox::YesRole);
        box.setWindowFlags(Qt::Widget);
        box.move(300,300);
        //消息框运行，等待点击按键退出#/
        box.exec();
        if (box.clickedButton() == bno)
            return;
        else
        {
            uiPDO04.N1_set_mix=G_COND_PROFILE_RELVULE;

            buttonOk.setText(DE_TR_APPLY);

            G_COND_PROFILE_FLAG=false;
            str=DE_TR_PROFILECANCEL;
            labelPrompt.setText(str);
            return;
        }
    }
    else if(G_COND_PROFILE_FLAG==false) //该曲线未应用,是否应用#/
    {
        //确定是否应用#/
        QMessageBox box(QMessageBox::Question,DE_TR_PROMPT,DE_TR_AREYOUSURE);
        QAbstractButton *bno =box.addButton(DE_TR_NO,QMessageBox::YesRole);
        box.addButton(DE_TR_YES,QMessageBox::YesRole);
        box.setWindowFlags(Qt::Widget);
        box.move(300,300);
        //消息框运行，等待点击按键退出#/
        box.exec();
        if (box.clickedButton() == bno)
            return;


        buttonOk.setText(DE_TR_CANCEL);
        G_COND_PROFILE_FLAG=true;

        //曲线应用处理#/
        for(i=0;i<10;i++)
        {
            A_COND_PROFILE[i].curevalue=curveArray[i]->naValue;
            A_COND_PROFILE[i].flag=false;
        }
        G_COND_PROFILE_NUM=0;      //曲线当前运行第几段#/
        G_COND_PROFILE_START=G_SYS_CURTIME;  //曲线开始时间#/
        G_COND_PROFILE_END=G_SYS_CURTIME+residue_secCount;    //曲线结束时间#/

        G_COND_PROFILE_RELVULE=uiPDO04.N1_set_mix;    //曲线运行前的设置值#/
    }

    //应用处理#/
    str=DE_TR_CURVERUNSUCCESS;

    labelPrompt.setText(str);
    MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);

    logInfo log;
    //log.logId=1;
    log.opration="Cond profile Run";
    log.type=DE_LOGINFO_OPER;
    log.status=PDOstate.state.N1_status;
    log.note="Null";
    LogRun->addLog(log);
}

//取消#/
void SceneSetupCondCurve::buttonCancelClicked()
{
    PageShow("Setup Preflush");
}





