#ifndef SCENE_NOTE_H
#define SCENE_NOTE_H

#include <QtGui>
#include "config_ui.h"
#include "lang_switch.h"
#include "page_info.h"
#include "widget_button.h"

class QMyEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QMyEdit(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private slots:

signals:
    void LineEditClicked();
};

class Window_Note : public QDialog , public LangSetText
{
    Q_OBJECT
public:
    explicit Window_Note(QWidget *parent = 0, const QString title = "Messages",
                         const QString brushPath = "");
    ~Window_Note();
    void CustomSetText();
    void showNote(bool isShow);
    bool isShow();
signals:
    void showNoteSignal(bool isshow);
    void NoteSelSignal(int sel);
private:
    inline void setUi(const QString title, const QString brushBk);
    void showNoteText();

public slots:
    void btnCloseDialog();
    void btnYesClick();
    void btnNoClick();
    //void btnReleaseClick();

private:
    QWidget widget;
    QLabel labeltitle;
    QLabel labelInfo;
    QPushButton btnYes;
    QPushButton btnNo;
    QPushButton btnClose;

private:
    QString NoteName;
    QString title;
    QMap<qint64, QString> mapValue;
    QString btnUpStyle;
    QString btnClickStyle;
    bool showFlag;
};

//HJX实现的是否确认提示框
class Window_YESNO : public QDialog , public LangSetText
{
    Q_OBJECT
public:
    explicit Window_YESNO(QWidget *parent = 0);
    ~Window_YESNO();
    void CustomSetText();
    void showNote(bool isShow);
    bool isShow();
    void showMessage(QString &title,QString &info);

signals:
    void showNoteSignal(bool isshow);
    void NoteSelSignal(int sel);

private:
    void setUi();

public slots:
    void btnYesClick();
    void btnNoClick();

private:
    QWidget widget;
    QLabel labeltitle;
    QLabel labelInfo;
    QPushButton btnYes;
    QPushButton btnNo;

private:
    QString btnUpStyle;
    QString btnClickStyle;
};

//HJX实现的确认提示框
class Window_YES : public QDialog , public LangSetText
{
    Q_OBJECT
public:
    explicit Window_YES(QWidget *parent = 0);
    ~Window_YES();
    void CustomSetText();
    void showNote(bool isShow);
    bool isShow();
    void showMessage(QString &title,QString &info);    
    void showMessageNoYes(QString &title,QString &info);     //显示对话框，并且不显示确认按钮#/

signals:
    void showNoteSignal(bool isshow);
    void NoteSelSignal(int sel);

private:
    void setUi();

public slots:
    void btnYesClick();

private:
    QWidget widget;
    QLabel labeltitle;
    QLabel labelInfo;
    QPushButton btnYes;

private:
    QString btnUpStyle;
    QString btnClickStyle;
};

//--------------------------------------------------------------

class Window_Passwd : public QDialog , public LangSetText//,public mylineedit
{
    Q_OBJECT
public:
    explicit Window_Passwd(QWidget *parent = 0, const QString title = "Please input password",
                            const QString brushPath = "");
     ~Window_Passwd();
    void CustomSetText();
    void showNote(bool isShow);
    bool isShow();
protected:
    //重写mousePressEvent事件
    //virtual void mousePressEvent(QMouseEvent *event);
signals:
    void showNoteSignal(bool isshow);
    void NoteSelSignal(int sel);
private:
    inline void setUi(const QString title, const QString brushBk);
    void showNoteText();

public slots:
    void btnCloseDialog();
    void btnYesClick();
    void PasswdInputClicked();
private:
    QWidget widget;
    QLabel labeltitle;
    QLabel labelEorror;
    QMyEdit EditPasswd;
    QPushButton btnYes;
    QPushButton btnClose;

private:
    QString NoteName;
    QString title;
    QMap<qint64, QString> mapValue;
    QString btnUpStyle;
    QString btnClickStyle;
    bool showFlag;
    QString passWordEditString;
};

class Window_Confirm : public QDialog , public LangSetText//与Window_Note窗口相比，少了no，close键
{
    Q_OBJECT
public:
    explicit Window_Confirm(QWidget *parent = 0, const QString title = "Messages",
                            const QString brushPath = "");
     ~Window_Confirm();
    void CustomSetText();
    void showNote(bool isShow);
    bool isShow();
signals:
    void showNoteSignal(bool isshow);
    void NoteSelSignal(int sel);
private:
    inline void setUi(const QString title, const QString brushBk);
    void showNoteText();

public slots:
    void btnYesClick();
    //void btnReleaseClick();

private:
    QWidget widget;
    QLabel labeltitle;
    QLabel labelInfo;
    QPushButton btnYes;

private:
    QString NoteName;
    QString title;
    QMap<qint64, QString> mapValue;
    QString btnUpStyle;
    QString btnClickStyle;
    bool showFlag;

};


class Window_ChangeFilter : public QDialog , public LangSetText
{
    Q_OBJECT
public:
    explicit Window_ChangeFilter(QWidget *parent = 0);
    ~Window_ChangeFilter();
    void CustomSetText();
    void showNote(bool isShow);
    bool isShow();
    void showMessage(QString &title,QString &info);
    bool isDry();
signals:
    void showNoteSignal(bool isshow);
    void NoteSelSignal(int sel);
    void RstSelSignal(bool sel);
    void DrySelSignal(bool sel);

private:
    void setUi();

public slots:
    void btnCancelClick();
    void btnResetClick();
    void btnDryClick();
private:
    QWidget widget;
    QLabel labeltitle;
    QLabel labelInfo;
    QPushButton btnCancel;
    QPushButton btnReset;
    QPushButton btnDry;

private:
    QString btnUpStyle;
    QString btnClickStyle;
    bool Dry;
};


/*******************min keyboard start********************/
#ifdef HW_ARM
#define PASSWORD_CONFIG_FILE "/usr/local/app/conf/engineering_passwd"
#else
#define PASSWORD_CONFIG_FILE "./conf/engineering_passwd"
#endif

#define MIN_KEY_BOTTOM 235
class Mini_Keyboard:public QWidget
{
    Q_OBJECT

public:
    explicit Mini_Keyboard(QLineEdit *LineEdit = NULL,  QString *paramete = NULL);
    ~Mini_Keyboard();
private slots:
    void buttonClicked(int btn);
    void buttonPressed(int btn);
    void buttonReleased(int btn);
    void button_confirm_Clicked();
    void button_add_Clicked();
    void button_sub_Clicked();
    void button_del_Clicked();
    void closeKeyboard();
protected:
    void paintEvent(QPaintEvent *event);
private:
    QList<QPushButton *> allButtons;
    QSignalMapper signalClick;
    QSignalMapper signalPressed;
    QSignalMapper signalReleased;
    QPixmap pix;

    WidgetButton * button_0;
    QPushButton * button_1;
    QPushButton * button_2;
    QPushButton * button_3;
    QPushButton * button_4;
    QPushButton * button_5;
    QPushButton * button_6;
    QPushButton * button_7;
    QPushButton * button_8;
    QPushButton * button_9;
    QPushButton * button_add;
    QPushButton * button_sub;
    QPushButton * button_del;
    //QPushButton * button_period;
    QPushButton * button_confirm;
    QPushButton * button_cancel;
private:
    QString *outString;
    QLineEdit *outEdit;

};//MiniKeyboard

/*******************min keyboard end*********************/
#endif // SCENE_NOTE_H
