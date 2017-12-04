/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-18
** 修改人：Huangjunxian
** 修改时间：2014-11-26
** 描述：
** 修改人：Huangjunxian
** 修改时间：2014-12-5
** 描述：添加保存按钮，将保存和应用分离
**    UF曲线功能 《源文件》
*****************************************************/
#include "scene_setup_layout.h"
#include "scene_setup_curve_uf.h"
#include "curve_datahandle.h"
#include "global_var.h"
#include "common_tr_string.h"
#include "ScreenSnaper.h"

#define SCENE_SETUP_CURVE_START_X              170
#define SCENE_SETUP_CURVE_START_Y              185
#define SCENE_SETUP_CURVE_STRIP_WIDTH          40
#define SCENE_SETUP_CURVE_STRIP_HIGH           280

//#define CONFIG_COLOR_WHITE_CHARS  "border:2px groove gray;color:black;background-color:#1A2131"

#define CONFIG_STYLE_RED_CHARS  "border:2px groove gray;color:red;background-color:white"

#define COLUMNAR_UNCLICKED_HD "border-width: 3px; border-style: outset; \
                               border-color: gray; background-color: #237d3a"

#define COLUMNAR_CLICKED_HD   "border-width: 3px; border-style: inset; \
                               border-color: blue; background-color: #237d3a"

#define COLUMNAR_UNCLICKED_SEQ "border-width: 3px; border-style: outset; \
                                border-color: gray; background-color: yellow"

#define COLUMNAR_CLICKED_SEQ   "border-width: 3px; border-style: inset; \
                                border-color: blue; background-color: yellow"

#define BUTTON_CLICKED_CSS "border:3px inset blue;border-radius:3px;background-color:gray"

#define BUTTON_RELEASE_CSS "border:3px outset gray;border-radius:3px;background-color:white"

bool ufEnableEdit=false; //预定义曲线是否可编辑 true 可以 false 不可以#/
char select_ufcure_num=0;   //从外部进来该页面时，选择的曲线#/

//与外部接口的变量#/
//static int sumUfValue=0;   //总超滤量 单位 ml#/
//static int sumCureTime=0;  //总治疗时间 单位 s#/
static float averageUf=0; //平均超滤率 [ml/m]#/


/*-------------------------柱状图类----------------------------------*/
//界面设置函数#/
inline void ColumnarCurveUf::setUi()
{
    this->setGeometry(xAxis,yAxis,
                      SCENE_SETUP_CURVE_STRIP_WIDTH,SCENE_SETUP_CURVE_STRIP_HIGH);
    this->setStyleSheet(COLUMNAR_UNCLICKED_HD);

    labeltop.setGeometry(xAxis-2,yAxis-25,45,20);
    labeltop.setAlignment(Qt::AlignHCenter | Qt::AlignHCenter);
    labeltop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                                CONFIG_FONT_NORMAL));
    labeltop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelbottom.setGeometry(xAxis-2,yAxis+280,45,20);
    labelbottom.setAlignment(Qt::AlignHCenter | Qt::AlignHCenter);
    labelbottom.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                                CONFIG_FONT_NORMAL));
    labelbottom.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
}

//构造函数#/
ColumnarCurveUf::ColumnarCurveUf(QWidget *parent,SceneSetupCurveUF *p,int x,int y,int order):
QLabel(parent),orderNum(order),xAxis(x),yAxis(y),topscene(p),
labeltop(parent),labelbottom(parent)
{
    setUi();
    setFocusPolicy(Qt::ClickFocus); //设置该控件有点击焦点，才能触发focusEvent#/
    selectMark=0;
    seqMode=false;
    passMark=false;
    ufrate=0;
    periodTime=0;
}

//鼠标点击处理事件，标准触摸屏点击也是调用这个事件#/
void ColumnarCurveUf::mousePressEvent(QMouseEvent *ev)
{
    float newvalue;
    if(selectMark==0)
        return;
    if(selectMark==1)  //为了避免选中时会改变柱状图的值，特加多一个标记数#/
    {
        selectMark=2;
        return;
    }

    passMark=true;
    newvalue=(274-ev->y())*(200.0/274);
    ufrate=reviseCurve(ufrate,newvalue);
    update();
//  ev->ignore();   //忽略该事件#/
//  qDebug()<<"mousePressEvent";
}

//鼠标移动事件#/
void ColumnarCurveUf::mouseMoveEvent(QMouseEvent *ev)
{
    if(passMark==false)
        return;
    float newvalue;
    newvalue=(274-ev->y())*(200.0/274);
    ufrate=reviseCurve(ufrate,newvalue);
    update();
//  ev->ignore();   //忽略该事件#/
//  qDebug()<<"mouseMoveEvent";
}

//鼠标释放事件#/
void ColumnarCurveUf::mouseReleaseEvent(QMouseEvent *ev)
{
    passMark=false;
    ev->ignore();
//  qDebug()<<"mouseReleaseEvent";#/
}

//焦点进入#/
void ColumnarCurveUf::focusInEvent(QFocusEvent *ev)
{
    //0----7 号曲线是预定义的，不能编辑#/
    if(topscene->nowSelectCurve<=7&&ufEnableEdit==false)
    {
        topscene->labelPrompt.setText(DE_TR_PREDEFINED);
        return;
    }
    selectMark=1; //标记选中该柱状图#/

    if(seqMode)
    {
        this->setStyleSheet(COLUMNAR_CLICKED_SEQ);
        topscene->buttonSEQ.setStyleSheet(BUTTON_CLICKED_CSS);
        topscene->buttonHD.setStyleSheet(BUTTON_RELEASE_CSS);
    }
    else
    {
        this->setStyleSheet(COLUMNAR_CLICKED_HD);
        topscene->buttonSEQ.setStyleSheet(BUTTON_RELEASE_CSS);
        topscene->buttonHD.setStyleSheet(BUTTON_CLICKED_CSS);
    }

    QString str(DE_TR_SELECTING);
    str=str+" "+QString().setNum(orderNum+1,10)+"\n";
    str=str+QString().setNum(periodTime,'f',1)+"[min]";
    topscene->labelCurveTime.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);
    topscene->labelCurveTime.setText(str);
    topscene->nowSelectNum=orderNum;
    ev->ignore();
//  qDebug()<<"focusInEvent";
}

//焦点移出#/
void ColumnarCurveUf::focusOutEvent(QFocusEvent *ev)
{
    ev=ev;
    selectMark=0;
    if(seqMode)
        this->setStyleSheet(COLUMNAR_UNCLICKED_SEQ);
    else
        this->setStyleSheet(COLUMNAR_UNCLICKED_HD);

    topscene->buttonSEQ.setStyleSheet(BUTTON_RELEASE_CSS);
    topscene->buttonHD.setStyleSheet(BUTTON_RELEASE_CSS);
}

//重绘事件#/
void ColumnarCurveUf::paintEvent(QPaintEvent *)
{
    QPainter paint(this);

    ufrate=ufrate>200?200:ufrate;
    ufrate=ufrate<0?0:ufrate;
    float high=((200-ufrate)/200)*(SCENE_SETUP_CURVE_STRIP_HIGH-6);
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(QColor(166,166,166)));
    paint.drawRect(3,3,SCENE_SETUP_CURVE_STRIP_WIDTH-6,high);

    if(topscene->nowSelectNum==orderNum)
        topscene->lineEditSetNum.setText(QString().setNum(ufrate,'f',1));
    float rate=ufrate/100; //其值是百分比#/
    labeltop.setText(QString().setNum(rate*averageUf*60,'f',0));
    labelbottom.setText(QString().setNum(rate*averageUf*periodTime,'f',0));
}

//显示#/
void ColumnarCurveUf::setShow(bool vis)
{
    this->setVisible(vis);
    labeltop.setVisible(vis);
    labelbottom.setVisible(vis);
}


//刷新当前时段（不包含）#/
//右边剩余时段的数值#/
void ColumnarCurveUf::calHandle(float value, bool add)
{
    float sum=0,revise=0;
    int i=0;

    sum=value/(9-orderNum);
    revise=value;
    if(add==true) //增加方式#/
    {
        while(revise>0.01)
        {
            for(i=orderNum+1;i<10;i++)
            {
                if(topscene->curveArray[i]->ufrate<=(200-sum))
                {
                    topscene->curveArray[i]->ufrate +=sum;
                    revise -=sum;
                }
                else if(topscene->curveArray[i]->ufrate<200)
                {
                    if(revise>=(200-topscene->curveArray[i]->ufrate))
                    {
                        revise -=(200-topscene->curveArray[i]->ufrate);
                        topscene->curveArray[i]->ufrate=200;
                    }
                    else
                    {
                        topscene->curveArray[i]->ufrate +=revise;
                        revise=0;
                    }
                }
                if(revise<0.1)
                    break;
            }

            if(revise>1)
                sum=revise/(9-orderNum);
            else if(revise>0.1)
                sum=revise;
            else
                revise=0;
        }
    }
    else
    {
        while(revise>0.01)
        {
            for(i=orderNum+1;i<10;i++)
            {
                if(topscene->curveArray[i]->ufrate>=sum)
                {
                    topscene->curveArray[i]->ufrate -=sum;
                    revise -=sum;
                }
                else if(topscene->curveArray[i]->ufrate>0)
                {
                     if(revise>=topscene->curveArray[i]->ufrate)
                     {
                        revise -=topscene->curveArray[i]->ufrate;
                        topscene->curveArray[i]->ufrate=0;
                     }
                     else
                     {
                        topscene->curveArray[i]->ufrate -=revise;
                        revise=0;
                     }
                }
                if(revise<0.1)
                    break;
            }
            if(revise>1)
                sum=revise/(9-orderNum);
            else if(revise>0.1)
                sum=revise;
            else
                revise=0;
        }   // while(revise>0.01)
    }//   else
}

//修正所有曲线值，避免超过超滤量#/
float ColumnarCurveUf::reviseCurve(float oldvalue,float newvalue)
{
    float maxrate=0,minrate=0;
    float reviseadd=0,revisesub=0;
    float sum=0,tmpleft=0,tmpright=0,retvalue=0;
    int i=0;

    if(newvalue==oldvalue)
        return newvalue;

    for(i=0;i<orderNum;i++)
    {
        sum += topscene->curveArray[i]->ufrate;
    }  
    tmpleft=orderNum*100-sum;

    sum=0;
    for(i=orderNum;i<10;i++)
    {
        sum += topscene->curveArray[i]->ufrate;
    }
    tmpright=(10-orderNum)*100-sum;

    sum=tmpleft+tmpright;
    if(sum>0)
    {
        calHandle(sum,true);
    }
    else if(sum<0)
    {
        sum *=-1;
        calHandle(sum,false);
    }

    sum=0;
    for(i=orderNum+1;i<10;i++)
    {
        sum += topscene->curveArray[i]->ufrate;
    }

    if(sum>0)
    {
        maxrate=sum+oldvalue;
        maxrate=maxrate>200?200:maxrate;
    }
    else
        maxrate=oldvalue;

    tmpright=(9-orderNum)*200-sum;
    minrate=oldvalue>tmpright?tmpright:oldvalue;
    minrate=oldvalue-minrate;
    if(minrate<0)minrate=0;

    if(newvalue>oldvalue)
    {
        revisesub=newvalue>maxrate?maxrate:newvalue;
        retvalue=revisesub;
        revisesub=revisesub-oldvalue;
        calHandle(revisesub,false);
    }
    else if(newvalue<oldvalue)
    {
        reviseadd=newvalue>minrate?newvalue:minrate;
        retvalue=reviseadd;
        reviseadd=oldvalue-reviseadd;
        calHandle(reviseadd,true);
    }
    //刷新时段显示#/
    for(i=orderNum+1;i<10;i++)
    {
        topscene->curveArray[i]->update();
    }

    return retvalue;
}

/*-------------------------曲线页面显示类-------------------------------*/
//界面设置函数#/
inline void SceneSetupCurveUF::setUi()
{
    /*
    labelTitleTop.setGeometry(255,70,350,40);
    labelTitleTop.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
//    labelTitleTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_SECOND,
//                                CONFIG_FONT_WEIGHT));
    labelTitleTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                                CONFIG_FONT_WEIGHT));
    labelTitleTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelPrompt.setGeometry(180,105,500,35);
    labelPrompt.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelPrompt.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                                CONFIG_FONT_WEIGHT));
    labelPrompt.setStyleSheet(CONFIG_COLOR_RED_CHARS);

    labelYaxisUnit.setGeometry(110,140,70,40);
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

    labelLeftMiddle.setGeometry(20,290,100,80);
    labelLeftMiddle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelLeftMiddle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                  CONFIG_FONT_WEIGHT));
    labelLeftMiddle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    buttonHD.setGeometry(20,360,60,40);
    BUTTON_SIMPLE_STYLE(buttonHD);
    buttonHD.setStyleSheet(BUTTON_RELEASE_CSS);

    buttonSEQ.setGeometry(20,425,60,40);
    BUTTON_SIMPLE_STYLE(buttonSEQ);
    buttonSEQ.setStyleSheet(BUTTON_RELEASE_CSS);

    label200.setGeometry(120,170,50,38);
    label200.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label200.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label200.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label150.setGeometry(120,236,50,38);
    label150.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label150.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label150.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label100.setGeometry(120,304,50,38);
    label100.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label100.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label100.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label50.setGeometry(120,372,50,38);
    label50.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label50.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label50.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label0.setGeometry(120,440,50,38);
    label0.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label0.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_WEIGHT));
    label0.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

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

    labelRightTop.setGeometry(725,160,110,80);
    labelRightTop.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelRightTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_WEIGHT));
    labelRightTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelCurveNum.setGeometry(710,245,130,40);
    labelCurveNum.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelCurveNum.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                               CONFIG_FONT_WEIGHT));
    labelCurveNum.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelRightMiddle.setGeometry(725,300,110,80);
    labelRightMiddle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelRightMiddle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_WEIGHT));
    labelRightMiddle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelTopUnit.setGeometry(680,160,70,20);
    labelTopUnit.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    labelTopUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                           CONFIG_FONT_WEIGHT));
    labelTopUnit.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelBottomUnit.setGeometry(680,465,50,20);
    labelBottomUnit.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    labelBottomUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                           CONFIG_FONT_WEIGHT));
    labelBottomUnit.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);
    */

    labelTitleTop.setGeometry(255,70,350,40);
    labelTitleTop.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelTitleTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                                CONFIG_FONT_NORMAL));
    labelTitleTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelPrompt.setGeometry(180,105,500,35);
    labelPrompt.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    labelPrompt.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                                CONFIG_FONT_NORMAL));
    labelPrompt.setStyleSheet(CONFIG_COLOR_RED_CHARS);

    labelYaxisUnit.setGeometry(110,140,70,40);
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

    labelLeftMiddle.setGeometry(20,290,100,80);
    labelLeftMiddle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelLeftMiddle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                                  CONFIG_FONT_NORMAL));
    labelLeftMiddle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    buttonHD.setGeometry(20,360,60,40);
    BUTTON_SIMPLE_STYLE(buttonHD);
    buttonHD.setStyleSheet(BUTTON_RELEASE_CSS);

    buttonSEQ.setGeometry(20,425,60,40);
    BUTTON_SIMPLE_STYLE(buttonSEQ);
    buttonSEQ.setStyleSheet(BUTTON_RELEASE_CSS);

    label200.setGeometry(120,170,50,38);
    label200.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label200.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label200.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label150.setGeometry(120,236,50,38);
    label150.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label150.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label150.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label100.setGeometry(120,304,50,38);
    label100.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label100.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label100.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label50.setGeometry(120,372,50,38);
    label50.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label50.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label50.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

    label0.setGeometry(120,440,50,38);
    label0.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    label0.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                           CONFIG_FONT_NORMAL));
    label0.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);

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

    labelRightTop.setGeometry(725,160,110,80);
    labelRightTop.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelRightTop.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_NORMAL));
    labelRightTop.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelCurveNum.setGeometry(710,245,130,40);
    labelCurveNum.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelCurveNum.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_THIRD,
                               CONFIG_FONT_NORMAL));
    labelCurveNum.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelRightMiddle.setGeometry(725,300,110,80);
    labelRightMiddle.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelRightMiddle.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_TEXT,
                               CONFIG_FONT_NORMAL));
    labelRightMiddle.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelTopUnit.setGeometry(680,160,70,20);
    labelTopUnit.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    labelTopUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                           CONFIG_FONT_NORMAL));
    labelTopUnit.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    labelBottomUnit.setGeometry(680,465,50,20);
    labelBottomUnit.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    labelBottomUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),CFG_FONT_SIZE_FOURTH,
                           CONFIG_FONT_NORMAL));
    labelBottomUnit.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);

    buttonSelectAdd.setGeometry(725,390,85,40);
    BUTTON_SIMPLE_STYLE(buttonSelectAdd);

    buttonSelectSub.setGeometry(725,450,85,40);
    BUTTON_SIMPLE_STYLE(buttonSelectSub);

    buttonSave.setGeometry(160,650,80,40);
    BUTTON_SIMPLE_STYLE(buttonSave);

    buttonOk.setGeometry(370,650,80,40);
    BUTTON_SIMPLE_STYLE(buttonOk);

    buttonCancel.setGeometry(600,650,80,40);
    BUTTON_SIMPLE_STYLE(buttonCancel);
}

//构造函数#/
SceneSetupCurveUF::SceneSetupCurveUF(QWidget *parent):
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
labelTopUnit(parent),
labelBottomUnit(parent),
label200(parent),
label150(parent),
label100(parent),
label50(parent),
label0(parent),
lineEditSetNum(parent),
buttonHD(parent),
buttonSEQ(parent),
buttonSelectAdd(parent),
buttonSelectSub(parent),
buttonSave(parent),
buttonOk(parent),
buttonCancel(parent)
{
    setUi();

    PageAppendPiece("Setup CurveUF", this);
    this->CustomSetText();

    //将软键盘和要输入的控制框建立联系#/
    ic->appendWidget(&lineEditSetNum,NumInputMethod,LineEdit,NULL,0,200,
                      1,"UF Value:","UF",false,INT);

    //编辑框数值改变时#/
    connect(&lineEditSetNum,SIGNAL(InFocus()),this,SLOT(lineEditInFocus()));
    connect(&lineEditSetNum,SIGNAL(editingFinished()),this,SLOT(lineEditInFocus()));


    connect(&buttonHD,SIGNAL(clicked()),this,SLOT(buttonHDClicked()));
    connect(&buttonSEQ,SIGNAL(clicked()),this,SLOT(buttonSEQClicked()));
    connect(&buttonSelectAdd,SIGNAL(clicked()),this,SLOT(selectAddClicked()));
    connect(&buttonSelectSub,SIGNAL(clicked()),this,SLOT(selectSubClicked()));
    connect(&buttonSave,SIGNAL(clicked()),this,SLOT(buttonSaveClicked()));
    connect(&buttonOk,SIGNAL(clicked()),this,SLOT(buttonOkClicked()));
    connect(&buttonCancel,SIGNAL(clicked()),this,SLOT(buttonCancelClicked()));

    nowSelectCurve=0;
    for(int i=0;i<10;i++)
    {
        curveArray[i]=new ColumnarCurveUf(parent,this,SCENE_SETUP_CURVE_START_X+52*i,
                                        SCENE_SETUP_CURVE_START_Y,i);
        curveArray[i]->seqMode=false;
        curveArray[i]->ufrate=0;
        curveArray[i]->periodTime=0;
        curveArray[i]->setVisible(false);
    }
    nowSelectNum=-1;
    lineEditSetNum.setText("0");
}

SceneSetupCurveUF::~SceneSetupCurveUF()
{
    for(int i=0;i<10;i++)
    {
        if(curveArray[i]!=NULL)
            delete curveArray[i];
    }
}

//界面文字显示设置#/
void SceneSetupCurveUF::CustomSetText()
{

    labelTitleTop.setText(DE_TR_SEQUENTIALUF);

    labelLeftTop.setText(DE_TR_THESELECT2);
    labelLeftMiddle.setText(DE_TR_UFMODE);
    labelRightTop.setText(DE_TR_CURRENTCURVE);
    labelRightMiddle.setText(DE_TR_BROWSECURVE);


    labelSumUF.setText(DE_TR_SUMUF);
    labelSumTime.setText(DE_TR_TREATMENT);
    labelPeriodTime.setText(DE_TR_EACHPERIOD);
    labelCurveTime.setText(DE_TR_NOTSELECT);

    labelYaxisUnit.setText("[%]");
    labelTopUnit.setText("[ml/h]");
    labelBottomUnit.setText("[ml]");
    label200.setText("200-");
    label150.setText("150-");
    label100.setText("100-");
    label50.setText(" 50-");
    label0.setText(" 0-");

    buttonHD.setText("HD");
    buttonSEQ.setText("SEQ");
    buttonSelectAdd.setText(DE_TR_PREVIOUS);
    buttonSelectSub.setText(DE_TR_NEXT);
    buttonSave.setText(DE_TR_SAVE);
    buttonOk.setText(DE_TR_APPLY);
    buttonCancel.setText(DE_TR_RETURN);
}

//页面显示调用函数--#/
void SceneSetupCurveUF::setCusVisible(bool vis, char *para)
{
    para=para;


    for(int i=0;i<10;i++)
    {
        if(curveArray[i]!=0)
            curveArray[i]->setShow(vis);
    }

//    bool bfvis=vis;
//    vis=false;

    //各种文本标签#/
    labelTitleTop.setVisible(vis);
    labelPrompt.setVisible(vis);
    labelYaxisUnit.setVisible(vis);
    labelLeftTop.setVisible(vis);
    labelLeftMiddle.setVisible(vis);
    labelRightTop.setVisible(vis);
    labelRightMiddle.setVisible(vis);
    labelCurveNum.setVisible(vis);
    labelSumUF.setVisible(vis);
    labelSumTime.setVisible(vis);
    labelPeriodTime.setVisible(vis);
    labelCurveTime.setVisible(vis);
    labelTopUnit.setVisible(vis);
    labelBottomUnit.setVisible(vis);
    //y坐标轴标签#/
    label200.setVisible(vis);
    label150.setVisible(vis);
    label100.setVisible(vis);
    label50.setVisible(vis);
    label0.setVisible(vis);

//        vis=bfvis;

    //每个治疗时段数值编辑框#/
    lineEditSetNum.setVisible(vis);
    //页面相应的按钮#/
    buttonHD.setVisible(vis);//全加#/
    buttonSEQ.setVisible(vis);//全减#/
    buttonSelectAdd.setVisible(vis);//曲线选择+#/
    buttonSelectSub.setVisible(vis);//曲线选择-#/
    buttonSave.setVisible(vis);
    buttonOk.setVisible(vis);//应用按钮#/
    buttonCancel.setVisible(vis);//返回按钮#/

    if(vis==false)
    {
        ufEnableEdit=false;
        select_ufcure_num=-1;
    }

    if((vis==true)&&(select_ufcure_num>=0))
    {
        selectedCurve(select_ufcure_num);
    }
}

void SceneSetupCurveUF::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupCurveUF-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0) ) {
        return;
    }

    if(name==PACKET_REBIRTH_HANDLE) //重生恢复操作处理#/
    {
        int i;
        //UF曲线治疗相关#/
        G_UF_PROFILE_FLAG=G_P_REBIRTH->G_UF_PROFILE_FLAG;  //UF曲线开始标志#/
        G_UF_PROFILE_NUM=G_P_REBIRTH->G_UF_PROFILE_NUM;   //UF曲线从第几段开始#/
        G_UF_PROFILE_START=G_P_REBIRTH->G_UF_PROFILE_START;  //UF曲线开始时间#/
        G_UF_PROFILE_END=G_P_REBIRTH->G_UF_PROFILE_END;   //UF曲线结束时间#/
        memcpy(A_UF_PROFILE,G_P_REBIRTH->A_UF_PROFILE,sizeof(UF_Profile_Array)*10);
        G_UF_PROFILE_STARTVOL=G_P_REBIRTH->G_UF_PROFILE_STARTVOL; //UF曲线时段初始超滤量#/
        G_UF_PROFILE_PERTOTALVOL=G_P_REBIRTH->G_UF_PROFILE_PERTOTALVOL; //UF曲线每时段目标超滤量#/
        G_UF_SELECTCURVE_NUM=G_P_REBIRTH->G_UF_SELECTCURVE_NUM;    //当前选择运行的曲线号#/

        selectedCurve(G_UF_SELECTCURVE_NUM);

        for(i=0;i<10;i++)
        {
            curveArray[i]->seqMode=A_UF_PROFILE[i].seqmode;
            curveArray[i]->ufrate=A_UF_PROFILE[i].ufrate;

            if(curveArray[i]->seqMode)
                curveArray[i]->setStyleSheet(COLUMNAR_UNCLICKED_SEQ);
            else
                curveArray[i]->setStyleSheet(COLUMNAR_UNCLICKED_HD);
            curveArray[i]->update();
        }

    }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupCurveUF-ActionHandle-out");
#endif
}


//根据选择的曲线号，读取对应的数据文件显示曲线#/
void SceneSetupCurveUF::selectedCurve(int num)
{
    int i=0;
    float sumtime=0;
    QString str;
    nowSelectCurve=num;

    if(num<0)
        return;

    if(num>16)
        return;

    if(num<=7)
        str=DE_TR_DEFINE;
    else
        str=DE_TR_CUSTOM;

    str=str+QString().setNum(num);
    labelCurveNum.setText(str);

    //总超滤量#/
    str=DE_TR_SUMUF+"\n"+QString().setNum(G_UF_CURETARGET)+"[ml]";
    labelSumUF.setText(str);

    //治疗时间#/
    sumtime=(float)total_secCount/60;
    str=DE_TR_TREATMENT+"\n"+QString().setNum((int)(total_secCount/3600));
    str=str+":"+QString().setNum((int)(total_secCount%3600)/60)+"[h:min]";
    labelSumTime.setText(str);
    averageUf=G_UF_CURETARGET/sumtime; //平均超滤率 [ml/m]#/

    //每个时段时间#/
    sumtime=sumtime/10; //默认是10个时段#/
    str=DE_TR_TREATMENT+"\n"+QString().setNum(sumtime,'f',1)+"[min]";
    labelPeriodTime.setText(str);

    if(curveDataObject.checkCurve(UFCURVETYPE,num))
        curveDataObject.readCurve(UFCURVETYPE,num,(char *)ufParaArray,sizeof(Curve_UfPara)*10);
    else
    {
        for(i=0;i<10;i++)
        {
            ufParaArray[i].seqmode=false;
            ufParaArray[i].ufrate=100;
        }
    }

    //0----7 号曲线是预定义的，不能编辑#/
    if(nowSelectCurve<=7&&ufEnableEdit==false)
        lineEditSetNum.setFocusPolicy(Qt::NoFocus);
    else
        lineEditSetNum.setFocusPolicy(Qt::ClickFocus);

    labelPrompt.setText(" ");
    nowSelectNum=-1;
    labelCurveTime.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    labelCurveTime.setText(DE_TR_NOTSELECT);

    for(i=0;i<10;i++)
    {
        curveArray[i]->seqMode=ufParaArray[i].seqmode;
        curveArray[i]->ufrate=ufParaArray[i].ufrate;
        curveArray[i]->periodTime=sumtime;  //每个治疗时段时间#/

        if(curveArray[i]->seqMode)
            curveArray[i]->setStyleSheet(COLUMNAR_UNCLICKED_SEQ);
        else
            curveArray[i]->setStyleSheet(COLUMNAR_UNCLICKED_HD);
        curveArray[i]->update();
    }
}

//编辑框选择时#/
void SceneSetupCurveUF::lineEditInFocus()
{
    float newvalue=0;

    if(nowSelectNum<0)
        return;

    if(curveArray[nowSelectNum]->selectMark==0)
    {
        qDebug("lineEditInFocus--1");
        curveArray[nowSelectNum]->selectMark=1;
        if(curveArray[nowSelectNum]->seqMode)
            curveArray[nowSelectNum]->setStyleSheet(COLUMNAR_CLICKED_SEQ);
        else
            curveArray[nowSelectNum]->setStyleSheet(COLUMNAR_CLICKED_HD);
        return;
    }
    else
    {
        newvalue=lineEditSetNum.text().toFloat();
/*
        QString str;       
        str="newvalue=";
        str += QString().setNum(newvalue,'f',1);
        qDebug(str.toLatin1());
*/
        curveArray[nowSelectNum]->ufrate=curveArray[nowSelectNum]->reviseCurve(curveArray[nowSelectNum]->ufrate,newvalue);
/*
        str="ufrate=";
        str += QString().setNum(curveArray[nowSelectNum]->ufrate,'f',1);
        qDebug(str.toLatin1());
*/
        curveArray[nowSelectNum]->setFocus();
    }
}

//HD按钮点击时#/
void SceneSetupCurveUF::buttonHDClicked()
{
    if(nowSelectNum<0)
    {
        labelPrompt.setText(DE_TR_PLEASESELECTCURVE);
        return;
    }
    //0----7 号曲线是预定义的，不能编辑#/
    if(nowSelectCurve<=7&&ufEnableEdit==false)
    {
        labelPrompt.setText(DE_TR_PREDEFINED);
        return;
    }

    curveArray[nowSelectNum]->seqMode=false;
    curveArray[nowSelectNum]->setFocus();
}

//SEQ按钮点击#/
void SceneSetupCurveUF::buttonSEQClicked()
{
    if(nowSelectNum<0)
    {
        labelPrompt.setText(DE_TR_PLEASESELECTCURVE);
        return;
    }
    //0----7 号曲线是预定义的，不能编辑#/
    if(nowSelectCurve<=7&&ufEnableEdit==false)
    {
        labelPrompt.setText(DE_TR_PREDEFINED);
        return;
    }

    curveArray[nowSelectNum]->seqMode=true;
    curveArray[nowSelectNum]->setFocus();
}

//曲线选择按钮---+#/
void SceneSetupCurveUF::selectAddClicked()
{
    nowSelectCurve++;
    if(nowSelectCurve>=16)
        nowSelectCurve=0;
    selectedCurve(nowSelectCurve);
}

//曲线选择按钮-- -#/
void SceneSetupCurveUF::selectSubClicked()
{
    nowSelectCurve--;
    if(nowSelectCurve<0)
        nowSelectCurve=15;
    selectedCurve(nowSelectCurve);
}

void SceneSetupCurveUF::buttonSaveClicked()//保存按钮#/
{
    QString str;
    int i=0;
    bool ret;
    float sumuf=0,rate=0;
    ScreenSnaper snaper;

    if(nowSelectCurve<=7&&ufEnableEdit==false)//预定义曲线应用处理#/
    {
         labelPrompt.setText(DE_TR_PREDEFINED);
         return;
    }

//  ret=snaper.startSnaper(QRect(167,180,523,288));
    ret=snaper.startWidgetSnaper(labelTitleTop.parentWidget(),QRect(167,180,523,288));
    if(!ret)
    {
       MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_PNGFILEERROR);
       return;
    }

    QMessageBox box(QMessageBox::Question,DE_TR_PROMPT,DE_TR_AREYOUSAVE);
    QAbstractButton *bno =box.addButton(DE_TR_NO,QMessageBox::YesRole);
    box.addButton(DE_TR_YES,QMessageBox::YesRole);
    box.setWindowFlags(Qt::Widget);
    box.move(300,300);
    //消息框运行，等待点击按键退出#/
    box.exec();
    if (box.clickedButton() == bno)
        return;

    for(i=0;i<10;i++)
    {
        rate=curveArray[i]->ufrate/100;
        sumuf += rate*averageUf*curveArray[i]->periodTime;
    }

    sumuf=sumuf-G_UF_CURETARGET;
    if(sumuf>=0.5)  //超滤量误差控制 单位ml#/
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_MORETHANUF);
        return;
    }
    else if(sumuf<=-0.5)
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_LESSTHANUF);
        return;
    }

    for(i=0;i<10;i++)
    {
        ufParaArray[i].seqmode=curveArray[i]->seqMode;
        ufParaArray[i].ufrate=curveArray[i]->ufrate;
        ufParaArray[i].curetime=curveArray[i]->periodTime;
    }

    ret=curveDataObject.saveCurve(UFCURVETYPE,nowSelectCurve,
                                  (char *)ufParaArray,sizeof(Curve_UfPara)*10);
    if(ret)
        str=DE_TR_CURVESAVESUCCESS;
    else
        str=DE_TR_CURVSAVEFAIL;

    if(ret&&(nowSelectCurve>7||ufEnableEdit==true))
    {
        QString file("./db/");
        file += DE_UFPNGFILE_STR;
        file += QString().setNum(nowSelectCurve);
        QByteArray ba=file.toLatin1();
        snaper.saveSnaper(ba.data(),"png",80,60);

        logInfo log;
        //log.logId=1;
        log.opration="Save uf profile"+QString::number(nowSelectCurve);
        log.type=DE_LOGINFO_OPER;
        log.status=PDOstate.state.N1_status;
        log.note="Null";
        LogRun->addLog(log);
    }
    labelPrompt.setText(str);
    MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
}

//应用按钮点击，将曲线数据保存为数据文件，并且应用#/
void SceneSetupCurveUF::buttonOkClicked()
{
    QString str;
    int i=0;
    float sumuf=0,rate=0;

    if(total_secCount<1800)//小于30分钟不给应用#/
    {
        str=DE_TR_CANNOTLESS;
        str += QString(" 30 ")+DE_TR_MINTUES;
        labelPrompt.setText(str);
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
        return;
    }

    if(nowSelectCurve==G_UF_SELECTCURVE_NUM&&G_UF_PROFILE_FLAG==true)  //该曲线正在应用,不能重复#/
    {
        str=DE_TR_THISCURVEISRUNING;
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);
        return;
    }

    QMessageBox box(QMessageBox::Question,DE_TR_PROMPT,DE_TR_AREYOUSURE);
    QAbstractButton *bno =box.addButton(DE_TR_NO,QMessageBox::YesRole);
    box.addButton(DE_TR_YES,QMessageBox::YesRole);
    box.setWindowFlags(Qt::Widget);
    box.move(300,300);
    //消息框运行，等待点击按键退出#/
    box.exec();
    if (box.clickedButton() == bno)
        return;

    for(i=0;i<10;i++)
    {
        rate=curveArray[i]->ufrate/100;
        sumuf += rate*averageUf*curveArray[i]->periodTime;
    }

    sumuf=sumuf-G_UF_CURETARGET;
    if(sumuf>=0.5)  //超滤量误差控制 单位ml#/
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_MORETHANUF);
        return;
    }
    else if(sumuf<=-0.5)
    {
        MESSAGE_PROMPT_BOX(DE_TR_PROMPT,DE_TR_LESSTHANUF);
        return;
    }

    float perufvol=0;
    perufvol=(G_UF_CURETARGET-G_UF_CUREFINISH)/10.0; //每段平均超滤量#/
    //曲线应用处理#/
    for(i=0;i<10;i++)
    {
        ufParaArray[i].seqmode=curveArray[i]->seqMode;
        ufParaArray[i].ufrate=curveArray[i]->ufrate;
        ufParaArray[i].curetime=curveArray[i]->periodTime;

        A_UF_PROFILE[i].flag=false;
        A_UF_PROFILE[i].seqmode=curveArray[i]->seqMode;
        A_UF_PROFILE[i].ufrate=curveArray[i]->ufrate;
        A_UF_PROFILE[i].ufvol=(curveArray[i]->ufrate/100.0)*perufvol; //该时段的超滤目标量#/
    }

    G_UF_PROFILE_FLAG=true;  //UF曲线开始标志#/
    G_UF_PROFILE_STARTVOL=G_UF_CUREFINISH; //UF曲线时段初始超滤量#/
    G_UF_PROFILE_NUM=0;   //UF曲线从第几段开始#/
    G_UF_PROFILE_START=G_SYS_CURTIME;  //UF曲线开始时间#/
    G_UF_PROFILE_END=G_SYS_CURTIME+residue_secCount;   //UF曲线结束时间#/

    G_UF_SELECTCURVE_NUM=nowSelectCurve;    //当前选择运行的曲线号#/

    //应用处理#/
    str=DE_TR_CURVERUNSUCCESS;

    //fail handle
//    str=DE_TR_CURVEERROR;

    labelPrompt.setText(str);
    MESSAGE_PROMPT_BOX(DE_TR_PROMPT,str);

    logInfo log;
    //log.logId=1;
    log.opration="select uf profile"+QString::number(nowSelectCurve);
    log.type=DE_LOGINFO_OPER;
    log.status=PDOstate.state.N1_status;
    log.note="Null";
    LogRun->addLog(log);
}

//取消#/
void SceneSetupCurveUF::buttonCancelClicked()
{
    PageShow("Setup Sequential");
}





