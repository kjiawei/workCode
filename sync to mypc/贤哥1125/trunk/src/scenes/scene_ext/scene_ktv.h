#ifndef SCENE_KTV_H
#define SCENE_KTV_H

#include <QtGui>
#include "page_info.h"
#include "lang_switch.h"
#include "widget_curve.h"

#include "input_panel_context.h"
#include "scene_setup_layout.h"
#include "keysetready.h"

#define SCENE_PATIENT_LABEL_X       0
#define SCENE_PATIENT_UNIT_X        340
#define SCENE_PATIENT_LINEEDIT_X    215

#define SCENE_DIALYSIS_LABEL_X    0
#define SCENE_DISLYSIS_UNIT_X     420
#define SCENE_DISLYSIS_LINEEDIT_X 305

#define SCENE_KTV_LABEL_Y_0 10
#define SCENE_KTV_LABEL_Y_1 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_0 + 7)//55
#define SCENE_KTV_LABEL_Y_2 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_1 + 7)//100
#define SCENE_KTV_LABEL_Y_3 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_2 + 7)//145
#define SCENE_KTV_LABEL_Y_4 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_3 + 7)//190
#define SCENE_KTV_LABEL_Y_5 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_4 + 7)//235
#define SCENE_KTV_LABEL_Y_6 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_5 + 7)//280
#define SCENE_KTV_LABEL_Y_7 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_6 + 7)//325
#define SCENE_KTV_LABEL_Y_8 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_7 + 7)//370
#define SCENE_KTV_LABEL_Y_9 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_8 + 7)//415
#define SCENE_KTV_LABEL_Y_10 (SCENE_SETUP_WIDGET_HIGH + SCENE_KTV_LABEL_Y_9 + 7)//460


#define SCENE_KTV_LABLE(p,x,y,w)           \
do {                                        \
    (p).setGeometry(QRect(x,          \
                SCENE_KTV_LABEL_Y_##y, (w),                \
                SCENE_SETUP_WIDGET_HIGH));                \
    (p).setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
    (p).setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    (p).setStyleSheet(CONFIG_COLOR_LABEL);                \
} while(0)

#define SCENE_KTV_UNIT(p,x,y)            \
        do {                                    \
            (p).setGeometry(QRect(x,       \
                        SCENE_KTV_LABEL_Y_##y,                \
                        SCENE_SETUP_COL_WIDTH_UNIT,         \
                        SCENE_SETUP_WIDGET_HIGH));          \
            (p).setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
            (p).setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
            (p).setStyleSheet(CONFIG_COLOR_LABEL);    \
        } while(0)

#define SCENE_KTV_EDIT(p, x, y)  \
do {                                        \
    (p).setGeometry(QRect((x), SCENE_KTV_LABEL_Y_##y, \
    SCENE_SETUP_COL_WIDTH_EDIT,SCENE_SETUP_WIDGET_HIGH));\
} while(0)

#define SCENE_KTV_LINEEDIT(p,x,y)        \
    do {                                        \
        SCENE_KTV_EDIT((p), x, y);           \
        LINEEDIT_STYLE(p);                      \
    } while(0)


class SceneKTV : public QWidget, public PieceVisible, public LangSetText
        , public KeySetReady
{
    Q_OBJECT

public:
    SceneKTV(QWidget *parent=0);
    void setUi(void);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void lineEdit_init();
    void lineEdit_data_set(const char* name = NULL, const char* data = NULL) {
        name = name;
        data = data;
    }

    void lineEdit_close(const char *name) {
        name = name;
    }

private:
    void mouseMoveEvent (QMouseEvent * event);
private:
    bool visible;

    QLabel labelTitle;

    QLabel *labelKtvHeight;
    QLabel *labelKtvWeight;
    QLabel *labelKtvAge;
    QLabel *labelKtvGender;

    QLineEdit *lineEditKtvHeight;
    QLineEdit *lineEditKtvWeight;
    QLineEdit *lineEditKtvAge;
    QLineEdit *lineEditPreDialysis;
    QLineEdit *lineEditPostDialysis;

    QLabel *labelHeightUnit;
    QLabel *labelWeightUnit;
    QLabel *labelAgeUnit;
    QLabel *labelCaliText;
    QLabel *labelPreDialysis;
    QLabel *labelPostDialysis;
    QLabel *labelPreDialysisUnit;
    QLabel *labelPostDialysisUnit;
    QLabel *labelKtVFunction;

    QPushButton *startCaliButton;
    QProgressBar *CaliProgressBar;

    QRadioButton *radioButtonSelectMale;
    QRadioButton *radioButtonSelectFemale;
    QRadioButton *radioButtonStartKTV;
    QRadioButton *radioButtonBAN;

    /****************URR Page*************************/
    QLabel *labelURRText;
    QLabel *labelFirstPreDia;
    QLabel *labelFirstPostDia;
    QLabel *labelFirstPreDiaUnit;
    QLabel *labelFirstPostDiaUnit;
    QLabel *labelReduceRate;
    QLabel *labelReduceRateValue;

    QLineEdit *lineEditFirstPreDia;
    QLineEdit *lineEditFirstPostDia;
    /****************SRI2pool Page*************************/
    QLabel *labelSRIText;
    QLabel *labelFirstPreDiaBUN;
    QLabel *labelFirstPostDiaBUN;
    QLabel *labelUFVolume;
    QLabel *labelFirstPreDiaBUNUnit;
    QLabel *labelFirstPostDiaBUNUnit;
    QLabel *labelUFVolumeUnit;
    QLabel *labelSRI2pool;
    QLabel *labelSRI2poolValue;

    QLineEdit *EditFirstPreDiaBUN;
    QLineEdit *EditFirstPostDiaBUN;
    QLineEdit *EditUFVolume;
    /****************nPCR Page*************************/
    QLabel *labelnPCRText;
    QLabel *labelUrineVolume;
    QLabel *labelIntervalTime;
    QLabel *labelUrineBUN;
    QLabel *labelFirstPostDiaBUNII;
    QLabel *labelSecPostDiaBUN;
    QLabel *labelnPCR;
    QLabel *labelnPCRValue;

    QLabel *labelUrineVolumeUnit;
    QLabel *labelIntervalTimeUnit;
    QLabel *labelUrineBUNUnit;
    QLabel *labelFirstPostDiaBUNIIUnit;
    QLabel *labelSecPostDiaBUNUnit;
    QLabel *labelnPCRUnit;

    QLineEdit *EditUrineVolume;
    QLineEdit *EditIntervalTime;
    QLineEdit *EditUrineBUN;
    QLineEdit *EditFirstPostDiaBUNII;
    QLineEdit *EditSecPostDiaBUN;
    /****************TACurea Page*************************/
    QLabel *labelnTACureaText;
    QLabel *labelTACFirstPreDiaBUN;
    QLabel *labelTACFirstPostDiaBUN;
    QLabel *labelTACSecPreDiaBUN;
    QLabel *labelTACDiaTime;
    QLabel *labelTACDiaInterTime;
    QLabel *labelnTAC;
    QLabel *labelnTACValue;

    QLabel *labelTACFirstPreDiaBUNUnit;
    QLabel *labelTACFirstPostDiaBUNUnit;
    QLabel *labelTACSecPreDiaBUNUnit;
    QLabel *labelTACDiaTimeUnit;
    QLabel *labelTACDiaInterTimeUnit;
    QLabel *labelnTACUnit;

    QLineEdit *EditTACFirstPreDiaBUN;
    QLineEdit *EditTACFirstPostDiaBUN;
    QLineEdit *EditTACSecPreDiaBUN;
    QLineEdit *EditTACDiaTime;
    QLineEdit *EditTACDiaInterTime;

    WidgetCurve *KTVCurve;
    QLabel *labelCurve;

    QTabWidget KtVTabWidget;
    QWidget *Ktv;
    QWidget *URR;
    QWidget *SRI2pool;
    QWidget *nPCR;
    QWidget *TACurea;
    QWidget *Curve;
    QWidget *PerInfo;
};

#endif
