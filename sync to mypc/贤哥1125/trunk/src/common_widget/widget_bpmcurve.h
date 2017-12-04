#ifndef WIDGET_BPMCURVE_H
#define WIDGET_BPMCURVE_H

#include <QtGui>
#include <QWidget>
#include <QPainter>
#include <QTimer>
//#include "widget_curve.h"

struct BpmCurvePoint  //实时数据曲线#/
{
    int offset;
    int value;
};

struct BpmTargetCurve //目标曲线#/
{
    int offset;
    int longTime;
    int value;
};

enum BpmTimeUnit
{
    bpmSecond  = 1,
    bpmMinute  = 2,
    bpmHour    = 3
};

struct BpmdrawLine //曲线结构#/
{
    QList <BpmCurvePoint> testPointList;
    QVector <BpmTargetCurve> targetCurveList;
    int valueRange;
    int minValue;
    int maxValue;
    float valueZoom;
    QString unit;
    QImage bufferImage;
    int finishInit;
    int lastX;
    int lastY;
    QTimer timer;
    int sec;   //定时秒数#/
    bool isStart;
    int drawIndex; //曲线序号#/
    QColor color;
};

class WidgetBpmCurve : public QWidget
{
    Q_OBJECT
public:
    WidgetBpmCurve(QWidget * parent = 0, Qt::WindowFlags f = 0);
    void setDrawCurveCount(int drawCurveCount = 1);
    void setTestTime(int testTime);
    void addTargetCurve(int offset,int longTime,int value, int drawIndex = 0);
    void delTargetCurve(int drawIndex = 0);
    void delAllTargetCurve(int drawIndex = 0);
    void addTestPoint(int value, int drawIndex = 0);
    void delAllTestPoint(int drawIndex = 0);
    void setMinValue(int minValue, int drawIndex = 0);
    void setMaxValue(int maxValue, int drawIndex = 0);
    void setMode(int mode);
    void setValueZoom(float valueZoom, int drawIndex = 0);
    void setHorizontalScale(int horizontalScale);
    void setVerticalScale(int verticalScale);
    void setUnit(QString unit, int drawIndex = 0);
    void setXOffset(int xOffset);
    void setYOffset(int yOffset);
    ~WidgetBpmCurve();
    void setGeometry(int x, int y, int w, int h);
    void setGeometry(const QRect & a);
    void start();
    void stop();
    void setSecZero();
    int getSec(int drawIndex = 0);
    void setUnitX(BpmTimeUnit unitX);
    void setLineColor(QColor lineColor, int drawIndex = 0);

public slots:
    void draw();
    void secCount();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int curveMode;
    int rect_width;
    int rect_height;
    int testTime;

    void setValueRange(int drawIndex = 0);
    int horizontalScale;
    int verticalScale;
    int yOffset;
    int xOffset;
    int xEndOffset;

    BpmTimeUnit unitX;

    struct BpmdrawLine *line; //曲线结构数组#/

    void paintPoint(int offset, int value, int option, int drawIndex = 0);
    void initPos();

    int drawCurveCount;
};

#endif // WIDGET_CURVE_H
