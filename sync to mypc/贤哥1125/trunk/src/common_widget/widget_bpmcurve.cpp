#include <QtGui>
#include "config_ui.h"
#include "widget_bpmcurve.h"

WidgetBpmCurve::WidgetBpmCurve(QWidget *parent,Qt::WindowFlags f) :
    QWidget(parent,f)
{
    unitX = bpmSecond;
    yOffset = 30;
    xOffset = 70;
    xEndOffset=50;
    line = NULL;
    drawCurveCount = 0;
    setDrawCurveCount();
}

//根据曲线数量创建曲线#/
void WidgetBpmCurve::setDrawCurveCount(int drawCurveCount)
{
    if (drawCurveCount <= 0)
    {
        qDebug() << "drawCurveCount is less equal 0";
        return;
    }

    if (line != NULL)
    {
        for(int i=0;i<this->drawCurveCount;i++)
        {
            line[i].testPointList.clear();
            line[i].targetCurveList.clear();

            disconnect(&line[i].timer,SIGNAL(timeout()),this,SLOT(secCount()));
        }

        delete [] line;
        line = NULL;
    }

    this->drawCurveCount = drawCurveCount;
    line = new struct BpmdrawLine[drawCurveCount];

    for(int i=0;i<drawCurveCount;i++)
    {
        line[i].color = Qt::yellow;
        line[i].unit = "";
        line[i].finishInit = 0;
        line[i].sec = 0;
        line[i].isStart = false;
        line[i].valueZoom = 1;
        line[i].testPointList.clear();
        line[i].targetCurveList.clear();

        line[i].bufferImage = QImage(geometry().width(),geometry().height(),
                             QImage::Format_ARGB32);

        line[i].bufferImage.fill(qRgba(0xa0,0xa0,0xa4,0));

        connect(&line[i].timer,SIGNAL(timeout()),this,SLOT(secCount()));
    }
}

int WidgetBpmCurve::getSec(int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return -1;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return -1;
    }

    return line[drawIndex].sec;
}

void WidgetBpmCurve::start()
{
    for(int i=0;i<drawCurveCount;i++)
    {
        line[i].timer.start(1000);
        line[i].isStart = true;
    }
}

void WidgetBpmCurve::stop()
{
    for (int i=0;i<drawCurveCount;i++)
    {
        line[i].timer.stop();
        line[i].isStart = false;
    }
}

void WidgetBpmCurve::secCount()
{
    for(int i=0;i<drawCurveCount;i++)
    {
        line[i].sec++;
    }
}

void WidgetBpmCurve::setSecZero()
{
    for(int i=0;i<drawCurveCount;i++)
    {
        if (line[i].isStart == true)
        {
            line[i].isStart = false;
            line[i].timer.stop();
            line[i].sec = 0;
            line[i].isStart = true;
            line[i].timer.start();
        }
        else
        {
            line[i].sec = 0;
        }
    }
}

void WidgetBpmCurve::setTestTime(int testTime)
{
    this->testTime = testTime;
    initPos();
}

void WidgetBpmCurve::setValueRange(int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    line[drawIndex].valueRange = line[drawIndex].maxValue - line[drawIndex].minValue;
}

void WidgetBpmCurve::setMinValue(int minValue, int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    line[drawIndex].minValue = minValue;
    setValueRange(drawIndex);
}

void WidgetBpmCurve::setMaxValue(int maxValue, int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    line[drawIndex].maxValue = maxValue;
    setValueRange(drawIndex);
}

void WidgetBpmCurve::setValueZoom(float valueZoom, int drawIndex)
{   
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    line[drawIndex].valueZoom = valueZoom;
}

void WidgetBpmCurve::setHorizontalScale(int horizontalScale)
{
    if ( horizontalScale < 1 )
    {
        return ;
    }

    this->horizontalScale = horizontalScale;
}

void WidgetBpmCurve::setVerticalScale(int verticalScale)
{
    if ( verticalScale < 1 )
    {
        return ;
    }

    this->verticalScale =  verticalScale;
}

void WidgetBpmCurve::addTargetCurve(int offset, int longTime, int value, int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    if ( longTime <= 0 )
    {
        return ;
    }

    if ( ( value < line[drawIndex].minValue ) || ( value > line[drawIndex].maxValue ) )
    {
        return ;
    }

    struct BpmTargetCurve curve;
    curve.offset = offset;
    curve.longTime = longTime; //secs
    curve.value = value;
    line[drawIndex].targetCurveList.append(curve);
}

void WidgetBpmCurve::delTargetCurve(int drawIndex)
{
    int theStartTime;
    int endTime;
    int currentDateTime;
    int i;
    int j = -1;
    int n;
    int len;
    QVector <BpmTargetCurve> ::iterator begin;
    QVector <BpmTargetCurve> ::iterator end;

    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    begin = line[drawIndex].targetCurveList.begin();
    end = line[drawIndex].targetCurveList.end();
    len = line[drawIndex].targetCurveList.size();

    for(i=0;begin != end;i++,begin++)
    {
        if ( ( (*begin).offset >= line[drawIndex].sec ) && (j == -1) )
        {
            j = i;
            break;
        }
    }

    if (j != -1)
    {
        n = len - j;
        line[drawIndex].targetCurveList.remove(j,n);
    }

    if ( (j != -1) && ( j > 0 ) )
    {
        theStartTime = line[drawIndex].targetCurveList.at(j-1).offset;
        endTime = theStartTime +  line[drawIndex].targetCurveList.at(j-1).longTime ;
        currentDateTime = line[drawIndex].sec;

        if ( ( theStartTime < currentDateTime) && ( endTime > currentDateTime) )
        {
            line[drawIndex].targetCurveList[j-1].longTime = currentDateTime - theStartTime;
        }
    }
}

void WidgetBpmCurve::delAllTargetCurve(int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    line[drawIndex].targetCurveList.remove(0,line[drawIndex].targetCurveList.size());
}

void WidgetBpmCurve::draw()
{
    update();
}

void WidgetBpmCurve::setMode(int mode)
{
    curveMode = mode;
}

void WidgetBpmCurve::addTestPoint(int value, int drawIndex)
{
    BpmCurvePoint point;

    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    if (line[drawIndex].isStart == true)
    {
        point.value = value;
        point.offset = line[drawIndex].sec;
        line[drawIndex].testPointList.append(point);

        if (line[drawIndex].finishInit != 0)
        {
            paintPoint(point.offset,point.value,1,drawIndex);
        }
    }
}

void WidgetBpmCurve::delAllTestPoint(int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    line[drawIndex].testPointList.clear();
}

void WidgetBpmCurve::setUnit(QString unit, int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    line[drawIndex].unit = unit;
}

void WidgetBpmCurve::setUnitX(BpmTimeUnit unitX)
{
    this->unitX = unitX;
}

void WidgetBpmCurve::setXOffset(int xOffset)
{
    if ( ( xOffset > geometry().width() ) || ( xOffset < 0 ) )
    {
        return;
    }

    this->xOffset = xOffset;

    rect_width = geometry().width();
    rect_width -= xOffset;

    for (int i=0;i<drawCurveCount;i++)
    {
        line[i].lastX = this->xOffset;
    }

}

void WidgetBpmCurve::setYOffset(int yOffset)
{
    if ( ( yOffset >=  geometry().height() ) || ( yOffset < 0 ) )
    {
        return;
    }

    this->yOffset = yOffset;

    rect_height = geometry().height();
    rect_height -= yOffset;

    for(int i=0;i<drawCurveCount;i++)
    {
        line[0].lastY = rect_height;
    }
}

void WidgetBpmCurve::setGeometry(const QRect & a)
{
    QWidget::setGeometry(a);

    for(int i=0;i<drawCurveCount;i++)
    {
        line[i].bufferImage = QImage(geometry().width(),geometry().height(),
                             QImage::Format_ARGB32);

        line[i].bufferImage.fill(qRgba(0xa0,0xa0,0xa4,0));

        rect_width = geometry().width();
        rect_height = geometry().height();

        rect_height -= yOffset;
        rect_width -= (xOffset+xEndOffset);

        line[i].lastX = xOffset;
        line[i].lastY = rect_height;
    }

}

void WidgetBpmCurve::setGeometry(int x, int y, int w, int h)
{
    QWidget::setGeometry(x,y,w,h);

    for(int i=0;i<drawCurveCount;i++)
    {
        line[i].bufferImage = QImage(geometry().width(),geometry().height(),
                             QImage::Format_ARGB32);

        line[i].bufferImage.fill(qRgba(0xa0,0xa0,0xa4,0));

        rect_width = geometry().width();
        rect_height = geometry().height();

        rect_height -= yOffset;
        rect_width -= xOffset;

        line[i].lastX = xOffset;
        line[i].lastY = rect_height;
    }
}

WidgetBpmCurve::~WidgetBpmCurve()
{
    if (line != NULL)
    {
        for(int i=0;i<drawCurveCount;i++)
        {
            line[i].testPointList.clear();
            line[i].targetCurveList.clear();
        }

        delete [] line;
        line = NULL;
    }
}

void WidgetBpmCurve::setLineColor(QColor lineColor,int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    line[drawIndex].color = lineColor;
}

void WidgetBpmCurve::initPos()
{
    QList <BpmCurvePoint> ::iterator beginAddPos;
    QList <BpmCurvePoint> ::iterator endAddPos;

    for (int i=0;i<drawCurveCount;i++)
    {
        beginAddPos = line[i].testPointList.begin();
        endAddPos = line[i].testPointList.end();

        line[i].bufferImage.fill(qRgba(0xa0,0xa0,0xa4,0));

        line[i].finishInit = 0;

        int j = 0;

        while(beginAddPos != endAddPos)
        {
            if (j==0)
            {
                paintPoint((*beginAddPos).offset,(*beginAddPos).value,0,i);
            }
            else
            {
                paintPoint((*beginAddPos).offset,(*beginAddPos).value,1,i);
            }

            beginAddPos++;
            j++;
        }

        line[i].finishInit = 1;
    }

    update();
}

void WidgetBpmCurve::paintPoint(int offset,int value,int option,int drawIndex)
{
    if (drawIndex < 0)
    {
        qDebug() << "drawIndex is less 0";
        return ;
    }

    if (drawIndex >= drawCurveCount)
    {
        qDebug() << "drawIndex more than or equal drawCurveCount";
        return ;
    }

    QPainter painter(&line[drawIndex].bufferImage);
    int x;
    int y;
    int offset_tmp;

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(line[drawIndex].color);
    if(drawIndex==3)
    pen.setWidth(3);
    //painter.setPen(QPen(Qt::red, 2)); //设置画笔颜色和大小
    painter.setPen(pen);

    offset_tmp = offset;

    if (offset_tmp >= 0)
    {
        x = ( rect_width * offset_tmp ) / testTime;
        x = x + xOffset;
        y = ( rect_height * (line[drawIndex].valueRange - (value - line[drawIndex].minValue) ) )/ line[drawIndex].valueRange;

        //qDebug("x = %d ,y =%d",x,y);

        painter.drawPoint(x,y);

        if (option != 0)
        {
            painter.drawLine(x,y,line[drawIndex].lastX,line[drawIndex].lastY);
        }

        line[drawIndex].lastX = x;
        line[drawIndex].lastY = y;
    }
}

void WidgetBpmCurve::paintEvent(QPaintEvent *event)
{

    event = event;

    QPainter painter(this);

    QVector <BpmTargetCurve> ::iterator beginTargetCurve;
    QVector <BpmTargetCurve> ::iterator endTargetCurve;

    int i;
    int x;
    int x1;
    int x2;
    int y;
    int y1;
    int y2;
    int value;
    int offset;
    int rectHeight;
    int rectWidth;
    int tmp;
    float mark;
    QString str;
    QPen pen;

    if (curveMode == 1)
    {
        beginTargetCurve = line[0].targetCurveList.begin();
        endTargetCurve = line[0].targetCurveList.end();

        painter.fillRect(0,0,this->width(),this->height(),QColor(Qt::gray));
        painter.fillRect(xOffset,0,rect_width,rect_height, QColor(24, 30, 43));

        painter.setPen(QColor(Qt::gray));

        painter.drawLine(QPoint(xOffset,rect_height),QPoint(rect_width + xOffset,rect_height));
        painter.drawLine(QPoint(xOffset,0),QPoint(xOffset,rect_height));
        painter.drawLine(QPoint(xOffset,0),QPoint(rect_width + xOffset,0));
        painter.drawLine(QPoint(rect_width + xOffset - 1,0),QPoint(rect_width + xOffset - 1,rect_height));

        for(i=0;i<=verticalScale;i++)
        {
            tmp =  ( i *  line[0].valueRange ) / verticalScale;
            mark = line[0].minValue + tmp;
            mark = mark / line[0].valueZoom;
            x1 = xOffset;
            x2 = xOffset + rect_width;
            y = ( rect_height * (line[0].valueRange - tmp ) ) / line[0].valueRange;

            if (i > 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::gray));
                painter.setPen(pen);

                painter.drawLine(x1,y,x2,y);
            }

            str.setNum(mark);
            str += line[0].unit;

            if (i == verticalScale)
            {
                y = y + painter.fontMetrics().height();
            }

            x = xOffset - painter.fontMetrics().width(str);

            if (x >= 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,y,str);
            }
            else
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(0,y,str);
            }
        }

        for(i=0;i<=horizontalScale;i++)
        {
            tmp = ( i * testTime ) / horizontalScale;
            x = ( rect_width * tmp ) / testTime;
            x = x + xOffset;
            y1 = rect_height;
            y2 = rect_height - rect_height;

            if (i == horizontalScale)
            {
                x--;
            }

            if (i > 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::gray));
                painter.setPen(pen);

                painter.drawLine(x,y1,x,y2);
            }

            offset = tmp;

            switch (unitX)
            {
                case bpmSecond:
                    str.setNum(offset);
                    str += "s";
                    break;
                case bpmMinute:
                    mark = offset / (float) 60;
                    str.setNum(mark);
                    str += "min";
                    break;
                case bpmHour:
                    mark = offset / (float) 3600;
                    str.setNum(mark);
                    str += "h";
                    break;
            }


            if (i == horizontalScale)
            {
                x -= painter.fontMetrics().width(str);
            }

            y = yOffset - painter.fontMetrics().height();

            if (y >= 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,rect_height+y,str);
            }
            else
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,rect_height+yOffset,str);
            }
        }

        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        painter.setPen(QColor(Qt::green));

        painter.drawImage(QPoint(0,0),line[0].bufferImage);

        for(i=0;beginTargetCurve != endTargetCurve;i++,beginTargetCurve++)
        {
            value = (*beginTargetCurve).value;
            offset = (*beginTargetCurve).offset;
            x1 = offset;
            x2 = offset + (*beginTargetCurve).longTime;
            x1 = ( rect_width * x1 ) /testTime;
            x2 = ( rect_width * x2 ) /testTime;
            x1 = x1 + xOffset;
            x2 = x2 + xOffset;
            y = ( rect_height * (line[0].valueRange - (value - line[0].minValue) ) ) / line[0].valueRange;

            painter.drawLine(x1,y,x2,y);

        }
    }
    else if (curveMode == 2)
    {
        beginTargetCurve = line[0].targetCurveList.begin();
        endTargetCurve = line[0].targetCurveList.end();

        painter.fillRect(0,0,this->width(),this->height(),QColor(Qt::gray));
        painter.fillRect(xOffset,0,rect_width,rect_height, QColor(24, 30, 43));

        painter.setPen(QColor(Qt::gray));

        painter.drawLine(QPoint(xOffset,rect_height),QPoint(rect_width + xOffset,rect_height));
        painter.drawLine(QPoint(xOffset,0),QPoint(xOffset,rect_height));
        painter.drawLine(QPoint(xOffset,0),QPoint(rect_width + xOffset,0));
        painter.drawLine(QPoint(rect_width + xOffset - 1,0),QPoint(rect_width + xOffset - 1,rect_height));

        for(i=0;i<=verticalScale;i++)
        {
            tmp =  ( i *  line[0].valueRange ) / verticalScale;
            mark = line[0].minValue + tmp;
            mark = mark / line[0].valueZoom;
            x1 = xOffset;
            x2 = xOffset + rect_width;
            y = ( rect_height * (line[0].valueRange - tmp ) ) / line[0].valueRange;

            if (i > 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::gray));
                painter.setPen(pen);

                painter.drawLine(x1,y,x2,y);
            }

            str.setNum(mark);
            str += line[0].unit;

            if (i == verticalScale)
            {
                y = y + painter.fontMetrics().height();
            }

            x = xOffset - painter.fontMetrics().width(str);

            if (x >= 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,y,str);
            }
            else
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(0,y,str);
            }
        }

        for(i=0;i<=horizontalScale;i++)
        {
            tmp = ( i * testTime ) / horizontalScale;
            x = ( rect_width * tmp ) / testTime;
            x = x + xOffset;
            y1 = rect_height;
            y2 = rect_height - rect_height;

            if (i == horizontalScale)
            {
                x--;
            }

            if (i > 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::gray));
                painter.setPen(pen);

                painter.drawLine(x,y1,x,y2);
            }

            offset = tmp;

            switch (unitX)
            {
                case bpmSecond:
                    str.setNum(offset);
                    str += "s";
                    break;
                case bpmMinute:
                    mark = offset / (float) 60;
                    str.setNum(mark);
                    str += "min";
                    break;
                case bpmHour:
                    mark = offset / (float) 3600;
                    str.setNum(mark);
                    str += "h";
                    break;
            }


            if (i == horizontalScale)
            {
                x -= painter.fontMetrics().width(str);
            }

            y = yOffset - painter.fontMetrics().height();

            if (y >= 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,rect_height+y,str);
            }
            else
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,rect_height+yOffset,str);
            }
        }

        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);


        painter.setPen(QColor(Qt::green));

        for(i=0;beginTargetCurve != endTargetCurve;i++,beginTargetCurve++)
        {
            value = (*beginTargetCurve).value;
            offset = (*beginTargetCurve).offset;
            x1 = offset;
            x2 = offset + (*beginTargetCurve).longTime;
            x1 = ( rect_width * x1 ) /testTime;
            x2 = ( rect_width * x2 ) /testTime;
            x1 = x1 + xOffset;
            x2 = x2 + xOffset;
            y = ( rect_height * (line[0].valueRange - (value - line[0].minValue) ) ) / line[0].valueRange;
            rectHeight = ( rect_height *  (value - line[0].minValue) ) / line[0].valueRange;
            rectWidth = x2 - x1;

            painter.fillRect(x1,y,rectWidth,rectHeight,QBrush(QColor(Qt::green)));
        }

        painter.setPen(QColor(145,145,145));
        beginTargetCurve = line[0].targetCurveList.begin();
        endTargetCurve = line[0].targetCurveList.end();

        for(i=0;beginTargetCurve != endTargetCurve;i++,beginTargetCurve++)
        {
            if ( (*beginTargetCurve).offset <= line[0].sec )
            {
                value = (*beginTargetCurve).value;
                offset = (*beginTargetCurve).offset;
                x1 = offset;
                y = ( rect_height * (line[0].valueRange - (value - line[0].minValue) ) )/ line[0].valueRange;

                if ( (offset + (*beginTargetCurve).longTime) <=  line[0].sec )
                {
                    x2 = offset + (*beginTargetCurve).longTime;
                    x1 = ( rect_width * x1 ) /testTime;
                    x2 = ( rect_width * x2 ) /testTime;
                    x1 = x1 + xOffset;
                    x2 = x2 + xOffset;
                    rectHeight = ( rect_height *  (value - line[0].minValue) ) / line[0].valueRange;
                    rectWidth = x2 - x1;

                    painter.fillRect(x1,y,rectWidth,rectHeight,QBrush(QColor(Qt::yellow)));
                }
                else
                {
                    x2 = line[0].sec;
                    x1 = ( rect_width * x1 ) /testTime;
                    x2 = ( rect_width * x2 ) /testTime;
                    x1 = x1 + xOffset;
                    x2 = x2 + xOffset;
                    rectHeight = ( rect_height *  (value - line[0].minValue) ) / line[0].valueRange;
                    rectWidth = x2 - x1;

                    painter.fillRect(x1,y,rectWidth,rectHeight,QBrush(QColor(Qt::yellow)));
                    break;
                }
            }
        }
    }
    else if (curveMode == 3)
    {
        painter.fillRect(0,0,this->width(),this->height(),QColor(Qt::gray));
        painter.fillRect(xOffset,0,rect_width,rect_height, QColor(24, 30, 43));

        painter.setPen(QColor(Qt::gray));

        painter.drawLine(QPoint(xOffset,rect_height),QPoint(rect_width + xOffset,rect_height));
        painter.drawLine(QPoint(xOffset,0),QPoint(xOffset,rect_height));
        painter.drawLine(QPoint(xOffset,0),QPoint(rect_width + xOffset,0));
        painter.drawLine(QPoint(rect_width + xOffset - 1,0),QPoint(rect_width + xOffset - 1,rect_height));

        for(i=0;i<=verticalScale;i++)
        {
            tmp =  ( i *  line[0].valueRange ) / verticalScale;
            mark = line[0].minValue + tmp;
            mark = mark / line[0].valueZoom;
            x1 = xOffset;
            x2 = xOffset + rect_width;
            y = ( rect_height * (line[0].valueRange - tmp ) ) / line[0].valueRange;

            if (i > 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::gray));
                painter.setPen(pen);

                painter.drawLine(x1,y,x2,y);
            }

            str.setNum(mark);
            str += line[0].unit;

            if (i == verticalScale)
            {
                y = y + painter.fontMetrics().height();
            }

            x = xOffset - painter.fontMetrics().width(str);

            if (x >= 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,y,str);
            }
            else
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(0,y,str);
            }
        }

        //======
        //if(xEndOffset==50)
        //{
        for(i=0;i<=16;i++)
        {
            tmp =  ( i *  (160) ) / 16;
            mark =  tmp;
            y = ( rect_height * (160 - tmp ) ) / 160;

            str.setNum(mark);
            str += "bpm";

            if (i == 16)
            {
                y = y + painter.fontMetrics().height();
            }

            //x = xOffset - painter.fontMetrics().width(str);

            x = xOffset +rect_width;//painter.fontMetrics().width(str);

            if (x >= 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,y,str);
            }

        }
        //}
        //======

        for(i=0;i<=horizontalScale;i++)
        {
            tmp = ( i * testTime ) / horizontalScale;
            x = ( rect_width * tmp ) / testTime;
            x = x + xOffset;
            y1 = rect_height;
            y2 = rect_height - rect_height;

            if (i == horizontalScale)
            {
                x--;
            }

            if (i > 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::gray));
                painter.setPen(pen);

                painter.drawLine(x,y1,x,y2);
            }

            offset = tmp;

            switch (unitX)
            {
                case bpmSecond:
                    str.setNum(offset);
                    str += "s";
                    break;
                case bpmMinute:
                    mark = offset / (float) 60;
                    str.setNum(mark);
                    str += "min";
                    break;
                case bpmHour:
                    mark = offset / (float) 3600;
                    str.setNum(mark);
                    str += "h";
                    break;
            }


            if (i == horizontalScale)
            {
                x -= painter.fontMetrics().width(str);
            }

            y = yOffset - painter.fontMetrics().height();

            if (y >= 0)
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,rect_height+y,str);
            }
            else
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x,rect_height+yOffset,str);
            }
        }

        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        painter.setPen(QColor(Qt::green));

        for(i=0;i<drawCurveCount;i++)
        {
            painter.drawImage(QPoint(0,0),line[i].bufferImage);
        }
    }
}
