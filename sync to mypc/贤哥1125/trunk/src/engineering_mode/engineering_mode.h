#ifndef ENGINEERING_MODE_H
#define ENGINEERING_MODE_H

#include <QDialog>
#include "scene_maintain_project_params_common.h"
#include "scene_maintain_project_eeprom.h"
#include "scene_maintain_project_curve.h"
#include "scene_maintain.h"
#include "scene_maintain_project_eeprom.h"
#include "scene_maintain_project_formulaset.h"
#include "xml_config.h"
#include "config_xml.h"
#include "data_set.h"
#include "scene_note.h"
#include "scene_disinfect.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog,public XmlConfig
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
     void ui_translate();
     void CustomXmlConfig(const char *file=DEFAULT_XML_FILE, const char *cmd=DEFAULT_XML_LOAD);
     void getversion();
     void version(int id,int index,PacketMasterCtrlSdo *ver);
     void showCond(QString bCond,QString mixCond,QString scal_b,QString scal_water);
     void showNaHCO3(QString na,QString hco3);
     EngineeNAHO3 setNaHco3();
     void showFilterTimes(FilterTimes *ft1=NULL,FilterTimes *ft2=NULL);
     int curTabIndex();
     int getFormulaID();
     void showFormulaPage();
     int curFormulaID;
public slots:
     void rstShowNaHCO3(int id);
signals:
    void setFilterTimes(FilterTimes ft1,FilterTimes ft2);
protected:
    void showEvent (QShowEvent * event);
private:
    //void WriteXmlEngineerConfig(const char *file=DEFAULT_XML_FILE, const char *cmd=DEFAULT_XML_LOAD);
     void saveEngineerConfig(const char *file = DEFAULT_XML_FILE);
     void saveUntilFilter();
     void nextID();
private slots:
    void on_pushButton_clicked(); //关闭窗口按钮处理#/
    void index_change(int);

    void on_increase_clicked();  //声音加#/

    void on_decrease_clicked();

    void on_autoOff_clicked();

    void on_remainingTime_clicked();

    void on_lastPrograms_clicked();

    void on_filterChange_clicked();

    void on_radioPeraceticAcid_clicked();

    void on_radioSodiumCitrate_clicked();

    void on_radioChemical3_clicked();

    void on_radioCentralHot_clicked();

    void on_radioCentralChemical_clicked();

    void on_PrimeAuto_clicked();

    void on_PrimeManual_clicked();

    void on_PrimeOnline_clicked();

    void on_HeparinYes_clicked();

    void on_HeparinNo_clicked();

    void on_SaveBTN_clicked();

    void on_RestoreBTN_clicked();

    void on_pbtn_update_clicked();

    void newPasswdInputClicked();

    void confirmPasswdInputClicked();

    void oldPasswdInputClicked();

    void on_pushButton_2_clicked();

    void on_pushButton_checkver_clicked();

    void buttonReturn_clicked();

    void readver();

private:
    Ui::Dialog *ui;
    SceneProjectModeParamsCommon *modeParamsCommon;  //参数设置#/
    SceneProjectModeEeprom *modeEEprom;//读EEprom#/
    SceneProjectModeCurve *modeCurve;   //曲线设置#/
    SceneProjectModeFormulaSet *modeFormula;//配方#/
    QLineEdit disVolume;
    QMyEdit *oldPassword;
    QMyEdit *newPassword;
    QMyEdit *confirmPassword;
    QLabel   *setPasswordMesssage;
    QPushButton *buttonReturn;
    int volumeLevel;
    char PrimeModel;
    bool heparin_flag;
    QString oldPasswordStr;
    QString newPasswordStr;
    QString confirmPasswordStr;
    QTimer verTimer;
    bool readFlag;
    int CurID;
    int CurIndex;
    int readTimes;
    FilterTimes filter1Info,filter2Info;
    EngineeNAHO3 nahco3;
};

#endif // ENGINEERING_MODE_H
