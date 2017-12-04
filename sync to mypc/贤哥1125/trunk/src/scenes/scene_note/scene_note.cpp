#include "scene_note.h"
#include "config_ui.h"
#include "scene_setup_layout.h"
#include "window_main.h"
#include "global_var.h"
#include "QString"

Window_Note::Window_Note(QWidget *parent, const QString title, const QString brushPath ) :
    QDialog(parent) ,
    widget(parent) ,
    labeltitle(&widget),
    labelInfo(&widget),
    btnYes(&widget),
    btnNo(&widget),
    btnClose(&widget)
{
    setUi(title, brushPath);
    connect(this, SIGNAL(showNoteSignal(bool)), this, SLOT(setShown(bool)));
    connect(&btnClose, SIGNAL(pressed()), this, SLOT(btnCloseDialog()));
    connect(&btnYes, SIGNAL(pressed()), this, SLOT(btnYesClick()));
    connect(&btnNo, SIGNAL(pressed()), this, SLOT(btnNoClick()));

    btnClose.setObjectName("Notice_btnClose");
    btnYes.setObjectName("Notice_btnYes");
    btnNo.setObjectName("Notice_btnNo");
}

inline void Window_Note::setUi(const QString title, const QString brushPath)
{
    Q_UNUSED(brushPath);
    //Q_UNUSED(title);
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

#if 1
    QString DialogStyle = "QLabel {background-color:#F0F0F0;"//192131
            "border: 5px groove gray; border-radius: 5px; padding: 8px 8px;}";

    //setStyleSheet(DialogStyle);
    setStyleSheet("background-color: #A2A2A2");

    btnUpStyle.append("border-radius: 6px;"
                      "background: #DCDDE0;"
                      "padding: 4px 4px;");

    btnClickStyle.append("border-radius: 6px;"
                      "background: #192131;"
                      "padding: 4px 4px;");

    QString infoStyle;
    if(title.compare("Cure Mode") == 0 ||title.compare("CureLeave") == 0)
    {
        infoStyle = "QLabel {background-color:#F0F0F0;"//192131
                    "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

        setGeometry(QRect(335, 305, 360, 180));
        labeltitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
        infoStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";}");
        btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
        btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

    }
    if(title.compare("Return to ready") == 0)
    {
        infoStyle = "QLabel {background-color:#F0F0F0;"//192131
                    "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

        setGeometry(QRect(335, 305, 360, 180));
        labeltitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
        infoStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";}");
        btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
        btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

    }

    if(title.compare("WorkModel") == 0 )
    {
        infoStyle = "QLabel {background-color:#F0F0F0;"//192131
                    "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

        setGeometry(QRect(335, 305, 360, 180));
        labeltitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
        infoStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";}");
        btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
        btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

    }

    if(title.compare("CureFinish") == 0 )
    {
        infoStyle = "QLabel {background-color:#F0F0F0;"//192131
                    "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

        setGeometry(QRect(335, 305, 360, 180));
        labeltitle.setStyleSheet(CONFIG_COLOR_RED_CHARS);
        infoStyle.append(CONFIG_COLOR_RED_CHARS).append(";}");
        btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
        btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

    }

    this -> title = title;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(&widget);
    //widget.setObjectName("NoteDialog");
    this->setObjectName("NoteDialogParent");
    setFocusPolicy(Qt::StrongFocus);
    widget.setGeometry(QRect(-20, -20, 160, 160));

    labeltitle.setGeometry(QRect(120, -2, 100, 20));
    labeltitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labeltitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    labelInfo.setGeometry(QRect(5, 20, 330, 100));

    labelInfo.setAlignment(Qt::AlignLeft);
    labelInfo.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelInfo.setStyleSheet(infoStyle);
    labelInfo.setWordWrap(true);                //自动换行

    btnNo.setGeometry(QRect(150, 130, 60, 28));
    BUTTON_SIMPLE_STYLE(btnNo);
    btnNo.setStyleSheet(btnUpStyle);

    btnYes.setGeometry(QRect(50, 130, 60, 28));
    BUTTON_SIMPLE_STYLE(btnYes);
    btnYes.setStyleSheet(btnUpStyle);

    btnClose.setGeometry(QRect(250, 130, 60, 28));
    BUTTON_SIMPLE_STYLE(btnClose);
    btnClose.setStyleSheet(btnUpStyle);

    CustomSetText();
    setLayout(layout);

#endif

    //圆角矩形窗口#/
    QBitmap bmp(this->size());
    bmp.fill(this,0,0);
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);//Anti aliasing
    p.drawRoundedRect(bmp.rect(), 10, 10, Qt::AbsoluteSize); //Four corners are rounded corners
    setMask(bmp);
}

 void Window_Note::showNoteText()
{
     if(title.compare("Cure Mode") == 0 )//启动治疗是弹出当前的治疗模式#/
     {
      QString text ;
      PacketWorkMode workModeTMP;
      dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
      text=tr("Do you want to start the \"");
      switch(workModeTMP.treatmode.current){
      case HD_SNSP:
          text.append(tr("HD SNSP"));
          break;
      case HD_double:
          text.append(tr("HD double"));
          break;
      case HD_SNDP:
          text.append(tr("HD SNDP"));
          break;

      case HDF_pre_dilute:
          text.append(tr("HDF pre"));
          break;
      case HDF_post_dilute:
          text.append(tr("HDF post"));
          break;

      case HF_pre_dilute:
          text.append(tr("HF pre"));
          break;
      case HF_post_dilute:
          text.append(tr("HF post"));
          break;

      case ISOUF_SNSP:
          text.append(tr("ISOUF SNSP"));
          break;
      case ISOUF_DNDP:
          text.append(tr("ISOUF DNSP"));
          break;
      case ISOUF_SNDP:
          text.append(tr("ISOUF SNDP"));
          break;

      case PE:
          text.append(tr("PE"));
          break;
      default:
          text.append(tr("HD"));
          break;
      }
      text.append(tr("\" Cure Mode?"));
      labelInfo.setText(text);
     }

     if(title.compare("Return to ready") == 0 )
     {
      QString text ;
      text=tr("Return to ready status,keep or reset parameters? Yes-keep,No-reset");
      labelInfo.setText(text);
     }

    if(title.compare("WorkModel") == 0 )
    {
     QString text ;
     text=tr("Do you want to change the work mode ?");
     labelInfo.setText(text);
    }
    if(title.compare("CureLeave") == 0 )
    {
     QString text ;
     text=tr("Do you want to leave cure ?");
     labelInfo.setText(text);
    }
    if(title.compare("CureFinish") == 0 )
    {
     QString textinfo ;
     textinfo=tr("Cure Finished ? Press'No' to continut ,Press'Yes' to return blood .");
     labelInfo.setText(textinfo);//trUtf8(text.toLatin1())
    }

    //labelInfo.setText(text);//trUtf8(text.toLatin1())
}

 void Window_Note::CustomSetText()
 {
     btnClose.setText(tr("Cancel"));
     btnNo.setText(tr("No"));
     btnYes.setText(tr("Yes"));
     labeltitle.setText(tr("Confirm"));
     showNoteText();
 }

 void Window_Note::showNote(bool isShow)
 {
     emit showNoteSignal(isShow);
     this->raise();

 }

bool Window_Note::isShow()
 {
     return this->isVisible();
 }

 Window_Note::~Window_Note()
 {
     this->close();
 }

 void Window_Note::btnCloseDialog()
 {
   showNote(false);
   emit NoteSelSignal(3);
 }

 void Window_Note::btnYesClick()
 {
   showNote(false);
   emit NoteSelSignal(1);
 }

 void Window_Note::btnNoClick()
 {
   showNote(false);
   emit NoteSelSignal(2);
 }



//HJX实现的是否确认提示框#/

 Window_YESNO::Window_YESNO(QWidget *parent) :
     QDialog(parent) ,
     widget(parent) ,
     labeltitle(&widget),
     labelInfo(&widget),
     btnYes(&widget),
     btnNo(&widget)
 {
     setUi();
     connect(&btnYes, SIGNAL(pressed()), this, SLOT(btnYesClick()));
     connect(&btnNo, SIGNAL(pressed()), this, SLOT(btnNoClick()));
     btnYes.setObjectName("Notice_btnYes");
     btnNo.setObjectName("Notice_btnNo");
 }

void Window_YESNO::setUi()
 {
     setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

     setStyleSheet("background-color: #A2A2A2");

     btnUpStyle.append("border-radius: 6px;"
                       "background: #DCDDE0;"
                       "padding: 4px 4px;");

     btnClickStyle.append("border-radius: 6px;"
                          "background: #192131;"
                          "padding: 4px 4px;");

     QString infoStyle;

     infoStyle = "QLabel {background-color:#F0F0F0;"//192131
             "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

     setGeometry(QRect(335, 305, 360, 180));
     labeltitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
     infoStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";}");
     btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
     btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

     QHBoxLayout *layout = new QHBoxLayout;
     layout->addWidget(&widget);
     this->setObjectName("qWindow_YESNO");
     setFocusPolicy(Qt::StrongFocus);
     widget.setGeometry(QRect(-20, -20, 160, 160));

     labeltitle.setGeometry(QRect(100, -2, 200, 20));
     labeltitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
     labeltitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                              CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

     labelInfo.setGeometry(QRect(5, 20, 330, 100));

     labelInfo.setAlignment(Qt::AlignLeft);
     labelInfo.setFont(QFont(QString(CONFIG_FONT_STYLE),
                             CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
     labelInfo.setStyleSheet(infoStyle);
     labelInfo.setWordWrap(true);                //自动换行#/

     btnNo.setGeometry(QRect(180, 130, 60, 28));
     BUTTON_SIMPLE_STYLE(btnNo);
     btnNo.setStyleSheet(btnUpStyle);

     btnYes.setGeometry(QRect(80, 130, 60, 28));
     BUTTON_SIMPLE_STYLE(btnYes);
     btnYes.setStyleSheet(btnUpStyle);

     CustomSetText();
     setLayout(layout);

     //圆角矩形窗口#/
     QBitmap bmp(this->size());
     bmp.fill(this,0,0);
     QPainter p(&bmp);
     p.setPen(Qt::NoPen);
     p.setBrush(Qt::black);
     p.setRenderHint(QPainter::Antialiasing);//Anti aliasing
     p.drawRoundedRect(bmp.rect(), 10, 10, Qt::AbsoluteSize); //Four corners are rounded corners
     setMask(bmp);
 }

 void Window_YESNO::showMessage(QString &title,QString &info)
 {
     labeltitle.setText(title);
     labelInfo.setText(info);
     this->showNote(true);
 }

 void Window_YESNO::CustomSetText()
 {
     btnNo.setText(tr("No"));
     btnYes.setText(tr("Yes"));
 }

 void Window_YESNO::showNote(bool isShow)
 {
     setVisible(isShow);
     emit showNoteSignal(isShow);
     if(isShow==true)
        this->raise();
 }

 bool Window_YESNO::isShow()
 {
     return this->isVisible();
 }

 Window_YESNO::~Window_YESNO()
 {
     this->close();
 }

 void Window_YESNO::btnYesClick()
 {
     showNote(false);
     emit NoteSelSignal(true);
 }

 void Window_YESNO::btnNoClick()
 {
     showNote(false);
     emit NoteSelSignal(false);
 }

//HJX实现的确认提示框#/
 Window_YES::Window_YES(QWidget *parent) :
     QDialog(parent) ,
     widget(parent) ,
     labeltitle(&widget),
     labelInfo(&widget),
     btnYes(&widget)
 {
     setUi();
     connect(&btnYes, SIGNAL(pressed()), this, SLOT(btnYesClick()));
 }

 void Window_YES::setUi()
 {
     setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

     setStyleSheet("background-color: #A2A2A2");

     btnUpStyle.append("border-radius: 6px;"
                       "background: #DCDDE0;"
                       "padding: 4px 4px;");

     btnClickStyle.append("border-radius: 6px;"
                          "background: #192131;"
                          "padding: 4px 4px;");

     QString infoStyle;

     infoStyle = "QLabel {background-color:#F0F0F0;"//192131
             "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

     setGeometry(QRect(335, 305, 360, 180));
     labeltitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
     infoStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";}");
     btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
     btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

     QHBoxLayout *layout = new QHBoxLayout;
     layout->addWidget(&widget);
     this->setObjectName("qWindow_YESNO");
     setFocusPolicy(Qt::StrongFocus);
     widget.setGeometry(QRect(-20, -20, 160, 160));

     labeltitle.setGeometry(QRect(100, -2, 200, 20));
     labeltitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
     labeltitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                              CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

     labelInfo.setGeometry(QRect(5, 20, 330, 100));

     labelInfo.setAlignment(Qt::AlignLeft);
     labelInfo.setFont(QFont(QString(CONFIG_FONT_STYLE),
                             CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
     labelInfo.setStyleSheet(infoStyle);
     labelInfo.setWordWrap(true);                //自动换行#/

     btnYes.setGeometry(QRect(120, 130, 80, 28));
     BUTTON_SIMPLE_STYLE(btnYes);
     btnYes.setStyleSheet(btnUpStyle);

     CustomSetText();
     setLayout(layout);

     //圆角矩形窗口#/
     QBitmap bmp(this->size());
     bmp.fill(this,0,0);
     QPainter p(&bmp);
     p.setPen(Qt::NoPen);
     p.setBrush(Qt::black);
     p.setRenderHint(QPainter::Antialiasing);//Anti aliasing
     p.drawRoundedRect(bmp.rect(), 10, 10, Qt::AbsoluteSize); //Four corners are rounded corners
     setMask(bmp);
 }

 void Window_YES::showMessage(QString &title,QString &info)
 {
     labeltitle.setText(title);
     labelInfo.setText(info);
     this->showNote(true);
 }

 void Window_YES::showMessageNoYes(QString &title,QString &info)     //显示对话框，并且不显示确认按钮#/
 {
     labeltitle.setText(title);
     labelInfo.setText(info);
     this->showNote(true);
     btnYes.setVisible(false);
 }

 void Window_YES::CustomSetText()
 {
     btnYes.setText(tr("Confirmation"));
 }

 void Window_YES::showNote(bool isShow)
 {
     setVisible(isShow);
     emit showNoteSignal(isShow);
     if(isShow==true)
        this->raise();
 }

 bool Window_YES::isShow()
 {
     return this->isVisible();
 }

 Window_YES::~Window_YES()
 {
     this->close();
 }

 void Window_YES::btnYesClick()
 {
     showNote(false);
     emit NoteSelSignal(true);
 }


//--------------------------------------------------------------


Window_Confirm::Window_Confirm(QWidget *parent, const QString title, const QString brushPath ) :
    QDialog(parent) ,
    widget(parent) ,
    labeltitle(&widget),
    labelInfo(&widget),
    btnYes(&widget)
{
    setUi(title, brushPath);
    connect(this, SIGNAL(showNoteSignal(bool)), this, SLOT(setShown(bool)));
    connect(&btnYes, SIGNAL(pressed()), this, SLOT(btnYesClick()));


    btnYes.setObjectName("Notice_btnYes");
}

inline void Window_Confirm::setUi(const QString title, const QString brushPath)
{
    Q_UNUSED(brushPath);
    //Q_UNUSED(title);
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

#if 1
    QString DialogStyle = "QLabel {background-color:#F0F0F0;"//192131
            "border: 5px groove gray; border-radius: 5px; padding: 8px 8px;}";

    //setStyleSheet(DialogStyle);
    setStyleSheet("background-color: #A2A2A2");

    btnUpStyle.append("border-radius: 6px;"
                      "background: #DCDDE0;"
                      "padding: 4px 4px;");

    btnClickStyle.append("border-radius: 6px;"
                      "background: #192131;"
                      "padding: 4px 4px;");

    QString infoStyle;
   // if(title.compare("Confirm AP") == 0 )
    {
        infoStyle = "QLabel {background-color:#F0F0F0;"//192131
                    "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

        setGeometry(QRect(335, 305, 360, 180));
        labeltitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
        infoStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";}");
        btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
        btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

    }

    this -> title = title;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(&widget);
    //widget.setObjectName("NoteDialog");
    this->setObjectName("NoteDialogParent");
    setFocusPolicy(Qt::StrongFocus);
    widget.setGeometry(QRect(-20, -20, 160, 160));

    labeltitle.setGeometry(QRect(120, -2, 100, 20));
    labeltitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labeltitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    labelInfo.setGeometry(QRect(5, 20, 330, 100));

    labelInfo.setAlignment(Qt::AlignLeft);
    labelInfo.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelInfo.setStyleSheet(infoStyle);
    labelInfo.setWordWrap(true);                //自动换行#/


    btnYes.setGeometry(QRect(140, 130, 60, 28));
    BUTTON_SIMPLE_STYLE(btnYes);
    btnYes.setStyleSheet(btnUpStyle);

    CustomSetText();
    setLayout(layout);

#endif

    //圆角矩形窗口#/
    QBitmap bmp(this->size());
    bmp.fill(this,0,0);
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);//Anti aliasing
    p.drawRoundedRect(bmp.rect(), 10, 10, Qt::AbsoluteSize); //Four corners are rounded corners
    setMask(bmp);
}

 void Window_Confirm::showNoteText()
{
    if(title.compare("Confirm AP") == 0 )
    {
     QString textinfo ;
     textinfo=tr("Press'Yes' to auto scale AP!");
     labelInfo.setText(textinfo);//trUtf8(text.toLatin1())
     //text.append("\r\n or return blood ? ");
    }
    if(title.compare("Confirm VP") == 0 )
    {
     QString textinfo ;
     textinfo=tr("Press'Yes' to auto scale VP!");
     labelInfo.setText(textinfo);//trUtf8(text.toLatin1())
     //text.append("\r\n or return blood ? ");
    }
    if(title.compare("Confirm TMP") == 0 )
    {
     QString textinfo ;
     textinfo=tr("Press'Yes' to auto scale TMP!");
     labelInfo.setText(textinfo);//trUtf8(text.toLatin1())
     //text.append("\r\n or return blood ? ");
    }

    //labelInfo.setText(text);//trUtf8(text.toLatin1())
}

 void Window_Confirm::CustomSetText()
 {
     btnYes.setText(tr("Yes"));
     labeltitle.setText(tr("Confirm"));
     showNoteText();
 }

 void Window_Confirm::showNote(bool isShow)
 {
     emit showNoteSignal(isShow);
     this->raise();
 }

bool Window_Confirm::isShow()
 {
     return this->isVisible();
 }

 Window_Confirm::~Window_Confirm()
 {
     this->close();
 }

 void Window_Confirm::btnYesClick()
 {
   showNote(false);
   emit NoteSelSignal(true);
 }

 ////////////////////////////////////////////////////////////////////////////
 Window_Passwd::Window_Passwd(QWidget *parent, const QString title, const QString brushPath ) :
     QDialog(parent) ,
     widget(parent) ,
     labeltitle(&widget),
     labelEorror(&widget),
     EditPasswd(&widget),
     btnYes(&widget),
     btnClose(&widget)
 {
     setUi(title, brushPath);
     connect(this, SIGNAL(showNoteSignal(bool)), this, SLOT(setShown(bool)));
     connect(&btnClose, SIGNAL(pressed()), this, SLOT(btnCloseDialog()));
     connect(&btnYes, SIGNAL(pressed()), this, SLOT(btnYesClick()));
    // connect(&btnNo, SIGNAL(pressed()), this, SLOT(btnNoClick()));

     btnClose.setObjectName("PasswdDialog_btnClose");
     btnYes.setObjectName("PasswdDialog_btnYes");
     connect(&EditPasswd, SIGNAL(LineEditClicked()), this, SLOT(PasswdInputClicked()));

 }
void Window_Passwd::PasswdInputClicked()
{
    if(NumberKeyboard==NULL){
        passWordEditString.clear();
        EditPasswd.setText("");
        NumberKeyboard= new Mini_Keyboard(&EditPasswd,&passWordEditString);
        }
    NumberKeyboard->setShown(true);
    NumberKeyboard->raise();
}

//重写mousePressEvent事件,检测事件类型是不是点击了鼠标左键#/


 inline void Window_Passwd::setUi(const QString title, const QString brushPath)
 {
     Q_UNUSED(brushPath);
     setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

 #if 1
     QString DialogStyle = "QLabel {background-color:#F0F0F0;"
             "border: 5px groove gray; border-radius: 5px; padding: 8px 8px;}";

     setStyleSheet("background-color: #A2A2A2");

     btnUpStyle.append("border-radius: 6px;"
                       "background: #DCDDE0;"
                       "padding: 4px 4px;");

     btnClickStyle.append("border-radius: 6px;"
                       "background: #192131;"
                       "padding: 4px 4px;");

     QString infoStyle;

     infoStyle = "QLabel {background-color:#F0F0F0;"
                     "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

     setGeometry(QRect(335, 305, 360, 180));
     labeltitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
     infoStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";}");
     btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
     btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

     this -> title = title;

     QHBoxLayout *layout = new QHBoxLayout;
     layout->addWidget(&widget);
     this->setObjectName("PasswdDialogParent");
     setFocusPolicy(Qt::StrongFocus);
     widget.setGeometry(QRect(-20, -20, 160, 160));

     labeltitle.setGeometry(QRect(0, -2, 240, 20));
     labeltitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
     labeltitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                 CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

     EditPasswd.setGeometry(QRect(110, 40, 120, 25));

     labelEorror.setGeometry(QRect(90, 80, 240, 20));
     labelEorror.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
     labelEorror.setFont(QFont(QString(CONFIG_FONT_STYLE),
                 CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

     btnYes.setGeometry(QRect(70, 130, 80, 28));
     BUTTON_SIMPLE_STYLE(btnYes);
     btnYes.setStyleSheet(btnUpStyle);

     btnClose.setGeometry(QRect(190, 130, 80, 28));
     BUTTON_SIMPLE_STYLE(btnClose);
     btnClose.setStyleSheet(btnUpStyle);

     CustomSetText();
     setLayout(layout);

 #endif

     //圆角矩形窗口#/
     QBitmap bmp(this->size());
     bmp.fill(this,0,0);
     QPainter p(&bmp);
     p.setPen(Qt::NoPen);
     p.setBrush(Qt::black);
     p.setRenderHint(QPainter::Antialiasing);//Anti aliasing
     p.drawRoundedRect(bmp.rect(), 10, 10, Qt::AbsoluteSize); //Four corners are rounded corners
     setMask(bmp);
 }

  void Window_Passwd::showNoteText()
 {


 }

  void Window_Passwd::CustomSetText()
  {
      btnClose.setText(tr("Cancel"));
      btnYes.setText(tr("Yes"));
      labeltitle.setText(tr("Please Input Password:"));
      labelEorror.setText(tr(""));
      showNoteText();
  }

  void Window_Passwd::showNote(bool isShow)
  {
      emit showNoteSignal(isShow);
      this->raise();

  }

 bool Window_Passwd::isShow()
  {
      return this->isVisible();
  }

  Window_Passwd::~Window_Passwd()
  {
      this->close();
  }

  void Window_Passwd::btnCloseDialog()
  {
    labelEorror.setText("");
    showNote(false);
    emit NoteSelSignal(false);
  }

  void Window_Passwd::btnYesClick()
  {

    //showNote(false);
    //emit NoteSelSignal(true);

    QFile f(PASSWORD_CONFIG_FILE);
    if(!f.open(QIODevice::ReadOnly ))qDebug("Open Passwd config file failed.") ;
    else{
        QString str(f.readAll());
        f.close();
 //       passWordEditString.append('\n');
        if(passWordEditString == str){
            showNote(false);
            emit NoteSelSignal(true);
            if(alarmmodule!=NULL)
                alarmmodule->Set_WorkState(DE_HIDDED_FLAG);
            enginerDialog->show();
            }
        else{
             qDebug()<<"passWord error !";
             labelEorror.setText(tr("Password Error !"));
            }
       }
    EditPasswd.setText("");
    passWordEditString.clear();
  }


/*******************min keyboard start********************/
Mini_Keyboard::Mini_Keyboard(QLineEdit *LineEdit ,  QString *paramete )
{
    //QPalette palette;
    //palette.setColor(QPalette::Background, QColor(192,253,123));
    outEdit = LineEdit;
    outString = paramete;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool |Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setObjectName(QString::fromUtf8("Number_Form"));
    //this->resize(243, 356);
    this->setWindowOpacity(1.0);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFocusPolicy(Qt::NoFocus);
    //this->setPalette(palette);
    setGeometry(QRect(640, 200, 360, 180));

    pix.load(":/resource/images/virtualkeyboard/mini-keyboard/mini-keyboard.png",0,
             Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    resize(pix.size());
    setMask(QBitmap(pix.mask()));




    button_0 = new WidgetButton(this);
    button_0->setStyleNormal(":/resource/images/virtualkeyboard/mini-keyboard/number-0.png",
            QRect(KEY_LEFT, MIN_KEY_BOTTOM, KEY_WIDTH, KEY_HEIGHT));
    button_0->setObjectName(QString::fromUtf8("button_0"));
    button_0->setFocusPolicy(Qt::NoFocus);
    button_0->setProperty("codeValue", QVariant(QString("0x30")));
    button_0->setProperty("echoCode", QVariant(QString("0")));
    button_0->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-0.png")));
    button_0->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-0.png")));
    //button_0->setText(tr("0"));
    button_0->setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    button_0->setStyleSheet(CONFIG_COLOR_LABEL);

    button_confirm = new QPushButton(this);
    button_confirm->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/empty.png"));
    button_confirm->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_confirm->setObjectName(QString::fromUtf8("button_confirm"));
    button_confirm->setGeometry(KEY_LEFT + 101 + KEY_DISTANCE * 1, MIN_KEY_BOTTOM, KEY_WIDTH, KEY_HEIGHT);
    button_confirm->setFocusPolicy(Qt::NoFocus);
    button_confirm->setProperty("codeValue", QVariant(QString("")));
    button_confirm->setProperty("echoCode", QVariant(QString("")));
    button_confirm->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/empty.png")));
    button_confirm->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/empty.png")));
    button_confirm->setText("Input");
    button_confirm->setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                                         CFG_FONT_SIZE_FOURTH, CONFIG_FONT_NORMAL));


    button_1 = new QPushButton(this);
    button_1->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-1.png"));
    button_1->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_1->setObjectName(QString::fromUtf8("button_1"));
    button_1->setGeometry(KEY_LEFT, MIN_KEY_BOTTOM - KEY_HEIGHT - KEY_DISTANCE,
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
                          MIN_KEY_BOTTOM - KEY_HEIGHT - KEY_DISTANCE, KEY_WIDTH, KEY_HEIGHT);
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
                          MIN_KEY_BOTTOM - KEY_HEIGHT - KEY_DISTANCE, KEY_WIDTH, KEY_HEIGHT);
    button_3->setFocusPolicy(Qt::NoFocus);
    button_3->setProperty("codeValue", QVariant(QString("0x33")));
    button_3->setProperty("echoCode", QVariant(QString("3")));
    button_3->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-3.png")));
    button_3->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-3.png")));




    button_4 = new QPushButton(this);
    button_4->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-4.png"));
    button_4->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_4->setObjectName(QString::fromUtf8("button_4"));
    button_4->setGeometry(KEY_LEFT, MIN_KEY_BOTTOM - KEY_HEIGHT * 2 - KEY_DISTANCE * 2,
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
                          MIN_KEY_BOTTOM - KEY_HEIGHT * 2 - KEY_DISTANCE * 2, KEY_WIDTH, KEY_HEIGHT);
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
                          MIN_KEY_BOTTOM - KEY_HEIGHT * 2 - KEY_DISTANCE * 2, KEY_WIDTH, KEY_HEIGHT);
    button_6->setFocusPolicy(Qt::NoFocus);
    button_6->setProperty("codeValue", QVariant(QString("0x36")));
    button_6->setProperty("echoCode", QVariant(QString("6")));
    button_6->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-6.png")));
    button_6->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/number-6.png")));


    button_7 = new QPushButton(this);
    button_7->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/number-7.png"));
    button_7->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_7->setObjectName(QString::fromUtf8("button_7"));
    button_7->setGeometry(KEY_LEFT ,
                          MIN_KEY_BOTTOM - KEY_HEIGHT * 3 - KEY_DISTANCE * 3, KEY_WIDTH, KEY_HEIGHT);
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
                          MIN_KEY_BOTTOM - KEY_HEIGHT * 3 - KEY_DISTANCE * 3, KEY_WIDTH, KEY_HEIGHT);
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
                           MIN_KEY_BOTTOM - KEY_HEIGHT * 3 - KEY_DISTANCE * 3, KEY_WIDTH ,KEY_HEIGHT);
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
    button_del->setGeometry(KEY_LEFT + KEY_DISTANCE * 1 + KEY_WIDTH * 1,
                            MIN_KEY_BOTTOM - KEY_HEIGHT * 0 - KEY_DISTANCE * 0, KEY_WIDTH, KEY_HEIGHT);
    button_del->setFocusPolicy(Qt::NoFocus);
    button_del->setProperty("codeValue", QVariant(QString("")));
    button_del->setProperty("echoCode", QVariant(QString("")));
    button_del->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/clr.png")));
    button_del->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/clr.png")));

    button_cancel = new QPushButton(this);
    button_cancel->setIcon(QIcon(":/resource/images/virtualkeyboard/mini-keyboard/close.png"));
    button_cancel->setIconSize(QSize(KEY_WIDTH, KEY_HEIGHT));
    button_cancel->setObjectName(QString::fromUtf8("button_cancel"));
    button_cancel->setGeometry(KEY_LEFT + 101 + KEY_DISTANCE   , MIN_KEY_BOTTOM - KEY_HEIGHT * 4 - KEY_DISTANCE * 4,
                               KEY_WIDTH, KEY_HEIGHT);
    button_cancel->setFocusPolicy(Qt::NoFocus);
    button_cancel->setProperty("codeValue", QVariant(QString("")));
    button_cancel->setProperty("echoCode", QVariant(QString("")));
    button_cancel->setProperty("Pressed",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/close.png")));
    button_cancel->setProperty("Released",
                            QVariant(QString(":/resource/images/virtualkeyboard/mini-keyboard/close.png")));


    //connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
           // this, SLOT(saveFocusWidget(QWidget*,QWidget*)));

    connect(button_cancel,SIGNAL(clicked()),this,SLOT(closeKeyboard()));
    connect(button_confirm,SIGNAL(clicked()),this,SLOT(button_confirm_Clicked()));
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
Mini_Keyboard::~Mini_Keyboard()
{
    this->close();
}
void Mini_Keyboard::buttonClicked(int btn){
    qDebug()<<"buttonClicked"<<btn;
}

void Mini_Keyboard::buttonPressed(int btn){
   QString str;
   QString num;
   if(outString->length()>100)qDebug()<<"input String is too long";//限制输入字符数#/
   switch(btn){
        case 0:
       outString->append(num.setNum(0));
        break;
        case 1:
        return;
        break;
        case 2:
       outString->append(num.setNum(1));
        break;
        case 3:
       outString->append(num.setNum(2));
        break;
        case 4:
       outString->append(num.setNum(3));
        break;
        case 5:
       outString->append(num.setNum(4));
        break;
        case 6:
       outString->append(num.setNum(5));
        break;
        case 7:
       outString->append(num.setNum(6));
        break;
        case 8:
       outString->append(num.setNum(7));
        break;
        case 9:
       outString->append(num.setNum(8));
        break;
        case 10:
       outString->append(num.setNum(9));
        break;
        case 11:
      outString->append(num.setNum(7));//cleare
      outString->clear();
        break;
       default:
       return ;
       break;
   }
    //outString->append(num.setNum(btn));
    //outEdit->setText(*outString);

    if(*outString != NULL){
        str=outEdit->text();
        str.append("*");
        outEdit->setText(str);
    }
    else{
        outEdit->setText("");
    }

    outEdit->update();
}
void Mini_Keyboard::buttonReleased(int btn){
    btn = btn;
    //qDebug()<<"buttonReleased"<<btn;
}
void Mini_Keyboard::button_confirm_Clicked(){
    //qDebug()<<"button_confirm_Clicked";
    if(NumberKeyboard!=NULL){
        delete NumberKeyboard;
        NumberKeyboard=NULL;
    }
}
void Mini_Keyboard::button_add_Clicked(){
    //qDebug()<<"button_confirm_Clicked";
}

void Mini_Keyboard::button_sub_Clicked(){
    //qDebug()<<"button_confirm_Clicked";
}
void Mini_Keyboard::button_del_Clicked(){
    //qDebug()<<"button_confirm_Clicked";
}
void Mini_Keyboard::closeKeyboard(){
    qDebug()<<"closeKeyboard";
    if(NumberKeyboard!=NULL){
        delete NumberKeyboard;
        NumberKeyboard=NULL;
    }
}
void Mini_Keyboard::paintEvent(QPaintEvent *event)
{
    event = event;
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(":/resource/images/virtualkeyboard/mini-keyboard/mini-keyboard.png"));
}


//重写mousePressEvent事件,检测事件类型是不是点击了鼠标左键#/
QMyEdit::QMyEdit(QWidget *parent) :
    QLineEdit(parent)
{

}


void QMyEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
       emit LineEditClicked();
    }
    QLineEdit::mousePressEvent(event);
}
/*******************min keyboard end*********************/


//
Window_ChangeFilter::Window_ChangeFilter(QWidget *parent) :
    QDialog(parent) ,
    widget(parent) ,
    labeltitle(&widget),
    labelInfo(&widget),
    btnCancel(&widget),
    btnReset(&widget),
    btnDry(&widget)
{
    setUi();
    connect(&btnCancel, SIGNAL(pressed()), this, SLOT(btnCancelClick()));
    connect(&btnReset, SIGNAL(pressed()), this, SLOT(btnResetClick()));
    connect(&btnDry, SIGNAL(pressed()), this, SLOT(btnDryClick()));

}

void Window_ChangeFilter::setUi()
{
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    setStyleSheet("background-color: #A2A2A2");

    btnUpStyle.append("border-radius: 6px;"
                      "background: #DCDDE0;"
                      "padding: 4px 4px;");

    btnClickStyle.append("border-radius: 6px;"
                         "background: #192131;"
                         "padding: 4px 4px;");

    QString infoStyle;

    infoStyle = "QLabel {background-color:#F0F0F0;"//192131
            "border: 10px groove gray; border-radius: 10px; padding: 8px 8px;";

    setGeometry(QRect(335, 305, 360, 180));
    labeltitle.setStyleSheet(CONFIG_COLOR_BLACK_CHARS);
    infoStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";}");
    btnUpStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");
    btnClickStyle.append(CONFIG_COLOR_BLACK_CHARS).append(";");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(&widget);
    this->setObjectName("qWindow_Filter");
    setFocusPolicy(Qt::StrongFocus);
    widget.setGeometry(QRect(-20, -20, 160, 160));

    labeltitle.setGeometry(QRect(100, -2, 200, 20));
    labeltitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labeltitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                             CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    labelInfo.setGeometry(QRect(5, 20, 330, 80));

    labelInfo.setAlignment(Qt::AlignLeft);
    labelInfo.setFont(QFont(QString(CONFIG_FONT_STYLE),
                            CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelInfo.setStyleSheet(infoStyle);
    labelInfo.setWordWrap(true);                //自动换行

    btnReset.setGeometry(QRect(15, 120, 90, 35));
    BUTTON_SIMPLE_STYLE(btnReset);
    btnReset.setStyleSheet(btnUpStyle);

    btnDry.setGeometry(QRect(125, 120, 90, 35));
    BUTTON_SIMPLE_STYLE(btnDry);
    btnDry.setStyleSheet(btnUpStyle);

    btnCancel.setGeometry(QRect(235, 120, 90, 35));
    BUTTON_SIMPLE_STYLE(btnCancel);
    btnCancel.setStyleSheet(btnUpStyle);

    CustomSetText();
    setLayout(layout);

    //圆角矩形窗口
    QBitmap bmp(this->size());
    bmp.fill(this,0,0);
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);//Anti aliasing
    p.drawRoundedRect(bmp.rect(), 10, 10, Qt::AbsoluteSize); //Four corners are rounded corners
    setMask(bmp);
    Dry=false;
}

void Window_ChangeFilter::showMessage(QString &title,QString &info)
{
    labeltitle.setText(title);
    labelInfo.setText(info);
    this->showNote(true);
}

void Window_ChangeFilter::CustomSetText()
{
    btnCancel.setText(tr("Cancel"));
    btnReset.setText(tr("Reset Filter"));
    if(!Dry)
     btnDry.setText(tr("Dry Filter"));
    else
     btnDry.setText(tr("Stop Dry"));
}

void Window_ChangeFilter::showNote(bool isShow)
{
    setVisible(isShow);
    emit showNoteSignal(isShow);
    if(isShow==true)
       this->raise();

}

bool Window_ChangeFilter::isShow()
{
    return this->isVisible();
}

Window_ChangeFilter::~Window_ChangeFilter()
{
    this->close();
}

void Window_ChangeFilter::btnCancelClick()
{
    btnDry.setText(tr("Dry Filter"));
    Dry=false;
    showNote(false);
    emit NoteSelSignal(true);
}

void Window_ChangeFilter::btnResetClick()
{
    showNote(false);
    emit RstSelSignal(true);
}

void Window_ChangeFilter::btnDryClick()
{
    Dry=!Dry;
    if(Dry)
    {
      btnDry.setText(tr("Stop Dry"));
      emit DrySelSignal(true);
    }
    else
    {
        btnDry.setText(tr("Dry Filter"));
        emit DrySelSignal(false);
    }
}

bool Window_ChangeFilter::isDry()
{
    return Dry;
}
