#ifndef ALLINPUTPANEL_H
#define ALLINPUTPANEL_H

#include <QtGui>
#include <QtCore>


#include "virtualKeyboard_public.h"
//#include "ui_all_keyboard.h"

struct InputAllWidget
{
    QWidget *watch;
    int whichInput;
};

//! [0]

class AllInputPanel : public QWidget
{
    Q_OBJECT

public:
    AllInputPanel();
    ~AllInputPanel();
    QWidget * getFocusWidget();
    void addButton(QPushButton ** button,QIcon icon,QRect rect,QString button_name,QString keyCode,QString string);
    void changeTextCaps(bool isCaps);
    void changeTextShift(bool isShift);
    void appendWidget(QWidget *watch,int whichInput = NumInputMethod);
    void removeWidget(QWidget *watch);

signals:
    void characterGenerated(int keyId,Qt::KeyboardModifiers Modifier,QString ch,int involvedKeys);
    void wakeUpKeyboard(int whichInput);
    void hideKeyboard();

protected:
    bool event(QEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus);
    void buttonClicked(int btn);
    void btnShiftRight_toggled(bool checked);
    void btnShiftLeft_toggled(bool checked);
    void btnCaps_toggled(bool checked);

    void tilde_button_pressed();
    void tilde_button_released();
    void number_1_button_pressed();
    void number_1_button_released();
    void number_2_button_pressed();
    void number_2_button_released();
    void number_3_button_pressed();
    void number_3_button_released();
    void number_4_button_pressed();
    void number_4_button_released();
    void number_5_button_pressed();
    void number_5_button_released();
    void number_6_button_pressed();
    void number_6_button_released();
    void number_7_button_pressed();
    void number_7_button_released();
    void number_8_button_pressed();
    void number_8_button_released();
    void number_9_button_pressed();
    void number_9_button_released();
    void number_0_button_pressed();
    void number_0_button_released();
    void minus_sign_button_pressed();
    void minus_sign_button_released();
    void plus_sign_button_pressed();
    void plus_sign_button_released();
    void backspace_button_pressed();
    void backspace_button_released();

    void tab_button_pressed();
    void tab_button_released();
    void q_button_pressed();
    void q_button_released();
    void w_button_pressed();
    void w_button_released();
    void e_button_pressed();
    void e_button_released();
    void r_button_pressed();
    void r_button_released();
    void t_button_pressed();
    void t_button_released();
    void y_button_pressed();
    void y_button_released();
    void u_button_pressed();
    void u_button_released();
    void i_button_pressed();
    void i_button_released();
    void o_button_pressed();
    void o_button_released();
    void p_button_pressed();
    void p_button_released();
    void Left_Square_Bracket_pressed();
    void Left_Square_Bracket_released();
    void Right_Square_Bracket_pressed();
    void Right_Square_Bracket_released();
    void backslash_pressed();
    void backslash_released();

    void caps_lock_button_pressed();
    void caps_lock_button_released();
    void a_button_pressed();
    void a_button_released();
    void s_button_pressed();
    void s_button_released();
    void d_button_pressed();
    void d_button_released();
    void f_button_pressed();
    void f_button_released();
    void g_button_pressed();
    void g_button_released();
    void h_button_pressed();
    void h_button_released();
    void j_button_pressed();
    void j_button_released();
    void k_button_pressed();
    void k_button_released();
    void l_button_pressed();
    void l_button_released();
    void semicolon_button_pressed();
    void semicolon_button_released();
    void apostrophe_button_pressed();
    void apostrophe_button_released();
    void enter_button_pressed();
    void enter_button_released();

    void left_shift_button_pressed();
    void left_shift_button_released();
    void z_button_pressed();
    void z_button_released();
    void x_button_pressed();
    void x_button_released();
    void c_button_pressed();
    void c_button_released();
    void v_button_pressed();
    void v_button_released();
    void b_button_pressed();
    void b_button_released();
    void n_button_pressed();
    void n_button_released();
    void m_button_pressed();
    void m_button_released();
    void comma_button_pressed();
    void comma_button_released();
    void period_button_pressed();
    void period_button_released();
    void slash_button_pressed();
    void slash_button_released();
    void right_shift_button_pressed();
    void right_shift_button_released();

private:
    //Ui::All_Keyboard form;
    QWidget *lastFocusedWidget;
    QSignalMapper signalMapper;
    QList<QPushButton *> allButtons;

    QPushButton * tilde_button;
    QPushButton * number_1_button;
    QPushButton * number_2_button;
    QPushButton * number_3_button;
    QPushButton * number_4_button;
    QPushButton * number_5_button;
    QPushButton * number_6_button;
    QPushButton * number_7_button;
    QPushButton * number_8_button;
    QPushButton * number_9_button;
    QPushButton * number_0_button;
    QPushButton * minus_sign_button;
    QPushButton * plus_sign_button;
    QPushButton * backspace_button;

    QPushButton * tab_button;
    QPushButton * q_button;
    QPushButton * w_button;
    QPushButton * e_button;
    QPushButton * r_button;
    QPushButton * t_button;
    QPushButton * y_button;
    QPushButton * u_button;
    QPushButton * i_button;
    QPushButton * o_button;
    QPushButton * p_button;
    QPushButton * Left_Square_Bracket;
    QPushButton * Right_Square_Bracket;
    QPushButton * backslash;

    QPushButton * caps_lock_button;
    QPushButton * a_button;
    QPushButton * s_button;
    QPushButton * d_button;
    QPushButton * f_button;
    QPushButton * g_button;
    QPushButton * h_button;
    QPushButton * j_button;
    QPushButton * k_button;
    QPushButton * l_button;
    QPushButton * semicolon_button;
    QPushButton * apostrophe_button;
    QPushButton * enter_button;

    QPushButton * left_shift_button;
    QPushButton * z_button;
    QPushButton * x_button;
    QPushButton * c_button;
    QPushButton * v_button;
    QPushButton * b_button;
    QPushButton * n_button;
    QPushButton * m_button;
    QPushButton * comma_button;
    QPushButton * period_button;
    QPushButton * slash_button;
    QPushButton * right_shift_button;


    QPushButton * close_button;

    QPoint dragPosition;
    QPixmap pix;
    bool isCaps;
    bool isShift;
    QList <InputAllWidget> focusWidgetList;
    bool insertMode;

public:
    int typeInput;
    int my_typeInput;

};

//! [0]
#endif // ALLINPUTPANEL_H
