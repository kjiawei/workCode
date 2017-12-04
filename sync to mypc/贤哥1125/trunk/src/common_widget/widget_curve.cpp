#include <QtGui>
#include "config_ui.h"
#include "widget_curve.h"

WidgetCurve::WidgetCurve(QWidget *parent,Qt::WindowFlags f) :
    QWidget(parent,f)
{
    unitX = Second;
    yOffset = 30;
    xOffset = 70;
    line = NULL;
    drawCurveCount = 0;
    setDrawCurveCount();
}

void WidgetCurve::setDrawCurveCount(int drawCurveCount)
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
    line = new struct drawLine[drawCurveCount];

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

int WidgetCurve::getSec(int drawIndex)
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

void WidgetCurve::start()
{
    for(int i=0;i<drawCurveCount;i++)
    {
        line[i].timer.start(1000);
        line[i].isStart = true;
    }
}

void WidgetCurve::stop()
{
    for (int i=0;i<drawCurveCount;i++)
    {
        line[i].timer.stop();
        line[i].isStart = false;
    }
}

void WidgetCurve::secCount()
{
    for(int i=0;i<drawCurveCount;i++)
    {
        line[i].sec++;
    }
}

void WidgetCurve::setSecZero()
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

void WidgetCurve::setTestTime(int testTime)
{
    this->testTime = testTime;
    initPos();
}

void WidgetCurve::setValueRange(int drawIndex)
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

void WidgetCurve::setMinValue(int minValue, int drawIndex)
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

void WidgetCurve::setMaxValue(int maxValue, int drawIndex)
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

void WidgetCurve::setValueZoom(float valueZoom, int drawIndex)
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

void WidgetCurve::setHorizontalScale(int horizontalScale)
{
    if ( horizontalScale < 1 )
    {
        return ;
    }

    this->horizontalScale = horizontalScale;
}

void WidgetCurve::setVerticalScale(int verticalScale)
{
    if ( verticalScale < 1 )
    {
        return ;
    }

    this->verticalScale =  verticalScale;
}

void WidgetCurve::addTargetCurve(int offset, int longTime, int value, int drawIndex)
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

    struct TargetCurve curve;
    curve.offset = offset;
    curve.longTime = longTime; //secs
    curve.value = value;
    line[drawIndex].targetCurveList.append(curve);
}

void WidgetCurve::delTargetCurve(int drawIndex)
{
    int theStartTime;
    int endTime;
    int currentDateTime;
    int i;
    int j = -1;
    int n;
    int len;
    QVector <TargetCurve> ::iterator begin;
    QVector <TargetCurve> ::iterator end;

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

void WidgetCurve::delAllTargetCurve(int drawIndex)
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

void WidgetCurve::draw()
{
    update();
}

void WidgetCurve::setMode(int mode)
{
    curveMode = mode;
}

void WidgetCurve::addTestPoint(int value, int drawIndex)
{
    CurvePoint point;

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

void WidgetCurve::delAllTestPoint(int drawIndex)
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

void WidgetCurve::setUnit(QString unit, int drawIndex)
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

void WidgetCurve::setUnitX(TimeUnit unitX)
{
    this->unitX = unitX;
}

void WidgetCurve::setXOffset(int xOffset)
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

void WidgetCurve::setYOffset(int yOffset)
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

void WidgetCurve::setGeometry(const QRect & a)
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
        rect_width -= xOffset;

        line[i].lastX = xOffset;
        line[i].lastY = rect_height;
    }

}

void WidgetCurve::setGeometry(int x, int y, int w, int h)
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

WidgetCurve::~WidgetCurve()
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

void WidgetCurve::setLineColor(QColor lineColor,int drawIndex)
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

void WidgetCurve::initPos()
{
    QList <CurvePoint> ::iterator beginAddPos;
    QList <CurvePoint> ::iterator endAddPos;

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

void WidgetCurve::paintPoint(int offset,int value,int option,int drawIndex)
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

void WidgetCurve::paintEvent(QPaintEvent *event)
{

    event = event;

    QPainter painter(this);

    QVector <TargetCurve> ::iterator beginTargetCurve;
    QVector <TargetCurve> ::iterator endTargetCurve;

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
                case Second:
                    str.setNum(offset);
                    str += "s";
                    break;
                case Minute:
                    mark = offset / (float) 60;
                    str.setNum(mark);
                    str += "m";
                    break;
                case Hour:
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

                painter.drawText(x-5,rect_height+y,str);
            }
            else
            {
                pen.setStyle(Qt::DashLine);
                pen.setColor(QColor(Qt::black));
                painter.setPen(pen);

                painter.drawText(x-5,rect_height+yOffset,str);
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
                case Second:
                    str.setNum(offset);
                    str += "s";
                    break;
                case Minute:
                    mark = offset / (float) 60;
                    str.setNum(mark);
                    str += "min";
                    break;
                case Hour:
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
                case Second:
                    str.setNum(offset);
                    str += "s";
                    break;
                case Minute:
                    mark = offset / (float) 60;
                    str.setNum(mark);
                    str += "min";
                    break;
                case Hour:
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
