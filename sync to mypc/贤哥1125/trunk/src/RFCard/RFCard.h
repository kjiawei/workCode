#ifndef RFCARD_H
#define RFCARD_H

#include "lang_switch.h"
#include "widget_common.h"
#include "keysetready.h"

#include <QLabel>
#include <QPalette>


#define DE_NAPROFILE_BIT     0x01
#define DE_UFPROFILE_BIT     0x02
#define DE_DIAPROFILE_BIT    0x04
#define DE_HEPPROFILE_BIT    0x08
#define DE_CONDPROFILE_BIT   0x10
#define DE_TEMPPROFILE_BIT   0x20
#define DE_HCO3PROFILE_BIT   0x40


#define DE_DATAPAGE_ROWNUM   12   //数据表格一个页面是多少行


typedef struct save_data_packet
{
    int BP;//血压
    int Pulse;//脉博
    int AP;
    int VP;
    int TMP;
}saveDataPacket;



class PatientMessage : public QWidget, public PieceVisible,public KeySetReady, public LangSetText
{
    Q_OBJECT

public:
    PatientMessage(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

    void lineEdit_init();
    void lineEdit_close(const char *name);
    void lineEdit_data_set(const char *name, const char *data);

public slots:
    void on_Read_clicked(void);  //读取
    void on_Write_clicked(void);  //写入
    void on_Choose_clicked(void);  //选择
    void on_ShowTable_clicked(void);
    void on_TimesPrePage_clicked(void);
    void on_TimesNxtPage_clicked(void);
    void on_FirstPage_clicked(void);
    void on_PrePage_clicked(void);
    void on_NxtPage_clicked(void);
    void on_LastPage_clicked(void);
    void on_DataAnalysis_clicked(void);

private:
 void setUi(QWidget *parent=0);
 void UpdatePatientInfor(stpatientinfor infor);  //根据读取的数据更新页面
 void showDataTable(int page);   //根据页数来刷新数据表格

protected:
    void paintEvent(QPaintEvent *event);


private:

    QLabel labelID;  //编号
    QLineEdit editIDNum;
    QLabel labelCureTimes;  //治疗次数

    QLabel labelCureSel;  //治疗次数选择
    QLabel labelSelNow;  //当前正在进行治疗的ID号

    QLabel labelName;//姓名首字母
    QLineEdit editName;
    QLabel labelSex;
    QComboBox comboBoxSex;
    QLabel labelAge;
    QLineEdit editAge;
 //   QLabel labelFlag;
    QLabel labelDepartment;//科室
    QComboBox comboBoxDepartment;
    QLabel labelDryWeight;//干体重
    QLineEdit editDryWeight;
    QLabel labelDiagnose;//诊断
    QComboBox comboBoxDiagnose;

    QLabel labelPulse;//脉博
    QLabel labelSYS;//SYS血压
    QLabel labelDIA;//DIA血压
    QLabel labelAP;
    QLabel labelVP;
    QLabel labelTMP;

    QLabel labelTime;
    QLabel labelTemperature;
    QLabel labelNa;
    QLabel labelHco3;
    QLabel labelUltrafiltrationVol;//超滤量
    QLabel labelBloodPumpSpeed;
    QLabel labelSubVol;   //置换量
    QLabel labelHeparinVol;   //肝素量
    QLabel labelCureMode;   //治疗方式
    QLabel labelCureProfile;   //治疗曲线


    QLabel labelInformationPrompt; //提示信息显示

    QLineEdit editTime;
    QLineEdit editTemperature;
    QLineEdit editNa;
    QLineEdit editHco3;
    QLineEdit editUltrafiltrationVol;
    QLineEdit editBloodPumpSpeed;

    QPushButton buttonWrite;
    QPushButton buttonRead;
    QPushButton buttonChoose;

    QTableWidget tableWidget;  //记录表对象#/

    QPushButton buttonTimesPrePage;
    QLabel labelTimesInfor;
    QPushButton buttonTimesNxtPage;

    QPushButton buttonFirstPage;
    QPushButton buttonPrePage;
    QLabel labelPageInfor;
    QPushButton buttonNxtPage;
    QPushButton buttonLastPage;
    QPushButton buttonDataAnalysis;  //数据分析

    QLabel labelDataAnalysis;

    QPushButton buttonShowTable;  //切换显示表格数据

    bool showtable_flag;
    int L_idnum;
    int L_curesum;
    int L_curenum_now;

    int L_page_now;  //数据当前页
    int L_page_sum;  //数据总页数

};



#endif // RFCARD_H
