#include "all_input_panel.h"

//! [0]

AllInputPanel::AllInputPanel()
    : QWidget(0, Qt::Tool | Qt::WindowStaysOnTopHint| Qt::FramelessWindowHint),
      lastFocusedWidget(0)
{
    insertMode = true;

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setObjectName(QString::fromUtf8("All_Keyboard"));
    this->resize(600,210);
    this->setWindowOpacity(1.0);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFocusPolicy(Qt::NoFocus);

    pix.load(":/resource/images/virtualkeyboard/all/keyboard.png",0,Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    resize(pix.size());
    setMask(QBitmap(pix.mask()));

    addButton((&tilde_button),
              QIcon(":/resource/images/virtualkeyboard/all/tilde.png"),
              QRect(10,36,36,35),
              QString("tilde_button"),
              QString("0x60"),
              QString("'") );

    connect(tilde_button,SIGNAL(pressed()),this,SLOT(tilde_button_pressed()));
    connect(tilde_button,SIGNAL(released()),this,SLOT(tilde_button_released()));

    addButton((&number_1_button),
              QIcon(":/resource/images/virtualkeyboard/all/1.png"),
              QRect(50,36,36,35),
              QString("number_1_button"),
              QString("0x31"),
              QString("1") );

    connect(number_1_button,SIGNAL(pressed()),this,SLOT(number_1_button_pressed()));
    connect(number_1_button,SIGNAL(released()),this,SLOT(number_1_button_released()));

    addButton((&number_2_button),
              QIcon(":/resource/images/virtualkeyboard/all/2.png"),
              QRect(90,36,36,35),
              QString("number_2_button"),
              QString("0x32"),
              QString("2") );

    connect(number_2_button,SIGNAL(pressed()),this,SLOT(number_2_button_pressed()));
    connect(number_2_button,SIGNAL(released()),this,SLOT(number_2_button_released()));

    addButton((&number_3_button),
              QIcon(":/resource/images/virtualkeyboard/all/3.png"),
              QRect(130,36,36,35),
              QString("number_3_button"),
              QString("0x33"),
              QString("3") );

    connect(number_3_button,SIGNAL(pressed()),this,SLOT(number_3_button_pressed()));
    connect(number_3_button,SIGNAL(released()),this,SLOT(number_3_button_released()));

    addButton((&number_4_button),
              QIcon(":/resource/images/virtualkeyboard/all/4.png"),
              QRect(170,36,36,35),
              QString("number_4_button"),
              QString("0x34"),
              QString("4") );

    connect(number_4_button,SIGNAL(pressed()),this,SLOT(number_4_button_pressed()));
    connect(number_4_button,SIGNAL(released()),this,SLOT(number_4_button_released()));

    addButton((&number_5_button),
              QIcon(":/resource/images/virtualkeyboard/all/5.png"),
              QRect(210,36,36,35),
              QString("number_5_button"),
              QString("0x35"),
              QString("5") );

    connect(number_5_button,SIGNAL(pressed()),this,SLOT(number_5_button_pressed()));
    connect(number_5_button,SIGNAL(released()),this,SLOT(number_5_button_released()));

    addButton((&number_6_button),
              QIcon(":/resource/images/virtualkeyboard/all/6.png"),
              QRect(250,36,36,35),
              QString("number_6_button"),
              QString("0x36"),
              QString("6") );

    connect(number_6_button,SIGNAL(pressed()),this,SLOT(number_6_button_pressed()));
    connect(number_6_button,SIGNAL(released()),this,SLOT(number_6_button_released()));

    addButton((&number_7_button),
              QIcon(":/resource/images/virtualkeyboard/all/7.png"),
              QRect(290,36,36,35),
              QString("number_7_button"),
              QString("0x37"),
              QString("7") );

    connect(number_7_button,SIGNAL(pressed()),this,SLOT(number_7_button_pressed()));
    connect(number_7_button,SIGNAL(released()),this,SLOT(number_7_button_released()));

    addButton((&number_8_button),
              QIcon(":/resource/images/virtualkeyboard/all/8.png"),
              QRect(330,36,36,35),
              QString("number_8_button"),
              QString("0x38"),
              QString("8") );

    connect(number_8_button,SIGNAL(pressed()),this,SLOT(number_8_button_pressed()));
    connect(number_8_button,SIGNAL(released()),this,SLOT(number_8_button_released()));

    addButton((&number_9_button),
              QIcon(":/resource/images/virtualkeyboard/all/9.png"),
              QRect(370,36,36,35),
              QString("number_9_button"),
              QString("0x39"),
              QString("9") );

    connect(number_9_button,SIGNAL(pressed()),this,SLOT(number_9_button_pressed()));
    connect(number_9_button,SIGNAL(released()),this,SLOT(number_9_button_released()));

    addButton((&number_0_button),
              QIcon(":/resource/images/virtualkeyboard/all/0.png"),
              QRect(410,36,36,35),
              QString("number_0_button"),
              QString("0x30"),
              QString("0") );

    connect(number_0_button,SIGNAL(pressed()),this,SLOT(number_0_button_pressed()));
    connect(number_0_button,SIGNAL(released()),this,SLOT(number_0_button_released()));

    addButton((&minus_sign_button),
              QIcon(":/resource/images/virtualkeyboard/all/minus.png"),
              QRect(450,36,36,35),
              QString("minus_sign_button"),
              QString("0x2d"),
              QString("-") );

    connect(minus_sign_button,SIGNAL(pressed()),this,SLOT(minus_sign_button_pressed()));
    connect(minus_sign_button,SIGNAL(released()),this,SLOT(minus_sign_button_released()));

    addButton((&plus_sign_button),
              QIcon(":/resource/images/virtualkeyboard/all/plus.png"),
              QRect(490,36,36,35),
              QString("plus_sign_button"),
              QString("13"),
              QString("=") );

    connect(plus_sign_button,SIGNAL(pressed()),this,SLOT(plus_sign_button_pressed()));
    connect(plus_sign_button,SIGNAL(released()),this,SLOT(plus_sign_button_released()));

    addButton((&backspace_button),
              QIcon(":/resource/images/virtualkeyboard/all/del.png"),
              QRect(530,36,59,35),
              QString("backspace_button"),
              QString("0x01000003"),
              QString("") );

    connect(backspace_button,SIGNAL(pressed()),this,SLOT(backspace_button_pressed()));
    connect(backspace_button,SIGNAL(released()),this,SLOT(backspace_button_released()));

    addButton((&tab_button),
              QIcon(":/resource/images/virtualkeyboard/all/tab.png"),
              QRect(10,76,59,35),
              QString("tab_button"),
              QString("0x01000001"),
              QString("") );

    connect(tab_button,SIGNAL(pressed()),this,SLOT(tab_button_pressed()));
    connect(tab_button,SIGNAL(released()),this,SLOT(tab_button_released()));

    addButton((&q_button),
              QIcon(":/resource/images/virtualkeyboard/all/Q.png"),
              QRect(73,76,36,35),
              QString("q_button"),
              QString("0x51"),
              QString("q") );

    connect(q_button,SIGNAL(pressed()),this,SLOT(q_button_pressed()));
    connect(q_button,SIGNAL(released()),this,SLOT(q_button_released()));

    addButton((&w_button),
              QIcon(":/resource/images/virtualkeyboard/all/W.png"),
              QRect(113,76,36,35),
              QString("w_button"),
              QString("0x57"),
              QString("w") );

    connect(w_button,SIGNAL(pressed()),this,SLOT(w_button_pressed()));
    connect(w_button,SIGNAL(released()),this,SLOT(w_button_released()));

    addButton((&e_button),
              QIcon(":/resource/images/virtualkeyboard/all/E.png"),
              QRect(153,76,36,35),
              QString("e_button"),
              QString("0x45"),
              QString("e") );

    connect(e_button,SIGNAL(pressed()),this,SLOT(e_button_pressed()));
    connect(e_button,SIGNAL(released()),this,SLOT(e_button_released()));

    addButton((&r_button),
              QIcon(":/resource/images/virtualkeyboard/all/R.png"),
              QRect(193,76,36,35),
              QString("r_button"),
              QString("0x52"),
              QString("r") );

    connect(r_button,SIGNAL(pressed()),this,SLOT(r_button_pressed()));
    connect(r_button,SIGNAL(released()),this,SLOT(r_button_released()));

    addButton((&t_button),
              QIcon(":/resource/images/virtualkeyboard/all/T.png"),
              QRect(233,76,36,35),
              QString("t_button"),
              QString("0x54"),
              QString("t") );

    connect(t_button,SIGNAL(pressed()),this,SLOT(t_button_pressed()));
    connect(t_button,SIGNAL(released()),this,SLOT(t_button_released()));

    addButton((&y_button),
              QIcon(":/resource/images/virtualkeyboard/all/Y.png"),
              QRect(273,76,36,35),
              QString("y_button"),
              QString("0x59"),
              QString("y") );

    connect(y_button,SIGNAL(pressed()),this,SLOT(y_button_pressed()));
    connect(y_button,SIGNAL(released()),this,SLOT(y_button_released()));

    addButton((&u_button),
              QIcon(":/resource/images/virtualkeyboard/all/U.png"),
              QRect(313,76,36,35),
              QString("u_button"),
              QString("0x55"),
              QString("u") );

    connect(u_button,SIGNAL(pressed()),this,SLOT(u_button_pressed()));
    connect(u_button,SIGNAL(released()),this,SLOT(u_button_released()));

    addButton((&i_button),
              QIcon(":/resource/images/virtualkeyboard/all/I.png"),
              QRect(353,76,36,35),
              QString("i_button"),
              QString("0x49"),
              QString("i") );

    connect(i_button,SIGNAL(pressed()),this,SLOT(i_button_pressed()));
    connect(i_button,SIGNAL(released()),this,SLOT(i_button_released()));

    addButton((&o_button),
              QIcon(":/resource/images/virtualkeyboard/all/O.png"),
              QRect(393,76,36,35),
              QString("o_button"),
              QString("0x4f"),
              QString("o") );

    connect(o_button,SIGNAL(pressed()),this,SLOT(o_button_pressed()));
    connect(o_button,SIGNAL(released()),this,SLOT(o_button_released()));

    addButton((&p_button),
              QIcon(":/resource/images/virtualkeyboard/all/P.png"),
              QRect(433,76,36,35),
              QString("p_button"),
              QString("0x50"),
              QString("p") );

    connect(p_button,SIGNAL(pressed()),this,SLOT(p_button_pressed()));
    connect(p_button,SIGNAL(released()),this,SLOT(p_button_released()));

    addButton((&Left_Square_Bracket),
              QIcon(":/resource/images/virtualkeyboard/all/Left_Square_Bracket.png"),
              QRect(473,76,36,35),
              QString("Left_Square_Bracket"),
              QString("0x5b"),
              QString("[") );

    connect(Left_Square_Bracket,SIGNAL(pressed()),this,SLOT(Left_Square_Bracket_pressed()));
    connect(Left_Square_Bracket,SIGNAL(released()),this,SLOT(Left_Square_Bracket_released()));

    addButton((&Right_Square_Bracket),
              QIcon(":/resource/images/virtualkeyboard/all/Right_Square_Bracket.png"),
              QRect(513,76,36,35),
              QString("Right_Square_Bracket"),
              QString("0x5d"),
              QString("]") );

    connect(Right_Square_Bracket,SIGNAL(pressed()),this,SLOT(Right_Square_Bracket_pressed()));
    connect(Right_Square_Bracket,SIGNAL(released()),this,SLOT(Right_Square_Bracket_released()));

    addButton((&backslash),
              QIcon(":/resource/images/virtualkeyboard/all/backslash.png"),
              QRect(553,76,36,35),
              QString("backslash"),
              QString("0x5c"),
              QString("\\") );

    connect(backslash,SIGNAL(pressed()),this,SLOT(backslash_pressed()));
    connect(backslash,SIGNAL(released()),this,SLOT(backslash_released()));

    addButton((&caps_lock_button),
              QIcon(":/resource/images/virtualkeyboard/all/CapsLock.png"),
              QRect(10,115,68,35),
              QString("caps_lock_button"),
              QString("0x01000024"),
              QString("") );

    connect(caps_lock_button,SIGNAL(pressed()),this,SLOT(caps_lock_button_pressed()));
    connect(caps_lock_button,SIGNAL(released()),this,SLOT(caps_lock_button_released()));

    addButton((&a_button),
              QIcon(":/resource/images/virtualkeyboard/all/A.png"),
              QRect(82,115,36,35),
              QString("a_button"),
              QString("0x41"),
              QString("a") );

    connect(a_button,SIGNAL(pressed()),this,SLOT(a_button_pressed()));
    connect(a_button,SIGNAL(released()),this,SLOT(a_button_released()));

    addButton((&s_button),
              QIcon(":/resource/images/virtualkeyboard/all/S.png"),
              QRect(122,115,36,35),
              QString("s_button"),
              QString("0x53"),
              QString("s") );

    connect(s_button,SIGNAL(pressed()),this,SLOT(s_button_pressed()));
    connect(s_button,SIGNAL(released()),this,SLOT(s_button_released()));

    addButton((&d_button),
              QIcon(":/resource/images/virtualkeyboard/all/D.png"),
              QRect(162,115,36,35),
              QString("d_button"),
              QString("0x44"),
              QString("d") );

    connect(d_button,SIGNAL(pressed()),this,SLOT(d_button_pressed()));
    connect(d_button,SIGNAL(released()),this,SLOT(d_button_released()));

    addButton((&f_button),
              QIcon(":/resource/images/virtualkeyboard/all/F.png"),
              QRect(202,115,36,35),
              QString("f_button"),
              QString("0x46"),
              QString("f") );

    connect(f_button,SIGNAL(pressed()),this,SLOT(f_button_pressed()));
    connect(f_button,SIGNAL(released()),this,SLOT(f_button_released()));

    addButton((&g_button),
              QIcon(":/resource/images/virtualkeyboard/all/G.png"),
              QRect(242,115,36,35),
              QString("g_button"),
              QString("0x47"),
              QString("g") );

    connect(g_button,SIGNAL(pressed()),this,SLOT(g_button_pressed()));
    connect(g_button,SIGNAL(released()),this,SLOT(g_button_released()));

    addButton((&h_button),
              QIcon(":/resource/images/virtualkeyboard/all/H.png"),
              QRect(282,115,36,35),
              QString("h_button"),
              QString("0x48"),
              QString("h") );

    connect(h_button,SIGNAL(pressed()),this,SLOT(h_button_pressed()));
    connect(h_button,SIGNAL(released()),this,SLOT(h_button_released()));

    addButton((&j_button),
              QIcon(":/resource/images/virtualkeyboard/all/J.png"),
              QRect(322,115,36,35),
              QString("j_button"),
              QString("0x4a"),
              QString("j") );

    connect(j_button,SIGNAL(pressed()),this,SLOT(j_button_pressed()));
    connect(j_button,SIGNAL(released()),this,SLOT(j_button_released()));

    addButton((&k_button),
              QIcon(":/resource/images/virtualkeyboard/all/K.png"),
              QRect(362,115,36,35),
              QString("k_button"),
              QString("0x4b"),
              QString("k") );

    connect(k_button,SIGNAL(pressed()),this,SLOT(k_button_pressed()));
    connect(k_button,SIGNAL(released()),this,SLOT(k_button_released()));

    addButton((&l_button),
              QIcon(":/resource/images/virtualkeyboard/all/L.png"),
              QRect(402,115,36,35),
              QString("l_button"),
              QString("0x4c"),
              QString("l") );

    connect(l_button,SIGNAL(pressed()),this,SLOT(l_button_pressed()));
    connect(l_button,SIGNAL(released()),this,SLOT(l_button_released()));

    addButton((&semicolon_button),
              QIcon(":/resource/images/virtualkeyboard/all/semicolon.png"),
              QRect(442,115,36,35),
              QString("semicolon_button"),
              QString("0x3b"),
              QString(";") );

    connect(semicolon_button,SIGNAL(pressed()),this,SLOT(semicolon_button_pressed()));
    connect(semicolon_button,SIGNAL(released()),this,SLOT(semicolon_button_released()));

    addButton((&apostrophe_button),
              QIcon(":/resource/images/virtualkeyboard/all/apostrophe.png"),
              QRect(482,115,36,35),
              QString("apostrophe_button"),
              QString("0x27"),
              QString("'") );

    connect(apostrophe_button,SIGNAL(pressed()),this,SLOT(apostrophe_button_pressed()));
    connect(apostrophe_button,SIGNAL(released()),this,SLOT(apostrophe_button_released()));

    addButton((&enter_button),
              QIcon(":/resource/images/virtualkeyboard/all/enter.png"),
              QRect(522,115,67,35),
              QString("enter_button"),
              QString("0x01000005"),
              QString("") );

    connect(enter_button,SIGNAL(pressed()),this,SLOT(enter_button_pressed()));
    connect(enter_button,SIGNAL(released()),this,SLOT(enter_button_released()));

    addButton((&left_shift_button),
              QIcon(":/resource/images/virtualkeyboard/all/Left_Shift.png"),
              QRect(10,154,87,35),
              QString("left_shift_button"),
              QString("0x01000020"),
              QString("") );

    connect(left_shift_button,SIGNAL(pressed()),this,SLOT(left_shift_button_pressed()));
    connect(left_shift_button,SIGNAL(released()),this,SLOT(left_shift_button_released()));

    addButton((&z_button),
              QIcon(":/resource/images/virtualkeyboard/all/Z.png"),
              QRect(101,154,36,35),
              QString("z_button"),
              QString("0x5a"),
              QString("z") );

    connect(z_button,SIGNAL(pressed()),this,SLOT(z_button_pressed()));
    connect(z_button,SIGNAL(released()),this,SLOT(z_button_released()));

    addButton((&x_button),
              QIcon(":/resource/images/virtualkeyboard/all/X.png"),
              QRect(141,154,36,35),
              QString("x_button"),
              QString("0x58"),
              QString("x") );

    connect(x_button,SIGNAL(pressed()),this,SLOT(x_button_pressed()));
    connect(x_button,SIGNAL(released()),this,SLOT(x_button_released()));

    addButton((&c_button),
              QIcon(":/resource/images/virtualkeyboard/all/C.png"),
              QRect(181,154,36,35),
              QString("c_button"),
              QString("0x43"),
              QString("c") );

    connect(c_button,SIGNAL(pressed()),this,SLOT(c_button_pressed()));
    connect(c_button,SIGNAL(released()),this,SLOT(c_button_released()));

    addButton((&v_button),
              QIcon(":/resource/images/virtualkeyboard/all/V.png"),
              QRect(221,154,36,35),
              QString("v_button"),
              QString("0x56"),
              QString("v") );

    connect(v_button,SIGNAL(pressed()),this,SLOT(v_button_pressed()));
    connect(v_button,SIGNAL(released()),this,SLOT(v_button_released()));

    addButton((&b_button),
              QIcon(":/resource/images/virtualkeyboard/all/B.png"),
              QRect(261,154,36,35),
              QString("b_button"),
              QString("0x42"),
              QString("b") );

    connect(b_button,SIGNAL(pressed()),this,SLOT(b_button_pressed()));
    connect(b_button,SIGNAL(released()),this,SLOT(b_button_released()));

    addButton((&n_button),
              QIcon(":/resource/images/virtualkeyboard/all/N.png"),
              QRect(301,154,36,35),
              QString("n_button"),
              QString("0x4e"),
              QString("n") );

    connect(n_button,SIGNAL(pressed()),this,SLOT(n_button_pressed()));
    connect(n_button,SIGNAL(released()),this,SLOT(n_button_released()));

    addButton((&m_button),
              QIcon(":/resource/images/virtualkeyboard/all/M.png"),
              QRect(341,154,36,35),
              QString("m_button"),
              QString("0x4d"),
              QString("m") );

    connect(m_button,SIGNAL(pressed()),this,SLOT(m_button_pressed()));
    connect(m_button,SIGNAL(released()),this,SLOT(m_button_released()));

    addButton((&comma_button),
              QIcon(":/resource/images/virtualkeyboard/all/comma.png"),
              QRect(381,154,36,35),
              QString("comma_button"),
              QString("0x2c"),
              QString(",") );

    connect(comma_button,SIGNAL(pressed()),this,SLOT(comma_button_pressed()));
    connect(comma_button,SIGNAL(released()),this,SLOT(comma_button_released()));

    addButton((&period_button),
              QIcon(":/resource/images/virtualkeyboard/all/period.png"),
              QRect(421,154,36,35),
              QString("period_button"),
              QString("0x2e"),
              QString(".") );

    connect(period_button,SIGNAL(pressed()),this,SLOT(period_button_pressed()));
    connect(period_button,SIGNAL(released()),this,SLOT(period_button_released()));

    addButton((&slash_button),
              QIcon(":/resource/images/virtualkeyboard/all/slash.png"),
              QRect(461,154,36,35),
              QString("slash_button"),
              QString("0x2f"),
              QString("/") );

    connect(slash_button,SIGNAL(pressed()),this,SLOT(slash_button_pressed()));
    connect(slash_button,SIGNAL(released()),this,SLOT(slash_button_released()));

    addButton((&right_shift_button),
              QIcon(":/resource/images/virtualkeyboard/all/Right_Shift.png"),
              QRect(501,154,87,35),
              QString("right_shift_button"),
              QString("0x4d"),
              QString("") );

    connect(right_shift_button,SIGNAL(pressed()),this,SLOT(right_shift_button_pressed()));
    connect(right_shift_button,SIGNAL(released()),this,SLOT(right_shift_button_released()));

    addButton((&close_button),
              QIcon(":/resource/images/virtualkeyboard/all/close.png"),
              QRect(572,11,17,16),
              QString("close_button"),
              QString(""),
              QString("") );


    //my_typeInput = AllKeyboardType;

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(saveFocusWidget(QWidget*,QWidget*)));

    allButtons = findChildren<QPushButton *>();
    for (int i=0;i<allButtons.count();i++) {
        connect(allButtons.at(i), SIGNAL(clicked()), &signalMapper, SLOT(map()));
        signalMapper.setMapping(allButtons.at(i), i);
    }

    connect(&signalMapper, SIGNAL(mapped(int)),
            this, SLOT(buttonClicked(int)));

    caps_lock_button->setCheckable(true);
    left_shift_button->setCheckable(true);
    right_shift_button->setCheckable(true);

    connect(right_shift_button,SIGNAL(toggled(bool)),this,SLOT(btnShiftRight_toggled(bool)));
    connect(left_shift_button,SIGNAL(toggled(bool)),this,SLOT(btnShiftLeft_toggled(bool)));
    connect(caps_lock_button,SIGNAL(toggled(bool)),this,SLOT(btnCaps_toggled(bool)));
    connect(close_button,SIGNAL(clicked()),this,SLOT(close()));
}

//! [0]

bool AllInputPanel::event(QEvent *e)
{
    switch (e->type()) {
//! [1]
    case QEvent::WindowActivate:
        if (lastFocusedWidget)
        {
            //lastFocusedWidget->activateWindow();
        }
        break;
//! [1]
    default:
        break;
    }

    return QWidget::event(e);
}

//! [2]
void AllInputPanel::appendWidget(QWidget * watch,int whichInput)
{
    InputAllWidget widget;
    widget.watch = watch;
    widget.whichInput = whichInput;
    focusWidgetList.append(widget);
}

void AllInputPanel::removeWidget(QWidget *watch)
{
    int i = 0;
    QList <InputAllWidget> ::iterator beginAddPos;
    QList <InputAllWidget> ::iterator endAddPos;
    beginAddPos = focusWidgetList.begin();
    endAddPos = focusWidgetList.end();

    while(beginAddPos != endAddPos)
    {
        if ((*beginAddPos).watch == watch)
        {
            focusWidgetList.removeAt(i);
            break;
        }
        beginAddPos++;
        i++;
    }
}

void AllInputPanel::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus) &&
            (lastFocusedWidget != newFocus) )
    {

        QList <InputAllWidget> ::iterator beginAddPos;
        QList <InputAllWidget> ::iterator endAddPos;
        beginAddPos = focusWidgetList.begin();
        endAddPos = focusWidgetList.end();

        while(beginAddPos != endAddPos)
        {
            if ((*beginAddPos).watch == newFocus)
            {
                lastFocusedWidget = newFocus;
                if((*beginAddPos).whichInput==AllInputMethod)
                    emit wakeUpKeyboard((*beginAddPos).whichInput);
                break;
            }
            beginAddPos++;
        }

        if (beginAddPos == endAddPos)
        {
            lastFocusedWidget = newFocus;
            emit hideKeyboard();
        }
    }
}

//! [2]

//! [3]

void AllInputPanel::buttonClicked(int btn)
{
    QString strKeyId;
    strKeyId = qvariant_cast<QString>(allButtons.at(btn)->property("codeValue"));

    bool isOk;
    int keyId = strKeyId.toInt(&isOk, 16);

    QString ch = qvariant_cast<QString>(allButtons.at(btn)->property("echoCode"));

    if (keyId==Qt::Key_Shift
        || keyId==Qt::Key_Control
        || keyId==Qt::Key_Alt
        || keyId==Qt::Key_Print
        )
        return;

    if (!keyId)
        return;

    int involvedKeys = 1;
    Qt::KeyboardModifiers Modifier = Qt::NoModifier;

    if (isShift) {
        Modifier = Modifier | Qt::ShiftModifier;
        involvedKeys++;
    }

    lastFocusedWidget->activateWindow();

    if (insertMode ==false)
    {
        emit characterGenerated(Qt::Key_Delete,Qt::NoModifier,"",1);
    }

    emit characterGenerated(keyId,Modifier,ch,involvedKeys);

    left_shift_button->setChecked(false);
    right_shift_button->setChecked(false);
}

//! [3]

QWidget * AllInputPanel::getFocusWidget()
{
    return lastFocusedWidget;
}

void AllInputPanel::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {

        dragPosition = event->globalPos() - frameGeometry().topLeft();

        event->accept();

    }

}

void AllInputPanel::mouseMoveEvent(QMouseEvent *event) {

    if (event->buttons() & Qt::LeftButton) {

        move(event->globalPos() - dragPosition);

        event->accept();

    }

}

void AllInputPanel::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(":/resource/images/virtualkeyboard/all/keyboard.png"));
}

void AllInputPanel::addButton(QPushButton ** button,
                              QIcon icon,
                              QRect rect,
                              QString button_name,
                              QString keyCode,
                              QString string)
{
    *button = new QPushButton(this);
    (*button)->setIcon(icon);
    (*button)->setIconSize(QSize(rect.width(),rect.height()));
    (*button)->setObjectName(button_name);
    (*button)->setGeometry(rect);
    (*button)->setFocusPolicy(Qt::NoFocus);
    (*button)->setProperty("codeValue", QVariant(keyCode));
    (*button)->setProperty("echoCode", QVariant(string));
}

void AllInputPanel::btnCaps_toggled(bool checked)
{
    changeTextCaps(checked);
    isCaps = checked;
}

void AllInputPanel::btnShiftLeft_toggled(bool checked)
{
    isShift = checked;
    if (isCaps) {
        changeTextShift(checked);
    }
    else {
        changeTextShift(checked);
        changeTextCaps(checked);
    }
}

void AllInputPanel::btnShiftRight_toggled(bool checked)
{
    btnShiftLeft_toggled(checked);
}

void AllInputPanel::changeTextShift(bool isShift)
{
    changeTextCaps(!isShift);
    if (isShift) {

        tilde_button->setProperty("echoCode", QVariant(QString("~")));
        number_1_button->setProperty("echoCode", QVariant(QString("!")));
        number_2_button->setProperty("echoCode", QVariant(QString("@")));
        number_3_button->setProperty("echoCode", QVariant(QString("#")));
        number_4_button->setProperty("echoCode", QVariant(QString("$")));
        number_5_button->setProperty("echoCode", QVariant(QString("%")));
        number_6_button->setProperty("echoCode", QVariant(QString("^")));
        number_7_button->setProperty("echoCode", QVariant(QString("&&")));
        number_8_button->setProperty("echoCode", QVariant(QString("*")));
        number_9_button->setProperty("echoCode", QVariant(QString("(")));
        number_0_button->setProperty("echoCode", QVariant(QString(")")));
        minus_sign_button->setProperty("echoCode", QVariant(QString("_")));
        plus_sign_button->setProperty("echoCode", QVariant(QString("+")));

        Left_Square_Bracket->setProperty("echoCode", QVariant(QString("{")));
        Right_Square_Bracket->setProperty("echoCode", QVariant(QString("}")));
        backslash->setProperty("echoCode", QVariant(QString("|")));

        semicolon_button->setProperty("echoCode", QVariant(QString(":")));
        apostrophe_button->setProperty("echoCode", QVariant(QString("\"")));

        comma_button->setProperty("echoCode", QVariant(QString("<")));
        period_button->setProperty("echoCode", QVariant(QString(">")));
        slash_button->setProperty("echoCode", QVariant(QString("?")));
    }
    else {

        tilde_button->setProperty("echoCode", QVariant(QString("`")));
        number_1_button->setProperty("echoCode", QVariant(QString("1")));
        number_2_button->setProperty("echoCode", QVariant(QString("2")));
        number_3_button->setProperty("echoCode", QVariant(QString("3")));
        number_4_button->setProperty("echoCode", QVariant(QString("4")));
        number_5_button->setProperty("echoCode", QVariant(QString("5")));
        number_6_button->setProperty("echoCode", QVariant(QString("6")));
        number_7_button->setProperty("echoCode", QVariant(QString("7")));
        number_8_button->setProperty("echoCode", QVariant(QString("8")));
        number_9_button->setProperty("echoCode", QVariant(QString("9")));
        number_0_button->setProperty("echoCode", QVariant(QString("0")));
        minus_sign_button->setProperty("echoCode", QVariant(QString("-")));
        plus_sign_button->setProperty("echoCode", QVariant(QString("=")));

        Left_Square_Bracket->setProperty("echoCode", QVariant(QString("[")));
        Right_Square_Bracket->setProperty("echoCode", QVariant(QString("]")));
        backslash->setProperty("echoCode", QVariant(QString("\\")));

        semicolon_button->setProperty("echoCode", QVariant(QString(";")));
        apostrophe_button->setProperty("echoCode", QVariant(QString("'")));

        comma_button->setProperty("echoCode", QVariant(QString(",")));
        period_button->setProperty("echoCode", QVariant(QString(".")));
        slash_button->setProperty("echoCode", QVariant(QString("/")));
    }
}

void AllInputPanel::changeTextCaps(bool isCaps)
{
    if (isCaps) {
        q_button->setProperty("echoCode", QVariant(QString("Q")));
        w_button->setProperty("echoCode", QVariant(QString("W")));
        e_button->setProperty("echoCode", QVariant(QString("E")));
        r_button->setProperty("echoCode", QVariant(QString("R")));
        t_button->setProperty("echoCode", QVariant(QString("T")));
        y_button->setProperty("echoCode", QVariant(QString("Y")));
        u_button->setProperty("echoCode", QVariant(QString("U")));
        i_button->setProperty("echoCode", QVariant(QString("I")));
        o_button->setProperty("echoCode", QVariant(QString("O")));
        p_button->setProperty("echoCode", QVariant(QString("P")));

        a_button->setProperty("echoCode", QVariant(QString("A")));
        s_button->setProperty("echoCode", QVariant(QString("S")));
        d_button->setProperty("echoCode", QVariant(QString("D")));
        f_button->setProperty("echoCode", QVariant(QString("F")));
        g_button->setProperty("echoCode", QVariant(QString("G")));
        h_button->setProperty("echoCode", QVariant(QString("H")));
        j_button->setProperty("echoCode", QVariant(QString("J")));
        k_button->setProperty("echoCode", QVariant(QString("K")));
        l_button->setProperty("echoCode", QVariant(QString("L")));

        z_button->setProperty("echoCode", QVariant(QString("Z")));
        x_button->setProperty("echoCode", QVariant(QString("X")));
        c_button->setProperty("echoCode", QVariant(QString("C")));
        v_button->setProperty("echoCode", QVariant(QString("V")));
        b_button->setProperty("echoCode", QVariant(QString("B")));
        n_button->setProperty("echoCode", QVariant(QString("N")));
        m_button->setProperty("echoCode", QVariant(QString("M")));
    }
    else {
        q_button->setProperty("echoCode", QVariant(QString("q")));
        w_button->setProperty("echoCode", QVariant(QString("w")));
        e_button->setProperty("echoCode", QVariant(QString("e")));
        r_button->setProperty("echoCode", QVariant(QString("r")));
        t_button->setProperty("echoCode", QVariant(QString("t")));
        y_button->setProperty("echoCode", QVariant(QString("y")));
        u_button->setProperty("echoCode", QVariant(QString("u")));
        i_button->setProperty("echoCode", QVariant(QString("i")));
        o_button->setProperty("echoCode", QVariant(QString("o")));
        p_button->setProperty("echoCode", QVariant(QString("p")));

        a_button->setProperty("echoCode", QVariant(QString("a")));
        s_button->setProperty("echoCode", QVariant(QString("s")));
        d_button->setProperty("echoCode", QVariant(QString("d")));
        f_button->setProperty("echoCode", QVariant(QString("f")));
        g_button->setProperty("echoCode", QVariant(QString("g")));
        h_button->setProperty("echoCode", QVariant(QString("h")));
        j_button->setProperty("echoCode", QVariant(QString("j")));
        k_button->setProperty("echoCode", QVariant(QString("k")));
        l_button->setProperty("echoCode", QVariant(QString("l")));

        z_button->setProperty("echoCode", QVariant(QString("z")));
        x_button->setProperty("echoCode", QVariant(QString("x")));
        c_button->setProperty("echoCode", QVariant(QString("c")));
        v_button->setProperty("echoCode", QVariant(QString("v")));
        b_button->setProperty("echoCode", QVariant(QString("b")));
        n_button->setProperty("echoCode", QVariant(QString("n")));
        m_button->setProperty("echoCode", QVariant(QString("m")));
    }
}

void AllInputPanel::tilde_button_pressed()
{
    tilde_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/tilde_pressed.png"));
}

void AllInputPanel::tilde_button_released()
{
    tilde_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/tilde.png"));
}

void AllInputPanel::number_1_button_pressed()
{
    number_1_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/1_pressed.png"));
}

void AllInputPanel::number_1_button_released()
{
    number_1_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/1.png"));
}

void AllInputPanel::number_2_button_pressed()
{
    number_2_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/2_pressed.png"));
}

void AllInputPanel::number_2_button_released()
{
    number_2_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/2.png"));
}

void AllInputPanel::number_3_button_pressed()
{
    number_3_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/3_pressed.png"));
}

void AllInputPanel::number_3_button_released()
{
    number_3_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/3.png"));
}

void AllInputPanel::number_4_button_pressed()
{
    number_4_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/4_pressed.png"));
}

void AllInputPanel::number_4_button_released()
{
    number_4_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/4.png"));
}

void AllInputPanel::number_5_button_pressed()
{
    number_5_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/5_pressed.png"));
}

void AllInputPanel::number_5_button_released()
{
    number_5_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/5.png"));
}

void AllInputPanel::number_6_button_pressed()
{
    number_6_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/6_pressed.png"));
}

void AllInputPanel::number_6_button_released()
{
    number_6_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/6.png"));
}

void AllInputPanel::number_7_button_pressed()
{
    number_7_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/7_pressed.png"));
}

void AllInputPanel::number_7_button_released()
{
    number_7_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/7.png"));
}

void AllInputPanel::number_8_button_pressed()
{
    number_8_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/8_pressed.png"));
}

void AllInputPanel::number_8_button_released()
{
    number_8_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/8.png"));
}

void AllInputPanel::number_9_button_pressed()
{
    number_9_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/9_pressed.png"));
}

void AllInputPanel::number_9_button_released()
{
    number_9_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/9.png"));
}

void AllInputPanel::number_0_button_pressed()
{
    number_0_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/0_pressed.png"));
}

void AllInputPanel::number_0_button_released()
{
    number_0_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/0.png"));
}

void AllInputPanel::minus_sign_button_pressed()
{
    minus_sign_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/minus_pressed.png"));
}

void AllInputPanel::minus_sign_button_released()
{
    minus_sign_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/minus.png"));
}

void AllInputPanel::plus_sign_button_pressed()
{
    plus_sign_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/plus_pressed.png"));
}

void AllInputPanel::plus_sign_button_released()
{
    plus_sign_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/plus.png"));
}

void AllInputPanel::backspace_button_pressed()
{
    backspace_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/del_pressed.png"));
}

void AllInputPanel::backspace_button_released()
{
    backspace_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/del.png"));
}

void AllInputPanel::tab_button_pressed()
{
    tab_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/tab_pressed.png"));
}

void AllInputPanel::tab_button_released()
{
    tab_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/tab.png"));
}


void AllInputPanel::q_button_pressed()
{
    q_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Q_pressed.png"));
}

void AllInputPanel::q_button_released()
{
    q_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Q.png"));
}

void AllInputPanel::w_button_pressed()
{
    w_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/W_pressed.png"));
}

void AllInputPanel::w_button_released()
{
    w_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/W.png"));
}

void AllInputPanel::e_button_pressed()
{
    e_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/E_pressed.png"));
}

void AllInputPanel::e_button_released()
{
    e_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/E.png"));
}

void AllInputPanel::r_button_pressed()
{
    r_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/R_pressed.png"));
}

void AllInputPanel::r_button_released()
{
    r_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/R.png"));
}

void AllInputPanel::t_button_pressed()
{
    t_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/T_pressed.png"));
}

void AllInputPanel::t_button_released()
{
    t_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/T.png"));
}

void AllInputPanel::y_button_pressed()
{
    y_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Y_pressed.png"));
}

void AllInputPanel::y_button_released()
{
    y_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Y.png"));
}

void AllInputPanel::u_button_pressed()
{
    u_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/U_pressed.png"));
}

void AllInputPanel::u_button_released()
{
    u_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/U.png"));
}

void AllInputPanel::i_button_pressed()
{
    i_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/I_pressed.png"));
}

void AllInputPanel::i_button_released()
{
    i_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/I.png"));
}

void AllInputPanel::o_button_pressed()
{
    o_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/O_pressed.png"));
}

void AllInputPanel::o_button_released()
{
    o_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/O.png"));
}

void AllInputPanel::p_button_pressed()
{
    p_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/P_pressed.png"));
}

void AllInputPanel::p_button_released()
{
    p_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/P.png"));
}

void AllInputPanel::Left_Square_Bracket_pressed()
{
    Left_Square_Bracket->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Left_Square_Bracket_pressed.png"));
}

void AllInputPanel::Left_Square_Bracket_released()
{
    Left_Square_Bracket->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Left_Square_Bracket.png"));
}

void AllInputPanel::Right_Square_Bracket_pressed()
{
    Right_Square_Bracket->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Right_Square_Bracket_pressed.png"));
}

void AllInputPanel::Right_Square_Bracket_released()
{
    Right_Square_Bracket->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Right_Square_Bracket.png"));
}

void AllInputPanel::backslash_pressed()
{
    backslash->setIcon(QIcon(":/resource/images/virtualkeyboard/all/backslash_pressed.png"));
}

void AllInputPanel::backslash_released()
{
    backslash->setIcon(QIcon(":/resource/images/virtualkeyboard/all/backslash.png"));
}

void AllInputPanel::caps_lock_button_pressed()
{
    caps_lock_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/CapsLock_pressed.png"));
}

void AllInputPanel::caps_lock_button_released()
{
    caps_lock_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/CapsLock.png"));
}

void AllInputPanel::a_button_pressed()
{
    a_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/A_pressed.png"));
}

void AllInputPanel::a_button_released()
{
    a_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/A.png"));
}

void AllInputPanel::s_button_pressed()
{
    s_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/S_pressed.png"));
}

void AllInputPanel::s_button_released()
{
    s_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/S.png"));
}

void AllInputPanel::d_button_pressed()
{
    d_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/D_pressed.png"));
}

void AllInputPanel::d_button_released()
{
    d_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/D.png"));
}

void AllInputPanel::f_button_pressed()
{
    f_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/F_pressed.png"));
}

void AllInputPanel::f_button_released()
{
    f_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/F.png"));
}

void AllInputPanel::g_button_pressed()
{
    g_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/G_pressed.png"));
}

void AllInputPanel::g_button_released()
{
    g_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/G.png"));
}

void AllInputPanel::h_button_pressed()
{
    h_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/H_pressed.png"));
}

void AllInputPanel::h_button_released()
{
    h_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/H.png"));
}

void AllInputPanel::j_button_pressed()
{
    j_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/J_pressed.png"));
}

void AllInputPanel::j_button_released()
{
    j_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/J.png"));
}

void AllInputPanel::k_button_pressed()
{
    k_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/K_pressed.png"));
}

void AllInputPanel::k_button_released()
{
    k_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/K.png"));
}

void AllInputPanel::l_button_pressed()
{
    l_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/L_pressed.png"));
}

void AllInputPanel::l_button_released()
{
    l_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/L.png"));
}

void AllInputPanel::semicolon_button_pressed()
{
    semicolon_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/semicolon_pressed.png"));
}

void AllInputPanel::semicolon_button_released()
{
    semicolon_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/semicolon.png"));
}

void AllInputPanel::apostrophe_button_pressed()
{
    apostrophe_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/apostrophe_pressed.png"));
}

void AllInputPanel::apostrophe_button_released()
{
    apostrophe_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/apostrophe.png"));
}

void AllInputPanel::enter_button_pressed()
{
    enter_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/enter_pressed.png"));
}

void AllInputPanel::enter_button_released()
{
    enter_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/enter.png"));
}

void AllInputPanel::left_shift_button_pressed()
{
    left_shift_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Left_Shift_pressed.png"));
}

void AllInputPanel::left_shift_button_released()
{
    left_shift_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Left_Shift.png"));
}

void AllInputPanel::z_button_pressed()
{
    z_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Z_pressed.png"));
}

void AllInputPanel::z_button_released()
{
    z_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Z.png"));
}

void AllInputPanel::x_button_pressed()
{
    x_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/X_pressed.png"));
}

void AllInputPanel::x_button_released()
{
    x_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/X.png"));
}

void AllInputPanel::c_button_pressed()
{
    c_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/C_pressed.png"));
}

void AllInputPanel::c_button_released()
{
    c_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/C.png"));
}

void AllInputPanel::v_button_pressed()
{
    v_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/V_pressed.png"));
}

void AllInputPanel::v_button_released()
{
    v_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/V.png"));
}

void AllInputPanel::b_button_pressed()
{
    b_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/B_pressed.png"));
}

void AllInputPanel::b_button_released()
{
    b_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/B.png"));
}

void AllInputPanel::n_button_pressed()
{
    n_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/N_pressed.png"));
}

void AllInputPanel::n_button_released()
{
    n_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/N.png"));
}

void AllInputPanel::m_button_pressed()
{
    m_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/M_pressed.png"));
}

void AllInputPanel::m_button_released()
{
    m_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/M.png"));
}

void AllInputPanel::comma_button_pressed()
{
    comma_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/comma_pressed.png"));
}

void AllInputPanel::comma_button_released()
{
    comma_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/comma.png"));
}

void AllInputPanel::period_button_pressed()
{
    period_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/period_pressed.png"));
}

void AllInputPanel::period_button_released()
{
    period_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/period.png"));
}

void AllInputPanel::slash_button_pressed()
{
    slash_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/slash_pressed.png"));
}

void AllInputPanel::slash_button_released()
{
    slash_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/slash.png"));
}

void AllInputPanel::right_shift_button_pressed()
{
    right_shift_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Right_Shift_pressed.png"));
}

void AllInputPanel::right_shift_button_released()
{
    right_shift_button->setIcon(QIcon(":/resource/images/virtualkeyboard/all/Right_Shift.png"));
}

AllInputPanel::~AllInputPanel()
{
    focusWidgetList.clear();
}
