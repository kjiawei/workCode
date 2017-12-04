#include "config_ui.h"
#include "scene_setup_layout.h"
#include "scene_setup_advert_timer.h"

inline void SceneSetupAdvertTimer::setUi()
{
    SCENE_SETUP_CREATE_TABLE(labelAbsolutTime, 1);//绝对时间

    labelAbsolutTimeUnit.setGeometry(QRect(SCENE_SETUP_COL_UNIT+50,
                SCENE_SETUP_ROW_1,
                SCENE_SETUP_COL_WIDTH_UNIT,
                SCENE_SETUP_WIDGET_HIGH));
    labelAbsolutTimeUnit.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelAbsolutTimeUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelAbsolutTimeUnit.setStyleSheet(CONFIG_COLOR_LABEL);
    labelAbsolutTimeUnit.setText("[h:min]");

    SCENE_SETUP_CREATE_TABLE(labelRelativeTime, 2);//相对时间

    labelRelativeTimeUnit.setGeometry(QRect(SCENE_SETUP_COL_UNIT+50,
                SCENE_SETUP_ROW_2,
                SCENE_SETUP_COL_WIDTH_UNIT,
                SCENE_SETUP_WIDGET_HIGH));
    labelRelativeTimeUnit.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelRelativeTimeUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelRelativeTimeUnit.setStyleSheet(CONFIG_COLOR_LABEL);
    labelRelativeTimeUnit.setText("[h:min]");

    OnetimeEdit.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT,
                    SCENE_SETUP_ROW_1, 130,
                    SCENE_SETUP_WIDGET_HIGH));

    OnetimeEdit.setDisplayFormat("h:m ");
    OnetimeEdit.setTime(QTime(1,35));
    OnetimeEdit.setStyleSheet("QDateTimeEdit {"
                "color:#cdcdcd;"
                "background: #123f75;"
                "}");
    OnetimeEdit.setAlignment( Qt::AlignVCenter | Qt::AlignRight);
    OnetimeEdit.setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    CyctimeEdit.setGeometry(QRect(SCENE_SETUP_COL_LINEEDIT,
                    SCENE_SETUP_ROW_2, 130,
                    SCENE_SETUP_WIDGET_HIGH));

    CyctimeEdit.setDisplayFormat("h:m ");
    CyctimeEdit.setTime(QTime(2,30));
    CyctimeEdit.setStyleSheet("QDateTimeEdit {"
                "color:#cdcdcd;"
                "background: #123f75;"
                "}");
    CyctimeEdit.setAlignment( Qt::AlignVCenter | Qt::AlignRight);
    CyctimeEdit.setFont(QFont(QString(CONFIG_FONT_STYLE),
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));

    SCENE_SETUP_CREATE_TABLE(labelRemainTime, 3);//相对时间
    SCENE_SETUP_CREATE_LINEEDIT(lineEditRemainTime, 3);

    labelRemainTimeUnit.setGeometry(QRect(SCENE_SETUP_COL_UNIT+50,
                SCENE_SETUP_ROW_3,
                200,
                SCENE_SETUP_WIDGET_HIGH));
    labelRemainTimeUnit.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelRemainTimeUnit.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelRemainTimeUnit.setStyleSheet(CONFIG_COLOR_LABEL);
    labelRemainTimeUnit.setText("[h:min:sec]");

    SCENE_SETUP_CREATE_TABLE(labelButtonOnetime, 5);//相对时间
    buttonOnetime.setGeometry(QRect(395, 255, 95, 39));
    buttonCyctime.setGeometry(QRect(505, 255, 95, 39));

    SCENE_SETUP_CREATE_TABLE(labelButtonSeleSound, 6);//
    ButtonSeleSound.setGeometry(QRect(395, 305, 95, 39));
    ButtonSeleOption.setGeometry(QRect(505, 305, 95, 39));

    buttonSave.setGeometry(QRect(620, 500, 64, 39));
    buttonCancel.setGeometry(QRect(694, 500, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonOnetime);
    BUTTON_SIMPLE_STYLE(buttonCyctime);
    BUTTON_SIMPLE_STYLE(ButtonSeleSound);
    BUTTON_SIMPLE_STYLE(ButtonSeleOption);

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    ic->appendWidget(&lineEditRemainTime,NumInputMethod);

    connect(&buttonOnetime, SIGNAL(clicked()), this, SLOT(button_Onetimer_clicked()));
    connect(&buttonCyctime, SIGNAL(clicked()), this, SLOT(button_Cyctimer_clicked()));
    connect(&timer_advert, SIGNAL(timeout()), this, SLOT(timerDone()));

    CustomSetText();
}

SceneSetupAdvertTimer::SceneSetupAdvertTimer(QWidget *parent) :
    QObject(parent),
    labelAbsolutTime(parent),
    labelAbsolutTimeUnit(parent),
    OnetimeEdit(parent),
    CyctimeEdit(parent),
    labelRelativeTime(parent),
    labelRelativeTimeUnit(parent),

    labelRemainTime(parent),
    lineEditRemainTime(parent),
    labelRemainTimeUnit(parent),

    labelButtonOnetime(parent),
    buttonOnetime(parent),
    buttonCyctime(parent),

    labelButtonSeleSound(parent),
    ButtonSeleSound(parent),
    ButtonSeleOption(parent),

    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();
    PageAppendPiece("Setup Timer", this);
    lineEditRemainTime.setEnabled(false);//setDisabled(true);
    onetime=false;
    cycletime=false;
}

void SceneSetupAdvertTimer::CustomSetText()
{
 //   labelAbsolutTime.setText(tr("AbsolutTime"));//绝对时间
 //   labelRelativeTime.setText(tr("RelativeTime"));//相对时间
    labelAbsolutTime.setText(tr("SingleTime"));//单次时间
    labelRelativeTime.setText(tr("CycleTime"));//循环时间
    labelRemainTime.setText(tr("RemainTime"));//剩余时间

    labelButtonOnetime.setText(tr("Stop/RUN"));//
    buttonOnetime.setText(tr("One-Time"));
    buttonCyctime.setText(tr("Cyc-Time"));

    labelButtonSeleSound.setText(tr("Prompt"));//
    ButtonSeleSound.setText(tr("Sound"));
    ButtonSeleOption.setText(tr("Option"));

    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));

    lineEditRemainTime.setText(tr("00:00:00"));

}

void SceneSetupAdvertTimer::button_Onetimer_clicked()
{

    if (buttonOnetime.isChecked() == false)
    {
     buttonCyctime.setDisabled(true);
     buttonOnetime.setCheckable(true);
     count=abs( OnetimeEdit.time().secsTo(QTime(0,0,0)) );
     onetime=true;
     cycletime=false;
     timer_advert.start(1000);
    }else {
     timer_advert.stop();
     buttonOnetime.setCheckable(false);
     buttonCyctime.setDisabled(false);
     onetime=false;
      }
}

void SceneSetupAdvertTimer::button_Cyctimer_clicked()
{
    if (buttonCyctime.isChecked() == false)
    {
     buttonOnetime.setDisabled(true);
     buttonCyctime.setCheckable(true);
     count=abs( CyctimeEdit.time().secsTo(QTime(0,0,0)) );
     onetime=false;
     cycletime=true;
     timer_advert.start(1000);

    }else {
     timer_advert.stop();
     buttonOnetime.setDisabled(false);
     buttonCyctime.setCheckable(false);
     cycletime=false;
      }
}

void SceneSetupAdvertTimer::timerDone()
{
    PacketOwnNote palarm_own;

  if(onetime==true)
  {
      if(count>0)
      {
          count--;
          lineEditRemainTime.setText((timeCount.addSecs(count)).toString(QString("hh:mm:ss")));
      }
      else
      {
          lineEditRemainTime.setText((timeCount.addSecs(0)).toString(QString("hh:mm:ss")));
          timer_advert.stop();
          buttonOnetime.setCheckable(false);
          buttonCyctime.setDisabled(false);
          onetime=false;

          dataRead(OWNNOTE, &palarm_own);
          palarm_own.note.note4 |=0x01;
          dataNodeUpdate(OWNNOTE, &palarm_own);
      }
  }
  else if(cycletime==true)
  {
      if(count>0)
      {
          count--;
          lineEditRemainTime.setText((timeCount.addSecs(count)).toString(QString("hh:mm:ss")));
      }
      else
      {
          count=abs( CyctimeEdit.time().secsTo(QTime(0,0,0)) );

          dataRead(OWNNOTE, &palarm_own);
          palarm_own.note.note4 |=0x02;
          dataNodeUpdate(OWNNOTE, &palarm_own);
      }
  }

}

void SceneSetupAdvertTimer::setCusVisible(bool vis, char* para)
{
    para = para;
    labelAbsolutTime.setVisible(vis);
    labelAbsolutTimeUnit.setVisible(vis);

    labelRelativeTime.setVisible(vis);
    labelRelativeTimeUnit.setVisible(vis);

    labelRemainTime.setVisible(vis);
    lineEditRemainTime.setVisible(vis);
    labelRemainTimeUnit.setVisible(vis);
    OnetimeEdit.setVisible(vis);
    CyctimeEdit.setVisible(vis);
    labelButtonOnetime.setVisible(vis);
    buttonOnetime.setVisible(vis);
    buttonCyctime.setVisible(vis);

    labelButtonSeleSound.setVisible(false);
    ButtonSeleSound.setVisible(false);
    ButtonSeleOption.setVisible(false);

    buttonSave.setVisible(false);
    buttonCancel.setVisible(false);
}
