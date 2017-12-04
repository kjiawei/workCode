#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_bloodtemp.h"

inline void SceneSetupBloodTemp::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelBloodTemp, 1);
    SCENE_SETUP_CREATE_VALUE(labelBloodTempValue, 1);
    SCENE_SETUP_CREATE_UNIT(labelBloodTempUnit, 1);
    labelBloodTempValue.setNum(0);
    labelBloodTempUnit.setText("'C");

    curveBloodTemp.setGeometry(QRect(170, 420, 570, 300));
    curveBloodTemp.setUnit("'C");
    curveBloodTemp.setValueZoom(1);
    curveBloodTemp.setHorizontalScale(6);
    curveBloodTemp.setVerticalScale(8);
    curveBloodTemp.setMode(1);
    curveBloodTemp.setMinValue(0.0);
    curveBloodTemp.setMaxValue(40.0);
    curveBloodTemp.setTestTime(60*60*6);
    curveBloodTemp.setUnitX(Hour);

    CustomSetText();
}

SceneSetupBloodTemp::SceneSetupBloodTemp(QWidget *parent) :
    QObject(parent),
    labelBloodTemp(parent),
    labelBloodTempValue(parent),
    labelBloodTempUnit(parent),
    curveBloodTemp(parent)
{
    setUi();
    dataNodeAddObject(PACKET_NAME_BLOODTEMP, this);
    PageAppendPiece("Setup BloodTemp", this);
}

void SceneSetupBloodTemp::CustomSetText()
{
    labelBloodTemp.setText(tr("Temp :"));
}

void SceneSetupBloodTemp::setCusVisible(bool vis, char* para)
{
    para = para;
    labelBloodTemp.setVisible(vis);
    labelBloodTempValue.setVisible(vis);
    labelBloodTempUnit.setVisible(vis);
    curveBloodTemp.setVisible(vis);
}
void SceneSetupBloodTemp::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupBloodTemp-ActionHandle-in");
#endif
    if(name==PACKET_NAME_BLOODTEMP){
        float bloodTemp = *(unsigned short int *) dataPtr;
        bloodTemp /=100;
        labelBloodTempValue.setNum(bloodTemp);
    }
}
