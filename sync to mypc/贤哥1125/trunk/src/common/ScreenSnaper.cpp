/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-26
** 修改人：Huangjunxian
** 修改时间：2014-11-27
** 描述：
**    实现屏幕截图相关功能 《源文件》
*****************************************************/
#include "ScreenSnaper.h"

ScreenSnaper::ScreenSnaper(QWidget *parent)
: QWidget(parent)
{
    isDrawing = false;
}


void ScreenSnaper::showEvent(QShowEvent *event)
{
    fullScreenMap = QPixmap::grabWindow(QApplication::desktop()->winId());
    fullTempMap = fullScreenMap;
    showFullScreen();
    setCursor(Qt::CrossCursor);
    event->ignore();
}


void ScreenSnaper::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (isDrawing)
    {
      painter.drawPixmap(0, 0, fullTempMap);
    }
    else
    {
      painter.drawPixmap(0, 0, fullTempMap);
    }
    event->ignore();
}

void ScreenSnaper::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pt = event->pos();
    if (isDrawing)
    {
      fullTempMap = fullScreenMap;
      endPonit = event->pos();
      paint(fullTempMap);
    }
    else
    {
      if (snapRect.contains(pt.x(), pt.y()))
      {
       setCursor(Qt::SizeAllCursor);
      }
      else
      {
       setCursor(Qt::CrossCursor);
      }
    }
}

void ScreenSnaper::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
      isDrawing = true;
      startPoint = event->pos();
    }
}

void ScreenSnaper::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
      isDrawing = false;
      endPonit = event->pos();
      snapRect = QRect(startPoint, endPonit);
      paint(fullTempMap);
    }
}

void ScreenSnaper::paint(QPixmap &pixmap)
{
    QPainter painter(&pixmap);
    painter.drawRect(QRect(startPoint, endPonit));
    update();
}

void ScreenSnaper::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
      close();
    }
    else if (event->key() == Qt::Key_Space)
    {
      if (!snapRect.isNull())
      {
       save();
      }
    }
}

void ScreenSnaper::save()
{
    QString filename;

    filename = QFileDialog::getSaveFileName(this, tr("save image"),
      QDir::currentPath(), tr("Image(*.jpg *.png *.bmp)"));
    if (filename.isNull())
    {
      return;
    }
    snapMap = QPixmap::grabWidget(this, snapRect);
    snapMap.save(filename);
}

void ScreenSnaper::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *actionClose = new QAction(tr("quit"),this);
    QAction *actionSave;
    QMenu menu(this);
    if (snapRect.contains(event->pos()))
    {
      actionSave = new QAction(tr("save as"), this);
      menu.addAction(actionSave);
      connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    }
    menu.addAction(actionClose);
    connect(actionClose, SIGNAL(triggered()), this, SLOT(close()));
    menu.exec(event->pos());
}

bool ScreenSnaper::snaperAction(const QRect &snapseat,const char *filename, const char *format)
{
    QString filestr=filename;
    filestr += ".";
    filestr += format;
    snapMap=QPixmap::grabWindow(QApplication::desktop()->winId(),snapseat.x(),snapseat.y(),snapseat.width(),snapseat.height());
    return snapMap.save(filestr,format);
}

bool ScreenSnaper::startSnaper(const QRect &snapseat)
{
    snapMap=QPixmap::grabWindow(QApplication::desktop()->winId(),snapseat.x(),snapseat.y(),snapseat.width(),snapseat.height());
    if(snapMap.isNull())
        return false;
    else
        return true;
}

bool ScreenSnaper::startWidgetSnaper(QWidget *win,const QRect &snapseat)
{
    snapMap=QPixmap::grabWindow(win->winId(),snapseat.x(),snapseat.y(),snapseat.width(),snapseat.height());
    if(snapMap.isNull())
        return false;
    else
        return true;
}

bool ScreenSnaper::saveSnaper(const char *filename,const char *format,int w,int h)
{
    QString filestr=filename;
    filestr += ".";
    filestr += format;
    if(w!=0&&h!=0)
        snapMap=snapMap.scaled(w,h);
    return snapMap.save(filestr,format);
}

bool ScreenSnaper::testSave()
{
    fullScreenMap = QPixmap::grabWindow(QApplication::desktop()->winId());

    QString format = "png";
    QString initialPath = QDir::currentPath() + tr("/hjx.") + format;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                       initialPath,
                                       tr("%1 Files (*.%2);;All Files (*)")
                                       .arg(format.toUpper())
                                       .arg(format));
    if (!fileName.isEmpty())
        return fullScreenMap.save(fileName, format.toAscii());
    else
        return false;
}






