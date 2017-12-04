/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-12-10
** 修改人：Huangjunxian
** 修改时间：2014-12-17
** 描述：
**    多功能数据曲线控件 《源文件》
*****************************************************/
#include "widget_data_curve.h"
#include <qpainter.h>
#include <math.h>



#define CONFIG_FONT_STYLE  "Droid Sans Fallback"
#define YAXIS_ADD_RATE     0.3     //Y轴最大最小值比实际值增加和减小的范围#/
#define XAXIS_GATE_RATE    0.2     //X轴最大最小值比实际值增加和减小的范围#/

//运行在PC端或者嵌入式版本的开关，由于两版本的差异，PC版本不能用于嵌入式平台#/
//0:嵌入式  1：PC
#define DE_PC_PLATFORM  0


//构造函数#/
Widget_Data_Curve::Widget_Data_Curve(QWidget *parent):
QWidget(parent),
labelcurve(this),
bzoomin(this),
bzoomout(this),
slider(this)
{
    labelcurve.setFrameStyle(QFrame::NoFrame);//无边框#/
//    labelcurve.setStyleSheet("background:yellow");#/
    labelcurve.setStyleSheet("background:transparent");//背景透明，不然会遮挡曲线#/

    curFont=QFont(QString(CONFIG_FONT_STYLE));

//    bzoomin.setText("+");
    //设置控件会自动填充区域，并可以伸缩#/
    bzoomin.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    bzoomin.setMinimumHeight(5);//可以缩小的最小高度#/
    bzoomin.setMinimumWidth(10);//可以缩小的最小宽度#/
    bzoomin.setIcon(QIcon(":resource/images/zoomin.png"));
    bzoomin.adjustSize();
    connect(&bzoomin,SIGNAL(clicked()),this,SLOT(zoomInt()));

//    bzoomout.setText("-");#/
    bzoomout.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    bzoomout.setMinimumHeight(5);
    bzoomout.setMinimumWidth(10);
    bzoomout.setIcon(QIcon(":resource/images/zoomout.png"));
    bzoomout.adjustSize();
    connect(&bzoomout,SIGNAL(clicked()),this,SLOT(zoomOut()));

    slider.setOrientation(Qt::Horizontal);
    slider.setMinimumHeight(5);
    slider.setMinimumWidth(10);
//    slider.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    slider.setRange(0,100);
    slider.setSingleStep(10);   //步进值#/
    slider.setStyleSheet("   \
                         QSlider::handle:horizontal { \
                         background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);\
                         border: 1px solid #5c5c5c;  \
                         height: 5px; \
                         width:  10px; \
                         margin: -2px 0;     \
                         border-radius: 3px;\
                         }  \
                         QSlider::add-page:horizontal {\
                            background: white;\
                         }\
                         QSlider::sub-page:horizontal {\
                            background: blue;\
                         }");

    connect(&slider,SIGNAL(clickedGroove()),this,SLOT(setSlideXaxis()));
    connect(&slider,SIGNAL(sliderReleased()),this,SLOT(setSlideXaxis()));

    //设置各控件在网格中的布局#/
    layout.addWidget(&labelcurve,0,0,1,3); //在0行0列开始，占1行3列#/
    layout.addWidget(&bzoomout,1,0,1,1);  //1行0列，占1行1列#/
    layout.addWidget(&slider,1,1,1,1);   //
    layout.addWidget(&bzoomin,1,2,1,1); //

    //设置各行高度比例，0行和1行比例为12：1#/
    layout.setRowStretch(0,12); //
    layout.setRowStretch(1,1); //

    //设置各列宽度比例#/
    layout.setColumnStretch(0,1);//#/
    layout.setColumnStretch(1,6);//
    layout.setColumnStretch(2,1);//

    layout.setSpacing(0);//网格间距为0#/
    layout.setMargin(0);//网格与外面的边距为0#/

    this->setLayout(&layout);//设置使用布局#/

    curvemap.clear(); //清空曲线结构容器#/
    xmargin=20;       //曲线左右边距#/
    ymargin=20;       //曲线上下边距#/
    minXaxis=0;       //默认的X轴范围#/
    maxXaxis=4;
    allMinXvalue=0;
    allMaxXvalue=4;
    minYaxis=0;
    minYaxis2=0;
    maxYaxis=100;
    maxYaxis2=100;
    enableYaxis2=false;
    pressMark=false;
    Xticknum=4;         //X轴坐标标记段数#/
    Yticknum=5;         //Y1轴坐标标记段数#/
    Yticknum2=6;        //Y2轴坐标标记段数#/
    xunit_name="H";     //X轴单位名称#/
    yunit_name="mm";    //Y轴单位名称#/
    yunit_name2="g";
    maxpointnum=200;
    zoominrange=4;
    zoomeachvalue=1;
}

//析构#/
Widget_Data_Curve::~Widget_Data_Curve()
{
    QMapIterator<int,struct CurveLine *> iter(curvemap);

    while(iter.hasNext())
    {
        iter.next();
        struct CurveLine *lineptr=iter.value();
        lineptr->pointlist.clear();
        delete lineptr;  //释放曲线结构空间#/
    }
    curvemap.clear();
}

//改变曲线窗口大小#/
void Widget_Data_Curve::resetSize(int x,int y,int w, int h)
{
    this->setGeometry(0,0,w,h);
    this->move(x,y);
    resetParameter();
}

//根据更改的窗口大小和边距，重新设置参数#/
void Widget_Data_Curve::resetParameter()
{
    int w=this->width();
    this->pFontw=w/33; //指示区内字体大小 宽度#/
    this->uFontw=w/70; //坐标区内字体大小#/
    this->ySpace=w/60; //Y轴上彼此间间隔#/
    this->xSpace=w/50; //X轴上彼此间间隔#/
    this->pointw=w/100;//曲线点宽度#/
}

//绘制曲线图上方的对应曲线颜色图标框和曲线名称文字#/
void Widget_Data_Curve::drawPrompt(QPainter *painter)
{
    int strwidth=0,textwidth=0;//图标+文字的长度、文字长度#/
    QMapIterator<int,struct CurveLine*> iter(curvemap);//建立曲线容器的迭代器#/
    int x=xmargin,y=0;     //开始绘制的x y点#/
    struct CurveLine *lineptr;  //指向曲线结构的指针#/

    QPen pen(QColor(Qt::white));//绘图画笔#/
    pen.setWidth(1);

    //这个方式设置字体，在PC端没有问题，在ARM板上，会出错死机！不能这样使用#/
//    painter->setFont(QFont(QString(CONFIG_FONT_STYLE),pFontw));

    QFont tmpfont;
#if DE_PC_PLATFORM
    tmpfont=QFont(curFont,painter->device());
    tmpfont.setPixelSize(pFontw);
    painter->setFont(tmpfont);
#else
    tmpfont=painter->font();
    tmpfont.setPixelSize(pFontw);
    painter->setFont(tmpfont);
#endif

    while(iter.hasNext())   //历遍曲线容器，获取各曲线结构#/
    {
        iter.next();     //从第一个内容开始 递增#/
        lineptr=iter.value();

        painter->setBrush(QBrush(lineptr->color));  //画刷#/

        textwidth=lineptr->curvename.size()*(pFontw); //曲线名称绘制需要的长度#/
        if(strwidth>=(pixmap.width()-xmargin-textwidth-pFontw))//换行条件#/
        {
            strwidth=0;
            y += (pFontw+ySpace);//ySpace是上下间隔#/
        }
        pen.setWidth(1);
        painter->setPen(pen);
        painter->drawRect(x+strwidth,y,pFontw,pFontw);//绘制曲线颜色指示框#/
        strwidth += pFontw;
        pen.setWidth(2);
        painter->setPen(pen);
        //绘制曲线名称#/
        painter->drawText(x+strwidth,y,textwidth,pFontw*2,Qt::AlignHCenter|Qt::AlignTop,lineptr->curvename);
        strwidth +=(textwidth+xSpace);  //累计绘制的长度，以便换行#/
    }//    while(iter.hasNext())#/
    this->curve_y=y+pFontw+2*ySpace;
}

//设置曲线框四周间隔#/
void Widget_Data_Curve::setMargin(int xmg,int ymg)
{
    this->xmargin=xmg;
    this->ymargin=ymg;
    resetParameter();
}

//设置X轴最大最小值#/
void Widget_Data_Curve::setXaxisrange(float min,float max)
{
    this->minXaxis=min;
    this->maxXaxis=max;
}

//设置Y轴最大最小值#/
void Widget_Data_Curve::setYaxisrange(float min,float max)
{
    this->minYaxis=min;
    this->maxYaxis=max;
}

//设置第2 Y轴最大最小值#/
void Widget_Data_Curve::setYaxisrange02(float min,float max)
{
    this->minYaxis2=min;
    this->maxYaxis2=max;
}

//通过设置X轴范围，自动计算Y,Y2轴范围#/
void Widget_Data_Curve::setAutomaticXaxis(float min,float max)
{
    this->minXaxis=min;
    this->maxXaxis=max;

    QMapIterator<int,struct CurveLine *>iter(curvemap);
    struct CurveLine *lineptr;
    float miny1=0,maxy1=0,miny2=0,maxy2=0;  //相应Y轴的范围#/
    float xval,yval;
    int i;
    while(iter.hasNext())   //历遍曲线结构容器#/
    {
        iter.next();        //从容器的头节点开始，第一次调用得到第一个节点#/
        lineptr=iter.value();
        for(i=0;i<lineptr->pointlist.count();i++)
        {
            xval=lineptr->pointlist[i].x();
            yval=lineptr->pointlist[i].y();
            if(xval>=minXaxis&&xval<=maxXaxis)      //X range#/
            {
                if(lineptr->yaxisnum==Y1_AXIS_ID)  //Y1
                {
                    if(miny1==0&&maxy1==0)        //用第一个点的值初始化#/
                    {
                        miny1=yval;
                        maxy1=yval;
                    }
                    if(yval<miny1)
                        miny1=yval;
                    if(yval>maxy1)
                        maxy1=yval;
                }
                else if(lineptr->yaxisnum==Y2_AXIS_ID) //Y2
                {
                    if(miny2==0&&maxy2==0)        //用第一个点的值初始化#/
                    {
                        miny2=yval;
                        maxy2=yval;
                    }
                    if(yval<miny2)
                        miny2=yval;
                    if(yval>maxy2)
                        maxy2=yval;
                }
            }//  if(xval>=minXaxis&&xval<=maxXaxis)  //X range
            else if(xval>maxXaxis)
                break;
        } // for(i=0;i<lineptr->pointlist.count();i++)#/
    }

    //按照比例计算Y轴的显示值#/
    if(miny1!=0||maxy1!=0)
    {
        this->minYaxis=miny1*(1-YAXIS_ADD_RATE);
        this->maxYaxis=maxy1*(1+YAXIS_ADD_RATE);
    }
    if(miny2!=0||maxy2!=0)
    {
        this->minYaxis2=miny2*(1-YAXIS_ADD_RATE);
        this->maxYaxis2=maxy2*(1+YAXIS_ADD_RATE);
    }
}

//使能第2 Y轴#/
void Widget_Data_Curve::enableYaxis02(bool enable)
{
    this->enableYaxis2=enable;
}

//设置指示区、坐标区的字体大小,0为使用默认#/
void Widget_Data_Curve::setFontWidth(int pFw,int uFw)
{
    if(pFw!=0)
        this->pFontw=pFw;

    if(uFw!=0)
        this->uFontw=uFw;
}

//设置所有曲线点数的上限#/
void Widget_Data_Curve::setMaxPointNum(int max)
{
    this->maxpointnum=max;
}

//设置坐标轴点数#/
void Widget_Data_Curve::setAxisTickNum(int xnum,int y1num,int y2num)
{
    this->Xticknum=xnum;
    this->Yticknum=y1num;
    this->Yticknum2=y2num;
}

//刷新并开始曲线绘制#/
void Widget_Data_Curve::refresh()
{
    int val=(this->maxXaxis/this->allMaxXvalue)*100.0;
    slider.setValue(val);                   //更新进度条位置#/
    setAutomaticXaxis(this->minXaxis,this->maxXaxis); //根据X轴范围，计算Y轴的值#/
    refreshPixmap();
}

//新添加一条曲线，并且基本设置#/
bool Widget_Data_Curve::addCurveLine(int id,enum CurveType curvetype,QString name,QColor color)
{
    if(curvemap.contains(id))  //查找该ID号是否已被使用#/
        return false;

    struct CurveLine *lineptr=new struct CurveLine();//创建曲线结构空间#/
    lineptr->type=curvetype;//曲线类型#/
    lineptr->curvename=name;//曲线名称#/
    lineptr->color=color;//曲线颜色#/
    lineptr->pointwidth=pointw;
    lineptr->yaxisnum=1;  //默认是对应左边的Y轴#/
    curvemap.insert(id,lineptr);//添加进曲线结构容器#/
    return true;
}

//添加曲线点数据#/
bool Widget_Data_Curve::setCurveData(int id,const QList<QPointF> &data)
{
    if(!curvemap.contains(id))//曲线不存在，不能添加#/
        return false;

    if(data.count()>maxpointnum)
        return false;

    struct CurveLine *lineptr=curvemap.value(id);
    lineptr->pointnum=data.count();
    lineptr->pointlist.clear();     //清空原有数据#/
    for(int i=0;i<data.count();i++) //历遍数据点，更新X轴的上下限#/
    {
        if(data[i].x()<allMinXvalue)
            allMinXvalue=data[i].x();
        if(data[i].x()>allMaxXvalue)
            allMaxXvalue=data[i].x();
    }
    lineptr->pointlist=data;  //复制数据点列表到曲线结构中#/
    return true;
}

//动态往某曲线添加一个数据点#/
bool Widget_Data_Curve::appendPoint(int id,QPointF point)
{
    if(!curvemap.contains(id))
        return false;

    struct CurveLine *lineptr=curvemap.value(id);
    if((lineptr->pointnum+1)>maxpointnum) //如果超过点数范围，则删除最前面的一个数据#/
        lineptr->pointlist.removeFirst();
    lineptr->pointlist.append(point);
    if(point.x()>allMaxXvalue)
        allMaxXvalue=point.x();
    maxXaxis=point.x();
    lineptr->pointnum=lineptr->pointlist.count();
    return true;
}

//从曲线结构中删除某一条曲线，并释放#/
bool Widget_Data_Curve::delCurveLine(int id)
{
    if(!curvemap.contains(id))//曲线不存在，则返回#/
        return false;

    struct CurveLine *lineptr=curvemap.value(id);
    lineptr->pointlist.clear();
    delete lineptr;
    if(curvemap.remove(id))
        return true;
    else
        return false;
}

//删除所有的曲线#/
void Widget_Data_Curve::delAllCurveLine()
{
    QMapIterator<int,struct CurveLine *> iter(curvemap);

    while(iter.hasNext())
    {
        iter.next();
        struct CurveLine *lineptr=iter.value();
        lineptr->pointlist.clear();
        delete lineptr;
    }
    curvemap.clear();
}

//设置曲线点的宽度#/
bool Widget_Data_Curve::setCurvePointWid(int id,int pw)
{
    if(!curvemap.contains(id)) //曲线不存在，则返回#/
        return false;

    struct CurveLine *lineptr=curvemap.value(id);
    lineptr->pointwidth=pw;
    return true;
}

//设置X轴单位名称#/
void Widget_Data_Curve::setXunitName(const QString &name)
{
    xunit_name=name;
}
//设置Y1轴单位名称#/
void Widget_Data_Curve::setY1unitName(const QString &name)
{
    yunit_name=name;
}
//设置Y2轴单位名称#/
void Widget_Data_Curve::setY2unitName(const QString &name)
{
    yunit_name2=name;
}

//设置曲线对应的Y轴坐标#/
bool Widget_Data_Curve::setCurveYtype(int id,int type)
{
    if(!curvemap.contains(id)) //曲线不存在，则返回#/
        return false;

    struct CurveLine *lineptr=curvemap.value(id);
    lineptr->yaxisnum=type;
    return true;
}

//设置X轴放大时，X轴两端值的最小极限范围#/
void Widget_Data_Curve::setMinInterval(float range)
{
    this->zoominrange=range;
}

//设置放大缩小时，X轴变化的值#/
void Widget_Data_Curve::setZoomChange(float val)
{
    this->zoomeachvalue=val;
}

//在画布pixmap的基础上绘制曲线相关内容，并刷新#/
//先将绘图内容放在pixmap，存在内存中，最后调用显示#/
void Widget_Data_Curve::refreshPixmap()
{
    int w,h;
    w=labelcurve.width();
    h=labelcurve.height();

    pixmap=QImage(w,h,QImage::Format_RGB16); //根据边距计算创建画布大小#/
    pixmap.fill(QColor(Qt::black));

    QPainter painter(&pixmap);//以pixmap为画布#/

    /* 测试用，结论：每一个画布都有自己独立的坐标系
     * 原点（0，0）即该画布左上角，与画布父窗口无关
    QPen pen(QColor(Qt::white));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(QColor(Qt::yellow));
    painter.drawRect(-10,-10,50,50);
    */
    drawPrompt(&painter);  //画指示信息#/
    drawGrid(&painter);    //画坐标系#/
    drawArrows(&painter);  //绘制坐标箭头#/
    drawCurves(&painter);  //画曲线#/
    update();
}

//绘制坐标系相关内容#/
//其中轴上坐标点可设置#/
void Widget_Data_Curve::drawGrid(QPainter *painter)
{
    int i=0,x=0,y=0,len=0;
    int unitspace=0;      //Y坐标值字符串占用的宽度#/
    int unitspace2=0;     //Y2坐标值字符串占用的宽度#/
    float dvalx=0,dvaly=0;  //x，y轴上每段对应的像素值#/
    float dvaly2=0;         //Y2轴上每段对应的像素值#/
    float dval=0;         //轴坐标点上的间隔数值#/
    float dval2=0;        //Y2轴坐标点上的间隔数值#/
    QString unitlabel;  //坐标数值和单位字符串#/
    QPen pen(QColor(Qt::white)); //画笔颜色#/
    pen.setWidth(1);

    dval=(maxYaxis-minYaxis)/Yticknum; //Y轴上间隔的数值#/
    unitlabel.setNum(dval,'f',1); //Y轴坐标值#/
    unitlabel.append(yunit_name);
    len=unitlabel.size();
    unitspace=len*uFontw; //Y轴坐标值字符串占用的宽度#/

    x=xmargin;                  //开始绘制坐标系的x坐标值#/
    y=pixmap.height()-ymargin;   //开始绘制坐标系的y坐标值#/
    dvalx=(pixmap.width()-2*xmargin)/Xticknum;
    dvaly=(pixmap.height()-ymargin-curve_y)/Yticknum;

    if(enableYaxis2==true)  //Y2
    {
        dval2=(maxYaxis2-minYaxis2)/Yticknum2; //Y2轴上间隔的数值#/
        unitlabel.setNum(dval2,'f',1); //Y轴坐标值#/
        unitlabel.append(yunit_name2);
        len=unitlabel.size();
        unitspace2=len*uFontw; //Y轴坐标值字符串占用的宽度#/
        dvaly2=(pixmap.height()-ymargin-curve_y)/Yticknum2;
    }

    painter->setPen(pen);

    //    painter->setFont(QFont(QString(CONFIG_FONT_STYLE),uFontw,2));#/
        QFont tmpfont;
    #if DE_PC_PLATFORM
        tmpfont=QFont(curFont,painter->device());
        tmpfont.setPixelSize(uFontw);
        tmpfont.setWeight(2);
        painter->setFont(tmpfont);
    #else
        tmpfont=painter->font();
        tmpfont.setPixelSize(uFontw);
        tmpfont.setWeight(2);
        painter->setFont(tmpfont);
    #endif

    dval=(maxXaxis-minXaxis)/Xticknum;  //x轴上间隔的数值#/

//----------------X轴绘制---------------------------
    painter->drawLine(x,y,pixmap.width()-xmargin,y);  //绘制X轴#/
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    for(i=0;i<=Xticknum;i++)
    {
        unitlabel.setNum(minXaxis+dval*i,'f',1); //X轴坐标值#/
        unitlabel.append(xunit_name);
        len=unitlabel.size();
        //绘制X轴坐标文字#/
        painter->drawText(x+dvalx*i-uFontw,y+3,len*uFontw,uFontw*2,Qt::AlignTop,unitlabel);
//      if(i!=Xticknum)//绘制X轴对应坐标竖线#/
            painter->drawLine(x+dvalx*i,y,x+dvalx*i,curve_y);
    }
//----------------Y轴绘制---------------------------
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawLine(x,y,x,curve_y); //绘制第一Y轴#/
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    dval=(maxYaxis-minYaxis)/Yticknum; //Y轴上间隔的数值#/
    for(i=0;i<=Yticknum;i++)
    {
        unitlabel.setNum(minYaxis+dval*i,'f',1);
        unitlabel.append(yunit_name);
        painter->drawText(x-unitspace,y-dvaly*i-uFontw,unitspace,uFontw*2,Qt::AlignRight,unitlabel);
//        if(i!=Yticknum)#/
            painter->drawLine(x,y-dvaly*i,pixmap.width()-xmargin,y-dvaly*i);
    }

    if(enableYaxis2==true)  //绘制第二Y轴#/
    {
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
        painter->drawLine(pixmap.width()-xmargin,y,pixmap.width()-xmargin,curve_y);
        for(i=0;i<=Yticknum2;i++)
        {
            unitlabel.setNum(minYaxis2+dval2*i,'f',1);
            unitlabel.append(yunit_name2);
            if(i==0)  //特别处理0点坐标的位置#/
                painter->drawText(pixmap.width()-xmargin,y-dvaly2*i-2*uFontw,unitspace2,uFontw*2,Qt::AlignLeft,unitlabel);
            else
                painter->drawText(pixmap.width()-xmargin,y-dvaly2*i-uFontw,unitspace2,uFontw*2,Qt::AlignLeft,unitlabel);
        }
    }

}

//将曲线结构中的点数据转换为坐标系上的坐标,ytype:Y轴对应的序号，以标示对应那个Y轴#/
QVector<QPointF> Widget_Data_Curve::transformxy(char ytype, struct CurveLine *ptr)
{
    QVector<QPointF> xycoor;
    float x,y;
    xycoor.clear();
    if(ytype==Y1_AXIS_ID)  //对应左边的Y轴#/
    {
        float dx=maxXaxis-minXaxis; //X轴的距离值#/
        float dy=maxYaxis-minYaxis;
        for(int i=0;i<ptr->pointlist.count();i++)
        {
            x=ptr->pointlist[i].x();//将曲线结构中的点数据#/
            y=ptr->pointlist[i].y();

            if(x>=minXaxis&&x<=maxXaxis)//只在当前的X轴范围内转换#/
            {
                x=xmargin+((x-minXaxis)/dx)*(pixmap.width()-2*xmargin);
                y=pixmap.height()-ymargin-(((y-minYaxis)/dy)*(pixmap.height()-curve_y-ymargin));
                //只精确到小数点后一位，不然画出的点会有偏差#/
//                x=((int)(x*10))/10.0;
//                y=((int)(y*10))/10.0;
                xycoor.append(QPointF(x,y));
            }
        }
    }
    else if(ytype==Y2_AXIS_ID)// 对应右边的Y轴#/
    {
        float dx=maxXaxis-minXaxis; //X轴的距离值#/
        float dy=maxYaxis2-minYaxis2;
        for(int i=0;i<ptr->pointlist.count();i++)
        {
            x=ptr->pointlist[i].x();//将曲线结构中的点数据#/
            y=ptr->pointlist[i].y();

            if(x>=minXaxis&&x<=maxXaxis)
            {
                x=xmargin+((x-minXaxis)/dx)*(pixmap.width()-2*xmargin);
                y=pixmap.height()-ymargin-((y-minYaxis2)/dy)*(pixmap.height()-curve_y-ymargin);
                xycoor.append(QPointF(x,y));
            }
        }
    }
    return xycoor;
}

//根据坐标点绘制点#/
void Widget_Data_Curve::drawPoint(QPainter *painter,const QVector<QPointF> &xy,const QColor &color,int width)
{
    QPen pen(color);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->setBrush(color);
    for(int i=0;i<xy.count();i++)
    {
//      painter->drawPoint(xy[i].x(),xy[i].y());
        painter->drawEllipse(xy[i].x()-width/2,xy[i].y()-width/2,width,width);
    }
}

//绘制曲线图形#/
void Widget_Data_Curve::drawCurves(QPainter *painter)
{
    struct CurveLine *lineptr;
    QPen pen;
    QVector<QPointF> xycoor;   //存放转换为坐标的点数据#/
    QMapIterator<int,struct CurveLine *>iter(curvemap);
    int i;
    while(iter.hasNext())   //历遍曲线结构容器#/
    {
        xycoor.clear();
        iter.next();        //从容器的头节点开始，第一次调用得到第一个节点#/
        lineptr=iter.value();
        pen.setColor(lineptr->color);
        pen.setWidth(1);
        xycoor=transformxy(lineptr->yaxisnum,lineptr);  //点数据转换为坐标数据#/
        drawPoint(painter,xycoor,lineptr->color,lineptr->pointwidth);//根据曲线参数绘制点#/
        switch(lineptr->type)
        {
        case Pointtype:  //离散点类型#/
            break;
        case Laddertype:  //阶梯线类型#/
//            drawPoint(painter,xycoor,lineptr->color,1);
            painter->setPen(pen);
            for(i=0;i<xycoor.count();i++)
            {
                if(i>0) //绘制两点间的连线#/
                    painter->drawLine(xycoor[i-1].x(),xycoor[i-1].y(),\
                            xycoor[i].x(),xycoor[i].y());
            }
            break;
        case Columntype:  //高低柱状线类型#/
//            drawPoint(painter,xycoor,lineptr->color,5);
            painter->setPen(pen);
            for(i=0;i<xycoor.count();i+=2)
            {
                if((i+1)<xycoor.count())
                    painter->drawLine(xycoor[i].x(),xycoor[i].y(),\
                            xycoor[i+1].x(),xycoor[i+1].y());
            }
            break;
        case Linetype:    //点连线类型#/
//            drawPoint(painter,xycoor,lineptr->color,5);
            painter->setPen(pen);
            for(i=0;i<xycoor.count();i++)
            {
                if(i>0)
                    painter->drawLine(xycoor[i-1].x(),xycoor[i-1].y(),\
                            xycoor[i].x(),xycoor[i].y());
            }
            break;
        default:
            break;
        }
    }
}

//绘制坐标系的箭头#/
void Widget_Data_Curve::drawArrows(QPainter *painter)
{
    QPen pen(Qt::white);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
//----------------绘制第一Y轴的箭头----------------------------
    float x1 = xmargin;           //起点#/
    float y1 = curve_y-1.5*ySpace;
    float x2 = xmargin;          //终点#/
    float y2 = curve_y;
    float l = ySpace;                   //箭头的那长度#/
    float a = 0.5;                      //箭头与线段角度#/
    float x3 = x2 - l * cos(atan2((y2 - y1) , (x2 - x1)) - a);
    float y3 = y2 - l * sin(atan2((y2 - y1) , (x2 - x1)) - a);
    float x4 = x2 - l * sin(atan2((x2 - x1) , (y2 - y1)) - a);
    float y4 = y2 - l * cos(atan2((x2 - x1) , (y2 - y1)) - a);
    painter->drawLine(x1,y1,x3,y3); //箭头斜线#/
    painter->drawLine(x1,y1,x4,y4); //箭头斜线#/
    painter->drawLine(x1,y1,x2,y2);
//----------------绘制X轴的箭头----------------------------#/
    x1 = pixmap.width()-xmargin+1.5*ySpace;           //起点#/
    y1 = pixmap.height()-ymargin;
    x2 = pixmap.width()-xmargin;         //终点#/
    y2 = pixmap.height()-ymargin;
    x3 = x2 - l * cos(atan2((y2 - y1) , (x2 - x1)) - a);
    y3 = y2 - l * sin(atan2((y2 - y1) , (x2 - x1)) - a);
    x4 = x2 - l * sin(atan2((x2 - x1) , (y2 - y1)) - a);
    y4 = y2 - l * cos(atan2((x2 - x1) , (y2 - y1)) - a);
    painter->drawLine(x1,y1,x3,y3); //箭头斜线#/
    painter->drawLine(x1,y1,x4,y4); //箭头斜线#/
    painter->drawLine(x1,y1,x2,y2);
//----------------绘制第二Y轴的箭头----------------------------#/
    if(enableYaxis2==true)
    {
        x1 = pixmap.width()-xmargin;          //起点#/
        y1 = curve_y-1.5*ySpace;
        x2 = pixmap.width()-xmargin;          //终点#/
        y2 = curve_y;
        x3 = x2 - l * cos(atan2((y2 - y1) , (x2 - x1)) - a);
        y3 = y2 - l * sin(atan2((y2 - y1) , (x2 - x1)) - a);
        x4 = x2 - l * sin(atan2((x2 - x1) , (y2 - y1)) - a);
        y4 = y2 - l * cos(atan2((x2 - x1) , (y2 - y1)) - a);
        painter->drawLine(x1,y1,x3,y3); //箭头斜线#/
        painter->drawLine(x1,y1,x4,y4); //箭头斜线#/
        painter->drawLine(x1,y1,x2,y2);
    }
}

void Widget_Data_Curve::paintEvent(QPaintEvent *event)
{
    event=event;

    QPainter painter(this);
    QPen pen(QColor(Qt::white));
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(QRect(0, 0, this->width(), this->height()));
    if(!pixmap.isNull())
        painter.drawImage(QPoint(0,0),pixmap);
}

//重新改变窗口大小事件，根据labelcurve的大小重新计算和刷新pixmap#/
void Widget_Data_Curve::resizeEvent(QResizeEvent * event)
{
    event=event;
    refreshPixmap();
}

//鼠标在坐标系内点击时，显示指示竖线，并且对应曲线显示值#/
void Widget_Data_Curve::mousePressEvent(QMouseEvent * event)
{
    int x,y;        //当前点击的坐标#/
    x=event->x();
    y=event->y();
    int x2,y2;     //开始绘制数据信息框的起始坐标 这些定义值都是为了便于修改，可用宏定义代替#/
    x2=xmargin+2*uFontw;
    y2=curve_y-uFontw;
    int Rectw=2*uFontw;  //颜色指示框宽度#/
    int strwidth=0;     //计算已绘制的内容长度#/
    float w,h,dw,dh;      //用于计算选中点数值#/
    int len,i,pointw;

    QString strnum;     //显示的数值字符串#/
    struct CurveLine *lineptr;
    QMapIterator<int,struct CurveLine *>iter(curvemap);
    QVector<QPointF> xycoor;

    QPen pen;
    pen.setWidth(1);
    QPainter painter;

    //判断点击是否落在坐标系之内#/
    if((x>=xmargin&&x<=(pixmap.width()-xmargin))&&(y>=curve_y&&y<=(pixmap.height()-ymargin)))
    {
        if(pressMark==true)
            refreshPixmap(); //刷新画布#/
        else
            pressMark=true;

        painter.begin(&pixmap);   //因为 refreshPixmap()之后，pixmap已经改变，必须重新加载，不然会出错#/

//        painter.setFont(QFont(QString(CONFIG_FONT_STYLE),uFontw));#/
        QFont tmpfont;
#if DE_PC_PLATFORM
        tmpfont=QFont(curFont,painter.device());
        tmpfont.setPixelSize(uFontw);
        painter.setFont(tmpfont);
#else
        tmpfont=painter.font();
        tmpfont.setPixelSize(uFontw);
        painter.setFont(tmpfont);
#endif

        pen.setColor(Qt::green);
        painter.setPen(pen);
        painter.drawLine(x,pixmap.height()-ymargin,x,curve_y);//绘制指示竖线#/

        //历遍曲线结构#/
        while(iter.hasNext())
        {
            xycoor.clear();    //清空容器#/
            strnum.clear();    //清空字符串#/
            iter.next();
            lineptr=iter.value();//获取具体曲线结构指针#/
            xycoor=transformxy(lineptr->yaxisnum,lineptr);  //点数据转换为坐标数据#/
            pointw=lineptr->pointwidth;  //点宽度#/
            switch(lineptr->type)
            {
            //这两类曲线，要选中具体点，方才显示数据#/
            case Pointtype:   //离散点类型#/
            case Columntype:  //高低柱状线类型#/
                for(i=0;i<xycoor.count();i++)
                {
                    //当点击中某个点时，必须转换为整数比较，不然带小数不相等#/
               //     if(((int)xycoor[i].x())==x)#/
                    if(x>(xycoor[i].x()-pointw)&&x<(xycoor[i].x()+pointw))
                    {

                        //将该点数据放进字符串#/
                        strnum.setNum(lineptr->pointlist[i].x(),'f',1);
                        strnum.append(" | ");
                        strnum.append(QString().setNum(lineptr->pointlist[i].y(),'f',1));
                        strnum.append(" ");
                        if(lineptr->yaxisnum==1)
                            strnum.append(yunit_name);
                        else if(lineptr->yaxisnum==2)
                            strnum.append(yunit_name2);
                        len=(strnum.size()-4)*uFontw;

                        if(strwidth>=(pixmap.width()-2*xmargin-Rectw-len-xSpace))//换行条件#/
                        {
                            strwidth=0;
                            y2 += (Rectw+ySpace);//ySpace是上下间隔#/
                        }

                        pen.setColor(lineptr->color);
                        painter.setPen(pen);
                        painter.setBrush(QBrush(lineptr->color));  //画刷#/

                        painter.drawRect(x2+strwidth,y2,Rectw,Rectw);//绘制曲线颜色指示框#/
                        strwidth += Rectw;

                        //绘制信息框和内容#/
                        pen.setColor(Qt::black);
                        painter.setPen(pen);
                        painter.setBrush(Qt::white);  //画刷#/
                        painter.drawRect(x2+strwidth+uFontw,y2,len,Rectw);//绘制颜色框#/
                        painter.drawText(x2+strwidth+uFontw,y2,len,Rectw,Qt::AlignHCenter,strnum);//数值信息
                        strwidth +=(uFontw+len);
                        strwidth += xSpace;    //与下一个点信息的间距#/

                        if(lineptr->type==Columntype)  //高低柱状线时，由于两个点同时X轴相等，因此要一次显示两个点的信息#/
                        {
                            ++i;                   //获取下一个点#/
                            //将该点数据放进字符串#/
                            strnum.setNum(lineptr->pointlist[i].x(),'f',1);
                            strnum.append(" | ");
                            strnum.append(QString().setNum(lineptr->pointlist[i].y(),'f',1));
                            strnum.append(" ");
                            if(lineptr->yaxisnum==1)
                                strnum.append(yunit_name);
                            else if(lineptr->yaxisnum==2)
                                strnum.append(yunit_name2);
                            len=(strnum.size()-4)*uFontw;

                            if(strwidth>=(pixmap.width()-2*xmargin-Rectw-len-xSpace))//换行条件#/
                            {
                                strwidth=0;
                                y2 += (Rectw+ySpace);//ySpace是上下间隔#/
                            }

                            painter.drawRect(x2+strwidth,y2,len,Rectw);//绘制颜色框#/
                            painter.drawText(x2+strwidth,y2,len,Rectw,Qt::AlignHCenter,strnum);//数值信息#/
                            strwidth +=len;
                            strwidth += xSpace;    //与下一个点信息的间距#/
                        }
                    }
                }   // for(i=0;i<xycoor.count();i++)#/
                break;

            //这两类曲线，只要指示线穿过曲线，便显示数据#/
            case Laddertype:  //阶梯线类型#/
            case Linetype:     //点连线类型#/
                for(i=0;i<xycoor.count();i++)
                {
                    //获取选中线右边的第一个点，然后获取左边的一个点，再计算出选中点的数值#/
              //      if(((int)xycoor[i].x())>=x)#/
                    if((xycoor[i].x()+pointw)>x)
                    {
                        if(i>=1)
                        {
                            w=xycoor[i].x()-xycoor[i-1].x();
                            h=xycoor[i].y()-xycoor[i-1].y();
                            dw=x-xycoor[i-1].x();
                            dh=dw*(h/w);
                            h=xycoor[i-1].y()+dh;  //计算出指示线与曲线交点的Y坐标值#/
                            w=x;                   //指示线与曲线交点的X坐标值#/
                        }
                        else if(i==0&&((x>(xycoor[i].x()-pointw)&&x<(xycoor[i].x()+pointw)))) //选中了曲线的第一个点
                        {
                            h=xycoor[i].y();
                            w=x;
                        }
                        else
                        {
                            break;    //直接跳出循环#/
                        }

                        //将坐标值转换为点数据值#/
                        w=((w-xmargin)/(pixmap.width()-2*xmargin))*(maxXaxis-minXaxis)+minXaxis;//X
                        if(lineptr->yaxisnum==1)
                            h=((pixmap.height()-ymargin-h)/(pixmap.height()-ymargin-curve_y))*(maxYaxis-minYaxis)+minYaxis;//Y
                        else
                            h=((pixmap.height()-ymargin-h)/(pixmap.height()-ymargin-curve_y))*(maxYaxis2-minYaxis2)+minYaxis2;//Y

                        //将该点数据放进字符串#/
                        strnum.setNum(w,'f',1);
                        strnum.append(" | ");
                        strnum.append(QString().setNum(h,'f',1));
                        strnum.append(" ");
                        if(lineptr->yaxisnum==1)
                            strnum.append(yunit_name);
                        else if(lineptr->yaxisnum==2)
                            strnum.append(yunit_name2);
                        len=(strnum.size()-4)*uFontw;

                        if(strwidth>=(pixmap.width()-2*xmargin-2*uFontw-len-xSpace))//换行条件#/
                        {
                            strwidth=0;
                            y2 += (Rectw+ySpace);//ySpace是上下间隔#/
                        }

                        pen.setColor(lineptr->color);
                        painter.setPen(pen);
                        painter.setBrush(QBrush(lineptr->color));  //画刷#/

                        painter.drawRect(x2+strwidth,y2,Rectw,Rectw);//绘制曲线颜色指示框#/
                        strwidth += Rectw;

                        //绘制信息框和内容#/
                        pen.setColor(Qt::black);
                        painter.setPen(pen);
                        painter.setBrush(Qt::white);  //画刷#/
                        painter.drawRect(x2+strwidth+uFontw,y2,len,Rectw);//绘制颜色框#/
                        painter.drawText(x2+strwidth+uFontw,y2,len,Rectw,Qt::AlignHCenter,strnum);//数值信息
                        strwidth +=(uFontw+len);
                        strwidth += xSpace;    //与下一个点信息的间距#/

                        break;    //直接跳出循环#/
                    }  //if(xycoor[i].x()>=x)
                }   // for(i=0;i<xycoor.count();i++)
                break;

            default:
                break;
            }

        }  //while(iter.hasNext())
        update();   //刷新显示#/
    }   //判断点击是否落在坐标系之内#/
    else
    {
        if(pressMark==true)
        {
            pressMark=false;
            refreshPixmap(); //刷新画布#/
        }
    }
}

//鼠标移动时处理#/
void Widget_Data_Curve::mouseMoveEvent(QMouseEvent * event)
{
    mousePressEvent(event);
}

//------------------------------各类信号槽处理-----------------------#/
//放大按钮点击#/
void Widget_Data_Curve::zoomInt()
{
    if(this->maxXaxis>=(this->minXaxis+zoominrange+zoomeachvalue))
        this->maxXaxis -= zoomeachvalue;
    else if(this->maxXaxis>(this->minXaxis+zoominrange))
        this->maxXaxis=this->minXaxis+zoominrange;

    this->refresh();
}

//缩小#/
void Widget_Data_Curve::zoomOut()
{
    float xmaxvalue;     // X轴的上限值#/
    int zoomeachvalue=1;    //每次放大缩小时，X轴右边值减少的数值#/
    xmaxvalue=this->allMaxXvalue;

    if((this->maxXaxis+zoomeachvalue)<=xmaxvalue)
        this->maxXaxis += zoomeachvalue;
    else if((this->maxXaxis)<xmaxvalue)
        this->maxXaxis = xmaxvalue;
    else if(this->maxXaxis==xmaxvalue)
        this->minXaxis=0;

    this->refresh();
}

//滑动条#/
void Widget_Data_Curve::setSlideXaxis()
{
    int slideval=slider.value();
    float xmaxvalue;
    //X轴门限值#/
    xmaxvalue=this->allMaxXvalue;

    float curval=(slideval/100.0)*xmaxvalue;   //当前滑动条位置对应的X轴最大值#/
    float dval=curval-this->maxXaxis;        //与目前X轴显示的最大值的差值#/

    if(dval>=0)  //往右拉动时#/
    {
        if((this->maxXaxis+dval)<=xmaxvalue)
        {
            this->maxXaxis += dval;
            this->minXaxis += dval;
        }
        else
        {
            curval=this->maxXaxis-this->minXaxis;
            this->maxXaxis=xmaxvalue;
            this->minXaxis=xmaxvalue-curval;
        }
    }
    else if(dval<0)  //往左拉动时#/
    {
        dval=dval*(-1);

        if((this->minXaxis-dval)>=allMinXvalue)
        {
            this->minXaxis -= dval;
            this->maxXaxis -= dval;
        }
        else if(this->minXaxis>allMinXvalue)
        {
            dval=this->maxXaxis-this->minXaxis;
            this->minXaxis=allMinXvalue;
            this->maxXaxis=allMinXvalue+dval;
        }
        else if(this->minXaxis==allMinXvalue)
        {
            if(this->maxXaxis>=(allMinXvalue+zoominrange+dval))
                this->maxXaxis -= dval;
            else
                this->maxXaxis=allMinXvalue+zoominrange;
        }
    }
    this->refresh();
}
