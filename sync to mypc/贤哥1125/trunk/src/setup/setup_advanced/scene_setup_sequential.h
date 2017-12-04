/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-18
** 修改人：Huangjunxian
** 修改时间：2014-11-26
** 描述：
**    UF曲线缩略图选择按钮 《头文件》
*****************************************************/
#ifndef SCENE_SETUP_SEQUENTIAL_H
#define SCENE_SETUP_SEQUENTIAL_H

#include <QtGui>
#include <QLabel>
#include "config_ui.h"
#include "lang_switch.h"
#include "page_info.h"
#include "widget_button.h"
#include "widget_curve.h"



class SceneSetupSequential : public QObject, public PieceVisible, public LangSetText
{
    Q_OBJECT
public:
    SceneSetupSequential(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private slots:
    void CurveUfActivate(QAbstractButton *button=NULL);
    void buttonEditClicked();
    void buttonAppClicked();

signals:
    void setWidgetShow(bool vis);
    void showUfCurve(int num);

private:
    inline void setUi();

private:
    //超滤曲线的标记和图形按钮#/
    QLabel labelUF;
    QLabel* labelUfText[16];
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
    QPushButton buttonCancel;

    signed char chooseUF;
    QVector <TargetCurve> targetCurveList;
};

#endif
