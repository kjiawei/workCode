#ifndef WIDGET_CURVE_H
#define WIDGET_CURVE_H

#include <QtGui>
#include <QWidget>
#include <QPainter>
#include <QTimer>

struct CurvePoint
{
    int offset;
    int value;
};

struct TargetCurve
{
    int offset;
    int longTime;
    int value;
};

enum TimeUnit
{
    Second  = 1,
    Minute  = 2,
    Hour    = 3
};

struct drawLine
{
    QList <CurvePoint> testPointList;
    QVector <TargetCurve> targetCurveList;
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
    int sec;
    bool isStart;
    int drawIndex;
    QColor color;
};

class WidgetCurve : public QWidget
{
    Q_OBJECT
public:
    WidgetCurve(QWidget * parent = 0, Qt::WindowFlags f = 0);
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
    ~WidgetCurve();
    void setGeometry(int x, int y, int w, int h);
    void setGeometry(const QRect & a);
    void start();
    void stop();
    void setSecZero();
    int getSec(int drawIndex = 0);
    void setUnitX(TimeUnit unitX);
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
    TimeUnit unitX;

    struct drawLine *line;

    void paintPoint(int offset, int value, int option, int drawIndex = 0);
    void initPos();

    int drawCurveCount;
};

#endif // WIDGET_CURVE_H
