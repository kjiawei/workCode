#include "version.h"
#include "engineering_mode.h"
#include "ui_engineering_mode.h"
#include "sound_adjust.h"
#include "xml_parse.h"
#include "global_var.h"
#include "common_tr_string.h"
#include "widget_common.h"
#include "Daemon.h"

bool gAutoOff=0;
bool gRemainingTime=1;
bool gLastPrograms=1;
bool gFilterChange=1;
int  gChemicalDisinfect=1;    //默认Peracetic Acid
int  gCentralDisinfect=1;       //默认Central Hot


//bool gPrimeStyle=0;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    oldPassword = new QMyEdit(ui->factory);
    oldPassword->setText("old passwrod");
    oldPassword->setGeometry(20,300,150,25);
    newPassword = new QMyEdit(ui->factory);
    newPassword->setText("new passwrod");
    newPassword->setGeometry(20,340,150,25);
    confirmPassword = new QMyEdit(ui->factory);
    confirmPassword->setText("confirm new passwrod");
    confirmPassword->setGeometry(20,380,150,25);
    setPasswordMesssage = new QLabel(ui->factory);
    setPasswordMesssage->setGeometry(270,320,150,25);
    setPasswordMesssage->setText("");
    buttonReturn=new QPushButton(ui->factory);
    buttonReturn->setText(DE_TR_RETURN);
    buttonReturn->setGeometry(800,570,91,51);

    modeParamsCommon = new SceneProjectModeParamsCommon(ui->params);
    modeEEprom = new SceneProjectModeEeprom(ui->EEPROM);
    modeCurve = new SceneProjectModeCurve(ui->curve);
    modeFormula = new SceneProjectModeFormulaSet(ui->tab_3);

    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 40px;}");


    PageAppendPiece("paramsEngineer", modeParamsCommon);
    PageAppendPiece("Maintain EEprom", modeEEprom);
    PageAppendPiece("Maintain Curve", modeCurve);
    PageAppendPiece("Maintain Formula", modeFormula);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)), this, SLOT(index_change(int)));
    connect(buttonReturn,SIGNAL(clicked()),this,SLOT(buttonReturn_clicked()));

    connect(oldPassword, SIGNAL(LineEditClicked()), this, SLOT(oldPasswdInputClicked()));
    connect(newPassword, SIGNAL(LineEditClicked()), this, SLOT(newPasswdInputClicked()));
    connect(confirmPassword, SIGNAL(LineEditClicked()), this, SLOT(confirmPasswdInputClicked()));
    connect(&verTimer,SIGNAL(timeout()),this,SLOT(readver()));
    QObject::connect(modeFormula, SIGNAL(changeID(int)), this, SLOT(rstShowNaHCO3(int)));

    ui->lineEditNa->setObjectName(tr("Curent Na"));
    ui->lineEditHco3->setObjectName(tr("Curent Hco3"));
    ui->allEdit->setObjectName(tr("Filter1 all Times"));
    ui->usedEdit->setObjectName(tr("Filter1 used Times"));
    ui->remainEdit->setObjectName(tr("Filter1 remain Times"));
    ui->allEdit_2->setObjectName(tr("Filter2 all Times"));
    ui->usedEdit_2->setObjectName(tr("Filter2 used Times"));
    ui->remainEdit_2->setObjectName(tr("Filter2 remain Times"));

    ic->appendWidget(ui->lineEditNa,NumInputMethod,LineEdit, modeFormula, 130, 150, 1);
    ic->appendWidget(ui->lineEditHco3,NumInputMethod,LineEdit, modeFormula, 20, 50, 1);
    ic->appendWidget(ui->allEdit,NumInputMethod,LineEdit, NULL, 0, 99, 1);
    ic->appendWidget(ui->usedEdit,NumInputMethod,LineEdit, NULL, 0, 99, 1);
    ic->appendWidget(ui->remainEdit,NumInputMethod,LineEdit, NULL, 0, 99, 1);
    ic->appendWidget(ui->allEdit_2,NumInputMethod,LineEdit, NULL, 0, 99, 1);
    ic->appendWidget(ui->usedEdit_2,NumInputMethod,LineEdit, NULL, 0, 99, 1);
    ic->appendWidget(ui->remainEdit_2,NumInputMethod,LineEdit, NULL, 0, 99, 1);

}

void Dialog::newPasswdInputClicked()
{
    qDebug()<<"PasswdInputClicked";
    setPasswordMesssage->setText("");
    if(NumberKeyboard==NULL){
        newPasswordStr.clear();
        newPassword->setText("");
        NumberKeyboard= new Mini_Keyboard(newPassword,&newPasswordStr);
        }
    NumberKeyboard->setShown(true);
    NumberKeyboard->raise();
}

void Dialog::confirmPasswdInputClicked( )
{
    qDebug()<<"PasswdInputClicked";
    setPasswordMesssage->setText("");
    if(NumberKeyboard==NULL){
        confirmPasswordStr.clear();
        confirmPassword->setText("");
        NumberKeyboard= new Mini_Keyboard(confirmPassword,&confirmPasswordStr);
        }
    NumberKeyboard->setShown(true);
    NumberKeyboard->raise();
}
void Dialog::oldPasswdInputClicked()
{
    qDebug()<<"PasswdInputClicked";
    setPasswordMesssage->setText("");
    if(NumberKeyboard==NULL){
        oldPasswordStr.clear();
        oldPassword->setText("");
        NumberKeyboard= new Mini_Keyboard(oldPassword,&oldPasswordStr);
        }
    NumberKeyboard->setShown(true);
    NumberKeyboard->raise();
}
Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showEvent (QShowEvent * event)
{
    QDialog::showEvent(event);

    if(G_SET_FORMULA==true)
    {
        ui->pushButton->hide();
    }
    else if(G_SET_PARAMFLAG==true)
    {
        ui->pushButton->hide();
    }

    if(G_SET_SETUPFLAG==false)
    {
        buttonReturn->setVisible(false);
    }
    else
    {
        ui->pushButton->hide();
        buttonReturn->setVisible(true);
    }


    index_change(ui->tabWidget->currentIndex());
    //ui->BuildTime->setText("");


    int tmp;
    if(event->type()==17)
    {
        tmp=volumeLevel*3;
        if(tmp>=93)
            tmp=100;
        else if(tmp<0)
            tmp=0;

        ui->disVolume->setText(QString::number(tmp,10)+"%");
    }

}

void Dialog::on_pushButton_clicked()  //关闭窗口按钮处理#/
{
    ui->tabWidget->setCurrentIndex(0);
    this->hide();
    if(alarmmodule!=NULL)
        alarmmodule->Set_WorkState(DE_NORMAL_FLAG);
    PageShow("SetupOther");
}

void Dialog::index_change(int index)
{
    if(G_SET_FORMULA==true)
    {
        ui->tabWidget->setTabEnabled(0,false);
        ui->tabWidget->setTabEnabled(1,false);
        ui->tabWidget->setTabEnabled(2,false);
        ui->tabWidget->setTabEnabled(3,true);
        ui->tabWidget->setTabEnabled(4,false);
        ui->tabWidget->setTabEnabled(5,false);
        PageShow("Maintain Formula");
        ui->lineEditNa->clearFocus();
        ui->lineEditHco3->clearFocus();
    }
    else if(G_SET_PARAMFLAG==true)
    {
        ui->tabWidget->setTabEnabled(0,false);
        ui->tabWidget->setTabEnabled(1,true);
        ui->tabWidget->setTabEnabled(2,false);
        ui->tabWidget->setTabEnabled(3,false);
        ui->tabWidget->setTabEnabled(4,false);
        ui->tabWidget->setTabEnabled(5,false);
        PageShow("paramsEngineer");
    }
    else if(G_SET_SETUPFLAG==true)
    {
        ui->tabWidget->setTabEnabled(0,false);
        ui->tabWidget->setTabEnabled(1,false);
        ui->tabWidget->setTabEnabled(2,false);
        ui->tabWidget->setTabEnabled(3,false);
        ui->tabWidget->setTabEnabled(4,true);
        ui->tabWidget->setTabEnabled(5,false);
        return;
    }
    else
    {
        switch(index) {
        case 1:
            PageShow("paramsEngineer");
            break;

        case 2:
            PageShow("Maintain Curve");
            break;
        case 3:
            PageShow("Maintain Formula");
            ui->lineEditNa->clearFocus();
            ui->lineEditHco3->clearFocus();
            break;

        case 5:
            PageShow("Maintain EEprom");
            break;

        default:
            break;
        }
    }
}

void Dialog::on_increase_clicked()
{       
    QString str;
    int volume, tmp;

    volumeLevel++;
    if(volumeLevel>=31) volumeLevel = 31;
    volume = volumeLevel*3;
//    if(volume == 93) volume = 100;
 //   volume = 100-volume;

    if(volume>=93)volume=100;

    if(volume==0)
        G_sound_shielding=true;     //工程模式声音静音标志#/
    else
        G_sound_shielding=false;

    if(volume == 0)
    {
      ui->disVolume->setText("0");
    }
    else
    {
      str = QString::number(volume,10);
      str += "%";

      ui->disVolume->setText(str);
    }
    tmp = 31-volumeLevel;
    tmp = tmp<<8 | tmp;
    SoundAdjustManual(tmp);
}

void Dialog::on_decrease_clicked()
{    
    QString str;
    int volume, tmp;

    volumeLevel--;
    if(volumeLevel<=0) volumeLevel = 0;

    volume=volumeLevel*3;
 //   volume = 100-volumeLevel*3;

    if(volume==0)
        G_sound_shielding=true;     //工程模式声音静音标志#/
    else
        G_sound_shielding=false;

    str = QString::number(volume,10);
    str += "%";

    ui->disVolume->setText(str);
    tmp = 31-volumeLevel;
    tmp = tmp<<8 | tmp;
    SoundAdjustManual(tmp);
}

void Dialog::on_autoOff_clicked()
{
    gAutoOff = !gAutoOff;
}

void Dialog::on_remainingTime_clicked()
{
    gRemainingTime = !gRemainingTime;
}

void Dialog::on_lastPrograms_clicked()
{
    gLastPrograms = !gLastPrograms;
}

void Dialog::on_filterChange_clicked()
{
    gFilterChange = !gFilterChange;
}

void Dialog::on_radioPeraceticAcid_clicked()
{
    gChemicalDisinfect = 1;
}

void Dialog::on_radioSodiumCitrate_clicked()
{
    gChemicalDisinfect = 2;
}

void Dialog::on_radioChemical3_clicked()
{
    gChemicalDisinfect = 3;
}

void Dialog::on_radioCentralHot_clicked()
{
    gCentralDisinfect = 1;
}

void Dialog::on_radioCentralChemical_clicked()
{
    gCentralDisinfect = 2;
}

void Dialog::ui_translate()
{
   ui->retranslateUi(this);
}

void Dialog::on_PrimeAuto_clicked()
{
   PrimeModel=0;

}

void Dialog::on_PrimeManual_clicked()
{
   PrimeModel=1;

}

void Dialog::on_PrimeOnline_clicked()
{
    PrimeModel=2;
}

void Dialog::on_HeparinYes_clicked()
{
    heparin_flag=1;
}

void Dialog::on_HeparinNo_clicked()
{
    heparin_flag=0;
}



//读取XML文件，根据配置设定预冲、消毒的选项#/
void Dialog::CustomXmlConfig(const char *file, const char *cmd)
{
    cmd = cmd;
#if DEBUG_XML_CONFIG
    Debug_insert("Dialog-CustomXmlConfig-in");
#endif
    QString path = "";
    QMap<QString, QString> mapV;
    QDomDocument doc;

    xml_DocReadOpen(&doc, file);
    path.append(tr("/Config/Engineer_Factoryset/PrimeMode"));
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //预冲方式选择#/
    PrimeModel = mapV["Model"].toInt();  //0:

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/CureMode"));
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //治疗模式选择#/
    engineer.factoryset.CureModeSelect = mapV["Model"].toInt();

    switch(engineer.factoryset.CureModeSelect)
    {
    case HD_double:
        ui->HD_DDBbut->setChecked(true);
        break;
    case HD_SNDP:
        ui->HD_SDPbut->setChecked(true);
        break;
    case HD_SNSP:
        ui->HD_SSPbut->setChecked(true);
        break;
    case HDF_pre_dilute://前置换 透析滤过
        ui->HDF_PREbut->setChecked(true);
        break;
    case HDF_post_dilute://后置换 透析滤过
        ui->HDF_POSTbut->setChecked(true);
        break;
    case HF_pre_dilute://
        ui->HF_PREbut->setChecked(true);
        break;
    case HF_post_dilute://
        ui->HF_POSTbut->setChecked(true);
        break;
    case ISOUF_DNDP://
        ui->ISO_DDPbut->setChecked(true);
        break;
    case ISOUF_SNDP://
        ui->ISO_SDPbut->setChecked(true);
        break;
    case ISOUF_SNSP://
        ui->ISO_SSPbut->setChecked(true);
        break;
    case PE ://
        ui->PEbut->setChecked(true);
        break;
    default:
        engineer.factoryset.CureModeSelect=0;
        ui->HD_DDBbut->setChecked(true);
        break;
    }

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/ProfileMode"));
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //治疗曲线的选择#/
    engineer.factoryset.naprofileflag = mapV["NA"].toInt();
    if(engineer.factoryset.naprofileflag==true)
    {
        ui->Na_Profile_Box->setChecked(true);
    }
    engineer.factoryset.ufprofileflag = mapV["UF"].toInt();
    if(engineer.factoryset.ufprofileflag==true)
    {
        ui->UF_Profile_Box->setChecked(true);
    }
    engineer.factoryset.diaprofileflag = mapV["DIA"].toInt();
    if(engineer.factoryset.diaprofileflag==true)
    {
        ui->Dia_Profile_Box->setChecked(true);
    }
    engineer.factoryset.tempprofileflag = mapV["TEMP"].toInt();
    if(engineer.factoryset.tempprofileflag==true)
    {
        ui->Temp_Profile_Box->setChecked(true);
    }
    engineer.factoryset.condprofileflag = mapV["COND"].toInt();
    if(engineer.factoryset.condprofileflag==true)
    {
        ui->Cond_Profile_Box->setChecked(true);
    }
    engineer.factoryset.hepprofileflag = mapV["HEP"].toInt();
    if(engineer.factoryset.hepprofileflag==true)
    {
        ui->Hep_Profile_Box->setChecked(true);
    }
    engineer.factoryset.hco3profileflag = mapV["HCO3"].toInt();
    if(engineer.factoryset.hco3profileflag==true)
    {
        ui->Hco3_Profile_Box->setChecked(true);
    }

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/fluidMode"));
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //供液模式选择#/
    engineer.factoryset.Fluidmode = mapV["Model"].toInt();

    switch(engineer.factoryset.Fluidmode)
    {
    case A_and_B:
        ui->AandB->setChecked(true);
        break;
    case A_and_BPower:
        ui->AandBPower->setChecked(true);
        break;
    case Acetate:
        ui->Acetate->setChecked(true);
        break;
    case Center:
        ui->Center->setChecked(true);
        break;
    default:
        engineer.factoryset.Fluidmode=0;
        ui->AandB->setChecked(true);
        break;
    }

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/GetBloodMode"));
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //引血选择#/
    engineer.factoryset.onlinegetblood = mapV["Model"].toInt();
    if(engineer.factoryset.onlinegetblood==1)
        ui->OnlineGB->setChecked(true);
    else
        ui->GBbutton->setChecked(true);

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/ReturnBloodMode"));
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //回血选择#/
    engineer.factoryset.onlinereturnblood = mapV["Model"].toInt();
    if(engineer.factoryset.onlinereturnblood==1)
        ui->OnlineRB->setChecked(true);
    else
        ui->RBbutton->setChecked(true);

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/HeparinMode"));
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //肝素泵使用选择#/
    heparin_flag = mapV["Model"].toInt();

    //在线模式时，才有online预冲
    if(engineer.factoryset.CureModeSelect==HDF_pre_dilute||engineer.factoryset.CureModeSelect==HDF_post_dilute||\
       engineer.factoryset.CureModeSelect==HF_pre_dilute||engineer.factoryset.CureModeSelect==HF_post_dilute)
    {
        engineer.factoryset.PrimeModel=PrimeModel;
    }
    else
    {
        if(PrimeModel==2)  //其他工作模式不能有online预冲#/
        {
            engineer.factoryset.PrimeModel=0;
        }
        else
            engineer.factoryset.PrimeModel=PrimeModel;
    }


    engineer.factoryset.HeparinSelect=heparin_flag;
    dataNodeUpdate(PACKET_ENGINEER, &engineer);

    if(PrimeModel==0)
     ui->PrimeAuto->setChecked(true);
    else if(PrimeModel==1)
     ui->PrimeManual->setChecked(true);
    else if(PrimeModel==2)
    {
        ui->PrimeOnline->setChecked(true);
    }

    if(heparin_flag==false)
        ui->HeparinNo->setChecked(true);
    else
        ui->HeparinYes->setChecked(true);

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/ChemicalDisinfect"));
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);
    gChemicalDisinfect = mapV["Model"].toInt();
    if(gChemicalDisinfect==1)
      ui->radioPeraceticAcid->setChecked(true);
    else if(gChemicalDisinfect==2)
      ui->radioSodiumCitrate->setChecked(true);
    else if(gChemicalDisinfect==3)
      ui->radioChemical3->setChecked(true);
    else
        gChemicalDisinfect=1;


    int tmp;
    QString str;
    mapV.clear();
    path.clear();
    path.append("/Config/Engineer_Factoryset/DisVolume");   //读取声量设置值#/
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);
    volumeLevel = mapV["Model"].toInt();
    tmp = 31-volumeLevel;
    tmp = tmp<<8 | tmp;
    SoundAdjustManual(tmp);

    tmp=volumeLevel*3;
    if(tmp<0)
        tmp=0;
    else if(tmp>100)
        tmp=100;

    if(tmp==0)
        G_sound_shielding=true;     //工程模式声音静音标志#/
    else
        G_sound_shielding=false;

    str = QString::number(tmp,10);
    str += "%";

    ui->disVolume->setText(str);

    mapV.clear();
    path.clear();
    path.append("/Config/Engineer_Factoryset/Rebirth");
    xml_NodeReadValue(&doc, path.toLatin1().data(), &mapV);  //治疗模式选择#/
    dietimes= mapV["Die"].toInt();         //死机次数#/
    rebirthtimes= mapV["Bir"].toInt();     //恢复次数#/

#if DEBUG_XML_CONFIG
    Debug_insert("Dialog-CustomXmlConfig-out");
#endif
}

void Dialog::saveEngineerConfig(const char *file)
{
    QString path = "";
    QMap<QString, QString> mapV;
    QDomDocument doc;

    xml_DocReadOpen(&doc, file);
    mapV.clear();
    path.append(tr("/Config/Engineer_Factoryset/PrimeMode"));
    mapV.insert("Model", QString::number(PrimeModel));//PrimeModel
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);


    if(ui->HD_DDBbut->isChecked())
        engineer.factoryset.CureModeSelect=HD_double;
    else if(ui->HD_SDPbut->isChecked())
        engineer.factoryset.CureModeSelect=HD_SNDP;
    else if(ui->HD_SSPbut->isChecked())
        engineer.factoryset.CureModeSelect=HD_SNSP;
    else if(ui->HDF_PREbut->isChecked())
        engineer.factoryset.CureModeSelect=HDF_pre_dilute;
    else if(ui->HDF_POSTbut->isChecked())
        engineer.factoryset.CureModeSelect=HDF_post_dilute;
    else if(ui->HF_PREbut->isChecked())
        engineer.factoryset.CureModeSelect=HF_pre_dilute;
    else if(ui->HF_POSTbut->isChecked())
        engineer.factoryset.CureModeSelect=HF_post_dilute;
    else if(ui->ISO_DDPbut->isChecked())
        engineer.factoryset.CureModeSelect=ISOUF_DNDP;
    else if(ui->ISO_SDPbut->isChecked())
        engineer.factoryset.CureModeSelect=ISOUF_SNDP;
    else if(ui->ISO_SSPbut->isChecked())
        engineer.factoryset.CureModeSelect=ISOUF_SNSP;
    else if(ui->PEbut->isChecked())
        engineer.factoryset.CureModeSelect=PE;

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/CureMode"));
    mapV.insert("Model", QString::number(engineer.factoryset.CureModeSelect));//Cure mode
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);


    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/ProfileMode"));
    if(ui->Na_Profile_Box->isChecked())
    {
        engineer.factoryset.naprofileflag=true;
        mapV.insert("NA", QString::number(1));
    }
    else
    {
        engineer.factoryset.naprofileflag=false;
        mapV.insert("NA", QString::number(0));
    }

    if(ui->UF_Profile_Box->isChecked())
    {
        engineer.factoryset.ufprofileflag=true;
        mapV.insert("UF", QString::number(1));
    }
    else
    {
        engineer.factoryset.ufprofileflag=false;
        mapV.insert("UF", QString::number(0));
    }

    if(ui->Dia_Profile_Box->isChecked())
    {
        engineer.factoryset.diaprofileflag=true;
        mapV.insert("DIA", QString::number(1));
    }
    else
    {
        engineer.factoryset.diaprofileflag=false;
        mapV.insert("DIA", QString::number(0));
    }

    if(ui->Cond_Profile_Box->isChecked())
    {
        engineer.factoryset.condprofileflag=true;
        mapV.insert("COND", QString::number(1));
    }
    else
    {
        engineer.factoryset.condprofileflag=false;
        mapV.insert("COND", QString::number(0));
    }

    if(ui->Temp_Profile_Box->isChecked())
    {
        engineer.factoryset.tempprofileflag=true;
        mapV.insert("TEMP", QString::number(1));
    }
    else
    {
        engineer.factoryset.tempprofileflag=false;
        mapV.insert("TEMP", QString::number(0));
    }

    if(ui->Hep_Profile_Box->isChecked())
    {
        engineer.factoryset.hepprofileflag=true;
        mapV.insert("HEP", QString::number(1));
    }
    else
    {
        engineer.factoryset.hepprofileflag=false;
        mapV.insert("HEP", QString::number(0));
    }

    if(ui->Hco3_Profile_Box->isChecked())
    {
        engineer.factoryset.hco3profileflag=true;
        mapV.insert("HCO3", QString::number(1));
    }
    else
    {
        engineer.factoryset.hco3profileflag=false;
        mapV.insert("HCO3", QString::number(0));
    }
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);


    if(ui->AandB->isChecked())
        engineer.factoryset.Fluidmode=A_and_B;
    else if(ui->AandBPower->isChecked())
        engineer.factoryset.Fluidmode=A_and_BPower;
    else if(ui->Acetate->isChecked())
        engineer.factoryset.Fluidmode=Acetate;
    else if(ui->Center->isChecked())
        engineer.factoryset.Fluidmode=Center;

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/fluidMode"));
    mapV.insert("Model", QString::number(engineer.factoryset.Fluidmode));//Cure mode
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);

    if(ui->OnlineGB->isChecked())
        engineer.factoryset.onlinegetblood=1;
    else if(ui->GBbutton->isChecked())
        engineer.factoryset.onlinegetblood=0;
    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/GetBloodMode"));
    mapV.insert("Model", QString::number(engineer.factoryset.onlinegetblood));//get blood mode
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);

    if(ui->OnlineRB->isChecked())
        engineer.factoryset.onlinereturnblood=1;
    else if(ui->RBbutton->isChecked())
        engineer.factoryset.onlinereturnblood=0;
    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/ReturnBloodMode"));
    mapV.insert("Model", QString::number(engineer.factoryset.onlinereturnblood));//return blood mode
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);

    mapV.clear();
    path.clear();
    path.append(tr("/Config/Engineer_Factoryset/ChemicalDisinfect"));
    mapV.insert("Model", QString::number(gChemicalDisinfect));
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);


    mapV.clear();
    path.clear();
    path.append("/Config/Engineer_Factoryset/DisVolume");   //SAVE声量设置值#/
    mapV.insert("Model", QString::number(volumeLevel));
    xml_NodeWriteValue(&doc, path.toLatin1().data(), &mapV);


    waiteToFile(&doc, file);

    engineer.factoryset.PrimeModel=PrimeModel;
    engineer.factoryset.HeparinSelect=heparin_flag;
    dataNodeUpdate(PACKET_ENGINEER, &engineer);
}

void Dialog::on_SaveBTN_clicked()
{
    saveEngineerConfig();
    saveUntilFilter();
    LogRun->insertLog();

    MESSAGE_ENGINEER_PROMPT_BOX(DE_TR_PROMPT,DE_TR_SAVESUCCESS);
}

void Dialog::on_RestoreBTN_clicked()
{
    msgBox->setInformativeText(DE_TR_WHETHERTORESTOREXMLFILE);
    int ret=msgBox->exec();
    if(ret==QMessageBox::Yes)
    {
        system("rm -rf ./conf/config.xml");
        system("cp ./conf/config_bk.xml ./conf/config.xml");
    }
    else
    {
        return;
    }

    MESSAGE_ENGINEER_PROMPT_BOX(DE_TR_PROMPT,DE_TR_SAVESUCCESS);
}

void Dialog::on_pbtn_update_clicked()
{
    SceneMaintain::isUsbStroageExist = true;//added for debug used
    if(SceneMaintain::isUsbStroageExist == true){
        QString packagePath;

        packagePath = QFileDialog::getExistingDirectory(this, tr("select the update Packag"), "/mnt/usbStorage/",\
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if (!packagePath.isEmpty()) {
            QMessageBox message(QMessageBox::NoIcon, "System Update", "Are you sure to flashing the system?", \
                                QMessageBox::Yes | QMessageBox::No, NULL);
            if(message.exec() == QMessageBox::Yes) {
                if(G_cpid>0)
                    kill(G_cpid,SIGKILL);
                execlp("update-ui", "update-ui", "-qws", packagePath.toLatin1().data(), NULL);
            }
        }
    }else if(SceneMaintain::isUsbStroageExist == false){
        QMessageBox::information(NULL, "warnning", "No usb disk available ", QMessageBox::Yes , QMessageBox::Yes);
    }
}

void Dialog::on_pushButton_2_clicked()
{
    QFile f(PASSWORD_CONFIG_FILE);

    if(!f.open(QIODevice::ReadOnly ))qDebug("Open Passwd config file failed.") ;
    else{
        QString str(f.readAll());
        oldPasswordStr.append('\n');
        if(oldPasswordStr != str){
            qDebug()<<"current passWord error !";
            setPasswordMesssage->setText("current passWord error !");
            }
        else if(newPasswordStr != confirmPasswordStr){
             qDebug()<<"new passWord input error !";
             setPasswordMesssage->setText("new passWord input error !");
            }
        else{
            f.close();
            newPasswordStr.append('\n');
            if(!f.open(QIODevice::WriteOnly | QIODevice::Text  ))qDebug("Open Passwd config file failed.") ;
            else{
                f.write(newPasswordStr.toUtf8());
                f.close();
                setPasswordMesssage->setText("new passWord set ok !");
            }
        }
       }
    oldPasswordStr.clear();
    newPasswordStr.clear();
    confirmPasswordStr.clear();
}


void Dialog::on_pushButton_checkver_clicked()
{
  //return;
  getversion();
}

void Dialog::buttonReturn_clicked()
{
    if(G_SET_SETUPFLAG==true)
    {
        G_SET_SETUPFLAG=false;
        enginerDialog->hide();
        PageShow("Widget DebugMode");
    }
}

void Dialog::version(int id,int index,PacketMasterCtrlSdo *ver)
{
    QString string;
    static unsigned char verdata[CANNODE_NUM][12];

    if(id>=CANNODE_NUM || index<2 || index >4)
        return;

    memcpy(&verdata[id][(index-2)*4], ver->dataSdo, 4); //分别将读到ID号、产品代码、修订号、序列号
    verdata[id][11]=0;
    string.sprintf("%s",&verdata[id][0]);

    readFlag=true;

    switch (id) {
    case CANNODE_BLOODPUMP:
            ui->BloodVStr_2->setText(string);
        break;
    case CANNODE_BLOODPUMP2:
            ui->Blood2VStr_2->setText(string);
        break;
    case CANNODE_MASTER_MONITOR:
            ui->MornitorVStr_2->setText(string);
        break;
    case CANNODE_HEPARINPUMP:
            ui->HeparinVStr_2->setText(string);
        break;
    case CANNODE_MASTER_CONTROL:
            ui->MasterVStr_2->setText(string);
        break;
    case CANNODE_POWER:
            ui->PowerVStr_2->setText(string);
        break;
    default:
        break;
    }

}


void Dialog::getversion()
{

        QString VStr,TStr;
        VStr.sprintf("%s REV %d", VER, REV);
        TStr.sprintf("%s", BUILD_TIME);
  //      ui->versionStr->setText(VStr);
        ui->versionStr->setText(DE_5058_VERSION);
        ui->builtimeStr->setText(TStr);

        ui->BloodVStr_2->setText("null");
        ui->Blood2VStr_2->setText("null");
        ui->MornitorVStr_2->setText("null");
        ui->HeparinVStr_2->setText("null");
        ui->MasterVStr_2->setText("null");
        ui->PowerVStr_2->setText("null");

        CurID=CANNODE_BLOODPUMP;
        CurIndex=2;
        readTimes=0;
        readFlag=false;
        verTimer.start(200);
}

void Dialog::readver()
{
   if(readFlag)
   {
       readTimes=0;
       readFlag=false;
       if(CurIndex==4)
       {
         if(CurID==CANNODE_POWER)
         {
           verTimer.stop();
           return;
         }
         nextID();
         MasterCtrol_CMD_ver_read(CurID,CurIndex);

       }
       else if(CurIndex<4)
       {
         CurIndex++; //01:ID号 02：产品代码 03：修订号 04：序列号
         if(CurIndex>4)CurIndex=4;
         MasterCtrol_CMD_ver_read(CurID,CurIndex);
       }

   }
   else
   {
     readTimes++;
     if(readTimes>10)
      {
         if(CurID==CANNODE_POWER)
         {
           verTimer.stop();
           return;
         }
        nextID();
      }
     if(readTimes==1||readTimes%5==0)
     {
        MasterCtrol_CMD_ver_read(CurID,CurIndex);
     }
   }

}

void Dialog::nextID()
{
    switch (CurID) {
    case CANNODE_BLOODPUMP:
        CurID=CANNODE_BLOODPUMP2;
        break;
    case CANNODE_BLOODPUMP2:
        CurID=CANNODE_MASTER_MONITOR;
        break;
    case CANNODE_MASTER_MONITOR:
        CurID=CANNODE_HEPARINPUMP;
        break;
    case CANNODE_HEPARINPUMP:
        CurID=CANNODE_MASTER_CONTROL;
        break;
    case CANNODE_MASTER_CONTROL:
        CurID=CANNODE_POWER;
        break;
    default:
        break;
    }

    readTimes=0;
    CurIndex=2;
    readFlag=false;

}

void Dialog::showCond(QString bCond,QString mixCond,QString scal_b,QString scal_water)
{
   ui->Target_b->setText("bCond Target:  "+bCond);
   ui->Target_mix->setText("mixCond Target:  "+mixCond);
   ui->CountScal->setText("Scal:    1 : "+scal_b+" : "+scal_water);

}

void Dialog::showNaHCO3(QString na,QString hco3)
{

    ui->lineEditNa->setText(na);
    ui->lineEditHco3->setText(hco3);
    this->nahco3.na=na;
    this->nahco3.hco3=hco3;

}

void Dialog::rstShowNaHCO3(int id)
{
    if(curFormulaID!=id)
    {
        curFormulaID=id;
        ui->lineEditNa->setText(this->nahco3.na);
        ui->lineEditHco3->setText(this->nahco3.hco3);
    }
}


EngineeNAHO3 Dialog::setNaHco3()
{
    EngineeNAHO3 nahco3;
    nahco3.na=ui->lineEditNa->text();
    nahco3.hco3=ui->lineEditHco3->text();
    this->nahco3=nahco3;
    return nahco3;
}

void Dialog::saveUntilFilter()
{
    filter1Info.id=1;
    filter1Info.usedableTime=ui->allEdit->text().toUInt();
    filter1Info.usedTime=ui->usedEdit->text().toUInt();
    filter1Info.remainTime=ui->remainEdit->text().toUInt();
    DataSql_UpdateFilterTimes(filter1Info);

    filter2Info.id=2;
    filter2Info.usedableTime=ui->allEdit_2->text().toUInt();
    filter2Info.usedTime=ui->usedEdit_2->text().toUInt();
    filter2Info.remainTime=ui->remainEdit_2->text().toUInt();
    DataSql_UpdateFilterTimes(filter2Info);
    emit setFilterTimes(filter1Info,filter2Info);
}

void Dialog::showFilterTimes(FilterTimes *ft1,FilterTimes *ft2)
{
    if(ft1!=NULL)
    {
       ui->allEdit->setText(QString::number( ft1->usedableTime));
       ui->usedEdit->setText(QString::number( ft1->usedTime));
       ui->remainEdit->setText(QString::number( ft1->remainTime));
    }
    if(ft2!=NULL)
    {
       ui->allEdit_2->setText(QString::number( ft2->usedableTime));
       ui->usedEdit_2->setText(QString::number( ft2->usedTime));
       ui->remainEdit_2->setText(QString::number( ft2->remainTime));
    }
}

int Dialog::curTabIndex()
{
    return ui->tabWidget->currentIndex();
}

int Dialog::getFormulaID()
{
    return modeFormula->curentFormulaID();
}

void Dialog::showFormulaPage()
{
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(1);
}
