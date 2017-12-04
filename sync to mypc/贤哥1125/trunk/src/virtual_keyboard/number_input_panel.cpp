#include "number_input_panel.h"
//#include <QLineEdit>
//#include "MyLineEdit.h"
#include "config_ui.h"
#include "global_var.h"

#include "scene_setup_ultrafiltration.h"
#include "scene_setup_cure_time.h"
#include "scene_setup_isouf.h"
#include "scene_setup_hemopump.h"
#include "scene_setup_subspump.h"

//! [0]

NumberInputPanel::NumberInputPanel()
    : QWidget(0, Qt::Tool| Qt::WindowStaysOnTopHint ),
      lastFocusedWidget(0),
      ins(this)
{
    //form.setupUi(this);

    isOnRun = false;
    insertMode = true;

    QPalette palette;

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool |Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setObjectName(QString::fromUtf8("Number_Form"));
    this->resize(243, 356);
    this->setWindowOpacity(1.0);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFocusPolicy(Qt::NoFocus);

    pix.load(":/resource/images/virtualkeyboard/mini-keyboard/keyboard.png",0,
             Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    resize(pix.size());
    setMask(QBitmap(pix.mask()));

    isNeg = false;

    button_0 = new WidgetButton(this);
    button_0->setStyleNormal(":/resource/images/virtualkeyboard/mini-keyboard/save.png",
            QRect(KEY_LEFT, KEY_BOTTOM, 101, KEY_HEIGHT));
    button_0->setObjectName(QString::fromUtf8("button_0"));
    button_0->setFocusPolicy(Qt::NoFocus);
    button_0->setProperty("codeValue", QVariant(QString("0x30")));
    button_0->setProperty("echoCode", QVariant(QString("0")));
    button_0->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/save.png")));
    button_0->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/save.png")));
    button_0->setText(tr("0"));
    button_0->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    button_0->setStyleSheet(CONFIG_COLOR_LABEL);

    button_period = new QPushButton(this);
    button_period->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/dot.png"));
    button_period->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_period->setObjectName(QString::fromUtf8("button_period"));
    button_period->setGeometry(KEY_LEFT + 101 + KEY_DISTANCE, KEY_BOTTOM, KEY_WIDTH, KEY_HEIGHT);
    button_period->setFocusPolicy(Qt::NoFocus);
    button_period->setProperty("codeValue", QVariant(QString("0x2e")));
    button_period->setProperty("echoCode", QVariant(QString(".")));
    button_period->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/dot.png")));
    button_period->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/dot.png")));


    button_confirm = new QPushButton(this);
    button_confirm->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/set.png"));
    button_confirm->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_confirm->setObjectName(QString::fromUtf8("button_confirm"));
    button_confirm->setGeometry(KEY_LEFT + 101 + KEY_DISTANCE * 2
                                + KEY_WIDTH, KEY_BOTTOM, KEY_WIDTH, KEY_HEIGHT);
    button_confirm->setFocusPolicy(Qt::NoFocus);
    button_confirm->setProperty("codeValue", QVariant(QString("")));
    button_confirm->setProperty("echoCode", QVariant(QString("")));
    button_confirm->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/set.png")));
    button_confirm->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/set.png")));


    button_1 = new QPushButton(this);
    button_1->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-1.png"));
    button_1->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_1->setObjectName(QString::fromUtf8("button_1"));
    button_1->setGeometry(KEY_LEFT, KEY_BOTTOM - KEY_HEIGHT - KEY_DISTANCE,
                          KEY_WIDTH, KEY_HEIGHT);
    button_1->setFocusPolicy(Qt::NoFocus);
    button_1->setProperty("codeValue", QVariant(QString("0x31")));
    button_1->setProperty("echoCode", QVariant(QString("1")));
    button_1->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-1.png")));
    button_1->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-1.png")));

    button_2 = new QPushButton(this);
    button_2->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-2.png"));
    button_2->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_2->setObjectName(QString::fromUtf8("button_2"));
    button_2->setGeometry(KEY_LEFT + KEY_DISTANCE + KEY_WIDTH,
                          KEY_BOTTOM - KEY_HEIGHT - KEY_DISTANCE, KEY_WIDTH, KEY_HEIGHT);
    button_2->setFocusPolicy(Qt::NoFocus);
    button_2->setProperty("codeValue", QVariant(QString("0x32")));
    button_2->setProperty("echoCode", QVariant(QString("2")));
    button_2->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-2.png")));
    button_2->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-2.png")));

    button_3 = new QPushButton(this);
    button_3->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-3.png"));
    button_3->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_3->setObjectName(QString::fromUtf8("button_3"));
    button_3->setGeometry(KEY_LEFT + KEY_DISTANCE * 2  + KEY_WIDTH * 2,
                          KEY_BOTTOM - KEY_HEIGHT - KEY_DISTANCE, KEY_WIDTH, KEY_HEIGHT);
    button_3->setFocusPolicy(Qt::NoFocus);
    button_3->setProperty("codeValue", QVariant(QString("0x33")));
    button_3->setProperty("echoCode", QVariant(QString("3")));
    button_3->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-3.png")));
    button_3->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-3.png")));

    button_sub = new QPushButton(this);
    button_sub->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/dec.png"));
    button_sub->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_sub->setObjectName(QString::fromUtf8("button_sub"));
    button_sub->setGeometry(KEY_LEFT + KEY_DISTANCE * 3  + KEY_WIDTH * 3,
                            KEY_BOTTOM - KEY_HEIGHT - KEY_DISTANCE, KEY_WIDTH, KEY_HEIGHT);
    button_sub->setFocusPolicy(Qt::NoFocus);
    button_sub->setProperty("codeValue", QVariant(QString("")));
    button_sub->setProperty("echoCode", QVariant(QString("")));
    button_sub->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/dec.png")));
    button_sub->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/dec.png")));


    button_4 = new QPushButton(this);
    button_4->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-4.png"));
    button_4->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_4->setObjectName(QString::fromUtf8("button_4"));
    button_4->setGeometry(KEY_LEFT, KEY_BOTTOM - KEY_HEIGHT * 2 - KEY_DISTANCE * 2,
                          KEY_WIDTH, KEY_HEIGHT);
    button_4->setFocusPolicy(Qt::NoFocus);
    button_4->setProperty("codeValue", QVariant(QString("0x34")));
    button_4->setProperty("echoCode", QVariant(QString("4")));
    button_4->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-4.png")));
    button_4->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-4.png")));

    button_5 = new QPushButton(this);
    button_5->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-5.png"));
    button_5->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_5->setObjectName(QString::fromUtf8("button_5"));
    button_5->setGeometry(KEY_LEFT + KEY_DISTANCE + KEY_WIDTH,
                          KEY_BOTTOM - KEY_HEIGHT * 2 - KEY_DISTANCE * 2, KEY_WIDTH, KEY_HEIGHT);
    button_5->setFocusPolicy(Qt::NoFocus);
    button_5->setProperty("codeValue", QVariant(QString("0x35")));
    button_5->setProperty("echoCode", QVariant(QString("5")));
    button_5->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-5.png")));
    button_5->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-5.png")));

    button_6 = new QPushButton(this);
    button_6->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-6.png"));
    button_6->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_6->setObjectName(QString::fromUtf8("button_6"));
    button_6->setGeometry(KEY_LEFT + KEY_DISTANCE * 2 + KEY_WIDTH * 2,
                          KEY_BOTTOM - KEY_HEIGHT * 2 - KEY_DISTANCE * 2, KEY_WIDTH, KEY_HEIGHT);
    button_6->setFocusPolicy(Qt::NoFocus);
    button_6->setProperty("codeValue", QVariant(QString("0x36")));
    button_6->setProperty("echoCode", QVariant(QString("6")));
    button_6->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-6.png")));
    button_6->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-6.png")));

    button_add = new QPushButton(this);
    button_add->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/add.png"));
    button_add->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_add->setObjectName(QString::fromUtf8("button_add"));
    button_add->setGeometry(KEY_LEFT + KEY_DISTANCE * 3 + KEY_WIDTH * 3,
                            KEY_BOTTOM - KEY_HEIGHT * 2 - KEY_DISTANCE * 2, KEY_WIDTH, KEY_HEIGHT);
    button_add->setFocusPolicy(Qt::NoFocus);
    button_add->setProperty("codeValue", QVariant(QString("")));
    button_add->setProperty("echoCode", QVariant(QString("")));
    button_add->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/add.png")));
    button_add->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/add.png")));


    button_7 = new QPushButton(this);
    button_7->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-7.png"));
    button_7->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_7->setObjectName(QString::fromUtf8("button_7"));
    button_7->setGeometry(KEY_LEFT ,
                          KEY_BOTTOM - KEY_HEIGHT * 3 - KEY_DISTANCE * 3, KEY_WIDTH, KEY_HEIGHT);
    button_7->setFocusPolicy(Qt::NoFocus);
    button_7->setProperty("codeValue", QVariant(QString("0x37")));
    button_7->setProperty("echoCode", QVariant(QString("7")));
    button_7->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-7.png")));
    button_7->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-7.png")));

    button_8 = new QPushButton(this);
    button_8->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-8.png"));
    button_8->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_8->setObjectName(QString::fromUtf8("button_8"));
    button_8->setGeometry(KEY_LEFT + KEY_DISTANCE * 1 + KEY_WIDTH * 1,
                          KEY_BOTTOM - KEY_HEIGHT * 3 - KEY_DISTANCE * 3, KEY_WIDTH, KEY_HEIGHT);
    button_8->setFocusPolicy(Qt::NoFocus);
    button_8->setProperty("codeValue", QVariant(QString("0x38")));
    button_8->setProperty("echoCode", QVariant(QString("8")));
    button_8->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-8.png")));
    button_8->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-8.png")));

    button_9 = new QPushButton(this);
    button_9->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-9.png"));
    button_9->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_9->setObjectName(QString::fromUtf8("button_9"));
    button_9->setGeometry(KEY_LEFT + KEY_DISTANCE * 2 + KEY_WIDTH * 2,
                           KEY_BOTTOM - KEY_HEIGHT * 3 - KEY_DISTANCE * 3, KEY_WIDTH ,KEY_HEIGHT);
    button_9->setFocusPolicy(Qt::NoFocus);
    button_9->setProperty("codeValue", QVariant(QString("0x39")));
    button_9->setProperty("echoCode", QVariant(QString("9")));
    button_9->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-9.png")));
    button_9->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-9.png")));

    button_del = new QPushButton(this);
    button_del->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/clr.png"));
    button_del->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_del->setObjectName(QString::fromUtf8("button_del"));
    button_del->setGeometry(KEY_LEFT + KEY_DISTANCE * 3 + KEY_WIDTH * 3,
                            KEY_BOTTOM - KEY_HEIGHT * 3 - KEY_DISTANCE * 3, KEY_WIDTH, KEY_HEIGHT);
    button_del->setFocusPolicy(Qt::NoFocus);
    button_del->setProperty("codeValue", QVariant(QString("")));
    button_del->setProperty("echoCode", QVariant(QString("")));
    button_del->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/clr.png")));
    button_del->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/clr.png")));

    Label_max = new QLabel(this);
    Label_max->setGeometry(QRect(KEY_LEFT, KEY_BOTTOM - KEY_HEIGHT * 4 - KEY_DISTANCE * 4, 101, KEY_HEIGHT));
    Label_max->setObjectName(QString::fromUtf8("Label_max"));
    Label_max->setFocusPolicy(Qt::NoFocus);
    Label_max->setProperty("codeValue", QVariant(QString("")));
    Label_max->setProperty("echoCode", QVariant(QString("")));
    Label_max->setText(tr("max  600"));
    Label_max->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL));

    CONFIG_KEYLABLE_STYLE(Label_max);

    Label_min = new QLabel(this);
    Label_min->setGeometry(QRect(KEY_LEFT + 101 + KEY_DISTANCE,
                                 KEY_BOTTOM - KEY_HEIGHT * 4 - KEY_DISTANCE * 4, 101, KEY_HEIGHT));
    Label_min->setObjectName(QString::fromUtf8("Label_min"));
    Label_min->setFocusPolicy(Qt::NoFocus);
    Label_min->setProperty("codeValue", QVariant(QString("")));
    Label_min->setProperty("echoCode", QVariant(QString("")));
    Label_min->setText(tr("min  300"));
    Label_min->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL));

    CONFIG_KEYLABLE_STYLE(Label_min);

    labelValueOld = new QLabel(this);
    labelValueOld->setGeometry(QRect(KEY_LEFT, KEY_BOTTOM - KEY_HEIGHT * 5 - KEY_DISTANCE * 5,
                                   KEY_LEFT + KEY_DISTANCE * 2 + KEY_WIDTH * 4, KEY_HEIGHT));
    labelValueOld->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL));
    labelValueOld->setFocusPolicy(Qt::NoFocus);

    CONFIG_KEYLABLE_STYLE(labelValueOld);

    labelValueOld->setText("old     1122");

    labelValueNewBk = new QLabel(this);
    labelValueNewBk->setGeometry(QRect(KEY_LEFT, KEY_BOTTOM - KEY_HEIGHT * 7 - KEY_DISTANCE * 6,
                                     KEY_LEFT + KEY_DISTANCE * 2 + KEY_WIDTH * 4,  KEY_HEIGHT * 2 ));
    labelValueNewBk->setFocusPolicy(Qt::NoFocus);
    labelValueNewBk->setStyleSheet(               \
            "QLabel {"                       \
            "color:#ffffff;"                    \
            "border-radius: 4px;"               \
            "padding: 0 4px;"                   \
            "background: #123f75;"              \
            "}");


    labelNewBk = new QLabel(this);
    labelNewBk->setGeometry(QRect(KEY_LEFT + 5, KEY_BOTTOM - KEY_HEIGHT * 7 - KEY_DISTANCE * 6,
                                     KEY_LEFT + 5 + 40,  KEY_HEIGHT * 2 ));
    labelNewBk->setAlignment(Qt::AlignCenter);
    labelNewBk->setFocusPolicy(Qt::NoFocus);
    labelNewBk->setFont(QFont(QString(CONFIG_FONT_STYLE),
                25, CONFIG_FONT_NORMAL));
    labelNewBk->setText("new");
    labelNewBk->setStyleSheet("QLabel {color:#ffffff;border-style:none;background-color:transparent;}");


    labelValueNew = new QLineEdit(this);
    labelValueNew->setGeometry(QRect(KEY_LEFT + 5 + 50 + 5, KEY_BOTTOM - KEY_HEIGHT * 7 - KEY_DISTANCE * 6,
                                     KEY_LEFT + KEY_DISTANCE * 2 + KEY_WIDTH * 4,  KEY_HEIGHT * 2 ));
    labelValueNew->setFont(QFont(QString(CONFIG_FONT_STYLE),
                25, CONFIG_FONT_NORMAL));
    labelValueNew->setFocusPolicy(Qt::NoFocus);
    labelValueNew->setAlignment(Qt::AlignLeft);

    labelValueNew->setStyleSheet("QLineEdit {color:#ffffff;border-style:none;background-color:transparent;}");

    labelName = new QLabel(this);
    labelName->setGeometry(QRect(KEY_LEFT, KEY_BOTTOM - KEY_HEIGHT * 8 - KEY_DISTANCE * 7,
                                   101 + KEY_WIDTH + 20, KEY_HEIGHT));
    labelName->setAlignment(Qt::AlignCenter);
    labelName->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_TITLE, CONFIG_FONT_NORMAL));
    labelName->setFocusPolicy(Qt::NoFocus);
    labelName->setText("cond");

    labelName -> setStyleSheet("QLabel {"       \
                "color:#ff0000;"                \
                "border-radius: 4px;"           \
                "background-color:transparent;" \
                "padding: 0 4px;"               \
                "}");

    button_cancel = new QPushButton(this);
    button_cancel->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/close.png"));
    button_cancel->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_cancel->setObjectName(QString::fromUtf8("button_cancel"));
    button_cancel->setGeometry(KEY_LEFT + 101 + KEY_DISTANCE * 2 + KEY_WIDTH, KEY_BOTTOM - KEY_HEIGHT * 8 - KEY_DISTANCE * 7,
                               KEY_WIDTH, KEY_HEIGHT);
    button_cancel->setFocusPolicy(Qt::NoFocus);
    button_cancel->setProperty("codeValue", QVariant(QString("")));
    button_cancel->setProperty("echoCode", QVariant(QString("")));
    button_cancel->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/close.png")));
    button_cancel->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/close.png")));


    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(saveFocusWidget(QWidget*,QWidget*)));

    connect(button_cancel,SIGNAL(clicked()),this,SLOT(closeKeyboard()));
    connect(button_confirm,SIGNAL(clicked()),this,SLOT(button_confirm_Clicked()));
    connect(button_add,SIGNAL(clicked()),this,SLOT(button_add_Clicked()));
    connect(button_sub,SIGNAL(clicked()),this,SLOT(button_sub_Clicked()));
    connect(button_del,SIGNAL(clicked()),this,SLOT(button_del_Clicked()));


    allButtons = findChildren<QPushButton *>();
    for (int i=0;i<allButtons.count();i++) {
        connect(allButtons.at(i), SIGNAL(clicked()), &signalClick, SLOT(map()));
        signalClick.setMapping(allButtons.at(i), i);
    }

    connect(&signalClick, SIGNAL(mapped(int)),
            this, SLOT(buttonClicked(int)));

    for (int i=0;i<allButtons.count();i++) {
        connect(allButtons.at(i), SIGNAL(pressed()), &signalPressed, SLOT(map()));
        signalPressed.setMapping(allButtons.at(i), i);
    }

    connect(&signalPressed, SIGNAL(mapped(int)),
            this, SLOT(buttonPressed(int)));

    for (int i=0;i<allButtons.count();i++) {
        connect(allButtons.at(i), SIGNAL(released()), &signalReleased, SLOT(map()));
        signalReleased.setMapping(allButtons.at(i), i);
    }

    connect(&signalReleased, SIGNAL(mapped(int)),
            this, SLOT(buttonReleased(int)));

}

#include <edittablewidgetitemdelege.h>

void NumberInputPanel::closeKeyboard()
{
    //lastFocusedWidget->clearFocus();
    //lastFocusedWidget->setFocusPolicy(Qt::NoFocus);
    //lastFocusedWidget->setFocus(Qt::MouseFocusReason);
    isOnRun = false;
     this->close();
    if(inputWidget.handler!= NULL )
        inputWidget.handler->lineEdit_close(inputWidget.name.toLatin1());
}


//! [0]

bool NumberInputPanel::event(QEvent *e)
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


void NumberInputPanel::setText()
{
    QString str;
    QString strValue;
    int i;
    int len;

    if (isOnRun == true)
    {
        return;
    }

    isOnRun = true;

    if(inputWidget.InputWidgetType == LineEdit)
    {
        str = "max  ";
        str.append(QString::number(inputWidget.checkMaxValue));
        Label_max->setText(str);

        str = "min  ";
        str.append(QString::number(inputWidget.checkMinValue));
        Label_min->setText(str);

        str = OLD_DEFAULT_VALUE;
        str.append(((QLineEdit *) inputWidget.watch)->text());

        this->labelValueOld->setText(str);

        //初始化输入框显示的内容#/
//        this->labelValueNew->setText(((QLineEdit *) inputWidget.watch)->text());
        this->labelValueNew->setText("");
        this->labelName->setText(inputWidget.nameDisplay);

    }
    else if(inputWidget.InputWidgetType == ComboBox)
    {
        str = "max  ";
        str.append(QString::number(inputWidget.checkMaxValue));
        Label_max->setText(str);

        str = "min  ";
        str.append(QString::number(inputWidget.checkMinValue));
        Label_min->setText(str);

        str = OLD_DEFAULT_VALUE;
        str.append(((QComboBox *) inputWidget.watch)->currentText());

        this->labelValueOld->setText(str);

        //初始化输入框显示的内容#/
//        this->labelValueNew->setText(((QLineEdit *) inputWidget.watch)->text());
        this->labelValueNew->setText("");
        this->labelName->setText(inputWidget.nameDisplay);

    }

    if ( inputWidget.InputWidgetType == TextEdit)
    {
        str = ( (QTextEdit *) inputWidget.watch)->toPlainText();
        len = ( (QTextEdit *) inputWidget.watch)->toPlainText().size();

        for(i=1,strValue=""; i < len; i++)
        {
            strValue += str.at(i);
        }

        this->labelValueNew->setText(str);
    }
}



//! [2]
void NumberInputPanel::appendWidget(QWidget *watch,
                                    int whichInput,
                                    inputWidgetType InputWidgetType,
                                    KeySetReady *handler,
                                    float checkMinValue,
                                    float checkMaxValue,
                                    float Step,                                    
                                    QString nameDisplay,
                                    QString name,
                                    bool isCheck,
                                    checkWidgetType CheckWidgetType)
{
    InputNumWidget widget;
    widget.watch = watch;
    widget.whichInput = whichInput;
    widget.InputWidgetType = InputWidgetType;
    widget.isCheck = isCheck;
    widget.checkMinValue = checkMinValue;
    widget.checkMaxValue = checkMaxValue;
    widget.CheckWidgetType = CheckWidgetType;
    widget.Step = Step;
    widget.handler = handler;
    widget.name = name;
    widget.nameDisplay = nameDisplay;
    focusWidgetList.append(widget);
}

void NumberInputPanel::removeWidget(QWidget *watch)
{
    int i = 0;
    QList <InputNumWidget> ::iterator beginAddPos;
    QList <InputNumWidget> ::iterator endAddPos;
    beginAddPos = focusWidgetList.begin();
    endAddPos = focusWidgetList.end();

    while(beginAddPos != endAddPos)
    {
        if ( (*beginAddPos).watch == watch)
        {
            focusWidgetList.removeAt(i);
            break;
        }
        i++;
        beginAddPos++;
    }
}

void NumberInputPanel::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    if(newFocus != 0)
    {
        //newFocus->GetParentFrame()->GetActiveFrame();
        //this->getFocusWidget()->objectName();
        //qDebug() << "\r\n Get dialog:"<<newFocus->objectName();
     }
    if(isOnRun == true)
    {
        //if (strcmp(PageCurrent(), "Disinfect") == 0)
        {
          #ifdef HW_ARM
          closeKeyboard();
          #endif
          return;
         }
    }
    if (newFocus != 0
        && !this->isAncestorOf(newFocus)
        && (lastFocusedWidget != newFocus) )
    {
        QList <InputNumWidget> ::iterator beginAddPos;
        QList <InputNumWidget> ::iterator endAddPos;
        beginAddPos = focusWidgetList.begin();
        endAddPos = focusWidgetList.end();

        while(beginAddPos != endAddPos)
        {
            if ( (*beginAddPos).watch == newFocus)
            {
                if((*beginAddPos).handler != NULL && (*beginAddPos).handler->getParentName().compare("SceneSetupUf") == 0) {
                    //在uF下需要实时修改最大和最小值，和发送键盘打开信号
                   // qDebug() << "into key set";
                    ((SceneSetupUf *)(*beginAddPos).handler)->lineEdit_start((*beginAddPos).name.toLatin1(), &(*beginAddPos).checkMaxValue
                                                                          , &(*beginAddPos).checkMinValue);
                }

                if((*beginAddPos).handler != NULL && (*beginAddPos).handler->getParentName().compare("SceneSetupSubspump") == 0) {
                    //在uF下需要实时修改最大和最小值，和发送键盘打开信号
                   // qDebug() << "into key set";
                    ((SceneSetupSubspump *)(*beginAddPos).handler)->lineEdit_start((*beginAddPos).name.toLatin1(), &(*beginAddPos).checkMaxValue
                                                                          , &(*beginAddPos).checkMinValue);
                }

                if((*beginAddPos).handler != NULL && (*beginAddPos).handler->getParentName().compare("SceneSetupHemopump") == 0) {
                    //在uF下需要实时修改最大和最小值，和发送键盘打开信号
                   // qDebug() << "into key set";
                    ((SceneSetupHemopump *)(*beginAddPos).handler)->lineEdit_start((*beginAddPos).name.toLatin1(), &(*beginAddPos).checkMaxValue
                                                                          , &(*beginAddPos).checkMinValue);
                }
                if((*beginAddPos).handler != NULL && (*beginAddPos).handler->getParentName().compare("SceneSetupCureTime") == 0) {
                    //在uF下需要实时修改最大和最小值，和发送键盘打开信号
                   // qDebug() << "into key set";
                    ((SceneSetupCureTime *)(*beginAddPos).handler)->lineEdit_start((*beginAddPos).name.toLatin1(), &(*beginAddPos).checkMaxValue
                                                                          , &(*beginAddPos).checkMinValue);
                }
                if((*beginAddPos).handler != NULL && (*beginAddPos).handler->getParentName().compare("SceneSetupIsoUF") == 0) {
                    //在uF下需要实时修改最大和最小值，和发送键盘打开信号
                   // qDebug() << "into key set";
                    ((SceneSetupIsoUF *)(*beginAddPos).handler)->lineEdit_start((*beginAddPos).name.toLatin1(), &(*beginAddPos).checkMaxValue
                                                                          , &(*beginAddPos).checkMinValue);
                }

                lastFocusedWidget = newFocus;
                inputWidget.watch = (*beginAddPos).watch;
                inputWidget.whichInput =(*beginAddPos). whichInput;
                inputWidget.InputWidgetType = (*beginAddPos).InputWidgetType;
                inputWidget.isCheck = (*beginAddPos).isCheck;
                inputWidget.checkMinValue = (*beginAddPos).checkMinValue;
                inputWidget.checkMaxValue = (*beginAddPos).checkMaxValue;
                inputWidget.CheckWidgetType = (*beginAddPos).CheckWidgetType;
                inputWidget.Step = (*beginAddPos).Step;
                inputWidget.handler = (*beginAddPos).handler;
                inputWidget.name = (*beginAddPos).name;
                inputWidget.nameDisplay = (*beginAddPos).nameDisplay;


                if(inputWidget.checkMaxValue == 0) //没有设置
                    inputWidget.checkMaxValue = 65535;

                setText();

                ins.lineEditNotInsert(this->labelValueNew);

                if((*beginAddPos).whichInput==NumInputMethod)
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

void NumberInputPanel::button_confirm_Clicked()
{
    float value = 0;
    QString str = "";
    str = labelValueNew->text();

    if(str.isEmpty())
    {
        closeKeyboard();
        return;
    }

    value = str.toFloat();

    if (value > inputWidget.checkMaxValue)
        value = inputWidget.checkMaxValue;

    else if(value < inputWidget.checkMinValue)
        value = inputWidget.checkMinValue;

    QString strValue = OLD_DEFAULT_VALUE;
    strValue.append(QString::number(value));

    QString front=OLD_DEFAULT_VALUE;
    QString old=labelValueOld->text().right(labelValueOld->text().length()-front.length());

    labelValueOld->setText(strValue);
    labelValueNew->setText(QString::number(value));
    if(inputWidget.InputWidgetType != ComboBox)
        ((QLineEdit *) inputWidget.watch)->setText(QString::number(value));
    else if(inputWidget.InputWidgetType == ComboBox)
        ((QComboBox *) inputWidget.watch)->setEditText(QString::number(value));

    if(inputWidget.handler!= NULL)
    {
        inputWidget.handler->lineEdit_data_set(inputWidget.name.toLatin1(), QString::number(value).toLatin1());
        logInfo info;
        info.opration.clear();
        if(inputWidget.InputWidgetType != ComboBox)
            info.objName=((QLineEdit *) inputWidget.watch)->objectName();
        else if(inputWidget.InputWidgetType == ComboBox)
            info.objName=((QComboBox *) inputWidget.watch)->objectName();

        info.strOld=old;
        info.type=DE_LOGINFO_PAGEDITOR;
        info.strNew=QString::number(value);
        if(!info.objName.isEmpty())
        emit RunLog(info);
    }

    closeKeyboard();
}

//! [3]

void NumberInputPanel::buttonClicked(int btn)
{
//    audio->beep();

    QString strKeyId;
    strKeyId = qvariant_cast<QString>(allButtons.at(btn)->property("codeValue"));

    bool isOk;
    int keyId = strKeyId.toInt(&isOk, 16);

    QString ch = qvariant_cast<QString>(allButtons.at(btn)->property("echoCode"));

    int involvedKeys = 1;
    Qt::KeyboardModifiers Modifier = Qt::NoModifier;

    //lastFocusedWidget->activateWindow();

    if (!keyId)
        return;

    if (insertMode == false)
    {
        emit characterGenerated(Qt::Key_Delete,Qt::NoModifier,"",1);
    }

    emit characterGenerated(keyId,Modifier,ch,involvedKeys);
}


void NumberInputPanel::button_del_Clicked() {

    labelValueNew->setText("");
}

//! [3]

void NumberInputPanel::button_add_Clicked()
{
    float value = 0;

    value = labelValueNew->text().toFloat();
    value += inputWidget.Step;

    if (value > inputWidget.checkMaxValue)
    {
        value = inputWidget.checkMaxValue;
    }

    labelValueNew->setText(QString::number(value));

}

void NumberInputPanel::button_sub_Clicked()
{
    float value = 0;

    value = labelValueNew->text().toFloat();
    value -= inputWidget.Step;

    if (value < inputWidget.checkMinValue)
    {
        value = inputWidget.checkMinValue;
    }

    labelValueNew->setText(QString::number(value));
}

void NumberInputPanel::buttonPressed(int btn)
{
    QString str = qvariant_cast<QString>(allButtons.at(btn)->property("Pressed"));

    if (str.isEmpty())
        return;

    allButtons.at(btn)->setIcon(QIcon(str));
}

void NumberInputPanel::buttonReleased(int btn)
{
    QString str = qvariant_cast<QString>(allButtons.at(btn)->property("Released"));

    if (str.isEmpty())
        return;

    allButtons.at(btn)->setIcon(QIcon(str));
}

QWidget * NumberInputPanel::getFocusWidget()
{
    return lastFocusedWidget;
}

QWidget * NumberInputPanel::getLineEdit()
{
    return labelValueNew;
}

void NumberInputPanel::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(":/resource/images/virtualkeyboard/mini-keyboard/keyboard.png"));
}

void NumberInputPanel::closeEvent(QCloseEvent *event)
{
    event = event;
    if(inputWidget.InputWidgetType != ComboBox)
    {
        connect(this,SIGNAL(editFinished()),((QLineEdit *)inputWidget.watch),SIGNAL(editingFinished()));
        emit editFinished();
        disconnect(this,0,((QLineEdit *)inputWidget.watch),0);
    }
}

NumberInputPanel::~NumberInputPanel()
{
    focusWidgetList.clear();
}
