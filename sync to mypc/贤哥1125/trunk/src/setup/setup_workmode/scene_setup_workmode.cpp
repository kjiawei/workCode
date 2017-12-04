#include "config_ui.h"
#include "xml_parse.h"
#include "global_var.h"
#include "packet_can.h"
#include "widget_common.h"
#include "scene_setup_workmode.h"
#include "common_tr_string.h"


#define ION_NAME_X 430
#define ION_VALUE_X ION_NAME_X+80
#define ION_UNIT_X  ION_VALUE_X+80
#define ION_NAME_Y 400

void SceneSetupWorkmode::setUi()
{
    radioButtonHD.setGeometry(QRect(60, 110, 200, 35));
    radioButtonHD.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonHD);

    radioButtonHDF.setGeometry(QRect(60, 110+40, 200, 35));
    radioButtonHDF.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonHDF);

    radioButtonHF.setGeometry(QRect(60, 110+120+40, 200, 35));
    radioButtonHF.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonHF);

    radioButtonIsouf.setGeometry(QRect(60, 110+120*2+40, 190, 35));
    radioButtonIsouf.setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
            RADIOBUTTON_STYLE(radioButtonIsouf);

    radioButtonPE.setGeometry(QRect(60, 110+120*2+80, 190, 35));
    radioButtonPE.setFont(QFont(QString(CONFIG_FONT_STYLE),
                        CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
            RADIOBUTTON_STYLE(radioButtonPE);

    stageTwo.setGeometry(QRect(140, 150+40, 200, 500));
    //HDF在线前稀释#/
    radioButtonDiluteFront.setGeometry(QRect(0, 0, 190, 35));
    radioButtonDiluteFront.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonDiluteFront);
    //HDF在线后稀释#/
    radioButtonDiluteBehind.setGeometry(QRect(0, 40, 190, 35));
    radioButtonDiluteBehind.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonDiluteBehind);
    //HF在线前稀释#/
    radioButton_HFDiluteFront.setGeometry(QRect(0, 80+40, 190, 35));
    radioButton_HFDiluteFront.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButton_HFDiluteFront);
    //HF在线后稀释#/
    radioButton_HFDiluteBehind.setGeometry(QRect(0, 120+40, 190, 35));
    radioButton_HFDiluteBehind.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButton_HFDiluteBehind);
    //单针单泵#/
    radioButtonNeedleSingle.setGeometry(QRect(0, 120*3+80, 200, 35));
    radioButtonNeedleSingle.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonNeedleSingle);
    //单针双泵#/
    radioButtonNeedleSingleDP.setGeometry(QRect(0, 120*3+40, 200, 35));
    radioButtonNeedleSingleDP.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonNeedleSingleDP);
    //双针#/
    radioButtonNeedleDouble.setGeometry(QRect(0, 120*3, 200, 35));
    radioButtonNeedleDouble.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonNeedleDouble);

    //A+B液#/
    radioButtonABf.setGeometry(QRect(430, 110, 200, 35));
    radioButtonABf.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonABf);
    //A+B干粉#/
    radioButtonAfBs.setGeometry(QRect(430, 180, 200, 35));
    radioButtonAfBs.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonAfBs);
    //醋酸盐#/
    radioButtonAcetate.setGeometry(QRect(430, 250, 200, 35));
    radioButtonAcetate.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonAcetate);
    //中央供液#/
    radioButtonCentreInflow.setGeometry(QRect(430, 320, 200, 35));
    radioButtonCentreInflow.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    RADIOBUTTON_STYLE(radioButtonCentreInflow);


    labelTitle.setGeometry(QRect(34, 21, 200, 30));
    LABEL_TITLE_STYLE(labelTitle);

    labelCureMode.setGeometry(QRect(40, 70, 200, 30));
    labelCureMode.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    labelCureMode.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelCureMode.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    labelFeedMode.setGeometry(QRect(410, 70, 200, 30));
    labelFeedMode.setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    labelFeedMode.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelFeedMode.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    frameLineFirst.setGeometry(QRect(380, 80, 4, 600));
    frameLineFirst.setFrameShape(QFrame::VLine);
    frameLineFirst.setStyleSheet("color: rgb(0, 0, 0);");
    frameLineTwo.setGeometry(QRect(30, 600-100, 350, 4));
    frameLineTwo.setFrameShape(QFrame::HLine);
    frameLineTwo.setStyleSheet("color: rgb(240, 240, 240);");


    labelScale.setGeometry(QRect(ION_NAME_X, ION_NAME_Y, 80, 20));
    labelScaleValue.setGeometry(QRect(ION_VALUE_X+10, ION_NAME_Y, 120, 20));

    labelNaAc.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+25, 80, 20));
    labelNaAcvalue.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+25, 80, 20));
    labelNaAcUnit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+25, 80, 20));

    labelNa.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+50, 80, 20));
    labelNavalue.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+50, 80, 20));
    labelNaUnit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+50, 80, 20));

    labelCl.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+75, 80, 20));
    labelClvalue.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+75, 80, 20));
    labelClUnit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+75, 80, 20));
    labelK.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+100, 80, 20));
    labelKvalue.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+100, 80, 20));
    labelKUnit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+100, 80, 20));
    labelHCO3.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+125, 80, 20));
    labelHCO3value.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+125, 80, 20));
    labelHCO3Unit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+125, 80, 20));
    labelMg.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+150, 80, 20));
    labelMgvalue.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+150, 80, 20));
    labelMgUnit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+150, 80, 20));
    labelAcetate.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+175, 80, 20));
    labelAcetatevalue.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+175, 80, 20));
    labelAcetateUnit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+175, 80, 20));
    labelCa.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+200, 80, 20));
    labelCavalue.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+200, 80, 20));
    labelCaUnit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+200, 80, 20));
    labelBNacl.setGeometry(QRect(ION_NAME_X, ION_NAME_Y+225, 80, 20));
    labelBNaclvalue.setGeometry(QRect(ION_VALUE_X, ION_NAME_Y+225, 80, 20));
    labelBNaclUnit.setGeometry(QRect(ION_UNIT_X, ION_NAME_Y+225, 80, 20));

    LABEL_REMIND_TITLE(labelScale);
    LABEL_REMIND_VALUE(labelScaleValue);
    LABEL_REMIND_TITLE(labelNaAc);
    LABEL_REMIND_VALUE(labelNaAcvalue);
    LABEL_REMIND_UNIT(labelNaAcUnit);
    LABEL_REMIND_TITLE(labelNa);
    LABEL_REMIND_VALUE(labelNavalue);
    LABEL_REMIND_UNIT(labelNaUnit);
    LABEL_REMIND_TITLE(labelCl);
    LABEL_REMIND_VALUE(labelClvalue);
    LABEL_REMIND_UNIT(labelClUnit);
    LABEL_REMIND_TITLE(labelK);
    LABEL_REMIND_VALUE(labelKvalue);
    LABEL_REMIND_UNIT(labelKUnit);
    LABEL_REMIND_TITLE(labelHCO3);
    LABEL_REMIND_VALUE(labelHCO3value);
    LABEL_REMIND_UNIT(labelHCO3Unit);
    LABEL_REMIND_TITLE(labelMg);
    LABEL_REMIND_VALUE(labelMgvalue);
    LABEL_REMIND_UNIT(labelMgUnit);
    LABEL_REMIND_TITLE(labelAcetate);
    LABEL_REMIND_VALUE(labelAcetatevalue);
    LABEL_REMIND_UNIT(labelAcetateUnit);
    LABEL_REMIND_TITLE(labelCa);
    LABEL_REMIND_VALUE(labelCavalue);
    LABEL_REMIND_UNIT(labelCaUnit);
    LABEL_REMIND_TITLE(labelBNacl);
    LABEL_REMIND_VALUE(labelBNaclvalue);
    LABEL_REMIND_UNIT(labelBNaclUnit);

    buttonSave.setGeometry(QRect(520, 700-30, 64, 39));
    buttonCancel.setGeometry(QRect(600, 700-30, 64, 39));

    BUTTON_SIMPLE_STYLE(buttonSave);
    BUTTON_SIMPLE_STYLE(buttonCancel);

    CustomSetText();
    PageAppendPiece("SetupWorkmode", this);
}

SceneSetupWorkmode::SceneSetupWorkmode(QWidget *parent) :
    QObject(parent),
    labelTitle(parent),
    labelCureMode(parent),
    labelFeedMode(parent),
    frameLineFirst(parent),
    frameLineTwo(parent),
    radioButtonHD(parent),
    radioButtonHDF(parent),
    radioButtonHF(parent),
    radioButtonIsouf(parent),
    radioButtonPE(parent),
    stageTwo(parent),
    radioButtonNeedleSingle(&stageTwo),
    radioButtonNeedleSingleDP(&stageTwo),
    radioButtonNeedleDouble(&stageTwo),
    radioButtonDiluteFront(&stageTwo),
    radioButtonDiluteBehind(&stageTwo),
    radioButton_HFDiluteFront(&stageTwo),
    radioButton_HFDiluteBehind(&stageTwo),
    radioButtonABf(parent),
    radioButtonAfBs(parent),
    radioButtonAcetate(parent),
    radioButtonCentreInflow(parent),
    groupCureMode(parent),
    groupDilute(parent),
    groupHF(parent),
    groupFeedMode(parent),
    groupOther(parent),
    labelScale(parent),
    labelScaleValue(parent),
    labelNaAc(parent),
    labelNaAcvalue(parent),
    labelNaAcUnit(parent),
    labelNa(parent),
    labelNavalue(parent),
    labelNaUnit(parent),
    labelCl(parent),
    labelClvalue(parent),
    labelClUnit(parent),
    labelK(parent),
    labelKvalue(parent),
    labelKUnit(parent),
    labelHCO3(parent),
    labelHCO3value(parent),
    labelHCO3Unit(parent),
    labelMg(parent),
    labelMgvalue(parent),
    labelMgUnit(parent),
    labelAcetate(parent),
    labelAcetatevalue(parent),
    labelAcetateUnit(parent),
    labelCa(parent),
    labelCavalue(parent),
    labelCaUnit(parent),
    labelBNacl(parent),
    labelBNaclvalue(parent),
    labelBNaclUnit(parent),
    buttonSave(parent),
    buttonCancel(parent)
{
    setUi();

    groupCureMode.addButton(&radioButtonHD);
    groupCureMode.addButton(&radioButtonHDF);
    groupCureMode.addButton(&radioButtonHF);
    groupCureMode.addButton(&radioButtonIsouf);
    groupCureMode.addButton(&radioButtonPE);

    groupDilute.addButton(&radioButtonDiluteFront);
    groupDilute.addButton(&radioButtonDiluteBehind);
    radioButtonDiluteFront.hide();
    radioButtonDiluteBehind.hide();

    groupHF.addButton(&radioButton_HFDiluteFront);
    groupHF.addButton(&radioButton_HFDiluteBehind);

    groupFeedMode.addButton(&radioButtonABf);
    groupFeedMode.addButton(&radioButtonAfBs);
    groupFeedMode.addButton(&radioButtonAcetate);
    groupFeedMode.addButton(&radioButtonCentreInflow);
    groupOther.addButton(&radioButtonNeedleSingle);
    groupOther.addButton(&radioButtonNeedleSingleDP);
    groupOther.addButton(&radioButtonNeedleDouble);

    connect(&groupCureMode,SIGNAL(buttonClicked(int)),this,SLOT(statusDilute(int)));
    connect(&buttonSave, SIGNAL(clicked()), this, SLOT(buttonSaveClicked()));
    connect(&buttonCancel, SIGNAL(clicked()), this, SLOT(buttonCancelClicked()));

    NoteDialog = new Window_Note(0, QObject::tr("WorkModel"), ":/resource/test.png");
    NoteDialog -> raise();
    connect(NoteDialog, SIGNAL(NoteSelSignal(int)), this, SLOT(NoteSel(int)),Qt::DirectConnection);

    dataNodeAddObject(PACKET_FORMULA_TARGET, this);
    dataNodeAddObject(PACKET_REBIRTH_HANDLE,this);  //重生操作处理#/

    workMode = dataNodeFind(PACKET_NAME_WORKMODE);
    memset(&workModeData, 0, sizeof(PacketWorkMode));
    strcpy(workModeData.modeNow, "HD");
    workModeData.flag=init_state;
    workModeData.treatmode.before=HD_double;
    workMode->Update(&workModeData);
    flagPageFrom = false;
    iSshow=false;
}

void SceneSetupWorkmode::CustomXmlConfig(const char *file, const char *cmd)
{
#if DEBUG_XML_CONFIG
    Debug_insert("SceneSetupWorkmode-CustomXmlConfig-in");
#endif
    /*
    QMap<QString, QString> mapV;
    QString str;
    QDomDocument doc;

    xml_DocReadOpen(&doc, file);

    if (strcmp(cmd, DEFAULT_XML_LOAD) == 0) {
        if ((xml_NodeReadValue(&doc, (char *)"/Config/WorkMode/Default", &mapV) ==0) &&
                (mapV.isEmpty() == false)) {
            str = mapV["treatMode"];
            if (str == "HD") {
                radioButtonHD.setChecked(true);
                str = mapV["needle"];
                if (str == "double") {
                    radioButtonNeedleDouble.setChecked(true);
                    workModeData.treatmode.current=HD_double;
                } else if (str == "single") {
                    radioButtonNeedleSingle.setChecked(true);
                    workModeData.treatmode.current=HD_SNDP;
                }
                strcpy(workModeData.modeNow, "HD");
                //workMode->Update(&workModeData);
            } else if (str == "HDF") {
                radioButtonHDF.setChecked(true);
                str = mapV["dilute"];
                if (str == "pre-dilute") {
                    radioButtonDiluteFront.setChecked(true);
                    workModeData.treatmode.current=HDF_pre_dilute;
                } else if (str == "post-dilute") {
                    radioButtonDiluteBehind.setChecked(true);
                    workModeData.treatmode.current=HDF_pre_dilute;
                }
                strcpy(workModeData.modeNow, "HDF");
                BloodPump_link_flag=true;
                //workMode->Update(&workModeData);
            } else if (str == "ISOUF") {
                radioButtonIsouf.setChecked(true);
                strcpy(workModeData.modeNow, "ISOUF");
                workModeData.treatmode.current=ISOUF_DNDP;
                //workMode->Update(&workModeData);
            }
            //设置默认值
            radioButtonHD.setChecked(true);
            radioButtonNeedleDouble.setChecked(true);
            radioButton_HFDiluteFront.setChecked(true);
            workModeData.treatmode.current=HD_double;
            workMode->Update(&workModeData);
            //
        }
        str = mapV["fluidMode"];
        if (str == "A+B") {
            radioButtonABf.setChecked(true);
            workModeData.fluidmode.current=A_and_B;
        } else if (str == "A+B(Dry)") {
            radioButtonAfBs.setChecked(true);
            workModeData.fluidmode.current=A_and_BPower;
        } else if (str == "Acetate") {
            radioButtonAcetate.setChecked(true);
            workModeData.fluidmode.current=Acetate;
        } else if (str == "CentralFluid") {
            radioButtonCentreInflow.setChecked(true);
            workModeData.fluidmode.current=Center;
        }
        workMode->Update(&workModeData);
    } else if (strcmp(cmd, DEFAULT_XML_SAVE) == 0) {
        mapV.clear();
        if (radioButtonHD.isChecked()) {
            mapV.insert("treatMode", "HD");
        } else if (radioButtonHDF.isChecked()) {
            mapV.insert("treatMode", "HDF");
        } else if (radioButtonIsouf.isChecked()) {
            mapV.insert("treatMode", "ISOUF");
        }

        if (radioButtonNeedleSingle.isChecked()) {
            mapV.insert("needle", "single");
        } else if (radioButtonNeedleDouble.isChecked()) {
            mapV.insert("needle", "double");
        }

        if (radioButtonDiluteFront.isChecked()) {
            mapV.insert("dilute", "pre-dilute");
        } else if (radioButtonDiluteBehind.isChecked()) {
            mapV.insert("dilute", "post-dilute");
        }

        if (radioButtonABf.isChecked()) {
            mapV.insert("fluidMode", "A+B");
        } else if (radioButtonAfBs.isChecked()) {
            mapV.insert("fluidMode", "A+B(Dry)");
        } else if (radioButtonAcetate.isChecked()) {
            mapV.insert("fluidMode", "Acetate");
        } else if (radioButtonCentreInflow.isChecked()) {
            mapV.insert("fluidMode", "CentralFluid");
        }
            QDomDocument doc;
            xml_DocReadOpen(&doc, file);
            xml_NodeWriteValue(&doc, "/Config/WorkMode/Default", &mapV);
            waiteToFile(&doc, file);
    }

    mapV.clear();
    */
    QMap<QString, QString> mapV;
    QDomDocument doc;
    unsigned char curemode=0;

    xml_DocReadOpen(&doc, file);

    if (strcmp(cmd, DEFAULT_XML_LOAD) == 0) {
        if ((xml_NodeReadValue(&doc, (char *)"/Config/Engineer_Factoryset/CureMode", &mapV) ==0) &&
                (mapV.isEmpty() == false)) {
            curemode=mapV["Model"].toInt();
            switch(curemode)
            {
            case HD_double:
                radioButtonHD.setChecked(true);
                radioButtonNeedleDouble.setChecked(true);
                break;
            case HD_SNDP:
                radioButtonHD.setChecked(true);
                radioButtonNeedleSingleDP.setChecked(true);
                break;
            case HD_SNSP:
                radioButtonHD.setChecked(true);
                radioButtonNeedleSingle.setChecked(true);
                break;
            case HDF_pre_dilute://前置换 透析滤过
                radioButtonHDF.setChecked(true);
                radioButtonDiluteFront.setChecked(true);
                break;
            case HDF_post_dilute://后置换 透析滤过
                radioButtonHDF.setChecked(true);
                radioButtonDiluteBehind.setChecked(true);
                break;
            case HF_pre_dilute://
                radioButtonHF.setChecked(true);
                radioButton_HFDiluteFront.setChecked(true);
                break;
            case HF_post_dilute://
                radioButtonHF.setChecked(true);
                radioButton_HFDiluteBehind.setChecked(true);
                break;
            case ISOUF_DNDP://
                radioButtonIsouf.setChecked(true);
                radioButtonNeedleDouble.setChecked(true);
                break;
            case ISOUF_SNDP://
                radioButtonIsouf.setChecked(true);
                radioButtonNeedleSingleDP.setChecked(true);
                break;
            case ISOUF_SNSP://
                radioButtonIsouf.setChecked(true);
                radioButtonNeedleSingle.setChecked(true);
                break;
            case PE ://
                radioButtonPE.setChecked(true);
                break;
            default:
                curemode=HD_double;
                radioButtonHD.setChecked(true);
                radioButtonNeedleDouble.setChecked(true);
                break;
            }
        }
        else
        {
            curemode=HD_double;
            radioButtonHD.setChecked(true);
            radioButtonNeedleDouble.setChecked(true);
        }


        if ((xml_NodeReadValue(&doc, (char *)"/Config/Engineer_Factoryset/fluidMode", &mapV) ==0) &&
                (mapV.isEmpty() == false)) {
            fluidmodetmp=mapV["Model"].toInt();
            switch(fluidmodetmp)
            {
            case A_and_B:
                radioButtonABf.setChecked(true);
                break;
            case A_and_BPower:
                radioButtonAfBs.setChecked(true);
                break;
            case Acetate:
                radioButtonAcetate.setChecked(true);
                break;
            case Center:
                radioButtonCentreInflow.setChecked(true);
                break;
            default:
                fluidmodetmp=A_and_B;
                radioButtonABf.setChecked(true);
                break;
            }
        }
        else
        {

            fluidmodetmp=A_and_B;
            radioButtonABf.setChecked(true);
        }

         workModeData.flag=setok_state;
         workModeData.fluidmode.target=fluidmodetmp;
         workModeData.fluidmode.current=fluidmodetmp;
         workModeData.treatmode.current=curemode;
         workModeData.treatmode.before=curemode;
         workModeData.treatmode.target=curemode;
         workMode->Update(&workModeData);
    }

    mapV.clear();
#if DEBUG_XML_CONFIG
    Debug_insert("SceneSetupWorkmode-CustomXmlConfig-out");
#endif
}

void SceneSetupWorkmode::ActionHandle(const void* dataPtr, int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupWorkmode-ActionHandle-in");
#endif
    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }
    if(name==PACKET_FORMULA_TARGET) {
        FormulaSet *formulaSet = (struct FormulaSet*)dataPtr;
        QString number;
        number.setNum(formulaSet->tempBuffer.stand_b_workmode, 'f', 3);
        QString str = "1 : " + number;//QString::number(formulaSet->tempBuffer.stand_b_workmode);
        str += " : ";
        str += QString::number(formulaSet->tempBuffer.stand_water_workmode);
        labelScaleValue.setText(str);
        labelNaAcvalue.setText(QString::number(formulaSet->tempBuffer.naac));
        labelNavalue.setText(QString::number(formulaSet->tempBuffer.stand_na));//stand_na_workmode
        labelClvalue.setText(QString::number(formulaSet->tempBuffer.cl));
        labelKvalue.setText(QString::number(formulaSet->tempBuffer.k));
        labelHCO3value.setText(QString::number(formulaSet->tempBuffer.stand_hco3));//stand_hco3_workmode
        labelMgvalue.setText(QString::number(formulaSet->tempBuffer.mg));
        labelAcetatevalue.setText(QString::number(formulaSet->tempBuffer.ace));
        labelCavalue.setText(QString::number(formulaSet->tempBuffer.ca));
        labelBNaclvalue.setText(QString::number(formulaSet->tempBuffer.bnacl));
    }        
    else if(name==PACKET_REBIRTH_HANDLE)//重生恢复操作处理#/
    {
        G_WORKMODE_VAL=G_P_REBIRTH->G_WORKMODE_VAL;
        workModeData=G_WORKMODE_VAL;
        modeltmp = workModeData.treatmode.current;
        fluidmodetmp =workModeData.fluidmode.current;

                switch(modeltmp)
                {
                case HD_double:
                    radioButtonHD.setChecked(true);
                    radioButtonNeedleDouble.setChecked(true);
                    break;
                case HD_SNDP:
                    radioButtonHD.setChecked(true);
                    radioButtonNeedleSingleDP.setChecked(true);
                    break;
                case HD_SNSP:
                    radioButtonHD.setChecked(true);
                    radioButtonNeedleSingle.setChecked(true);
                    break;
                case HDF_pre_dilute://前置换 透析滤过
                    radioButtonHDF.setChecked(true);
                    radioButtonDiluteFront.setChecked(true);
                    break;
                case HDF_post_dilute://后置换 透析滤过
                    radioButtonHDF.setChecked(true);
                    radioButtonDiluteBehind.setChecked(true);
                    break;
                case HF_pre_dilute://
                    radioButtonHF.setChecked(true);
                    radioButton_HFDiluteFront.setChecked(true);
                    break;
                case HF_post_dilute://
                    radioButtonHF.setChecked(true);
                    radioButton_HFDiluteBehind.setChecked(true);
                    break;
                case ISOUF_DNDP://
                    radioButtonIsouf.setChecked(true);
                    radioButtonNeedleDouble.setChecked(true);
                    break;
                case ISOUF_SNDP://
                    radioButtonIsouf.setChecked(true);
                    radioButtonNeedleSingleDP.setChecked(true);
                    break;
                case ISOUF_SNSP://
                    radioButtonIsouf.setChecked(true);
                    radioButtonNeedleSingle.setChecked(true);
                    break;
                case PE ://
                    radioButtonPE.setChecked(true);
                    break;
                default:
                    radioButtonHD.setChecked(true);
                    radioButtonNeedleDouble.setChecked(true);
                    break;
                }


                switch(fluidmodetmp)
                {
                case A_and_B:
                    radioButtonABf.setChecked(true);
                    break;
                case A_and_BPower:
                    radioButtonAfBs.setChecked(true);
                    break;
                case Acetate:
                    radioButtonAcetate.setChecked(true);
                    break;
                case Center:
                    radioButtonCentreInflow.setChecked(true);
                    break;
                default:
                    fluidmodetmp=A_and_B;
                    radioButtonABf.setChecked(true);
                    break;
                }

 //            workMode->Update(&workModeData);
        }

#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneSetupWorkmode-ActionHandle-out");
#endif
}

void SceneSetupWorkmode::statusDilute(int id)
{
    groupHF.setExclusive(false);
    groupDilute.setExclusive(false);
    radioButton_HFDiluteFront.setChecked(false);
    radioButton_HFDiluteBehind.setChecked(false);
    radioButtonDiluteFront.setChecked(false);
    radioButtonDiluteBehind.setChecked(false);
    groupDilute.setExclusive(true);
    groupHF.setExclusive(true);

    if (id == groupCureMode.id(&radioButtonHD)){
        strcpy(workModeData.modeNow, "HD");
//        workMode->Update(&workModeData);
    } else if (id == groupCureMode.id(&radioButtonHDF)) {
        radioButtonDiluteFront.setChecked(true);
        strcpy(workModeData.modeNow, "HDF");
//        workMode->Update(&workModeData);
    } else if(id == groupCureMode.id(&radioButtonHF)){
        radioButton_HFDiluteFront.setChecked(true);
        strcpy(workModeData.modeNow, "HF");
//        workMode->Update(&workModeData);
    }else if (id == groupCureMode.id(&radioButtonIsouf)) {
        strcpy(workModeData.modeNow, "ISOUF");
//        workMode->Update(&workModeData);
    }
    setCusVisible(true);
}

void SceneSetupWorkmode::NoteSel(int sel)
{
    //qDebug() << "\r\n selction: "<<sel;
    if(sel==1)
    {
        WorkState  stateTMP=GetWorkStateManage();
        if(stateTMP.tipState.current==TREAT_RUNNING&&modeltmp==PE)
        {
            showYesDialog(DE_TR_PROMPT,DE_TR_CANNOTSWITCHTOPE);
            return;
        }

        //workModeData.treatmode.target、workModeData.fluidmode.target该变量在这里保存设置参数，用于#/
        // PageShow("Setup Subspump")、PageShow("Setup Subspump")的跳转来源于模式设置页面的确认#/
        workModeData.treatmode.target=modeltmp ;
        workModeData.fluidmode.target=fluidmodetmp ;
        workModeData.flag=seting_state;
        workMode->Update(&workModeData);

        logInfo log;
        log.opration="Change WorkModel";
        log.type=DE_LOGINFO_OPER;
        log.status=PDOstate.state.N1_status;
        log.note="warning";
        LogRun->addLog(log);

    /*
        if ((radioButtonHDF.isChecked() == true)||(radioButtonHF.isChecked() == true)) {
            PageShow("Setup Subspump");//HDF,HF模式需要设置置换泵参数，该模式才能生效#/
            return;
        }
        */

        if ((radioButtonHDF.isChecked() == true)||(radioButtonHF.isChecked() == true))
            BloodPump_link_flag=true;

        if (radioButtonIsouf.isChecked() == true){
            PageShow("Setup ISO UF");//ISO模式需要设置单超参数，该模式才能生效#/
            return;
        }

        /*
        if (radioButtonHDF.isChecked() == true) {
            PageShow("Setup Subspump");//HDF模式需要设置置换泵参数，该模式才能生效#/
            return;
        }

        if (radioButtonIsouf.isChecked() == true){
            PageShow("Setup ISO UF");//ISO模式需要设置单超参数，该模式才能生效#/
            return;
        }
        */

        //其他模式即时生效#/
        workModeData.treatmode.current=modeltmp ;
        workModeData.fluidmode.current=fluidmodetmp ;
        workModeData.flag=setok_state;
        workMode->Update(&workModeData);

        //工作模式切换时，曲线治疗停止#/
        if((G_NA_PROFILE_FLAG==true||G_UF_PROFILE_FLAG==true)&&(stateTMP.tipState.current==TREAT_RUNNING))
        {
            G_NA_PROFILE_FLAG=false;
            G_NA_PROFILE_NUM=0;
            G_NA_SELECTCURVE_NUM=-1;

            G_UF_PROFILE_FLAG=false;
            G_UF_PROFILE_NUM=0;
            G_UF_SELECTCURVE_NUM=-1;

            showYesDialog(DE_TR_PROMPT,DE_TR_WORKMODECHANGEDPROFILECANCEL);
        }
        G_NA_PROFILE_FLAG=false;     //NA曲线工作模式切换时，曲线治疗停止#/
        G_UF_PROFILE_FLAG=false;     //UF曲工作模式切换时，曲线治疗停止#/


        PageShow("Main");
    }
}

void SceneSetupWorkmode::buttonCancelClicked()
{
    PageShow("Main");
}

void SceneSetupWorkmode::buttonSaveClicked()
{
    dataRead(PACKET_NAME_WORKMODE,&workModeData);
    modeltmp = workModeData.treatmode.current;
    fluidmodetmp =workModeData.fluidmode.current;

    if (radioButtonHD.isChecked() == true) {
        if (radioButtonNeedleSingle.isChecked() == true) {
            modeltmp=HD_SNSP;
        }
        if (radioButtonNeedleDouble.isChecked() == true) {
            modeltmp=HD_double;
        }
        if (radioButtonNeedleSingleDP.isChecked() == true) {
            modeltmp=HD_SNDP;
        }
    }
    else if(radioButtonHDF.isChecked() == true){
        if (radioButtonDiluteFront.isChecked() == true) {
            modeltmp=HDF_pre_dilute;
        }
        else if (radioButtonDiluteBehind.isChecked() == true) {
            modeltmp=HDF_post_dilute;
        }
    }
    else if(radioButtonHF.isChecked() == true){
         if (radioButton_HFDiluteFront.isChecked() == true) {
            modeltmp=HF_pre_dilute;
        }

        else if (radioButton_HFDiluteBehind.isChecked() == true) {
            modeltmp=HF_post_dilute;
        }
    }
    else if (radioButtonIsouf.isChecked() == true) {
        if (radioButtonNeedleSingle.isChecked() == true) {
            modeltmp=ISOUF_SNSP;
        }
        if (radioButtonNeedleDouble.isChecked() == true) {
            modeltmp=ISOUF_DNDP;
        }
        if (radioButtonNeedleSingleDP.isChecked() == true) {
            modeltmp=ISOUF_SNDP;
        }
    }
    else if(radioButtonPE.isChecked())
            modeltmp=PE;

    if (radioButtonABf.isChecked() == true) {
        fluidmodetmp=A_and_B;
    }

    if (radioButtonAfBs.isChecked() == true) {
        fluidmodetmp=A_and_BPower;
    }

    if (radioButtonAcetate.isChecked() == true) {
        fluidmodetmp=Acetate;
    }

    if (radioButtonCentreInflow.isChecked() == true) {
        fluidmodetmp=Center;
    }

    if(workModeData.treatmode.current!=modeltmp || workModeData.fluidmode.current!=fluidmodetmp)
    {  /*
        msgBox->setText(tr("     Please confirm !"));
        msgBox->setInformativeText(tr("Would you like to change workmodel ?"));
        msgBox->setDetailedText(tr("say something ..."));
        msgBox->setWindowFlags(Qt::FramelessWindowHint);
        msgBox->setIcon(QMessageBox::Question);//NoIcon
        int ret=msgBox->exec();
        if(ret==QMessageBox::Yes)
          {
            workModeData.treatmode.current=modeltmp ;
            workModeData.fluidmode.current=fluidmodetmp ;
          }
        else if(ret==QMessageBox::No)
         {
          return;
         }
      */
        logInfo log;
        log.opration="Change WorkModel";
        log.type=DE_LOGINFO_OPER;
        log.status=PDOstate.state.N1_status;
        log.note="warning";
        LogRun->addLog(log);

        showNoteDialog("WorkModel");
     }
    else
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_TREATMENTMODELNOTCHANGED);
    }

    return;
}

//每次进入模式设置都显示已设置项#/
void SceneSetupWorkmode::showCurrentMode()
{
    dataRead(PACKET_NAME_WORKMODE,&workModeData);

    groupHF.setExclusive(false);
    groupDilute.setExclusive(false);
    radioButton_HFDiluteFront.setChecked(false);
    radioButton_HFDiluteBehind.setChecked(false);
    radioButtonDiluteFront.setChecked(false);
    radioButtonDiluteBehind.setChecked(false);
    groupDilute.setExclusive(true);
    groupHF.setExclusive(true);


    switch(workModeData.treatmode.current){
    case HD_SNSP:
        radioButtonHD.setChecked(true);
        radioButtonNeedleSingle.setChecked(true);
        break;
    case HD_double:
        radioButtonHD.setChecked(true);
        radioButtonNeedleDouble.setChecked(true);
        break;
    case HD_SNDP:
        radioButtonHD.setChecked(true);
        radioButtonNeedleSingleDP.setChecked(true);
        break;

    case HDF_pre_dilute:
        radioButtonHDF.setChecked(true);
        radioButtonDiluteFront.setChecked(true);
        break;
    case HDF_post_dilute:
        radioButtonHDF.setChecked(true);
        radioButtonDiluteBehind.setChecked(true);
        break;

    case HF_pre_dilute:
        radioButtonHF.setChecked(true);
        radioButton_HFDiluteFront.setChecked(true);
        break;
    case HF_post_dilute:
        radioButtonHF.setChecked(true);
        radioButton_HFDiluteBehind.setChecked(true);
        break;

    case ISOUF_SNSP:
        radioButtonIsouf.setChecked(true);
        radioButtonNeedleSingle.setChecked(true);
        break;
    case ISOUF_DNDP:
        radioButtonIsouf.setChecked(true);
        radioButtonNeedleDouble.setChecked(true);
        break;
    case ISOUF_SNDP:
        radioButtonIsouf.setChecked(true);
        radioButtonNeedleSingleDP.setChecked(true);
        break;

    case PE:
        radioButtonPE.setChecked(true);
        break;
    default:
        break;
    }
    switch(workModeData.fluidmode.current){
    case A_and_B:
        radioButtonABf.setChecked(true);
        break;
    case A_and_BPower:
        radioButtonAfBs.setChecked(true);
        break;
    case Acetate:
        radioButtonAcetate.setChecked(true);
        break;
    case Center:
        radioButtonCentreInflow.setChecked(true);
        break;
    default:
        break;
    }
}
void SceneSetupWorkmode::CustomSetText()
{
    radioButtonHD.setText(tr("HD"));
    radioButtonNeedleSingle.setText(tr("Single Needle"));
    radioButtonNeedleSingleDP.setText(tr("SNDP"));
    radioButtonNeedleDouble.setText(tr("Double Needle"));

    radioButtonHDF.setText(tr("HDF (Online)"));
    radioButtonHF.setText(tr("HF (Online)"));
    radioButtonDiluteFront.setText(tr("Predilution"));
    radioButtonDiluteBehind.setText(tr("Postdilution"));
    radioButton_HFDiluteFront.setText(tr("Predilution"));
    radioButton_HFDiluteBehind.setText(tr("Postdilution"));
    radioButtonIsouf.setText(tr("ISO UF"));
    radioButtonPE.setText(tr("PE/HP"));
    radioButtonABf.setText(tr("A + B"));
    radioButtonAfBs.setText(tr("A + B(Dry Powder)"));
    radioButtonAcetate.setText(tr("Acetate"));
    radioButtonCentreInflow.setText(tr("Central fluid supply"));
    labelTitle.setText(tr("Work Mode"));
    labelCureMode.setText(tr("Treatmemt Mode:"));
    labelFeedMode.setText(tr("Supply Fluid Mode:"));
    buttonSave.setText(tr("Confirm"));
    buttonCancel.setText(tr("Cancel"));
    labelScale.setText(tr("Scale:"));
    labelNaAc.setText(tr("NaAc:"));
    labelNaAcUnit.setText(tr("g/l"));

    labelNa.setText(tr("Na:"));
    labelNaUnit.setText(tr("mmol/l"));

    labelCl.setText(tr("Cl:"));
    labelClUnit.setText(tr("mmol/l"));

    labelK.setText(tr("K:"));
    labelKUnit.setText(tr("mmol/l"));

    labelHCO3.setText(tr("HCO3:"));
    labelHCO3Unit.setText(tr("mmol/l"));

    labelMg.setText(tr("Mg:"));
    labelMgUnit.setText(tr("mmol/l"));

    labelAcetate.setText(tr("Acetate:"));
    labelAcetateUnit.setText(tr("mmol/l"));

    labelCa.setText(tr("Ca:"));
    labelCaUnit.setText(tr("mmol/l"));

    labelBNacl.setText(tr("B)NaCl:"));
    labelBNaclUnit.setText(tr("g/l"));
}

void SceneSetupWorkmode::setCusVisible(bool vis, char* para)
{
    para = para;
    labelTitle.setVisible(vis);
    labelCureMode.setVisible(vis);
    labelFeedMode.setVisible(vis);
    frameLineFirst.setVisible(vis);
    frameLineTwo.setVisible(vis);
    radioButtonHD.setVisible(vis);
    radioButtonHDF.setVisible(vis);
    radioButtonHF.setVisible(vis);
    radioButtonPE.setVisible(vis);
    stageTwo.setVisible(vis);
    radioButtonABf.setVisible(vis);
    radioButtonAfBs.setVisible(vis);
    radioButtonAcetate.setVisible(vis);
    radioButtonCentreInflow.setVisible(vis);
    buttonSave.setVisible(vis);
    buttonCancel.setVisible(vis);
    radioButtonIsouf.setVisible(vis);
    radioButtonPE.setVisible(vis);
    radioButtonDiluteBehind.setVisible(vis);
    radioButtonDiluteFront.setVisible(vis);
    radioButton_HFDiluteBehind.setVisible(vis);
    radioButton_HFDiluteFront.setVisible(vis);
    radioButtonNeedleSingle.setVisible(vis);
    radioButtonNeedleSingleDP.setVisible(vis);
    radioButtonNeedleDouble.setVisible(vis);
    labelScale.setVisible(vis);
    labelScaleValue.setVisible(vis);

    labelNaAc.setVisible(vis);
    labelNaAcvalue.setVisible(vis);
    labelNaAcUnit.setVisible(vis);

    labelNa.setVisible(vis);
    labelNavalue.setVisible(vis);
    labelNaUnit.setVisible(vis);

    labelCl.setVisible(vis);
    labelClvalue.setVisible(vis);
    labelClUnit.setVisible(vis);

    labelK.setVisible(vis);
    labelKvalue.setVisible(vis);
    labelKUnit.setVisible(vis);

    labelHCO3.setVisible(vis);
    labelHCO3value.setVisible(vis);
    labelHCO3Unit.setVisible(vis);

    labelMg.setVisible(vis);
    labelMgvalue.setVisible(vis);
    labelMgUnit.setVisible(vis);

    labelNaAc.setVisible(vis);
    labelNaAcvalue.setVisible(vis);
    labelNaAcUnit.setVisible(vis);

    labelAcetate.setVisible(vis);
    labelAcetatevalue.setVisible(vis);
    labelAcetateUnit.setVisible(vis);

    labelCa.setVisible(vis);
    labelCavalue.setVisible(vis);
    labelCaUnit.setVisible(vis);

    labelBNacl.setVisible(vis);
    labelBNaclvalue.setVisible(vis);
    labelBNaclUnit.setVisible(vis);
    if(!flagPageFrom){
        showCurrentMode();
        flagPageFrom = true;
    }
    if(vis==false)
    {
        flagPageFrom = false;
        if(iSshow)
        {
          iSshow=false;
          //radioButtonNeedleSingleDP.setChecked(true);
         }
        return;
    }
    iSshow=true;

    if(radioButtonHD.isChecked() == true){
        radioButtonNeedleSingle.setVisible(true);
        radioButtonNeedleDouble.setVisible(true);
        radioButtonNeedleSingleDP.setVisible(true);
        frameLineTwo.setVisible(true);

    } else if(radioButtonIsouf.isChecked() == true){//if((vis==true) && (strcmp(workModeData.modeNow, "ISOUF")==0)) {
        radioButtonNeedleSingle.setVisible(true);
        radioButtonNeedleDouble.setVisible(true);
        radioButtonNeedleSingleDP.setVisible(true);
        frameLineTwo.setVisible(true);

    }
    else if((vis==true) ) {//&& (strcmp(workModeData.modeNow, "HDF")==0)
        radioButtonNeedleSingle.setVisible(false);
        radioButtonNeedleDouble.setVisible(false);
        radioButtonNeedleSingleDP.setVisible(false);
        frameLineTwo.setVisible(false);

        }

    //if (strcmp(PageCurrent(), "SetupWorkmode") != 0) {

    //}

}

//void SceneSetupPreflush::radioChange(void)
//{

//    if(sender() == &radioAutoPreflush)
//    {}
//}

//void SceneSetupWorkmode::showEvent (QShowEvent * event)
//{
//    QDialog::showEvent(event);

//}
