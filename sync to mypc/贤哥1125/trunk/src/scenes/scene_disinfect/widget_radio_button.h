#ifndef WIDGET_RADIO_BUTTON_H
#define WIDGET_RADIO_BUTTON_H

#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>
#include "lang_switch.h"

#include "packet.h"

#define WIDGET_DISINFECT_RADIOBUTTON_X      40
#define WIDGET_DISINFECT_RADIOBUTTON_Y      74
#define WIDGET_DISINFECT_RADIOBUTTON_WIDTH  240
#define WIDGET_DISINFECT_RADIOBUTTON_HIGH   500

#define WIDGET_DISINFECT_RADIOBUTTON_LABEL_X        5

#define WIDGET_DISINFECT_RADIOBUTTON_LABEL_Y        480
#define WIDGET_DISINFECT_RADIOBUTTON_LABEL2_Y       180
#define WIDGET_DISINFECT_RADIOBUTTON_LABEL3_Y       410
#define WIDGET_DISINFECT_RADIOBUTTON_LABEL_WIDTH    230
#define WIDGET_DISINFECT_RADIOBUTTON_LABEL_HIGH     25

#define WIDGET_DISINFECT_RADIOBUTTON_BUTTON_X        6
#define WIDGET_DISINFECT_RADIOBUTTON_BUTTON_WIDTH    235
#define WIDGET_DISINFECT_RADIOBUTTON_BUTTON_HIGH     35

#define WIDGET_DISINFECT_RADIOBUTTON_FLUSH_Y            60
#define WIDGET_DISINFECT_RADIOBUTTON_HOTFLUSH_Y         110

#define WIDGET_DISINFECT_RADIOBUTTON_CHEMICALDISINFECT_Y  190

#define WIDGET_DISINFECT_RADIOBUTTON_HOTDISINFECT_Y       270

#define WIDGET_DISINFECT_RADIOBUTTON_CENTRALDISINFECT_Y   400

#define WIDGET_DISINFECT_RADIOBUTTON_HOTCITRICACID_Y      320

class WidgetDisinfectRadioButton : public QWidget, public LangSetText
{
    Q_OBJECT
public:
    WidgetDisinfectRadioButton(QWidget *parent=0);
    void CustomSetText();
    DisinfectWorkModel getDisinfectWorkModel();
    void slectModel(DisinfectWorkModel sle);
private slots:
    void radioChange(void);

private:

    QLabel labelSelected;

    //QLabel labelNote;

    QButtonGroup buttonGroup;
    QRadioButton radioButtonRinse;  //冲洗#/
    QRadioButton radioButtonHotRinse;  //热冲洗#/

    QRadioButton radioButtonChemicalDisinfect; //化学消毒#/

    QRadioButton radioButtonHotDisinfect;  //热消毒#/

    QRadioButton radioButtonCentralDisinfect;
    QRadioButton radioButtonHotCitricAcid;
};

#endif
