#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_sysinfo.h"

#include "version.h"


#define SCENE_SYSINFO_TABLE(x,y)             \
    do {                                     \
        x.setGeometry(QRect(170, SCENE_SETUP_ROW_##y,       \
                    150, SCENE_SETUP_WIDGET_HIGH));;        \
        x.setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        x.setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)

#define SCENE_SYSINFO_VALUE(x,y)            \
    do {                                    \
        x.setGeometry(QRect(325, SCENE_SETUP_ROW_##y,   \
                    500, SCENE_SETUP_WIDGET_HIGH));     \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));  \
        x.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);       \
        x.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS);     \
    } while(0)


inline void SceneSetupSysinfo::setUi()
{
    SCENE_SYSINFO_TABLE(labelVer, 1);
    SCENE_SYSINFO_VALUE(labelVerValue, 1);
    /*
#ifdef RELEASE
    QString strREV;
    strREV.setNum(REV);
    labelVerValue.setText(strREV);
#endif
*/
//#if HJX_DEBUG_FLAG
#if 1
    /*
    QString strREV;
    strREV.setNum(REV);
    strREV.append(" -- ");
    strREV.append(QString().sprintf("%s",BUILD_TIME));
    labelVerValue.setText(strREV);
    */
    labelVerValue.setText(DE_5058_VERSION);
#endif

    QString str;
    SCENE_SYSINFO_TABLE(labelCompileTime, 2);
    SCENE_SYSINFO_VALUE(labelCompileTimeValue, 2);
    /*
    str = __DATE__;
    str += ' ';
    str += __TIME__;
    */
    str="2014-05-19 16:15";
    labelCompileTimeValue.setText(str);

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    CustomSetText();
}

SceneSetupSysinfo::SceneSetupSysinfo(QWidget *parent) :
    QObject(parent),
    labelVer(parent),
    labelVerValue(parent),

    labelCompileTime(parent),
    labelCompileTimeValue(parent),

    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    PageAppendPiece("Setup System Info", this);
}

void SceneSetupSysinfo::CustomSetText()
{
    labelVer.setText(tr("Soft Version:"));
    labelCompileTime.setText(tr("Manufacture date:"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
}

void SceneSetupSysinfo::setCusVisible(bool vis, char* para)
{
    para = para;

    labelVer.setVisible(vis);
    labelVerValue.setVisible(vis);
    labelCompileTime.setVisible(false);
    labelCompileTimeValue.setVisible(false);
    buttonSave.setVisible(vis);
    buttonCancel.setVisible(vis);
}
