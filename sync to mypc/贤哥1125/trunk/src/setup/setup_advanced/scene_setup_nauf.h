/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-18
** 修改人：Huangjunxian
** 修改时间：2014-11-26
** 描述：
**    Na UF曲线缩略图选择按钮 《头文件》
*****************************************************/
#ifndef SCENE_SETUP_NAUF_H
#define SCENE_SETUP_NAUF_H

#include <QtGui>
#include <QLabel>
#include "config_ui.h"
#include "lang_switch.h"
#include "page_info.h"
#include "widget_button.h"
#include "widget_curve.h"


class SceneSetupNaUF : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneSetupNaUF(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private slots:
    void CurveNaActivate(QAbstractButton *button=NULL);
    void CurveUfActivate(QAbstractButton *button=NULL);
    void buttonEditClicked();
    void buttonAppClicked();

signals:
    void setWidgetShow(bool vis);
    void showNaCurve(int num);
    void showUfCurve(int num);

private:
    inline void setUi();

private:
    //纳曲线的标记和图形按钮#/
    QLabel labelNa;
    QLabel *labelNaText[16];
    QWidget widgetBackNa;
    QButtonGroup buttonGroupNa;
    WidgetButton buttonNaPre1;
    WidgetButton buttonNaPre2;
    WidgetButton buttonNaPre3;
    WidgetButton buttonNaPre4;
    WidgetButton buttonNaPre5;
    WidgetButton buttonNaPre6;
    WidgetButton buttonNaPre7;
    WidgetButton buttonNaPre8;
    WidgetButton buttonNaCus1;
    WidgetButton buttonNaCus2;
    WidgetButton buttonNaCus3;
    WidgetButton buttonNaCus4;
    WidgetButton buttonNaCus5;
    WidgetButton buttonNaCus6;
    WidgetButton buttonNaCus7;
    WidgetButton buttonNaCus8;

    //超滤曲线的标记和图形按钮#/
    QLabel labelUF;
    QLabel *labelUfText[16];
    QWidget widgetBackUf;
    QButtonGroup buttonGroupUf;
    WidgetButton buttonUfPre1;
    WidgetButton buttonUfPre2;
    WidgetButton buttonUfPre3;
    WidgetButton buttonUfPre4;
    WidgetButton buttonUfPre5;
    WidgetButton buttonUfPre6;
    WidgetButton buttonUfPre7;
    WidgetButton buttonUfPre8;
    WidgetButton buttonUfCus1;
    WidgetButton buttonUfCus2;
    WidgetButton buttonUfCus3;
    WidgetButton buttonUfCus4;
    WidgetButton buttonUfCus5;
    WidgetButton buttonUfCus6;
    WidgetButton buttonUfCus7;
    WidgetButton buttonUfCus8;

    QPushButton buttonEdit;
    QPushButton buttonApp;

    signed char chooseNa;
    signed char chooseUF;
    QVector <TargetCurve> targetCurveList;
};

#endif
