#ifndef SCENE_KTV2_H
#define SCENE_KTV2_H

#include <QtGui>
#include <QTimer>
#include "page_info.h"
#include "lang_switch.h"
#include "widget_curve.h"

#include "input_panel_context.h"
#include "scene_setup_layout.h"
#include "keysetready.h"
#include "ktv2.h"

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


#define SCENE_KTV2_DIS_LABLE(p,x,y,w)           \
    do {                                        \
    (p).setGeometry(QRect(x,          \
    y, (w),                \
    38));                \
    (p).setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
    (p).setFont(QFont(QString(CONFIG_FONT_STYLE),         \
    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    (p).setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)



#define SCENE_KTV2_LINEEDIT(p,x,y)        \
    do {                                        \
    (p).setGeometry(QRect((x), y, \
    SCENE_SETUP_COL_WIDTH_EDIT,SCENE_SETUP_WIDGET_HIGH));\
    LINEEDIT_STYLE(p);                      \
    } while(0)

#define SCENE_KTV2_UNIT(p,x,y)            \
    do {                                    \
    (p).setGeometry(QRect(x,       \
    y,                \
    SCENE_SETUP_COL_WIDTH_UNIT,         \
    SCENE_SETUP_WIDGET_HIGH));          \
    (p).setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
    (p).setFont(QFont(QString(CONFIG_FONT_STYLE),     \
    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    (p).setStyleSheet(CONFIG_COLOR_LABEL);    \
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

class SceneKTV2 : public QWidget, public PieceVisible, public LangSetText
        , public KeySetReady
{
    Q_OBJECT

public:
    SceneKTV2(QWidget *parent=0);
    void setUi(void);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void lineEdit_init();
    void lineEdit_data_set(const char* name = NULL, const char* data = NULL) {
        name = name;
        data = data;
    }
    void lineEdit_close(const char *name) {name = name;}

private:
    void mouseMoveEvent (QMouseEvent * event);

private slots:
    void on_btnPatientInfo_clicked();
    void on_btnPatientApply_clicked();
    void updateKTV();

private:
    //int CurrentBloodFlowRate;       //当前血流速
    //QString currentKTV;
    QTimer *update_timer;
    bool visible;
    QLabel labelTitle;
    QTabWidget KtVTabWidget;
    QWidget *Ktv;
    QWidget *Curve;
    QWidget *PerInfo;
    QWidget *DialyzerP;

    // Kt/V
    QLabel *labelCaliText;//使用说明
    QLabel *labelAttentionText;//注意
    QLabel *labelDialyserModel;//透析器型号
    QLabel *disDialyserModel;
    QLabel *labelBloodFlowRate;//血流量(设定)
    QLabel *disBloodFlowRate;
    QLabel *labelBloodFlowRateUnit;
    QLabel *disTime;//治疗时间
    QLabel *labelTime;
    QLabel *labelTimeUnit;
    QLabel *disUF;//超滤目标
    QLabel *labelUF;
    QLabel *labelUFUnit;
    QLabel *disDialysateFlowRate;//透析液流量
    QLabel *labelDialysateFlowRate;
    QLabel *labelDialysateFlowRateUnit;
    //QPushButton *btnPatientInfo;//病人身体值
    QPushButton *startCaliButton;
    QProgressBar *caliProgressBar;

    // PatientInfo
    QLabel *labelKtvHeight;
    QLineEdit *lineEditKtvHeight;
    QLabel *labelHeightUnit;
    QLabel *labelKtvWeight;
    QLineEdit *lineEditKtvWeight;
    QLabel *labelWeightUnit;
    QLabel *labelKtvAge;
    QLineEdit *lineEditKtvAge;
    QLabel *labelAgeUnit;
    QLabel *labelKtvGender;
    QButtonGroup *radioButtonSex;
    QRadioButton *radioButtonSelectMale;
    QRadioButton *radioButtonSelectFemale;
    QPushButton *btnPatientApply;
    QPushButton *buttonCancel;
    QLabel *disKtVTarget;//目标Kt/V
    QLabel *labelKtVTarget;
    QLabel *labelUpTo;//方案可达
    QLabel *disUpTo;
    QLabel *labelCurrentKtV;//当前
    QLabel *disCurrentKtV;

    // Dialyzerp
    //QLineEdit *lineEditPreDialysis;
    //QLineEdit *lineEditPostDialysis;
    QLabel *ureaExcRatio;//尿素清除量
    QLabel *ureaExcRatioUnit;//单位
    QLabel *ureaExcRatioUnit2;
    QLabel *defaultUreaExc;//默认
    QLabel *defaultUreaExc2;
    //QLineEdit *preDialysisLineEdit;
    //QLineEdit *postDialysisLineEdit;
    QLineEdit *ureaExc;//编辑框
    QLineEdit *ureaExc2;//编辑框
    QLabel *adpPro;//调整比例
    QLabel *adpProDef;//默认值
    QLabel *adpProDefV;//值
    QLineEdit *adpProEdit;//编辑框
    QLabel *labelKTV;//显示KTV三个字
    QLabel *ktvValue;//ktv值
    QLabel *treatTime;//治疗时间四个字
    QLabel *treatTimeUnit;//单位
    QLineEdit *treatTimeEdit;//治疗时间编辑框

    // Curve
    WidgetCurve *KTVCurve;
    QLabel *labelCurve;
};

#endif
