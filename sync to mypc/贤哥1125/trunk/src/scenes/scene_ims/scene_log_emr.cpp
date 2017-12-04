#include "config_ui.h"
#include "scene_log_emr.h"
#include "scene_setup_layout.h"

inline void SceneLogEmr::setUi()
{
    //title = tr("EMR Log");

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));
    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    CustomSetText();
}

SceneLogEmr::SceneLogEmr(QWidget *parent) :
    QObject(parent),
    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    PageAppendPiece("LogEmr", this);
}

void SceneLogEmr::CustomSetText()
{
    //title = tr("EMR Log");
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
}

void SceneLogEmr::setCusVisible(bool vis, char* para)
{
    para = para;
    buttonSave.setVisible(vis);
    buttonCancel.setVisible(vis);
}
