#include <QtGui>
#include <QObject>
#include "config_ui.h"
#include "widget_common.h"
#include "scene_disinfect.h"
#include "state.h"
#include "global_var.h"
#include "middle.h"
#include "common_func.h"
#include "common_tr_string.h"

extern int  gChemicalDisinfect;
extern bool gAutoOff;
extern bool gRemainingTime;
extern bool gLastPrograms;
extern bool gFilterChange;
extern int gValueScale;


SceneDisinfect::SceneDisinfect(QWidget *parent) :
    QObject(parent),
    labelTitle(parent),
    labelNote(parent),
    labelNote2(parent),
    labelCountDown(parent),
    widgetTempo(parent),
    widgetTemperature(parent),
    frameLine(parent),
    widgetButtonRun(parent),
    widgetButtonReturn(parent),
    widgetButtonShutoff(parent),
    widgetDisinfectRadioButton(parent),
    topLevelLabel(parent),
    lineEditAutoOff(parent),
    lineEditRemainingTime(parent),
    textEditLastPrograms(parent),
    LastProgramsLabelTitle(parent),
    LastProgramsLabelT(parent),
    LastProgramsLabel1(parent),
    LastProgramsLabel2(parent),
    LastProgramsLabel1_type(parent),
    LastProgramsLabel2_type(parent),
    textEditFilterChange(parent),

    FilterLabelTitle(parent),
    FilterNameLabel(parent),
    FilterAllLabel(parent),
    FilterUsedLabel(parent),
    FilterRemainLabel(parent),
    AllLTimesLabel(parent),
    UsedTimesLabel(parent),
    RemainTimesLabel(parent),
    buttonFilterChange1(parent),
    buttonFilterChange2(parent),
    buttonMaskPageSwitch(parent)
{
    buttonRunState = false;

    topLevelLabel.setGeometry(QRect(779, 655, 70, 98));
    topLevelLabel.setPixmap(QPixmap(":/resource/images/additional.png"));

    widgetDisinfectRadioButton.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_X,
                WIDGET_DISINFECT_RADIOBUTTON_Y,
                WIDGET_DISINFECT_RADIOBUTTON_WIDTH,
                WIDGET_DISINFECT_RADIOBUTTON_HIGH));

    labelTitle.setGeometry(QRect(34, 21, 100, 30));
    LABEL_TITLE_STYLE(labelTitle);
    labelTitle.setText(tr("Disinfect"));

    /*labelNote.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_LABEL_X+330,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL3_Y-200,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_WIDTH+300,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_HIGH*2+10));*/
    labelNote.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_LABEL_X+330,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL3_Y-110,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_WIDTH+300,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_HIGH*2+10));
    labelNote.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    labelNote.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelNote.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    labelNote2.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_LABEL_X+330,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL3_Y-110+25,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_WIDTH+300,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_HIGH*2+10));
    labelNote2.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    labelNote2.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelNote2.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelCountDown.setGeometry(QRect(WIDGET_DISINFECT_RADIOBUTTON_LABEL_X+355,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL3_Y-110+27+15,
                25,
                WIDGET_DISINFECT_RADIOBUTTON_LABEL_HIGH));
    labelCountDown.setAlignment( Qt::AlignCenter );
    labelCountDown.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelCountDown.setStyleSheet(CONFIG_COLOR_RED_CHARS);//CONFIG_COLOR_GREEN_CHARS

    //frameLine.setGeometry(QRect(280-50, 80, 4, 584));  //中间一条黑线
    frameLine.setGeometry(QRect(280-25, 80, 4, 584));  //中间一条黑线
    frameLine.setFrameShape(QFrame::VLine);
    frameLine.setStyleSheet("color: rgb(0, 0, 0);");

    widgetTemperature.setGeometry(QRect(321-60,185,60,524));

    widgetTempo.setGeometry(QRect(WIDGET_TEMPO_X,
                WIDGET_TEMPO_Y,
                WIDGET_TEMPO_WIDTH+50,
                WIDGET_TEMPO_HIGH));
    widgetTempo.setValueTotal(200);
    widgetTempo.setValue(0);

    lineEditAutoOff.setGeometry(QRect(30,185,200,100));
    lineEditAutoOff.setStyleSheet("background-color:blue;""color:white;");
    lineEditAutoOff.setText(tr("              Auto Off : 10min"));
    lineEditRemainingTime.setGeometry(QRect(30,435,200,100));
    lineEditRemainingTime.setStyleSheet("background-color:blue;""color:white;");

    textEditLastPrograms.setGeometry(QRect(335,160,350,100));
    textEditLastPrograms.setStyleSheet("background-color:blue;""color:white;font-size:17px");
//    textEditLastPrograms.setText(tr("                  Last Programs\r\n    ")+
//                                 tr("   Date    Time    Program Type\r\n"));

    LastProgramsLabelTitle.setGeometry(QRect(435, 165, 180, 30));
    LABEL_TITLE_STYLE(LastProgramsLabelTitle);

    LastProgramsLabelT.setGeometry(QRect(410-20, 190, 240, 30));
    LastProgramsLabelT.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    LastProgramsLabelT.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    LastProgramsLabelT.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    LastProgramsLabel1.setGeometry(QRect(410-20, 210, 150, 30));
    LastProgramsLabel1.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    LastProgramsLabel1.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    LastProgramsLabel1.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    LastProgramsLabel2.setGeometry(QRect(410-20, 230, 150, 30));
    LastProgramsLabel2.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    LastProgramsLabel2.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    LastProgramsLabel2.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    LastProgramsLabel1_type.setGeometry(QRect(410+155, 210, 120, 30));
    LastProgramsLabel1_type.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    LastProgramsLabel1_type.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    LastProgramsLabel1_type.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    LastProgramsLabel2_type.setGeometry(QRect(410+155, 230, 120, 30));
    LastProgramsLabel2_type.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    LastProgramsLabel2_type.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    LastProgramsLabel2_type.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    FilterLabelTitle.setGeometry(QRect(355, 400, 350, 30));
    LABEL_TITLE_STYLE(FilterLabelTitle);

    FilterNameLabel.setGeometry(QRect(450, 400+30, 240, 30));
    FilterNameLabel.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    FilterNameLabel.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    FilterNameLabel.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    FilterAllLabel.setGeometry(QRect(350+40, 400+50, 340, 30));
    FilterAllLabel.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    FilterAllLabel.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    FilterAllLabel.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    FilterUsedLabel.setGeometry(QRect(350+20, 400+70, 340, 30));
    FilterUsedLabel.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    FilterUsedLabel.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    FilterUsedLabel.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    FilterRemainLabel.setGeometry(QRect(352, 400+90, 340, 30));
    FilterRemainLabel.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    FilterRemainLabel.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    FilterRemainLabel.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    AllLTimesLabel.setGeometry(QRect(470+20, 400+50, 120, 30));
    AllLTimesLabel.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    AllLTimesLabel.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    AllLTimesLabel.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    UsedTimesLabel.setGeometry(QRect(470+20, 400+70, 120, 30));
    UsedTimesLabel.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    UsedTimesLabel.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    UsedTimesLabel.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    RemainTimesLabel.setGeometry(QRect(470+20, 400+90, 120, 30));
    RemainTimesLabel.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    RemainTimesLabel.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RemainTimesLabel.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);
    textEditFilterChange.setGeometry(QRect(335,400,350,150+20));
    textEditFilterChange.setStyleSheet("background-color:blue;""color:white;font-size:17px");
    //textEditFilterChange.setText(tr("  Remaining Time Until Filter Change\r\n             Filter1      Filter2\r\n"));
    textEditFilterChange.setDisabled(true);
    textEditLastPrograms.setDisabled(true);
    //textEditLastPrograms.setDisabled(true);
    buttonFilterChange1.setGeometry(QRect(450,530,80,35));
    buttonFilterChange2.setGeometry(QRect(560,530,80,35));
    BUTTON_SIMPLE_STYLE(buttonFilterChange1);
    BUTTON_SIMPLE_STYLE(buttonFilterChange2);

    buttonMaskPageSwitch.setGeometry(QRect(724,0,300,680));
    buttonMaskPageSwitch.setDisabled(true);
    buttonMaskPageSwitch.setVisible(false);

    widgetButtonRun.setStyleDisabled(":/resource/images/disinfect/run.png",\
                                       ":/resource/images/disinfect/run_active.png",\
                                       ":/resource/images/disinfect/run.png",\
                                           CONFIG_COLOR_WHITE,\
                                       QRect(WIDGET_BUTTON_DISINFECT_RUN_X,\
                                                WIDGET_BUTTON_DISINFECT_RUN_Y,\
                                                    WIDGET_BUTTON_DISINFECT_RUN_WIDTH,\
                                                        WIDGET_BUTTON_DISINFECT_RUN_HIGH));

    widgetButtonRun.setDisabled(false);

    widgetButtonRun.setTextFontPos(20, CONFIG_FONT_NORMAL, 0xFF031e37,
            QRect(0, 0, WIDGET_BUTTON_DISINFECT_RUN_WIDTH,
                WIDGET_BUTTON_DISINFECT_RUN_HIGH));
    widgetButtonRun.setText(tr("Run"));

    widgetButtonReturn.setStyleNormal(":/resource/images/disinfect/return.png",
            QRect(WIDGET_BUTTON_DISINFECT_RETURN_X,
                WIDGET_BUTTON_DISINFECT_RETURN_Y,
                WIDGET_BUTTON_DISINFECT_RETURN_WIDTH,
                WIDGET_BUTTON_DISINFECT_RETURN_HIGH));
    widgetButtonReturn.setTextFontPos(20, CONFIG_FONT_NORMAL, 0xFF031e37,
            QRect(0, 0, WIDGET_BUTTON_DISINFECT_RETURN_WIDTH,
                WIDGET_BUTTON_DISINFECT_RETURN_HIGH));
//    widgetButtonReturn.setText(tr("Return"));

    widgetButtonShutoff.setStyleNormal(":/resource/images/disinfect/shutdown.png",
            QRect(WIDGET_BUTTON_DISINFECT_SHUTDOWN_X,
                WIDGET_BUTTON_DISINFECT_SHUTDOWN_Y,
                WIDGET_BUTTON_DISINFECT_SHUTDOWN_WIDTH,
                WIDGET_BUTTON_DISINFECT_SHUTDOWN_HIGH));
    widgetButtonShutoff.setTextFontPos(20, CONFIG_FONT_NORMAL, 0xFF031e37,
            QRect(0, 0, WIDGET_BUTTON_DISINFECT_SHUTDOWN_WIDTH,
                WIDGET_BUTTON_DISINFECT_SHUTDOWN_HIGH));
    widgetButtonShutoff.setText(tr("Shut Off"));

    PageAppendPiece("Disinfect", this);

    connect(&widgetButtonShutoff, SIGNAL(clicked()),
            this, SLOT(buttonShutdown()));

    connect(&widgetButtonRun, SIGNAL(clicked()),
            this, SLOT(buttonRun()));

    connect(&widgetButtonReturn, SIGNAL(clicked()),
            this, SLOT(buttonReturn()));

    widgetButtonReturn.setDisabled(true);

    connect(&buttonFilterChange1, SIGNAL(clicked()), this, SLOT(on_FilterChange_clicked()));
    connect(&buttonFilterChange2, SIGNAL(clicked()), this, SLOT(on_FilterChange_clicked()));
    connect(enginerDialog, SIGNAL(setFilterTimes(FilterTimes,FilterTimes)), this, SLOT(updateFilterTimes(FilterTimes,FilterTimes)));

    dataNodeAddObject(PACKET_MAINTAIN_PARAMS, this);
    dataNodeAddObject(PACKET_NAME_WORKSTATE, this);
    dataNodeAddObject(PACKET_STATUS_MASTALARM, this);
    //dataNodeAddObject(PACKET_STATUS_NOTE, this);
    dataNodeAddObject(PACKET_NAME_POWER, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/
    //dataNodeAddObject(PACKET_MASTERCTRL_HEAT, this);
    timer->createTimer(this, 1000);
    labelNote.setVisible(false);
    labelNote2.setVisible(false);
    N3_errorstate=0;
    model=noChooseDisinfectWorkModel;
}

SceneDisinfect::~SceneDisinfect()
{
}

void SceneDisinfect::on_FilterChange_clicked(void)
{
    if(ChangeFilterDlg &&ChangeFilterDlg->isShow())
        return;
    if(sender()==&buttonFilterChange1)  //超净滤器1#/
    {
      FilterID=1;
      showFilterDialog(DE_TR_CHANGEFILTER1,DE_TR_FILTERINFO);
    }
    else{
      FilterID=2;
      showFilterDialog(DE_TR_CHANGEFILTER2,DE_TR_FILTERINFO);
      }
    connect(ChangeFilterDlg,SIGNAL(RstSelSignal(bool)),this,SLOT(ResetFilter(bool)));  //重设超净滤器次数#/
    connect(ChangeFilterDlg,SIGNAL(DrySelSignal(bool)),this,SLOT(DryFilter(bool))); //排空超净滤器#/
    connect(ChangeFilterDlg,SIGNAL(NoteSelSignal(int)),this,SLOT(ChangeFilterCancel(int)));//取消#/

}

void SceneDisinfect::buttonShutdown()
{
    //stateTemp = GetWorkStateManage();
    //if(stateTemp.subState.current == CLEANING_FINISHED)
    //widgetButtonShutoff.setChecked(true);

    connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmShotDown(int)));
    showYesNoDialog(DE_TR_PLEASECONFIRM,DE_TR_SHOTOFF);

}

void SceneDisinfect::ConfirmShotDown(int sel)
{
    if(sel)
    {
        MasterCtrol_CMD_Shutdown();
    }

    YesNoDialog->disconnect(this);
}

void SceneDisinfect::ConfirmStop(int sel)
{
    if(sel==true)
    {
       Stop();
    }

    YesNoDialog->disconnect(this);
}
void SceneDisinfect::ConfirmDisinfectTyp(int sel)
{
    sel=sel;
}

void SceneDisinfect::buttonRun()
{
    if(N3_errorstate&0x01)  //无液报警时#/
    {
     widgetButtonRun.setChecked(false);
     showYesDialog(DE_TR_PLEASECONFIRM,DE_TR_DISINFECTNOWATER);
     return;
    }

    chooseDisinfectWorkModel = widgetDisinfectRadioButton.getDisinfectWorkModel();  //获取选择的消毒模式#/
    if(chooseDisinfectWorkModel==noChooseDisinfectWorkModel)//
    {
        widgetButtonRun.setChecked(false);
        showYesDialog(DE_TR_PLEASECONFIRM,DE_TR_SELECTDISINFECTTYP);
        //connect(YesDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmDisinfectTyp(int)));
        return;
    }

    if (buttonRunState == false)
    {
        Run();
    }
    else
    {
        widgetButtonRun.setChecked(true);
        showYesNoDialog(DE_TR_PLEASECONFIRM,DE_TR_DISINFECTSTOP);
        connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmStop(int)));
        //Stop();
    }
}

void SceneDisinfect::buttonReturn()//重启
{

}

void SceneDisinfect::Run()
{
    timeCount=0;
    buttonRunState = true;
    MarkDisinfectFinish(false);

    Disinfect_secCount=0;
    powerShutdown=false;
    setCountDown=false;
    dataNodeUpdate(Disinfect_Work_Model,&chooseDisinfectWorkModel);

   //
    switch ( chooseDisinfectWorkModel)//设置消毒参数
        {
            case cold_rinse_B://冷冲洗#/
                Disinfect.preFlush_Time=0;//params->params_ColdFlush.preFlush_Time.attribute.default_value;
                Disinfect.hold_Time=0;
                Disinfect.loop_Time=params->params_ColdFlush.loop_Time.attribute.default_value;
                Disinfect.backFlush_Time=0;
                Disinfect.flush_flux=params->params_ColdFlush.flush_flux.attribute.default_value;
                Disinfect.loop_flux=Disinfect.flush_flux;
                Disinfect.targetTemp=0;
                Disinfect.runTemp=0;
                Disinfect.cond_up=0;//;
                Disinfect.cond_down=0;
                Disinfect.suck_Volume=0;
                break;

            case hot_rinse_B://热冲洗#/
                //Disinfect.preFlush_Time=params->params_HotFlush.preFlush_Time.attribute.default_value;
                Disinfect.preFlush_Time=2;
                Disinfect.hold_Time=0;
                Disinfect.loop_Time=params->params_HotFlush.loop_Time.attribute.default_value;
                //Disinfect.backFlush_Time=params->params_HotFlush.backFlush_Time.attribute.default_value;
                Disinfect.backFlush_Time=2;
                Disinfect.flush_flux=params->params_HotFlush.flush_flux.attribute.default_value;
                Disinfect.loop_flux=Disinfect.flush_flux;
                Disinfect.targetTemp=params->params_HotFlush.targetTemp.attribute.default_value;
                Disinfect.runTemp=params->params_HotFlush.realTmep.attribute.default_value;
                Disinfect.cond_up=0;
                Disinfect.cond_down=0;
                Disinfect.suck_Volume=0;
                break;

            case x_peracetic_B://化学消毒
                if(gChemicalDisinfect == 1){
                    Disinfect.preFlush_Time=params->params_Chymistry_Peracetic.preFlush_Time.attribute.default_value;
                    Disinfect.hold_Time=params->params_Chymistry_Peracetic.hold_Time.attribute.default_value;
                    Disinfect.loop_Time=params->params_Chymistry_Peracetic.loop_Time.attribute.default_value;
                    Disinfect.backFlush_Time=params->params_Chymistry_Peracetic.backFlush_Time.attribute.default_value;
                    Disinfect.flush_flux=params->params_Chymistry_Peracetic.flush_flux.attribute.default_value;
                    Disinfect.loop_flux=params->params_Chymistry_Peracetic.loop_flux.attribute.default_value;
                    Disinfect.targetTemp=params->params_Chymistry_Peracetic.targetTemp.attribute.default_value;
                    Disinfect.runTemp=params->params_Chymistry_Peracetic.realTmep.attribute.default_value;
                    Disinfect.cond_up=params->params_Chymistry_Peracetic.conduction_MAX.attribute.default_value;
                    Disinfect.cond_down=params->params_Chymistry_Peracetic.conduction_MIN.attribute.default_value;
                    Disinfect.suck_Volume=params->params_Chymistry_Peracetic.suck_flux.attribute.default_value;
                }
                else if(gChemicalDisinfect == 2){
                    Disinfect.preFlush_Time=params->params_Chymistry_Citric.preFlush_Time.attribute.default_value;
                    Disinfect.hold_Time=params->params_Chymistry_Citric.hold_Time.attribute.default_value;
                    Disinfect.loop_Time=params->params_Chymistry_Citric.loop_Time.attribute.default_value;
                    Disinfect.backFlush_Time=params->params_Chymistry_Citric.backFlush_Time.attribute.default_value;
                    Disinfect.flush_flux=params->params_Chymistry_Citric.flush_flux.attribute.default_value;
                    Disinfect.loop_flux=params->params_Chymistry_Citric.loop_flux.attribute.default_value;
                    Disinfect.targetTemp=params->params_Chymistry_Citric.targetTemp.attribute.default_value;
                    Disinfect.runTemp=params->params_Chymistry_Citric.realTmep.attribute.default_value;
                    Disinfect.cond_up=params->params_Chymistry_Citric.conduction_MAX.attribute.default_value;
                    Disinfect.cond_down=params->params_Chymistry_Citric.conduction_MIN.attribute.default_value;
                    Disinfect.suck_Volume=params->params_Chymistry_Citric.suck_flux.attribute.default_value;
                }
                else if(gChemicalDisinfect == 3){
                    Disinfect.preFlush_Time=params->params_Chymistry_Reserve3.preFlush_Time.attribute.default_value;
                    Disinfect.hold_Time=params->params_Chymistry_Reserve3.hold_Time.attribute.default_value;
                    Disinfect.loop_Time=params->params_Chymistry_Reserve3.loop_Time.attribute.default_value;
                    Disinfect.backFlush_Time=params->params_Chymistry_Reserve3.backFlush_Time.attribute.default_value;
                    Disinfect.flush_flux=params->params_Chymistry_Reserve3.flush_flux.attribute.default_value;
                    Disinfect.loop_flux=params->params_Chymistry_Reserve3.loop_flux.attribute.default_value;
                    Disinfect.targetTemp=params->params_Chymistry_Reserve3.targetTemp.attribute.default_value;
                    Disinfect.runTemp=params->params_Chymistry_Reserve3.realTmep.attribute.default_value;
                    Disinfect.cond_up=params->params_Chymistry_Reserve3.conduction_MAX.attribute.default_value;
                    Disinfect.cond_down=params->params_Chymistry_Reserve3.conduction_MIN.attribute.default_value;
                    Disinfect.suck_Volume=params->params_Chymistry_Reserve3.suck_flux.attribute.default_value;
                }
                else{
                    return ;
                }
                break;

            case x_citricAcid_B://柠檬酸
                Disinfect.preFlush_Time=params->params_Chymistry_Citric.preFlush_Time.attribute.default_value;
                Disinfect.hold_Time=params->params_Chymistry_Citric.hold_Time.attribute.default_value;
                Disinfect.loop_Time=params->params_Chymistry_Citric.loop_Time.attribute.default_value;
                Disinfect.backFlush_Time=params->params_Chymistry_Citric.backFlush_Time.attribute.default_value;
                Disinfect.flush_flux=params->params_Chymistry_Citric.flush_flux.attribute.default_value;
                Disinfect.loop_flux=params->params_Chymistry_Citric.loop_flux.attribute.default_value;
                Disinfect.targetTemp=params->params_Chymistry_Citric.targetTemp.attribute.default_value;
                Disinfect.runTemp=params->params_Chymistry_Citric.realTmep.attribute.default_value;
                Disinfect.cond_up=params->params_Chymistry_Citric.conduction_MAX.attribute.default_value;
                Disinfect.cond_down=params->params_Chymistry_Citric.conduction_MIN.attribute.default_value;
                Disinfect.suck_Volume=params->params_Chymistry_Citric.suck_flux.attribute.default_value;
                break;
            case x_sodiumHypochlorite_B://化(次氯酸钠) (化3)
                Disinfect.preFlush_Time=params->params_Chymistry_Reserve3.preFlush_Time.attribute.default_value;
                Disinfect.hold_Time=params->params_Chymistry_Reserve3.hold_Time.attribute.default_value;
                Disinfect.loop_Time=params->params_Chymistry_Reserve3.loop_Time.attribute.default_value;
                Disinfect.backFlush_Time=params->params_Chymistry_Reserve3.backFlush_Time.attribute.default_value;
                Disinfect.flush_flux=params->params_Chymistry_Reserve3.flush_flux.attribute.default_value;
                Disinfect.loop_flux=params->params_Chymistry_Reserve3.loop_flux.attribute.default_value;
                Disinfect.targetTemp=params->params_Chymistry_Reserve3.targetTemp.attribute.default_value;
                Disinfect.runTemp=params->params_Chymistry_Reserve3.realTmep.attribute.default_value;
                Disinfect.cond_up=params->params_Chymistry_Reserve3.conduction_MAX.attribute.default_value;
                Disinfect.cond_down=params->params_Chymistry_Reserve3.conduction_MIN.attribute.default_value;
                Disinfect.suck_Volume=params->params_Chymistry_Reserve3.suck_flux.attribute.default_value;
                break;

          case h_citricAcid_B://热柠檬酸
                Disinfect.preFlush_Time=params->params_Hot_Citric.preFlush_Time.attribute.default_value;
                Disinfect.hold_Time=params->params_Hot_Citric.hold_Time.attribute.default_value;
                Disinfect.loop_Time=params->params_Hot_Citric.loop_Time.attribute.default_value;
                Disinfect.backFlush_Time=params->params_Hot_Citric.backFlush_Time.attribute.default_value;
                Disinfect.flush_flux=params->params_Hot_Citric.flush_flux.attribute.default_value;
                Disinfect.loop_flux=params->params_Hot_Citric.loop_flux.attribute.default_value;
                Disinfect.targetTemp=params->params_Hot_Citric.targetTemp.attribute.default_value;
                Disinfect.runTemp=params->params_Hot_Citric.realTmep.attribute.default_value;
                Disinfect.cond_up=params->params_Hot_Citric.conduction_MAX.attribute.default_value;
                Disinfect.cond_down=params->params_Hot_Citric.conduction_MIN.attribute.default_value;
                Disinfect.suck_Volume=params->params_Hot_Citric.suck_flux.attribute.default_value;
                break;

            case h_water_B:
                Disinfect.preFlush_Time=params->params_Hot.preFlush_Time.attribute.default_value;
                Disinfect.hold_Time=0;//params->params_Hot.hold_Time.attribute.default_value;
                Disinfect.loop_Time=params->params_Hot.loop_Time.attribute.default_value;
                Disinfect.backFlush_Time=params->params_Hot.backFlush_Time.attribute.default_value;
                Disinfect.flush_flux=params->params_Hot.flush_flux.attribute.default_value;
                Disinfect.loop_flux=params->params_Hot.loop_flux.attribute.default_value;
                Disinfect.targetTemp=params->params_Hot.targetTemp.attribute.default_value;
                Disinfect.runTemp=params->params_Hot.realTmep.attribute.default_value;
//                Disinfect.cond_up=params->params_Hot.conduction_MAX.attribute.default_value;
//                Disinfect.cond_down=params->params_Hot.conduction_MIN.attribute.default_value;
//                Disinfect.suck_Volume=params->params_Hot.suck_volume.attribute.default_value;
                break;

            default:
                break;
        }


    if(chooseDisinfectWorkModel==cold_rinse_B ||
       chooseDisinfectWorkModel==hot_rinse_B   )
        ShutOffDelay=15*60;//
    else ShutOffDelay=5*60;

    //将工程模式的冲洗电导赋值
    disinfectCond.B_cond=params->clean_cond.b_cond.attribute.default_value;
    disinfectCond.Mix_cond=params->clean_cond.mix_cond.attribute.default_value;
    disinfectCond.out_cond=params->clean_cond.out_cond.attribute.default_value;

    insertRecord(chooseDisinfectWorkModel);

    widgetButtonRun.setText(tr("stop"));
    widgetTempo.setVisible(true);
    widgetTemperature.setVisible(true);
    widgetDisinfectRadioButton.setVisible(false);
    labelNote.setVisible(false);
    labelNote2.setVisible(false);
    labelCountDown.setVisible(false);
    //if(gAutoOff == 1)  lineEditAutoOff.setVisible(true);
    //else if(gAutoOff == 0) lineEditAutoOff.setVisible(false);
    textEditLastPrograms.setVisible(false);
    textEditFilterChange.setVisible(false);
    buttonFilterChange1.setVisible(false);
    buttonFilterChange2.setVisible(false);
    //workstate.current = cool_rinse_run;
    int modeltmp=0;  //消毒的模式#/
    int steptmp=0;   //当前消毒模式的阶段#/

    stateTemp = GetWorkStateManage();
    switch (chooseDisinfectWorkModel)//根据消毒方式设置当前状态，设置PDO（N1_blood_status）
        {
           case cold_rinse_B:
                    modeltmp=coldrinse;
                    steptmp=COLDRINSE;
                    widgetTempo.labelTitle.setText(tr("Rinse"));
//                    PDOstate.state.N1_blood_status =0x71;
                    workstate.current = COOL_RINSE;//cool_rinse_run
                    break;
            case hot_rinse_B:
                     modeltmp=hotrinse1;
                     steptmp=HOTRINSE1;
                     widgetTempo.labelTitle.setText(tr("Hot Rinse"));
                     workstate.current = HOT_RINSE;//hot_rinse_run;
//                     PDOstate.state.N1_blood_status =0x72;
                    break;

            case x_peracetic_B://化学消毒
                     modeltmp=preflush;
                     steptmp=PRERINSE;

                     //workstate.current = chemical_Disinfect_run;
                     workstate.current = CHEMICAL_PERACETIC;//chemical_Disinfect_run;

                     if(gChemicalDisinfect == 1){
                        widgetTempo.labelTitle.setText(tr("Chymistry Peracetic"));
                     }
                     else if(gChemicalDisinfect == 2){
                         widgetTempo.labelTitle.setText(tr("Chymistry Citric"));
                      }
                     else if(gChemicalDisinfect == 3){
                         widgetTempo.labelTitle.setText(tr("Chymistry Reserve3"));
                      }
                     break;
            case x_sodiumHypochlorite_B://化(次氯酸钠)#/
                     modeltmp=preflush;
                     steptmp=PRERINSE;
                     workstate.current = CHEMICAL3;//chemical_Disinfect_run;
                     widgetTempo.labelTitle.setText(tr("Chloros"));
                     break;
            case x_citricAcid_B://化(柠檬酸)#/
                     modeltmp=preflush;
                     steptmp=PRERINSE;
                     workstate.current = CHEMICAL_CITRICACID;//chemical_Disinfect_run;
                     widgetTempo.labelTitle.setText(tr("Sodium citrate"));
                     break;
            case h_citricAcid_B: //热(柠檬酸)#/
                     modeltmp=preflush;  //当前运行阶段标示 改变Disinfect.step#/
                     steptmp=PRERINSE;  //水路状态 改变PDOstate.state.N1_water_status#/
                     workstate.current = HOT_CHEMICAL;//消毒模式，改变PDOstate.state.N1_blood_status #/
                     widgetTempo.labelTitle.setText(tr("Sodium citrate 85"));
                     break;
            case h_water_B: //热(纯水)#/
                     modeltmp=preflush;//当前运行阶段标示 改变Disinfect.step#/
                     steptmp=PRERINSE; //水路状态 改变PDOstate.state.N1_water_status#/
                     workstate.current = HOT;//hot_Disinfect_run;
                     widgetTempo.labelTitle.setText(tr("hot Disinfect"));
                     break;
            default:
                break;
        }

    stateTemp.tipState.previous = stateTemp.tipState.current;
    stateTemp.tipState.current = CLEANING;//总状态设为清洁状态
    stateTemp.bloodState.current = workstate.current;//消毒的模式#/
    stateTemp.subState.current = CLEANING_RUN;   //消毒子状态#/
    stateTemp.waterState.current = steptmp;  //水路状态#/
    SetWorkStateManage(&stateTemp);

    Disinfect.step=modeltmp;  //消毒的阶段#/

    //PDOstate.state.N1_status =0x07;
    //PDOstate.state.N1_water_status=0x70;//
    /*
    WorkState stateTMP = GetWorkStateManage();
    stateTMP.tipState.current = CLEANING;
    stateTMP.waterState.current = CLEANING_READY;
    */

    //workstate.tip= Disinfect_run;
    //dataNodeUpdate(PACKET_NAME_WORKSTATE, &workstate);

    /*
    stateTemp = GetWorkStateManage();
    stateTemp.tipState.current = CLEANING;
    stateTemp.bloodState.current = workstate.current;
    stateTemp.subState.current = CLEANING_RUN;
    stateTemp.waterState.current = steptmp;
    SetWorkStateManage(&stateTemp);
    */

    us_sleep(6000);
    dataNodeUpdate(Disinfect_Work_Model,&chooseDisinfectWorkModel);
    //timer->createTimer(this, 1000);

    us_sleep(10000);
    MasterCtrol_CMD_Disinfect_start(modeltmp);
    //Disinfect.step=modeltmp;
    us_sleep(6000);

    StateSwitch("Disinfect");     //进入消毒处理循环#/

    timeCount=0;
    //根据消毒模式的总运行时间设置扇面图
    if(modeltmp==hotrinse1)
      widgetTempo.setValueTotal((Disinfect.loop_Time+Disinfect.backFlush_Time) *60);
    else
      widgetTempo.setValueTotal((Disinfect.preFlush_Time+Disinfect.hold_Time+Disinfect.loop_Time+Disinfect.backFlush_Time) *60);

    widgetTempo.setValue(0);//当前已进行的消毒时间#/

    if(gRemainingTime == 1)
    {
      int tmp;
      if(modeltmp==hotrinse1)
      {
        tmp = (100-gValueScale)*(Disinfect.loop_Time+Disinfect.backFlush_Time) *60/100;
      }
      else
      {
        tmp = (100-gValueScale)*(Disinfect.preFlush_Time+Disinfect.hold_Time+Disinfect.loop_Time+Disinfect.backFlush_Time) *60/100;
      }
      QString str = QString::number(tmp,10);
      str = tr("         Remaining Time : ") + str + tr("min");
      lineEditRemainingTime.setText(str);  //剩余的时间#/
      lineEditRemainingTime.setVisible(true);
    }
    else if(gRemainingTime == 0)
    {
      lineEditRemainingTime.setVisible(false);
    }

    MarkDisinfectFinish(false);//记录消毒状态（正在进行消毒）
    setCusVisible(true);            //主要屏蔽其它页面切换按钮
    #if 0
     Message mess;
     mess.cob_id = 0x183;
     mess.data[0] = 0x01;
     mess.data[1] = 0x04;
     mess.data[2] = 0x13;
     mess.data[3] = 0x46;
     mess.data[4] = 0x09;
     mess.data[5] = 0;
     mess.data[6] = 0;
     mess.data[7] = 0;
     mess.len = 8;
     MessageSignalMiddle(&mess);
     #endif

     logInfo log;
     //log.logId=1;
     log.opration="Begin Clean Program";
     log.type=DE_LOGINFO_OPER;
     log.status=PDOstate.state.N1_status;
     log.note="Null";
     LogRun->addLog(log);
}

void SceneDisinfect::Stop()
{
    buttonRunState = false;
    //qDebug()<<"widgetButtonRun text is " <<str;
    //labelNote.show();
    widgetButtonRun.setText(tr("Run"));
    widgetButtonRun.setChecked(false);
    widgetButtonRun.update();

    if (strcmp(PageCurrent(), "Disinfect") == 0)//只有在消毒界面有效
    {
        widgetTempo.setVisible(false);
        widgetTemperature.setVisible(false);
        lineEditAutoOff.setVisible(false);
        lineEditRemainingTime.setVisible(false);
        if(gLastPrograms==1)
        {
            textEditLastPrograms.setVisible(true);
            LastProgramsLabelTitle.setVisible(true);
            LastProgramsLabelT.setVisible(true);
            LastProgramsLabel1.setVisible(true);
            LastProgramsLabel2.setVisible(true);
            LastProgramsLabel1_type.setVisible(true);
            LastProgramsLabel2_type.setVisible(true);

        }
        else if(gLastPrograms==0)
        {
            textEditLastPrograms.setVisible(false);
            LastProgramsLabelTitle.setVisible(false);
            LastProgramsLabelT.setVisible(false);
            LastProgramsLabel1.setVisible(false);
            LastProgramsLabel2.setVisible(false);
            LastProgramsLabel1_type.setVisible(false);
            LastProgramsLabel2_type.setVisible(false);

        }
        if(gFilterChange==1)
        {
            textEditFilterChange.setVisible(true);
            FilterLabelTitle.setVisible(true);
            FilterAllLabel.setVisible(true);
            FilterUsedLabel.setVisible(true);
            FilterRemainLabel.setVisible(true);
            FilterNameLabel.setVisible(true);
            AllLTimesLabel.setVisible(true);
            UsedTimesLabel.setVisible(true);
            RemainTimesLabel.setVisible(true);
        }
        else if(gFilterChange==0)
        {
            textEditFilterChange.setVisible(false);
            FilterLabelTitle.setVisible(false);
            FilterAllLabel.setVisible(false);
            FilterUsedLabel.setVisible(false);
            FilterRemainLabel.setVisible(false);
            FilterNameLabel.setVisible(false);
            AllLTimesLabel.setVisible(false);
            UsedTimesLabel.setVisible(false);
            RemainTimesLabel.setVisible(false);
        }
        if(gFilterChange==1)
        {
            buttonFilterChange1.setVisible(true);
            buttonFilterChange2.setVisible(true);
        }
        else if(gFilterChange==0)
        {
            buttonFilterChange1.setVisible(false);
            buttonFilterChange2.setVisible(false);
        }

        widgetDisinfectRadioButton.setVisible(true);
       if(GetWorkStateManage().subState.current == CLEANING_FINISHED){
        labelCountDown.setText(QString().number(ShutOffDelay/60));//tr("10")
        labelNote.setVisible(true);
        labelNote2.setVisible(true);
        labelCountDown.setVisible(true);
        }
     }

    stateTemp=GetWorkStateManage();
    stateTemp.tipState.current=CLEANING;
    //if(workstate.tip ==Disinfect_finish)
    if(stateTemp.subState.current == CLEANING_FINISHED)
    {
     //timer->stopTimer(this);
    //workstate.tip= Disinfect_stop;
    //stateTemp.subState.current = CLEANING_RUN;
    MarkDisinfectFinish(true);   //消毒完成#/
    }
    else
    {
        //workstate.current= Disinfect_stop;
        //stateTemp.tipState.current = CLEANING_STOP;

        stateTemp.subState.current = CLEANING_STOP;

        //dataNodeUpdate(PACKET_NAME_WORKSTATE, &workstate);
     }

    SetWorkStateManage(&stateTemp);

    timePoint = QTime::currentTime();
    MasterCtrol_CMD_Disinfect_stop();
   #if 0
    Message mess;
    mess.cob_id = 0x183;
    mess.data[0] = 0x01;
    mess.data[1] = 0xf;
    mess.data[2] = 0x13;
    mess.data[3] = 0x46;
    mess.data[4] = 0x0;
    mess.data[5] = 0;
    mess.data[6] = 0;
    mess.data[7] = 0;
    mess.len = 8;
    MessageSignalMiddle(&mess);
    #endif
    ShowLastRecord();
}

void SceneDisinfect::CustomSetText()
{
    labelTitle.setText(tr("Disinfect"));

    if (buttonRunState == false)
    {
        widgetButtonRun.setText(tr("Run"));

    }
    else
    {
        widgetButtonRun.setText(tr("Stop"));
    }

 //   widgetButtonReturn.setText(tr("Return"));
    widgetButtonShutoff.setText(tr("Shut Off"));

    LastProgramsLabelTitle.setText(tr("Last Programs"));
    LastProgramsLabelT.setText(tr("     Date            Time          Program Type"));
    ShowLastRecord();

    FilterLabelTitle.setText(tr("Remaining Time Until Filter Change"));
    FilterNameLabel.setText(    tr("     Filter1           Filter2"));
    FilterAllLabel.setText(      tr("All Times :                                            "));
    FilterUsedLabel.setText(    tr("Used Times :                                            "));
    FilterRemainLabel.setText(tr("Remain Times :                                            "));
    buttonFilterChange1.setText(tr("Filter Change"));
    buttonFilterChange2.setText(tr("Filter Change"));
    ShowFilterTimes();

//    FilterLabelTitle.setText(tr("Remaining Time Until Filter Change"));
//    FilterNameLabel.setText(    tr("     Filter1           Filter2"));
//    FilterAllLabel.setText(    tr("     All Times :           days"));
//    FilterUsedLabel.setText(       tr("Used Times :           days"));
//    FilterRemainLabel.setText(tr("Remaining Times :           days"));
//    buttonFilterChange1.setText(tr("Filter Change"));
//    buttonFilterChange2.setText(tr("Filter Change"));
//    //labelNote.setText(tr("Disinfect completed! Machine will shutdown automatically \r\n in      minutes if no operation !"));
    labelNote.setText(tr("Disinfect completed! Machine will shutdown automatically"));
    labelNote2.setText(tr("in          minutes if no operation !"));

}

void SceneDisinfect::setCusVisible(bool vis, char* para)
{
    para = para;
    frameLine.setVisible(vis);
    widgetTempo.setVisible(vis);
    widgetTemperature.setVisible(vis);
    widgetButtonRun.setVisible(vis);
    widgetButtonReturn.setVisible(vis);
    widgetButtonShutoff.setVisible(vis);
    labelTitle.setVisible(vis);

    if(vis)
      widgetDisinfectRadioButton.slectModel(widgetDisinfectRadioButton.getDisinfectWorkModel());//选择默认模式

    if(GetWorkStateManage().subState.current==CLEANING_FINISHED)
    {
      labelNote.setVisible(vis);//
      labelNote2.setVisible(vis);//
      labelCountDown.setVisible(vis);//
    }

    widgetDisinfectRadioButton.setVisible(vis);
    topLevelLabel.setVisible(vis);
    lineEditAutoOff.setVisible(false);
    lineEditRemainingTime.setVisible(false);

    if(gLastPrograms==1 &&!buttonRunState)
    {
        textEditLastPrograms.setVisible(vis);
        LastProgramsLabelTitle.setVisible(vis);
        LastProgramsLabelT.setVisible(vis);
        LastProgramsLabel1.setVisible(vis);
        LastProgramsLabel2.setVisible(vis);
        LastProgramsLabel1_type.setVisible(vis);
        LastProgramsLabel2_type.setVisible(vis);

    }
    else if(gLastPrograms==0 ||buttonRunState)
    {
        textEditLastPrograms.setVisible(false);
        LastProgramsLabelTitle.setVisible(false);
        LastProgramsLabelT.setVisible(false);
        LastProgramsLabel1.setVisible(false);
        LastProgramsLabel2.setVisible(false);
        LastProgramsLabel1_type.setVisible(false);
        LastProgramsLabel2_type.setVisible(false);

   }
    if(gFilterChange==1 &&!buttonRunState)
    {
        textEditFilterChange.setVisible(vis);
        FilterLabelTitle.setVisible(vis);
        FilterAllLabel.setVisible(vis);
        FilterUsedLabel.setVisible(vis);
        FilterRemainLabel.setVisible(vis);
        FilterNameLabel.setVisible(vis);
        AllLTimesLabel.setVisible(vis);
        UsedTimesLabel.setVisible(vis);
        RemainTimesLabel.setVisible(vis);
    }
    else if(gFilterChange==0 ||buttonRunState)
    {
        textEditFilterChange.setVisible(false);
        FilterLabelTitle.setVisible(false);
        FilterAllLabel.setVisible(false);
        FilterUsedLabel.setVisible(false);
        FilterRemainLabel.setVisible(false);
        FilterNameLabel.setVisible(false);
        AllLTimesLabel.setVisible(false);
        UsedTimesLabel.setVisible(false);
        RemainTimesLabel.setVisible(false);
    }

    if(gFilterChange==1 &&!buttonRunState)
    {
        buttonFilterChange1.setVisible(vis);
        buttonFilterChange2.setVisible(vis);
    }
    else if(gFilterChange==0 ||buttonRunState)
    {
        buttonFilterChange1.setVisible(false);
        buttonFilterChange2.setVisible(false);
    }

    if ( (vis == true) && (buttonRunState == true))
    {
        widgetDisinfectRadioButton.setVisible(false);
        labelNote.setVisible(false);
        labelNote2.setVisible(false);
        labelCountDown.setVisible(false);
    }

    if ( (vis == true) && (buttonRunState == false))
    {
        widgetTempo.setVisible(false);
        widgetTemperature.setVisible(false);
        //labelNote.setVisible(true);
        //labelCountDown.setVisible(true);
    }
    WorkState workstate = GetWorkStateManage();
    if(CLEANING == workstate.tipState.current || CLEANING == workstate.tipState.previous){
        return;
        buttonMaskPageSwitch.setFlat(true);//如果曾经启动过消毒，或者当前处于消毒状态，则屏蔽其它按钮，一直处于消毒界面
        buttonMaskPageSwitch.setVisible(true);
        buttonMaskPageSwitch.raise();
    }

    if(vis==true)
    {
        ShowFilterTimes();
    }
}

void SceneDisinfect::ActionHandle(const void* dataPtr, int name)
{
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if (name==PACKET_NAME_WORKSTATE) {
              //workstate  = *((WorkState*) dataPtr);
             //if(workstate.tip==Disinfect_finish)
              stateTemp=GetWorkStateManage();

              if(stateTemp.subState.current==CLEANING_FINISHED && buttonRunState == true)
              {
                 Stop();
                 setCountDown=true;
              }
              else if(stateTemp.subState.current==CLEANING_STOP && buttonRunState == true)
              {
                 Stop();
              }
    }
    else if (name==PACKET_STATUS_MASTALARM){
        PacketStatusAlarm *p = (PacketStatusAlarm *) dataPtr;
        N3_errorstate=p->alarm.errorstate1;  //无液报警#/
        if((N3_errorstate&0x01)&&GetWorkStateManage().subState.current==CLEANING_RUN)
        {
         Stop();
        }
    }
    else if (name==PACKET_NAME_POWER){  //电源处理
       PacketPower p= *(PacketPower*) dataPtr;   //后备电源不消毒#/
       if((p.powerstate1==0x03)&&GetWorkStateManage().subState.current==CLEANING_RUN)
       {
        Stop();
       }
    }
    else if (name==PACKET_MAINTAIN_PARAMS) {
                params = (Params*)dataPtr;

               switch ( chooseDisinfectWorkModel)
                   {
                       case cold_rinse_B:
                           //Disinfect.preFlush_Time=params->params_ColdFlush.preFlush_Time.attribute.default_value;
                           Disinfect.preFlush_Time=10;
                           Disinfect.hold_Time=0;
                           Disinfect.loop_Time=params->params_ColdFlush.loop_Time.attribute.default_value;
                           Disinfect.backFlush_Time=0;
                           Disinfect.flush_flux=params->params_ColdFlush.flush_flux.attribute.default_value;
                           Disinfect.loop_flux=Disinfect.flush_flux;
                           Disinfect.targetTemp=0;
                           Disinfect.runTemp=0;
                           Disinfect.cond_up=0;
                           Disinfect.cond_down=0;
                           Disinfect.suck_Volume=0;
                           break;

                       case hot_rinse_B:
                           //Disinfect.preFlush_Time=params->params_HotFlush.preFlush_Time.attribute.default_value;
                           Disinfect.preFlush_Time=2;
                           Disinfect.hold_Time=0;
                           Disinfect.loop_Time=params->params_HotFlush.loop_Time.attribute.default_value;
                           //Disinfect.backFlush_Time=params->params_HotFlush.backFlush_Time.attribute.default_value;
                           Disinfect.backFlush_Time=2;
                           Disinfect.flush_flux=params->params_HotFlush.flush_flux.attribute.default_value;
                           Disinfect.loop_flux=Disinfect.flush_flux;
                           Disinfect.targetTemp=params->params_HotFlush.targetTemp.attribute.default_value;
                           Disinfect.runTemp=params->params_HotFlush.realTmep.attribute.default_value;
                           Disinfect.cond_up=0;
                           Disinfect.cond_down=0;
                           Disinfect.suck_Volume=0;
                           break;

                       case x_peracetic_B://化学消毒
                           if(gChemicalDisinfect == 1){
                               Disinfect.preFlush_Time=params->params_Chymistry_Peracetic.preFlush_Time.attribute.default_value;
                               Disinfect.hold_Time=params->params_Chymistry_Peracetic.hold_Time.attribute.default_value;
                               Disinfect.loop_Time=params->params_Chymistry_Peracetic.loop_Time.attribute.default_value;
                               Disinfect.backFlush_Time=params->params_Chymistry_Peracetic.backFlush_Time.attribute.default_value;
                               Disinfect.flush_flux=params->params_Chymistry_Peracetic.flush_flux.attribute.default_value;
                               Disinfect.loop_flux=params->params_Chymistry_Peracetic.loop_flux.attribute.default_value;
                               Disinfect.targetTemp=params->params_Chymistry_Peracetic.targetTemp.attribute.default_value;
                               Disinfect.runTemp=params->params_Chymistry_Peracetic.realTmep.attribute.default_value;
                               Disinfect.cond_up=params->params_Chymistry_Peracetic.conduction_MAX.attribute.default_value;
                               Disinfect.cond_down=params->params_Chymistry_Peracetic.conduction_MIN.attribute.default_value;
                               Disinfect.suck_Volume=params->params_Chymistry_Peracetic.suck_flux.attribute.default_value;
                           }
                           else if(gChemicalDisinfect == 2){
                               Disinfect.preFlush_Time=params->params_Chymistry_Citric.preFlush_Time.attribute.default_value;
                               Disinfect.hold_Time=params->params_Chymistry_Citric.hold_Time.attribute.default_value;
                               Disinfect.loop_Time=params->params_Chymistry_Citric.loop_Time.attribute.default_value;
                               Disinfect.backFlush_Time=params->params_Chymistry_Citric.backFlush_Time.attribute.default_value;
                               Disinfect.flush_flux=params->params_Chymistry_Citric.flush_flux.attribute.default_value;
                               Disinfect.loop_flux=params->params_Chymistry_Citric.loop_flux.attribute.default_value;
                               Disinfect.targetTemp=params->params_Chymistry_Citric.targetTemp.attribute.default_value;
                               Disinfect.runTemp=params->params_Chymistry_Citric.realTmep.attribute.default_value;
                               Disinfect.cond_up=params->params_Chymistry_Citric.conduction_MAX.attribute.default_value;
                               Disinfect.cond_down=params->params_Chymistry_Citric.conduction_MIN.attribute.default_value;
                               Disinfect.suck_Volume=params->params_Chymistry_Citric.suck_flux.attribute.default_value;
                           }
                           else if(gChemicalDisinfect == 3){
                               Disinfect.preFlush_Time=params->params_Chymistry_Reserve3.preFlush_Time.attribute.default_value;
                               Disinfect.hold_Time=params->params_Chymistry_Reserve3.hold_Time.attribute.default_value;
                               Disinfect.loop_Time=params->params_Chymistry_Reserve3.loop_Time.attribute.default_value;
                               Disinfect.backFlush_Time=params->params_Chymistry_Reserve3.backFlush_Time.attribute.default_value;
                               Disinfect.flush_flux=params->params_Chymistry_Reserve3.flush_flux.attribute.default_value;
                               Disinfect.loop_flux=params->params_Chymistry_Reserve3.loop_flux.attribute.default_value;
                               Disinfect.targetTemp=params->params_Chymistry_Reserve3.targetTemp.attribute.default_value;
                               Disinfect.runTemp=params->params_Chymistry_Reserve3.realTmep.attribute.default_value;
                               Disinfect.cond_up=params->params_Chymistry_Reserve3.conduction_MAX.attribute.default_value;
                               Disinfect.cond_down=params->params_Chymistry_Reserve3.conduction_MIN.attribute.default_value;
                               Disinfect.suck_Volume=params->params_Chymistry_Reserve3.suck_flux.attribute.default_value;
                           }
                           else{
                               return ;
                           }

                       case x_citricAcid_B://柠檬酸
                           Disinfect.preFlush_Time=params->params_Chymistry_Citric.preFlush_Time.attribute.default_value;
                           Disinfect.hold_Time=params->params_Chymistry_Citric.hold_Time.attribute.default_value;
                           Disinfect.loop_Time=params->params_Chymistry_Citric.loop_Time.attribute.default_value;
                           Disinfect.backFlush_Time=params->params_Chymistry_Citric.backFlush_Time.attribute.default_value;
                           Disinfect.flush_flux=params->params_Chymistry_Citric.flush_flux.attribute.default_value;
                           Disinfect.loop_flux=params->params_Chymistry_Citric.loop_flux.attribute.default_value;
                           Disinfect.targetTemp=params->params_Chymistry_Citric.targetTemp.attribute.default_value;
                           Disinfect.runTemp=params->params_Chymistry_Citric.realTmep.attribute.default_value;
                           Disinfect.cond_up=params->params_Chymistry_Citric.conduction_MAX.attribute.default_value;
                           Disinfect.cond_down=params->params_Chymistry_Citric.conduction_MIN.attribute.default_value;
                           Disinfect.suck_Volume=params->params_Chymistry_Citric.suck_flux.attribute.default_value;
                           break;
                       case x_sodiumHypochlorite_B://化(次氯酸钠) (化3)
                           Disinfect.preFlush_Time=params->params_Chymistry_Reserve3.preFlush_Time.attribute.default_value;
                           Disinfect.hold_Time=params->params_Chymistry_Reserve3.hold_Time.attribute.default_value;
                           Disinfect.loop_Time=params->params_Chymistry_Reserve3.loop_Time.attribute.default_value;
                           Disinfect.backFlush_Time=params->params_Chymistry_Reserve3.backFlush_Time.attribute.default_value;
                           Disinfect.flush_flux=params->params_Chymistry_Reserve3.flush_flux.attribute.default_value;
                           Disinfect.loop_flux=params->params_Chymistry_Reserve3.loop_flux.attribute.default_value;
                           Disinfect.targetTemp=params->params_Chymistry_Reserve3.targetTemp.attribute.default_value;
                           Disinfect.runTemp=params->params_Chymistry_Reserve3.realTmep.attribute.default_value;
                           Disinfect.cond_up=params->params_Chymistry_Reserve3.conduction_MAX.attribute.default_value;
                           Disinfect.cond_down=params->params_Chymistry_Reserve3.conduction_MIN.attribute.default_value;
                           Disinfect.suck_Volume=params->params_Chymistry_Reserve3.suck_flux.attribute.default_value;
                           break;

                     case h_citricAcid_B://热柠檬酸
                           Disinfect.preFlush_Time=params->params_Hot_Citric.preFlush_Time.attribute.default_value;
                           Disinfect.hold_Time=params->params_Hot_Citric.hold_Time.attribute.default_value;
                           Disinfect.loop_Time=params->params_Hot_Citric.loop_Time.attribute.default_value;
                           Disinfect.backFlush_Time=params->params_Hot_Citric.backFlush_Time.attribute.default_value;
                           Disinfect.flush_flux=params->params_Hot_Citric.flush_flux.attribute.default_value;
                           Disinfect.loop_flux=params->params_Hot_Citric.loop_flux.attribute.default_value;
                           Disinfect.targetTemp=params->params_Hot_Citric.targetTemp.attribute.default_value;
                           Disinfect.runTemp=params->params_Hot_Citric.realTmep.attribute.default_value;
                           Disinfect.cond_up=params->params_Hot_Citric.conduction_MAX.attribute.default_value;
                           Disinfect.cond_down=params->params_Hot_Citric.conduction_MIN.attribute.default_value;
                           Disinfect.suck_Volume=params->params_Hot_Citric.suck_flux.attribute.default_value;
                           break;

                       case h_water_B:
                           Disinfect.preFlush_Time=params->params_Hot.preFlush_Time.attribute.default_value;
//                           Disinfect.hold_Time=params->params_Hot.hold_Time.attribute.default_value;
                           Disinfect.loop_Time=params->params_Hot.loop_Time.attribute.default_value;
                           Disinfect.backFlush_Time=params->params_Hot.backFlush_Time.attribute.default_value;
                           Disinfect.flush_flux=params->params_Hot.flush_flux.attribute.default_value;
                           Disinfect.loop_flux=params->params_Hot.loop_flux.attribute.default_value;
                           Disinfect.targetTemp=params->params_Hot.targetTemp.attribute.default_value;
                           Disinfect.runTemp=params->params_Hot.realTmep.attribute.default_value;
//                           Disinfect.cond_up=params->params_Hot.conduction_MAX.attribute.default_value;
//                           Disinfect.cond_down=params->params_Hot.conduction_MIN.attribute.default_value;
//                           Disinfect.suck_Volume=params->params_Hot.suck_volume.attribute.default_value;
                           break;

                       default:
                           break;
                   }

               dataNodeUpdate(Disinfect_Work_Model,&chooseDisinfectWorkModel);

    }
    else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        Disinfect=G_P_REBIRTH->Disinfect;  //消毒参数全局变量#/
        disinfectCond=G_P_REBIRTH->disinfectCond;   //消毒电导#/

        timeCount=0;
        Disinfect_secCount=0;
        powerShutdown=false;
        setCountDown=false;

        if(workstate.tipState.current==CLEANING&&workstate.subState.current==CLEANING_RUN)
        {
            buttonRunState = true;

            switch (workstate.current)//消毒的模式#/
            {
            case COOL_RINSE:
                widgetTempo.labelTitle.setText(tr("Rinse"));
                chooseDisinfectWorkModel=cold_rinse_B;
                break;
            case HOT_RINSE:
                chooseDisinfectWorkModel=hot_rinse_B;
                widgetTempo.labelTitle.setText(tr("Hot Rinse"));
                break;
            case CHEMICAL_PERACETIC://化学消毒
                chooseDisinfectWorkModel=x_peracetic_B;
                if(gChemicalDisinfect == 1){
                    widgetTempo.labelTitle.setText(tr("Chymistry Peracetic"));
                }
                else if(gChemicalDisinfect == 2){
                    widgetTempo.labelTitle.setText(tr("Chymistry Citric"));
                }
                else if(gChemicalDisinfect == 3){
                    widgetTempo.labelTitle.setText(tr("Chymistry Reserve3"));
                }
                break;
            case CHEMICAL3://化(次氯酸钠)#/
                chooseDisinfectWorkModel=x_peracetic_B;
                widgetTempo.labelTitle.setText(tr("Chloros"));
                break;
            case CHEMICAL_CITRICACID://化(柠檬酸)#/
                chooseDisinfectWorkModel=x_citricAcid_B;
                widgetTempo.labelTitle.setText(tr("Sodium citrate"));
                break;
            case HOT_CHEMICAL: //热(柠檬酸)#/
                chooseDisinfectWorkModel=h_citricAcid_B;
                widgetTempo.labelTitle.setText(tr("Sodium citrate 85"));
                break;
            case HOT: //热(纯水)#/
                chooseDisinfectWorkModel=h_water_B;
                widgetTempo.labelTitle.setText(tr("hot Disinfect"));
                break;
            default:
                break;
            }

        widgetDisinfectRadioButton.slectModel(chooseDisinfectWorkModel);

        if(chooseDisinfectWorkModel==cold_rinse_B ||
           chooseDisinfectWorkModel==hot_rinse_B   )
            ShutOffDelay=15*60;//
        else ShutOffDelay=5*60;

        G_RebirthDisinfect_flag=true;  //消毒重生标志#/
        StateSwitch("Disinfect");     //进入消毒处理循环#/

        widgetButtonRun.setText(tr("stop"));
        widgetButtonRun.setChecked(true);
        widgetTempo.setVisible(true);
        widgetTemperature.setVisible(true);
        widgetDisinfectRadioButton.setVisible(false);
        labelNote.setVisible(false);
        labelNote2.setVisible(false);
        labelCountDown.setVisible(false);
        //if(gAutoOff == 1)  lineEditAutoOff.setVisible(true);
        //else if(gAutoOff == 0) lineEditAutoOff.setVisible(false);
        textEditLastPrograms.setVisible(false);
        textEditFilterChange.setVisible(false);
        buttonFilterChange1.setVisible(false);
        buttonFilterChange2.setVisible(false);

        timeCount=0;
        //根据消毒模式的总运行时间设置扇面图
        if(Disinfect.step==hotrinse1)
          widgetTempo.setValueTotal((Disinfect.loop_Time+Disinfect.backFlush_Time) *60);
        else
          widgetTempo.setValueTotal((Disinfect.preFlush_Time+Disinfect.hold_Time+Disinfect.loop_Time+Disinfect.backFlush_Time) *60);

        widgetTempo.setValue(0);//当前已进行的消毒时间#/

        if(gRemainingTime == 1)
        {
          int tmp;
          if(Disinfect.step==hotrinse1)
          {
            tmp = (100-gValueScale)*(Disinfect.loop_Time+Disinfect.backFlush_Time) *60/100;
          }
          else
          {
            tmp = (100-gValueScale)*(Disinfect.preFlush_Time+Disinfect.hold_Time+Disinfect.loop_Time+Disinfect.backFlush_Time) *60/100;
          }
          QString str = QString::number(tmp,10);
          str = tr("         Remaining Time : ") + str + tr("min");
          lineEditRemainingTime.setText(str);  //剩余的时间#/
          lineEditRemainingTime.setVisible(true);
        }
        else if(gRemainingTime == 0)
        {
          lineEditRemainingTime.setVisible(false);
        }
      }

  }
}

void SceneDisinfect::timer_handler(const int timeOut)
{
    Q_UNUSED(timeOut);

    if(iSPipeok==true && GetWorkStateManage().tipState.current==CLEANING)
    {
        //timeCount++;
        if((N3_errorstate & 0x08) ==0)  //当主控板没有无液报警时#/
        {
          widgetTempo.setValue(Disinfect_secCount);//timeCount
          //qDebug()<<"widgetTempo.setValue";
         }
    }

    //if(workstate.tip==Disinfect_finish)
    stateTemp=GetWorkStateManage();
    if(stateTemp.subState.current==CLEANING_FINISHED && !powerShutdown && setCountDown)
    {
        secCount=timePoint.elapsed()/1000;
        if(secCount>=ShutOffDelay)
        {
             labelCountDown.setText("0");
             powerShutdown=true;
             //widgetButtonRun.click();
             MasterCtrol_CMD_Shutdown();
             us_sleep(100000);
             MasterCtrol_CMD_Shutdown();
          }
        else
            labelCountDown.setText(QString::number(ShutOffDelay/60-secCount/60));
    }
}

/***************/
/*
上一次消毒没有完成把总状态设为消毒状态；
进入消毒界面；
 */
/***************/
bool DisinfectFinishLastTime(void)
{
    if(GetDisinfectState()){
        return true;//消毒完成
    }
    WorkState workstate = GetWorkStateManage();
    workstate.tipState.current = workstate.tipState.previous= CLEANING;
    workstate.subState.current=workstate.subState.previous=DISINFECT_STOP;
    SetWorkStateManage(&workstate);
    return false;
}


void SceneDisinfect::clickEvent( int key)
{
      Q_UNUSED(key);
      if(labelNote.isVisible())
      {
          setCountDown=false;
          labelNote.setVisible(false);
          labelNote2.setVisible(false);
          labelCountDown.setVisible(false);
      }

}

//显示最近记录
void SceneDisinfect::ShowLastRecord()
{
    DisinfcetInfo info;
    QMap<int,DisinfcetInfo> map=DataSql_GetinfoDisinfect(0,2);    
    QString timestr;
    QDateTime timeobj;

    if(map.size()>0)
    {
        info= map.find(map.size()-1).value();
        timeobj.setTime_t(info.time);
        timestr=timeobj.toString("yyyy-MM-dd   hh:mm");
        LastProgramsLabel1.setText(timestr);
        if(info.type==cold_rinse_B)
          {
            LastProgramsLabel1_type.setText(tr("Rinse"));
            model=cold_rinse_B;
        }
        else if(info.type==hot_rinse_B)
        {
          LastProgramsLabel1_type.setText(tr("Hot rinse"));
          model=hot_rinse_B;
        }
        else if(info.type==x_peracetic_B)
        {
          LastProgramsLabel1_type.setText(tr("Chemical"));
          model=x_peracetic_B;
        }
        else if(info.type==h_water_B)
        {
          LastProgramsLabel1_type.setText(tr("Hot Water"));
          model=h_water_B;
        }
        else if(info.type==h_citricAcid_B)
        {
          LastProgramsLabel1_type.setText(tr("Hot Acid"));
          model=h_citricAcid_B;
        }
        else if(info.type==88)
        {
            LastProgramsLabel1_type.setText("NULL");
            model=cold_rinse_B;
        }

        widgetDisinfectRadioButton.slectModel(model);//选择默认模式

        if(map.size()>1)
        {
           info= map.find(map.size()-2).value();
           timeobj.setTime_t(info.time);
           timestr=timeobj.toString("yyyy-MM-dd   hh:mm");
           LastProgramsLabel2.setText(timestr);
           if(info.type==cold_rinse_B)
             LastProgramsLabel2_type.setText(tr("Rinse"));
           else if(info.type==hot_rinse_B)
             LastProgramsLabel2_type.setText(tr("Hot rinse"));
           else if(info.type==x_peracetic_B)
             LastProgramsLabel2_type.setText(tr("Chemical"));
           else if(info.type==h_water_B)
             LastProgramsLabel2_type.setText(tr("Hot Water"));
           else if(info.type==h_citricAcid_B)
             LastProgramsLabel2_type.setText(tr("Hot Acid"));
           else if(info.type==88)
             LastProgramsLabel2_type.setText("NULL");
        }
    }
}

//插入记录
void SceneDisinfect::insertRecord(DisinfectWorkModel model)
{
    DisinfcetInfo info;
    /*
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    info.timeStr=date.toString("dd.MM.yyyy")+"   "+time.toString("hh:mm");
    */

    info.time=QDateTime().currentDateTimeUtc().toTime_t(); //时间戳#/

    switch (model) {
    case cold_rinse_B: //冷冲洗
        info.type=cold_rinse_B;
        break;
    case hot_rinse_B:
        info.type=hot_rinse_B;
        break;
    case x_peracetic_B:
    case x_sodiumHypochlorite_B:
    case x_citricAcid_B:
        info.type=x_peracetic_B;
        break;
    case h_water_B:
        info.type=h_water_B;
        break;
    case h_citricAcid_B:
        info.type=h_citricAcid_B;
        break;
    default:
        info.type=88;
        break;
    }

    DataSql_InsertDisinfectTable(info);
}


void SceneDisinfect::ChangeFilterCancel(int sel)
{
    sel=sel;
    if(ChangeFilterDlg->isDry())  //如果还在排空#/
       MasterCtrol_CMD_StopDryFilter();
    ChangeFilterDlg->disconnect(this);
}
//复位滤器数据
void SceneDisinfect::ResetFilter(bool sel)
{
   sel=sel;
   connect(YesNoDialog,SIGNAL(NoteSelSignal(int)),this,SLOT(ConfirmResetFilter(int)));
   showYesNoDialog(DE_TR_CHANGEFILTERCONFIRM,DE_TR_FILTERASK);
}
//排空滤器
void SceneDisinfect::DryFilter(bool sel)
{
   if(sel)
   {
       MasterCtrol_CMD_DryFilter();
   }
    else
   {
       MasterCtrol_CMD_StopDryFilter();
   }
}

void SceneDisinfect::ConfirmResetFilter(int sel)
{
    if(sel==true)
    {        
        FilterTimes filterInfo;
         if(FilterID==1)
            filterInfo=DataSql_GetFilterTimes(1);
         else
            filterInfo=DataSql_GetFilterTimes(2);

        filterInfo.remainTime=filterInfo.usedableTime;
        filterInfo.usedTime=0;
        DataSql_UpdateFilterTimes(filterInfo);
        ShowFilterTimes();
    }

    YesNoDialog->disconnect(this);
}

void SceneDisinfect::ShowFilterTimes()
{
    FilterTimes filter1Info,filter2Info;
    filter1Info=DataSql_GetFilterTimes(1);
    filter2Info=DataSql_GetFilterTimes(2);
    ShowFilterPara(filter1Info,filter2Info);
    enginerDialog->showFilterTimes(&filter1Info,&filter2Info);
}

void SceneDisinfect::ShowFilterPara(FilterTimes filter1Info,FilterTimes filter2Info)
{
    QString allLTimes,usedTimes,remainTimes;
    //allLTimes=QString().number( filter1Info.usedableTime);
    //allLTimes.append("                  "+QString().number( filter2Info.usedableTime));
    allLTimes= QString().sprintf("%02d",filter1Info.usedableTime);
    allLTimes.append("                  "+QString().sprintf("%02d",filter2Info.usedableTime));

    usedTimes= QString().sprintf("%02d",filter1Info.usedTime);
    usedTimes.append("                  "+QString().sprintf("%02d",filter2Info.usedTime));

    //remainTimes=QString().number( filter1Info.remainTime);
    //remainTimes.append("                  "+QString().number( filter2Info.remainTime));
    remainTimes= QString().sprintf("%02d",filter1Info.remainTime);
    remainTimes.append("                  "+QString().sprintf("%02d",filter2Info.remainTime));

    AllLTimesLabel.setText(allLTimes);
    UsedTimesLabel.setText(usedTimes);
    RemainTimesLabel.setText(remainTimes);
}

void SceneDisinfect::updateFilterTimes(FilterTimes ft1,FilterTimes ft2)
{
  ShowFilterPara(ft1,ft2);
}
