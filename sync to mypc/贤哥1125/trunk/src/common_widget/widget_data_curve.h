/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-12-10
** 修改人：Huangjunxian
** 修改时间：2014-12-17
** 描述：
**    多功能数据曲线控件 《头文件》
*****************************************************/
#ifndef WIDGET_DATA_CURVE_H
#define WIDGET_DATA_CURVE_H

#include <QMap>
#include <QPixmap>
#include <QList>
#include <QWidget>
#include <QObject>
#include <QVector>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QGridLayout>
#include <QMouseEvent>


#define Y1_AXIS_ID  1       //Y1轴宏定义#/
#define Y2_AXIS_ID  2       //Y2轴宏定义#/

enum CurveType{
    Pointtype,   //离散点类型#/
    Laddertype,  //阶梯线类型#/
    Columntype,  //高低柱状线类型#/
    Linetype     //点连线类型#/
};


struct CurveLine
{
    enum CurveType type;
    QString curvename;
    QList<QPointF>pointlist;
    int pointnum;               //曲线目前的点数#/
    int pointwidth;             //曲线点宽度#/
    char yaxisnum;
    QColor color;               //曲线颜色#/
};


//自定义滑动栏
class SliderClick : public QSlider
{
    Q_OBJECT
public:
    SliderClick(QWidget *parent = 0) : QSlider(parent)
    {
    }

signals:
    void clickedGroove();

protected:
    void mousePressEvent(QMouseEvent *ev)
    {
        QSlider::mousePressEvent(ev);
        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）#/
        double pos = ev->pos().x() / (double)width();
        setValue(pos * (maximum() - minimum()) + minimum());
        emit clickedGroove();
    }
};


class Widget_Data_Curve:public QWidget
{
    Q_OBJECT
public:
    Widget_Data_Curve(QWidget *parent=0);     //构造函数#/
    ~Widget_Data_Curve();                   //析构函数#/
    //设置曲线窗口尺寸#/
    void resetSize(int x,int y,int w, int h);
    //设置坐标系左右和下方的间隔#/
    void setMargin(int xmg,int ymg);
    //是否使能和显示Y2轴#/
    void enableYaxis02(bool enable);
    //设置标题栏和坐标系的字体大小#/
    void setFontWidth(int pFw=0,int uFw=0);
     //设置所有曲线点数上限，默认为200#/
    void setMaxPointNum(int max);
     //设置所有坐标轴分段数#/
    void setAxisTickNum(int xnum=4,int y1num=5,int y2num=6);
    //添加一个曲线（id为标识号），并设置类型等#/
    bool addCurveLine(int id,enum CurveType curvetype,QString name,QColor color);
    //为其中一个曲线添加数据点#/
    bool setCurveData(int id,const QList<QPointF> &data);
    //为其中一个曲线添加一个数据点,并动态改变曲线X轴#/
    bool appendPoint(int id,QPointF point);
    //删除选中的曲线#/
    bool delCurveLine(int id);
    //删除所有的曲线#/
    void delAllCurveLine();
    //设置选中的曲线的点大小#/
    bool setCurvePointWid(int id,int pw);
    //设置X轴单位名称#/
    void setXunitName(const QString &name);
    //设置Y1轴单位名称#/
    void setY1unitName(const QString &name);
    //设置Y2轴单位名称#/
    void setY2unitName(const QString &name);
    //设置选中的曲线归属那个Y轴，1：左边 2：右边#/
    bool setCurveYtype(int id,int type);
    //设置X轴放大时，两端值的最小极限范围#/
    void setMinInterval(float range);
     //设置放大缩小时，X轴变化的值#/
    void setZoomChange(float val);
    //刷新曲线图#/
    void refresh();

private:
    //设置X轴值范围#/
    void setXaxisrange(float min,float max);
    //设置Y轴值范围#/
    void setYaxisrange(float min,float max);
    //设置Y2轴值范围#/
    void setYaxisrange02(float min,float max);
    //刷新画布#/
    void refreshPixmap();
    //通过设置X轴范围，自动计算Y轴范围#/
    void setAutomaticXaxis(float min,float max);
    //绘制网格坐标#/
    void drawGrid(QPainter *painter);
    //绘制曲线图形#/
    void drawCurves(QPainter *painter);
    //绘制坐标系的箭头#/
    void drawArrows(QPainter *painter);
    //根据更改的窗口大小和边距，重新设置参数#/
    void resetParameter();
    //绘制曲线图上方的对应曲线颜色图标框和曲线名称文字#/
    void drawPrompt(QPainter *painter);
    //将曲线的数据点，转换为坐标点#/
    QVector<QPointF> transformxy(char ytype, struct CurveLine *ptr);
    //画点函数#/
    void drawPoint(QPainter *painter,const QVector<QPointF> &xy,const QColor &color,int width);

protected:
    void paintEvent(QPaintEvent *event);        //重绘事件#/
    void resizeEvent(QResizeEvent * event);     //重新改变窗口大小#/
    void mousePressEvent(QMouseEvent * event);  //点击事件#/
    void mouseMoveEvent(QMouseEvent * event);   //鼠标移动时处理#/

public slots:
    void zoomInt();
    void zoomOut();
    void setSlideXaxis();

private:
    QImage pixmap;          //画布，即绘图缓冲区#/
    QFont curFont;      
    QMap<int,struct CurveLine *> curvemap;//曲线结构对应列表#/
    QLabel labelcurve;      //显示曲线图的区域#/
    QPushButton bzoomin;    //放大按钮#/
    QPushButton bzoomout;   //缩小按钮#/
    SliderClick slider;     //滑动条#/
    QGridLayout layout;     //布局控件#/
    int xmargin;            //和曲线窗口左右间隔#/
    int ymargin;            //上下间隔#/
    int curve_y;            //开始绘制曲线坐标系的y坐标#/
    float minXaxis;         //当前曲线显示区域的X轴最小值#/
    float maxXaxis;         //当前X轴最大值#/
    float allMinXvalue;     //整个X轴最小值#/
    float allMaxXvalue;     //整个X轴最大值#/
    float minYaxis;         //Y轴最小#/
    float maxYaxis;         //Y轴最大#/
    float minYaxis2;        //第2 Y轴最小 右边#/
    float maxYaxis2;        //第2 Y轴最大 右边#/
    float zoominrange;      //X轴放大时，X轴两端值的最小极限范围#/
    float zoomeachvalue;    //每次放大/缩小时，X轴右边值减/增的数值 单位小时#/
    int Xticknum;           //X轴坐标标记点数#/
    int Yticknum;           //Y1轴坐标标记点数#/
    int Yticknum2;          //Y2轴坐标标记点数#/
    bool enableYaxis2;      //使能 第2 Y轴#/
    int maxpointnum;        //曲线点数最大值，不能超过这个范围#/

    QString xunit_name;     //X轴单位名称#/
    QString yunit_name;     //Y轴单位名称#/
    QString yunit_name2;    //第2 Y轴单位名称#/

    char pFontw;            //指示区内字体大小#/
    char uFontw;            //坐标区内字体大小#/
    int ySpace;             //Y轴上彼此间间隔#/
    int xSpace;             //X轴上彼此间间隔#/
    char pointw;            //曲线点宽度默认值#/
    bool pressMark;         //标记鼠标是否在坐标系内点击#/
};


#endif
