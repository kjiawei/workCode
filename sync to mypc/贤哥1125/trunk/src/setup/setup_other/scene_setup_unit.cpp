#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_unit.h"

inline void SceneSetupUnit::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelLen, 1);

    SCENE_SETUP_CREATE_COMBOBOX(comboLen, 1);
    comboLen.insertItem(0, QString("cm"));
    comboLen.insertItem(1, QString("inch"));

    SCENE_SETUP_CREATE_TABLE(labelPressure, 2);

    SCENE_SETUP_CREATE_COMBOBOX(comboPressure, 2);
    comboPressure.insertItem(0, QString("mmHg"));
    comboPressure.insertItem(1, QString("kPa"));

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    CustomSetText();
}

SceneSetupUnit::SceneSetupUnit(QWidget *parent) :
    QObject(parent),
    labelLen(parent),
    comboLen(parent),

    labelPressure(parent),
    comboPressure(parent),

    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    PageAppendPiece("Setup Unit", this);
}

void SceneSetupUnit::CustomSetText()
{
    labelLen.setText(tr("Length:"));
    labelPressure.setText(tr("Pressure:"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
}

void SceneSetupUnit::setCusVisible(bool vis, char* para)
{
    para = para;
    labelLen.setVisible(vis);
    comboLen.setVisible(vis);
    labelPressure.setVisible(vis);
    comboPressure.setVisible(vis);
    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);
}
