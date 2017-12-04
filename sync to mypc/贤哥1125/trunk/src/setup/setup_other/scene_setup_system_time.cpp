#include "config_ui.h"
#include "common_func.h"
#include "scene_setup_layout.h"
#include "scene_setup_system_time.h"

inline void SceneSetupSystemTime::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelDate, 1);
    dateEdit.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT,
                SCENE_SETUP_ROW_1, 150,
                SCENE_SETUP_WIDGET_HIGH));

    dateEdit.setDisplayFormat("MM-dd-yyyy ");
    dateEdit.setDate(QDate::currentDate());
    dateEdit.setStyleSheet("QDateTimeEdit {"
                "color:#cdcdcd;"
                "background: #123f75;"
                "}");
    dateEdit.setAlignment( Qt::AlignVCenter | Qt::AlignRight);
    dateEdit.setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    SCENE_SETUP_CREATE_TABLE(labelTime, 2);
    timeEdit.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT,
                    SCENE_SETUP_ROW_2, 150,
                    SCENE_SETUP_WIDGET_HIGH));

    timeEdit.setDisplayFormat("h:m  ap ");
    timeEdit.setTime(QTime::currentTime());
    timeEdit.setStyleSheet("QDateTimeEdit {"
                "color:#cdcdcd;"
                "background: #123f75;"
                "}");
    timeEdit.setAlignment( Qt::AlignVCenter | Qt::AlignRight);
    timeEdit.setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    SCENE_SETUP_CREATE_TABLE(labelZone, 3);
    SCENE_SETUP_CREATE_COMBOBOX(comboBoxZone, 3);
    comboBoxZone.insertItem(0, tr("China"), 8);
    comboBoxZone.insertItem(1, tr("Japan"), 9);
    comboBoxZone.insertItem(2, tr("India"), 5.5);
    comboBoxZone.insertItem(3, tr("Indonesia"), 7);
    comboBoxZone.insertItem(4, tr("Malaysia"), 8);
    comboBoxZone.insertItem(5, tr("Vietnam"), 7);

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    CustomSetText();
}

SceneSetupSystemTime::SceneSetupSystemTime(QWidget *parent) :
    QObject(parent),
    labelDate(parent),
    dateEdit(parent),
    labelTime(parent),
    timeEdit(parent),
    labelZone(parent),
    comboBoxZone(parent),
    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    connect(&buttonSave, SIGNAL(clicked()), this, SLOT(SlotSystemTimeSet()));
    PageAppendPiece("Setup System Time", this);
}

void SceneSetupSystemTime::SlotSystemTimeSet()
{
    QString string = dateEdit.date().toString("MMdd")+
        timeEdit.time().toString("hhmm")+
        dateEdit.date().toString("yyyy");

    //设置system时钟
    char argv0[] = "date";
    char *argv[] = {argv0, string.toAscii().data(), NULL};
    SystemShellCmd(argv0, argv);

    //同步rtc时钟，用system时钟更新rtc
    char hwclock[] = {"hwclock"};
    string = "-w";
    char *hwclock_argv[] = {hwclock,string.toAscii().data(),NULL};
    SystemShellCmd(hwclock,hwclock_argv);
}

void SceneSetupSystemTime::CustomSetText()
{
    labelDate.setText(tr("Date:"));
    labelTime.setText(tr("Time:"));
    labelZone.setText(tr("Time Zone:"));
    comboBoxZone.setItemText(0, tr("China"));
    comboBoxZone.setItemText(1, tr("Japan"));
    comboBoxZone.setItemText(2, tr("India"));
    comboBoxZone.setItemText(3, tr("Indonesia"));
    comboBoxZone.setItemText(4, tr("Malaysia"));
    comboBoxZone.setItemText(5, tr("Vietnam"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
}

void SceneSetupSystemTime::setCusVisible(bool vis, char* para)
{
    para = para;

    labelDate.setVisible(vis);
    dateEdit.setVisible(vis);
    labelTime.setVisible(vis);
    timeEdit.setVisible(vis);
    labelZone.setVisible(vis);
    comboBoxZone.setVisible(vis);
    buttonSave.setVisible(vis);
    buttonCancel.setVisible(vis);
}
