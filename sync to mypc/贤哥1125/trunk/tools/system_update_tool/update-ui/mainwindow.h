#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    //static bool isWait;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setdistext(QString disStr, int labelNum);
    int unPackZip(const char * path);
    void updateSystem(const char* path);
    void eventHandleDelay(void);

private:
    Ui::MainWindow *ui;

private:
    QLabel dis_updating[10];
    QFont fontlabelProgress;

    void setUi(void);


};

#endif // MAINWINDOW_H
