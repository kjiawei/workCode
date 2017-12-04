#include <QAbstractButton>
#include <QStylePainter>
#include <QStyleOptionButton>
#include "config_ui.h"
#include "widget_common.h"
#include "widget_radio_button.h"

extern int  gChemicalDisinfect;
extern int  gCentralDisinfect;

WidgetDisinfectRadioButton::WidgetDisinfectRadioButton(QWidget *parent) :
    QWidget(parent),
    labelSelected(this),
    //labelNote(this),
    buttonGroup(this),
    radioButtonRinse(this),
    radioButtonHotRinse(this),
    radioButtonChemicalDisinfect(this),
    radioButtonHotDisinfect(this),
    radioButtonCentralDisinfect(this),
    radioButtonHotCitricAcid(this)
{
    labelSelected.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_LABEL_X,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_Y,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_WIDTH,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_HIGH));
    labelSelected.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    labelSelected.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelSelected.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

//    labelNote.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_LABEL_X+200,
//                WIDGET_DISINFECT_RADIOBUTTON_LABEL3_Y,
//                WIDGET_DISINFECT_RADIOBUTTON_LABEL_WIDTH,
//                WIDGET_DISINFECT_RADIOBUTTON_LABEL_HIGH));
//    labelNote.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
//    labelNote.setFont(QFont(QString(CONFIG_FONT_STYLE),
//                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
//    labelNote.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    radioButtonRinse.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_BUTTON_X,
                WIDGET_DISINFECT_RADIOBUTTON_FLUSH_Y,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_WIDTH,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_HIGH));
    radioButtonRinse.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonRinse);

    radioButtonHotRinse.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_BUTTON_X,
                WIDGET_DISINFECT_RADIOBUTTON_HOTFLUSH_Y,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_WIDTH,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_HIGH));
    radioButtonHotRinse.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonHotRinse);


    radioButtonCentralDisinfect.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_BUTTON_X,
                WIDGET_DISINFECT_RADIOBUTTON_CENTRALDISINFECT_Y,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_WIDTH,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_HIGH));

    radioButtonCentralDisinfect.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    RADIOBUTTON_STYLE(radioButtonCentralDisinfect);


    radioButtonChemicalDisinfect.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_BUTTON_X,
                WIDGET_DISINFECT_RADIOBUTTON_CHEMICALDISINFECT_Y,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_WIDTH,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_HIGH));

    radioButtonChemicalDisinfect.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    RADIOBUTTON_STYLE(radioButtonChemicalDisinfect);


    radioButtonHotDisinfect.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_BUTTON_X,
                WIDGET_DISINFECT_RADIOBUTTON_HOTDISINFECT_Y,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_WIDTH,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_HIGH));

    radioButtonHotDisinfect.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    RADIOBUTTON_STYLE(radioButtonHotDisinfect);

    radioButtonHotCitricAcid.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_BUTTON_X,
                WIDGET_DISINFECT_RADIOBUTTON_HOTCITRICACID_Y,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_WIDTH,
                WIDGET_DISINFECT_RADIOBUTTON_BUTTON_HIGH));
    radioButtonHotCitricAcid.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonHotCitricAcid);

    CustomSetText();

    buttonGroup.addButton(&radioButtonRinse, 1);
    buttonGroup.addButton(&radioButtonHotRinse, 2);

    buttonGroup.addButton(&radioButtonChemicalDisinfect, 3);

    buttonGroup.addButton(&radioButtonHotDisinfect, 4);

    buttonGroup.addButton(&radioButtonCentralDisinfect, 5);
    buttonGroup.addButton(&radioButtonHotCitricAcid, 6);

    connect(&radioButtonRinse, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(&radioButtonHotRinse, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(&radioButtonChemicalDisinfect, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(&radioButtonHotDisinfect, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(&radioButtonHotCitricAcid, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(&radioButtonCentralDisinfect, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    radioButtonCentralDisinfect.setDisabled(true);
    radioButtonCentralDisinfect.setVisible(false);
}

void WidgetDisinfectRadioButton::radioChange(void)
{
    if(sender() == &radioButtonRinse)
    {
      labelSelected.setText(tr("Rinse Selected"));

    }
    else if(sender() == &radioButtonHotRinse)
    {
      labelSelected.setText(tr("Hot Rinse Selected"));
    }
    else if(sender() == &radioButtonChemicalDisinfect)
    {
      if(gChemicalDisinfect == 1)
      {
        labelSelected.setText(tr("Chymistry Peracetic Selecte"));
      }
      else if(gChemicalDisinfect == 2)
      {
        labelSelected.setText(tr("Chymistry Citric Selecte"));
      }
      else if(gChemicalDisinfect == 3)
      {
        labelSelected.setText(tr("Chymistry Reserve3 Selecte"));
      }
      else
      {
        labelSelected.setText(tr("Chymistry Disinfect Error!"));
      }
    }
    else if(sender() == &radioButtonHotDisinfect)
    {
      labelSelected.setText(tr("Hot Disinfect Selected"));
    }
    else if(sender() == &radioButtonHotCitricAcid)
    {
      labelSelected.setText(tr("Hot CitricAcid Selected"));
    }
    else if(sender() == &radioButtonCentralDisinfect)
    {
      if(gCentralDisinfect == 1)
      {
        labelSelected.setText(tr("Central Hot Selected"));
      }
      else if(gCentralDisinfect == 2)
      {
        labelSelected.setText(tr("Central Chemical Selected"));
      }
      else
      {
        labelSelected.setText(tr("Central Disinfect Error!"));
      }
    }
}

void WidgetDisinfectRadioButton::CustomSetText()
{

    labelSelected.setText(tr("No Work Model Selected"));

    radioButtonRinse.setText(tr("Rinse"));
    radioButtonHotRinse.setText(tr("Hot Rinse"));

    radioButtonChemicalDisinfect.setText(tr("Chemical Disinfect"));

    radioButtonHotDisinfect.setText(tr("Hot Disinfect"));

    radioButtonCentralDisinfect.setText(tr("Central Disinfect Mode"));
    radioButtonHotCitricAcid.setText(tr("Hot Acid"));
}

DisinfectWorkModel WidgetDisinfectRadioButton::getDisinfectWorkModel()
{
    if (radioButtonRinse.isChecked() == true)
    {
        return cold_rinse_B;
    }

    if (radioButtonHotRinse.isChecked() == true)
    {
        return hot_rinse_B;
    }


    if (radioButtonCentralDisinfect.isChecked() == true)
    {
        return x_citricAcid_B;
    }

    if (radioButtonHotCitricAcid.isChecked() == true)
    {
        return h_citricAcid_B;
    }


    if (radioButtonChemicalDisinfect.isChecked() == true)
    {
        return x_peracetic_B;
    }


    if (radioButtonHotDisinfect.isChecked() == true)
    {
        return h_water_B;//x_sodiumHypochlorite_B;
    }

    return noChooseDisinfectWorkModel;
}

void WidgetDisinfectRadioButton::slectModel(DisinfectWorkModel sle)
{
    switch (sle) {
    case cold_rinse_B: //冷冲洗
        radioButtonRinse.click();
        break;
    case hot_rinse_B:
        radioButtonHotRinse.click();
        break;
    case x_peracetic_B:
    case x_sodiumHypochlorite_B:
    case x_citricAcid_B:
        radioButtonChemicalDisinfect.click();
        break;
    case h_water_B:
        radioButtonHotDisinfect.click();
        break;
    case h_citricAcid_B:
        radioButtonHotCitricAcid.click();
        break;
    default:
       radioButtonRinse.click();
        break;
    }

}
