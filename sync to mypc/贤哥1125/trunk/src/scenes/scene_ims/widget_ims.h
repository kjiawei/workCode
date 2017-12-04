#ifndef WIDGET_IMS_H
#define WIDGET_IMS_H

#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QButtonGroup>
#include "lang_switch.h"
#include "widget_button.h"
#include "scene_log_run.h"
#include "scene_log_emr.h"
#include "scene_log_warn.h"
#include "scene_log_treatment.h"
#include "scene_record_curve.h"
#include "../../RFCard/RFCard.h"


#define Widget_Ims_Menu_X               12
#define Widget_Ims_Menu_Y               57
#define Widget_Ims_Menu_Width           147
#define Widget_Ims_Menu_High            696


class WidgetIms : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    WidgetIms(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();
    inline void setSubpageHide();

public slots:
    void SlotSceneActivate(int id);

private:
    SceneLogTreatment sceneLogTreat;
    SceneLogWarn sceneLogWarn;
    SceneLogEmr sceneLogEmr;
    SceneLogRun sceneLogRun;
    SceneRecordCurve sceneCurve;
    PatientMessage patientMessage;  //病人信息

    QWidget widget;
    QLabel labelTitle;

    QButtonGroup buttonGroupIms;
//    WidgetButton buttonLogTreat;
    WidgetButton buttonLogWarn;
//    WidgetButton buttonLogEmr;
    WidgetButton buttonLogRun;
    WidgetButton buttonRecordCurve;
    WidgetButton buttonPatientMessage;

    QString title;
};

#endif
