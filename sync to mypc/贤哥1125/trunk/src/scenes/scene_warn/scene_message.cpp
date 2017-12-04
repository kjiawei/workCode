/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-1-12
** 修改人：Huangjunxian
** 修改时间：2014-1-12
** 描述：
**    5058报警提示框 《源文件》
*****************************************************/

#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_message.h"
#include "Alarm_module.h"
#include "textitemdelegate.h"


inline void Window_Message::setUi(const QString title, const QString brushPath)
{
    Q_UNUSED(brushPath);
    //设置无边框#/
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
//    setWindowFlags( Qt::FramelessWindowHint);
    setStyleSheet("background: #DCDDE0;");
    QString infoStyle = "QLabel {background-color:#DCDCDC;"//#192131
            "border: 5px groove gray; border-radius: 5px; padding: 2px 4px;";

    btnUpStyle.append("border-radius: 13px;"
                      "background: #AAAAAA;"
                      "padding: 0 4px;");

    btnClickStyle.append("border-radius: 13px;"
                      "background: #192131;"
                      "padding: 0 4px;");

    if(title.compare("Messages") == 0 ) {
        setGeometry(QRect(55, 245, 780, 190));
        labeltitle.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);
        infoStyle.append(CONFIG_COLOR_GREEN_CHARS).append(";}");
        labelPage.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);
        btnUpStyle.append(CONFIG_COLOR_GREEN_CHARS).append(";");
        btnClickStyle.append(CONFIG_COLOR_GREEN_CHARS).append(";");

    } else if(title.compare("Warn") == 0) {
        setGeometry(QRect(45, 225, 780, 190));
        labeltitle.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);
        infoStyle.append(CONFIG_COLOR_YELLOW_CHARS).append(";}");
        labelPage.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);
        btnUpStyle.append(CONFIG_COLOR_YELLOW_CHARS).append(";");
        btnClickStyle.append(CONFIG_COLOR_YELLOW_CHARS).append(";");

    } else {
        setGeometry(QRect(35, 205, 780, 190));
        labeltitle.setStyleSheet(CONFIG_COLOR_RED_CHARS);
        infoStyle.append(CONFIG_COLOR_RED_CHARS).append(";}");
        labelPage.setStyleSheet(CONFIG_COLOR_RED_CHARS);
        btnUpStyle.append(CONFIG_COLOR_RED_CHARS).append(";");
        btnClickStyle.append(CONFIG_COLOR_RED_CHARS).append(";");
    }

    this -> title = title;

    layout = new QHBoxLayout;
    layout->addWidget(&widget);

    //widget.setObjectName("AlarmDialog");
    //this->setObjectName("AlarmDialog2");
    widget.setGeometry(QRect(-20, -20, 780, 190));

    labeltitle.setGeometry(QRect(0, 0, 500, 20));

    labeltitle.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labeltitle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    labelInfo.setGeometry(QRect(0, 30, 750, 110));

    labelInfo.setAlignment(Qt::AlignLeft);
    labelInfo.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelInfo.setStyleSheet(infoStyle);
    labelInfo.setWordWrap(true);                //自动换行#/

    btnconfirm.setGeometry(QRect(460, 140, 85, 30));
    BUTTON_SIMPLE_STYLE(btnconfirm);
    btnconfirm.setStyleSheet(btnUpStyle);

    btnLast.setGeometry(QRect(570, 140, 70, 30));
    BUTTON_SIMPLE_STYLE(btnLast);
    btnLast.setStyleSheet(btnUpStyle);

    labelPage.setAlignment(Qt::AlignCenter);
    labelPage.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelPage.setGeometry(QRect(650, 143, 30, 25));

    btnNext.setGeometry(QRect(690, 140, 70, 30));
    BUTTON_SIMPLE_STYLE(btnNext);
    btnNext.setStyleSheet(btnUpStyle);

    btnClose.setGeometry(QRect(690, 0, 65, 28));
    BUTTON_SIMPLE_STYLE(btnClose);
    btnClose.setStyleSheet(btnUpStyle);

    CustomSetText();

    setLayout(layout);

    this ->isMainShow = false;

    index = 1;      //显示当前索引号#/

    setStyleSheet("background: #DCDDE0;");

    /*
    //圆角矩形窗口#/
    QBitmap bmp(this->size());
    bmp.fill(this,0,0);
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);//Anti aliasing
    p.drawRoundedRect(bmp.rect(), 10, 10, Qt::AbsoluteSize); //Four corners are rounded corners
    setMask(bmp);

    QBitmap tmpbmp(this->size());
    bmp=tmpbmp;
    bmp.fill(this,0,0);
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);//Anti aliasing
    p.drawRoundedRect(bmp.rect(), 10, 10, Qt::AbsoluteSize); //Four corners are rounded corners
    setMask(bmp);
      */
}


Window_Message::Window_Message(QWidget *parent, const QString title, const QString brushPath ) :
    QDialog(parent) ,
    widget(parent) ,
    labeltitle(&widget),
    labelInfo(&widget),
    labelPage(&widget),
    btnconfirm(&widget),
    btnLast(&widget),
    btnNext(&widget),
    btnClose(&widget)
{
    setUi(title, brushPath);

    btnClose.setObjectName("btnClose");
    btnLast.setObjectName("btnLast");
    btnNext.setObjectName("btnNext");

    connect(&btnClose, SIGNAL(clicked()), this, SLOT(btnCloseDialog()));

    connect(&btnconfirm, SIGNAL(clicked()), this, SLOT(btnConfirmClick()));

    connect(&btnLast, SIGNAL(pressed()), this, SLOT(btnLastClick()));
    connect(&btnLast, SIGNAL(released()), this, SLOT(btnReleaseClick()));

    connect(&btnNext, SIGNAL(pressed()), this, SLOT(btnNextClick()));
    connect(&btnNext, SIGNAL(released()), this, SLOT(btnReleaseClick()));

    connect(this, SIGNAL(showDialogSignal(bool)), this, SLOT(setShown(bool)));

    btnLast.setEnabled(false);

    if(this->title.compare("Warn") == 0)//警告 延时消失提示框#/
    {
        connect(&timer,SIGNAL(timeout()),this,SLOT(timeHandle()));
    }
}

Window_Message::~Window_Message()
{
    this->close();
}

//清除该类提示框保存的内容#/
void Window_Message::removeAllRow()
{
    mapValue.clear();
    index = 1;
}

//清除某个选中的报警内容#/
void Window_Message::removeOneAlarm(int alarmid)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Window_Message-removeOneAlarm-in01");
#endif
    if(mapValue.contains(alarmid))
    {
        mapValue.remove(alarmid);
        if(mapValue.isEmpty())
        {
            mapValue.clear();
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Window_Message-removeOneAlarm-in02");
#endif
            this->showDialog(false);
        }
        if(this->isVisible())
            this->showMessText();
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Window_Message-removeOneAlarm-out");
#endif
}

//设置显示在所有页面上方#/
void Window_Message::setOnTop() {
    this->isMainShow =true;
}

bool Window_Message::isCanShow() {

    return this->isMainShow;
}

//显示或者隐藏设置#/
void Window_Message::showDialog(bool isShow) {
    QHash<int, AlarmInfor> maptmp;
    maptmp=mapValue;
    if(cleanmap==true && !isShow) //没有内容时隐藏#/
    {
        this->hide();
        return;
    }
    if(isShow==true)  //设置为显示时#/
    {

        this->show();
        this->raise();
        /*
           this->raise();
           this->show();
        */
    }
    else{   //设置为隐藏时，如果有需要确认的消息，则不隐藏#/
        if(maptmp.size()>0)
        {
            QHash<int, AlarmInfor>::iterator it;
            for ( it = maptmp.begin(); it != maptmp.end(); ++it )
            {
                if((it.value().code)&0x00010000)//需要确认的提示#/
                {
                   return;
                }
                else
                {
                    continue;
                }
            }
        }
        this->hide();
    }
}

bool Window_Message::isInfoNull()
{
    return this->mapValue.isEmpty();
}

void Window_Message::showMessText()
{
    QString titlestr,infostr,timestr;//标题、内容、数值、优先级#/
    int timesec,i;
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Window_Message-showMessText-in");
#endif
    QDateTime timeobj;
    QHash<int, AlarmInfor> maptmp;
    QHash<int, AlarmInfor>::iterator it;
    maptmp=mapValue;

    if(index>maptmp.size())
        index=maptmp.size();

    //越前面的消息，越后面显示#/
    for(i=1,it=maptmp.begin();it!=maptmp.end();it++,i++)
    {
        if(index == i)//显示当前指定页#/
        {
            timesec=it.value().time;
            timeobj.setTime_t(timesec);
            timestr=timeobj.toString("yyyy-MM-dd hh:mm");
            titlestr=QString::number(it.value().alarmid);
            titlestr.append(": ");
            Search_alarminfo(it.value().alarmid,titlestr,infostr);
            timestr.append("  ");
            timestr.append(infostr);
            if((it.value().code)&0x00010000) //必须确认#/
            {
                titlestr.append(" (");
                titlestr.append(tr("Must confirm"));
                titlestr.append(")");
                btnClose.setVisible(false);
            }
        }
    }
    insertRow(titlestr,timestr);   //显示标题和内容#/

    QString str = QString::number(index); //显示页码#/
    str.append("/");
    str.append(QString::number(maptmp.size()));
    labelPage.setText(str);

    if(maptmp.size() == 1) {
        btnNext.setEnabled(false);
        btnLast.setEnabled(false);
    } else if(index != maptmp.size()){
        btnNext.setEnabled(true);
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("Window_Message-showMessText-out");
#endif
}

void Window_Message::insertRow(QString warnResume, QString warnInfo)
{
    if(warnResume.length() == 0)  //题目为空#/
        return;
    labeltitle.setText(warnResume);
    QString text =tr("Operation guide:");
    text.append("\r\n            ");
    text.append(warnInfo);
    labelInfo.setText(text);
}

//显示下一条消息#/
void Window_Message::btnNextClick()
{
    if(index <= mapValue.size()) {
        index ++;
        showMessText();
        if(index == mapValue.size()) {
            btnNext.setEnabled(false);
        } else {
            btnNext.setStyleSheet(btnClickStyle);
        }
        if(!btnLast.isEnabled()) {
            btnLast.setEnabled(true);
        }
    }
    secpass=0;  //重设延时消失的时间#/
}

//显示上一条消息#/
void Window_Message::btnLastClick()
{
    if(index > 1) {
        index --;
        showMessText();
        if(index == 1) {
            btnLast.setEnabled(false);
        } else {
            btnLast.setStyleSheet(btnClickStyle);
        }
        if(!btnNext.isEnabled()) {
            btnNext.setEnabled(true);
        }
    }
    secpass=0;  //重设延时消失的时间#/
}

void Window_Message::btnReleaseClick()
{
    btnNext.setStyleSheet(btnUpStyle);
    btnLast.setStyleSheet(btnUpStyle);
}

//延时消失提示框#/
void Window_Message::timeHandle()
{
    if(mapValue.size()==0)
    {
        this->btnCloseDialog();
        return;
    }

    if(this->isHidden()==false)
    {
        this->btnCloseDialog();
        return;
    }

    secpass++;

    if(secpass>=10)
    {
        this->btnCloseDialog();
    }
}

void Window_Message::btnCloseDialog()
{
    timer.stop();
    secpass=0;
    btnClose.setVisible(true);
    showDialog(false);
}

//确认报警操作#/
void Window_Message::btnConfirmClick()
{
    int key,i;
    QHash<int, AlarmInfor> maptmp;
    maptmp=mapValue;
    QHash<int, AlarmInfor>::iterator it;
    //越前面的消息，越后面显示#/
    for(i=1,it=maptmp.begin();it!=maptmp.end();it++,i++)
    {
        if(index == i)//确认当前指定页#/
        {
            key=it.key();
            Try_Map_mutex();
            Confirm_to_histroy(it.value());
            Map_mutex.unlock();
            mapValue.remove(key);
            index--;
            if(index<1)
                index=1;
        }
    }
    if(mapValue.size()>0)  //还有报警消息时#/
        showMessText();
    else
    {
        mapValue.clear();
        this->btnCloseDialog();
    }

    secpass=0;  //重设延时消失的时间#/
}

void Window_Message::showItem(QMap<int, AlarmInfor> map) {

    if(map.size() == 0)
        return;

    if(cleanmap==true)
    {
        removeAllRow();
        cleanmap=false;
    }

    QMap<int, AlarmInfor>::iterator it;
    //添加新的报警内容#/
    for(it = map.begin(); it != map.end(); ++it ) {
        if(mapValue.contains(it.key()))
        {
            continue;
        }
        else
        {
            mapValue.insert(it.key(), it.value());
        }
    }

    if(mapValue.size() == 0)
        btnCloseDialog();

    secpass=0;
    if(title.compare("Warn") == 0)  //自动延时消失类型#/
    {
        if(!timer.isActive())
            timer.start(20000); //20S
    }

    index = 1;              //从第一页开始#/
    showMessText();         //显示报警队列中的内容#/
}

void Window_Message::CustomSetText()
{
    btnClose.setText(tr("close"));
    btnLast.setText(tr("last"));
    btnNext.setText(tr("next"));
    btnconfirm.setText(tr("confirm"));
}
