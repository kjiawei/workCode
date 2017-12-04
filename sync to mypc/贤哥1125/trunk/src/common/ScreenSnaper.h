/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-26
** 修改人：Huangjunxian
** 修改时间：2014-11-27
** 描述：
**    实现屏幕截图相关功能 《头文件》
*****************************************************/
#ifndef SCREENSNAPER_H
#define SCREENSNAPER_H
#include <QtGui/QtGui>

class ScreenSnaper : public QWidget
{
Q_OBJECT
public:
    ScreenSnaper(QWidget *parent=0);
private:
    QPixmap fullScreenMap;
    QPixmap snapMap; //最终获取的截图图片
    QPixmap fullTempMap;
    bool isDrawing; //是否正在拉动截图框
    QPoint startPoint;
    QPoint endPonit;
    QRect snapRect; //截图框的位置和大小

public:
    bool snaperAction(const QRect &snapseat,const char *filename,const char *format="png");
    bool startSnaper(const QRect &snapseat);
    bool saveSnaper(const char *filename,const char *format="png",int w=0,int h=0);
    bool startWidgetSnaper(QWidget *win,const QRect &snapseat);
    bool testSave();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paint(QPixmap &pixmap);
    void contextMenuEvent(QContextMenuEvent *event);
    void showEvent(QShowEvent *event);
    private slots:
    void save();
};
#endif // SCREENSNAPER_H
