#ifndef SCENE_MAINTAIN_CONFIG_LANGUAGE_H
#define SCENE_MAINTAIN_CONFIG_LANGUAGE_H

#include "lang_switch.h"
#include "page_info.h"
#include <QWidget>
#include <QCheckBox>
#include <QPushButton>

class SceneMaintainConfigLanguage : public QWidget, public LangSetText, public PieceVisible
{
    Q_OBJECT

public:
    SceneMaintainConfigLanguage(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~SceneMaintainConfigLanguage();
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);
    void setUI(QWidget * parent);
    void loadXml(QWidget * parent);

private slots:
    void saveXml();
    void cancelXml();

private:
    QWidget *pParentWidget;
    QCheckBox *pButton;
    QPushButton saveButton;
    QPushButton cancelButton;
    int numberNode;
    //可以为任意数，只为发送信号而用。。
    int sendSignal;
};

#endif // SCENE_MAINTAIN_CONFIG_LANGUAGE_H
