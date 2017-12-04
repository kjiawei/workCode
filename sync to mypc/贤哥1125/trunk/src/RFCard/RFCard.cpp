#include "RFCard.h"
#include "scene_setup_layout.h"
#include "widget_common.h"
#include "widget_setup_other.h"
#include <QLibrary>
#include "common_tr_string.h"
#include "data_sqllite.h"
#include "global_var.h"

#include "interface.h"

#define RFCARD_LABEL(p,x,y)             \
    do {                                        \
        p.setGeometry(QRect(x, SCENE_SETUP_ROW_##y,       \
                    180, SCENE_SETUP_WIDGET_HIGH));;        \
        p.setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
        p.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                    12, CONFIG_FONT_NORMAL)); \
        p.setStyleSheet(CONFIG_COLOR_LABEL);                \
    } while(0)


#define RFCARD_LABEL_INFO(p,x,y)             \
    do {                                        \
        p.setGeometry(QRect(x, SCENE_SETUP_ROW_##y,       \
                    600, SCENE_SETUP_WIDGET_HIGH));;        \
        p.setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
        p.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                    12, CONFIG_FONT_NORMAL)); \
        p.setStyleSheet(CONFIG_COLOR_RED_CHARS);                \
    } while(0)



#define RFCARD_DIS_LABEL(p,x,y)             \
    do {                                        \
        p.setGeometry(QRect(x-10, SCENE_SETUP_ROW_##y,       \
                    80, SCENE_SETUP_WIDGET_HIGH));;        \
        p.setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
        p.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                    12, CONFIG_FONT_NORMAL)); \
        p.setStyleSheet(CONFIG_COLOR_GREEN_CHARS);                \
    } while(0)


#define RFCARD_LINEEDIT(p,x,y)          \
    do {                                        \
        p.setGeometry(QRect(x, SCENE_SETUP_ROW_##y,   \
                    90,         \
                    SCENE_SETUP_WIDGET_HIGH));          \
        LINEEDIT_STYLE(p);                      \
    } while(0)




void PatientMessage::setUi(QWidget *parent)
{
    parent = parent;

    RFCARD_LABEL(labelID, 180, 1);
    RFCARD_LINEEDIT(editIDNum, 230, 1);
    labelCureTimes.setGeometry(QRect(330,76,120,38));
    labelCureTimes.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelCureTimes.setFont(QFont(QString(CONFIG_FONT_STYLE),12, CONFIG_FONT_NORMAL));
    labelCureTimes.setStyleSheet(CONFIG_COLOR_LABEL);

    buttonWrite.setGeometry(QRect(450, 76, 80, 38));
    BUTTON_SIMPLE_STYLE(buttonWrite);

    buttonRead.setGeometry(QRect(570,76,80,38));
    BUTTON_SIMPLE_STYLE(buttonRead);

    buttonChoose.setGeometry(QRect(690,76,80,38));
    BUTTON_SIMPLE_STYLE(buttonChoose);

    labelCureSel.setGeometry(QRect(180,121,320,38));
    labelCureSel.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelCureSel.setFont(QFont(QString(CONFIG_FONT_STYLE),12, CONFIG_FONT_NORMAL));
    labelCureSel.setStyleSheet(CONFIG_COLOR_LABEL);

    buttonTimesPrePage.setGeometry(QRect(330,121,80,30));
    buttonTimesPrePage.setFont(QFont(QString(CONFIG_FONT_STYLE), 12));

    labelTimesInfor.setGeometry(QRect(395,121,100,38));
    labelTimesInfor.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter);
    labelTimesInfor.setFont(QFont(QString(CONFIG_FONT_STYLE),15, CONFIG_FONT_NORMAL));
    labelTimesInfor.setStyleSheet(CONFIG_COLOR_LABEL);

    buttonTimesNxtPage.setGeometry(QRect(480,121,80,30));
    buttonTimesNxtPage.setFont(QFont(QString(CONFIG_FONT_STYLE), 12));

 //   labelSelNow.setGeometry(QRect(180,121,320,38));
    labelSelNow.setGeometry(QRect(180,166,320,38));
    labelSelNow.setAlignment( Qt::AlignVCenter | Qt::AlignLeft);
    labelSelNow.setFont(QFont(QString(CONFIG_FONT_STYLE),12, CONFIG_FONT_NORMAL));
    labelSelNow.setStyleSheet(CONFIG_COLOR_LABEL);

    RFCARD_LABEL(labelName, 180, 4);
    RFCARD_LINEEDIT(editName, 220, 4);
    RFCARD_LABEL(labelSex, 343, 4);
    comboBoxSex.setGeometry(395,211,80,38);


    RFCARD_LABEL(labelAge, 505, 4);
    RFCARD_LINEEDIT(editAge,555,4);
//    RFCARD_LABEL(labelFlag, 505, 2);
    RFCARD_LABEL(labelDepartment, 667, 4);
    comboBoxDepartment.setGeometry(720,211,80,38);


    RFCARD_LABEL(labelDryWeight, 180, 5);
    RFCARD_LINEEDIT(editDryWeight,280,5);
    RFCARD_LABEL(labelDiagnose, 505, 5);
    comboBoxDiagnose.setGeometry(555,256,80,38);


    /***********************************storage items*********************************/
    RFCARD_LABEL(labelPulse, 180, 6);
    RFCARD_LABEL(labelSYS, 343, 6);
    RFCARD_LABEL(labelDIA, 505, 6);
    RFCARD_LABEL(labelAP, 667, 6);

    RFCARD_LABEL(labelVP, 180, 7);
    RFCARD_LABEL(labelTMP, 343, 7);


    /***********************************setable items*********************************/
    RFCARD_LABEL(labelTime, 180, 8);
    RFCARD_LABEL(labelTemperature, 400, 8);
    RFCARD_LABEL(labelNa, 620, 8);

    RFCARD_LABEL(labelHco3, 180, 9);
    RFCARD_LABEL(labelUltrafiltrationVol, 400,9);
    RFCARD_LABEL(labelBloodPumpSpeed, 620, 9);

    RFCARD_LABEL(labelSubVol,180,10);
    RFCARD_LABEL(labelHeparinVol,400,10);
    RFCARD_LABEL(labelCureMode,620,10);

//    RFCARD_LABEL(labelCureProfile,180,11);

    labelCureProfile.setGeometry(QRect(180, SCENE_SETUP_ROW_11,       \
                500, SCENE_SETUP_WIDGET_HIGH));
    labelCureProfile.setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
    labelCureProfile.setFont(QFont(QString(CONFIG_FONT_STYLE),         \
                12, CONFIG_FONT_NORMAL));
    labelCureProfile.setStyleSheet(CONFIG_COLOR_LABEL);


    RFCARD_LABEL_INFO(labelInformationPrompt,180,12);


//    RFCARD_LABEL_INFO(labelInformationOffCard,180,9);


    tableWidget.setRowCount(12);//设置行数
    tableWidget.setColumnCount(9);//设置列数
    tableWidget.setGeometry(QRect(168, 130, 655, 400));//设置tableWidget的大小
 //   tableWidget.horizontalHeader()->setResizeMode(QHeaderView::Stretch); ////使列完全填充并平分
//  tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);//行自适应宽度


    tableWidget.horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    tableWidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //关闭编辑功能#/
    tableWidget.setEditTriggers(QAbstractItemView::NoEditTriggers);
    //单选模式#/
    tableWidget.setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget.setSelectionBehavior(QAbstractItemView::SelectRows);
    //水平和垂直表头的显示#/
    tableWidget.horizontalHeader()->setHidden(false);
    tableWidget.verticalHeader()->setHidden(true);

    buttonFirstPage.setGeometry(QRect(230,550,100,38));
    BUTTON_SIMPLE_STYLE(buttonFirstPage);

    buttonPrePage.setGeometry(QRect(350,550,100,38));
    BUTTON_SIMPLE_STYLE(buttonPrePage);

    labelPageInfor.setGeometry(QRect(450,550,100,38));
    labelPageInfor.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter);
    labelPageInfor.setFont(QFont(QString(CONFIG_FONT_STYLE),15, CONFIG_FONT_NORMAL));
    labelPageInfor.setStyleSheet(CONFIG_COLOR_LABEL);

    buttonNxtPage.setGeometry(QRect(550,550,100,38));
    BUTTON_SIMPLE_STYLE(buttonNxtPage);

    buttonLastPage.setGeometry(QRect(670,550,100,38));
    BUTTON_SIMPLE_STYLE(buttonLastPage);

    buttonDataAnalysis.setGeometry(QRect(200,610,120,40));
    BUTTON_SIMPLE_STYLE(buttonDataAnalysis);

    buttonShowTable.setGeometry(QRect(450,610,120,40));
    BUTTON_SIMPLE_STYLE(buttonShowTable);

    labelDataAnalysis.setGeometry(QRect(170,660,800,200));
    labelDataAnalysis.setAlignment(Qt::AlignLeft);
    labelDataAnalysis.setFont(QFont(QString(CONFIG_FONT_STYLE),12, CONFIG_FONT_NORMAL));
    labelDataAnalysis.setStyleSheet(CONFIG_COLOR_RED_CHARS);
}


PatientMessage::PatientMessage(QWidget *parent):QWidget(parent),
    editIDNum(parent),
    labelID(parent),
    labelCureSel(parent),
    labelCureTimes(parent),
    labelSelNow(parent),
    labelName(parent),
    editName(parent),
    labelSex(parent),
    comboBoxSex(parent),
    labelAge(parent),
    editAge(parent),
 //   labelFlag(parent),
    labelDepartment(parent),
    comboBoxDepartment(parent),
    labelDryWeight(parent),
    editDryWeight(parent),
    labelDiagnose(parent),
    comboBoxDiagnose(parent),
    labelTime(parent),
    labelTemperature(parent),
    labelNa(parent),
    labelHco3(parent),
    labelUltrafiltrationVol(parent),
    labelBloodPumpSpeed(parent),
    labelSubVol(parent),
    labelHeparinVol(parent),
    labelCureMode(parent),
    labelCureProfile(parent),
    labelInformationPrompt(parent),
    labelSYS(parent),
    labelPulse(parent),
    labelAP(parent),
    labelVP(parent),
    labelTMP(parent),
    labelDIA(parent),
    buttonWrite(parent),
    buttonRead(parent),
    buttonChoose(parent),
    tableWidget(parent),
    buttonTimesPrePage(parent),
    labelTimesInfor(parent),
    buttonTimesNxtPage(parent),
    buttonFirstPage(parent),
    buttonPrePage(parent),
    labelPageInfor(parent),
    buttonNxtPage(parent),
    buttonLastPage(parent),
    buttonDataAnalysis(parent),
    labelDataAnalysis(parent),
    buttonShowTable(parent)
{
    setUi(parent);
    PageAppendPiece("RFCard", this);

    connect(&buttonRead,SIGNAL(clicked()),this,SLOT(on_Read_clicked()));
    connect(&buttonWrite,SIGNAL(clicked()),this,SLOT(on_Write_clicked()));
    connect(&buttonChoose,SIGNAL(clicked()),this,SLOT(on_Choose_clicked()));
    connect(&buttonShowTable,SIGNAL(clicked()),this,SLOT(on_ShowTable_clicked()));

    connect(&buttonTimesPrePage,SIGNAL(clicked()),this,SLOT(on_TimesPrePage_clicked()));
    connect(&buttonTimesNxtPage,SIGNAL(clicked()),this,SLOT(on_TimesNxtPage_clicked()));

    connect(&buttonFirstPage,SIGNAL(clicked()),this,SLOT(on_FirstPage_clicked()));
    connect(&buttonPrePage,SIGNAL(clicked()),this,SLOT(on_PrePage_clicked()));
    connect(&buttonNxtPage,SIGNAL(clicked()),this,SLOT(on_NxtPage_clicked()));
    connect(&buttonLastPage,SIGNAL(clicked()),this,SLOT(on_LastPage_clicked()));

    connect(&buttonDataAnalysis,SIGNAL(clicked()),this,SLOT(on_DataAnalysis_clicked()));

    ic->appendWidget(&editIDNum, NumInputMethod,LineEdit,NULL,1,10000);
    ic->appendWidget(&editName,AllInputMethod,LineEdit);
    ic->appendWidget(&editAge,NumInputMethod);
    ic->appendWidget(&editDryWeight,NumInputMethod);

    showtable_flag=false;
    tableWidget.setVisible(false);
    buttonFirstPage.setVisible(false);
    buttonPrePage.setVisible(false);
    labelPageInfor.setVisible(false);
    buttonNxtPage.setVisible(false);
    buttonLastPage.setVisible(false);
    buttonDataAnalysis.setVisible(false);
    labelDataAnalysis.setVisible(false);

    L_idnum=1;
    L_curesum=0;
    L_curenum_now=0;

    G_MAP_PATIENT.clear();   //病人信息数据
    G_MAP_CUREDATA.clear(); //病人治疗记录数据
}


void PatientMessage::on_Read_clicked(void)
{
    L_idnum=editIDNum.text().toInt();

    if(L_idnum==0)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_READPARAMFAIL);
        return;
    }

    G_MAP_PATIENT=DataSql_GetpatientinforByIdnum(L_idnum);

    L_curesum=G_MAP_PATIENT.size();

    if(L_curesum==0)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_NOTTHISIDNUM);
        return;
    }
    else if(L_curesum>0)
    {
        L_curenum_now=1;
        labelTimesInfor.setText(QString::number(L_curenum_now)+"/"+QString::number(L_curesum));
        G_MAP_CUREDATA.clear(); //病人治疗记录数据
        //根据ID号和治疗次号将该次治疗的所有数据从病人信息数据表读取出来#/
        G_MAP_CUREDATA=DataSql_GetcuredatainforByIdnum(L_idnum,L_curenum_now);
    }


    labelCureTimes.setText(DE_TR_CURETIMES+" : "+QString::number(L_curesum));

    UpdatePatientInfor(G_MAP_PATIENT.value(0));

    if(L_curesum>0)
        showYesDialog(DE_TR_PROMPT,DE_TR_READINFORMATIONSUCESS);

}

void PatientMessage::on_Write_clicked(void)
{
    stpatientinfor infor;
    QList<int> list;

    L_idnum=editIDNum.text().toInt();

    if(L_idnum<10)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_THEWRITEIDNUMBER);
        return;
    }

    list=DataSql_patientinforGetNumByIdnum(L_idnum);

    //如果原来已经有了该病人的信息，则通过最后一次治疗信息时间戳timesec是否为0
    //来判断该次治疗是否已进行治疗过，如果没有进行治疗过，则删除该次信息
    //再重新写入。避免没有进行治疗的信息多条写入
    if(list.size()>0)
    {
        //从病人信息数据表里面读取一条数据#/
        infor=DataSql_Readpatientinfor(L_idnum,list.last());

        if(infor.timesec==0)
        {
            //根据ID删除某一次治疗的病人信息数据内容#/
            DataSql_DeletepatientinforByIdnum(L_idnum,list.last());
            //根据ID删除某一次治疗的病人数据#/
            DataSql_DeletecuredatainforByIdnum(L_idnum,list.last());

            infor.curenum=list.last();
        }
        else
            infor.curenum=list.size()+1;
    }
    else
       infor.curenum=1;

    infor.idnum=L_idnum;
//    infor.curenum=list.size()+1;
    infor.timesec=0;   //没进行过治疗，则时间戳为0
    infor.name=editName.text();
    infor.gender=comboBoxSex.currentIndex();
    infor.age=editAge.text().toInt();
    infor.depart=comboBoxDepartment.currentIndex();
    infor.weight=editDryWeight.text().toInt();
    infor.diagno=comboBoxDiagnose.currentIndex();

    infor.time=total_secCount/3600;

    PacketFluid packet;
    dataRead(PACKET_NAME_DIALYSATE,&packet);
    infor.temp=packet.tempTarget;

    FormulaSet formula;
    dataRead(PACKET_FORMULA_TARGET,&formula);
    infor.na=formula.tempBuffer.na;
    infor.hco3=formula.tempBuffer.hco3;

    PacketUF packetUF;
    dataRead(PACKET_NAME_UF, &packetUF);
    infor.ufgoal=packetUF.ufVolAim;

    infor.bldflow=G_SET_BPSPEED;
    infor.subvol=SubsPump_totalvolume;
    PacketPDOHeparinPump heparinPDO;
    dataRead(PACKET_PDO_HEPARINPUMP,&heparinPDO);
    infor.heprate=heparinPDO.speed;
    PacketWorkMode workModeTMP;
    dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
    infor.treatmode=workModeTMP.treatmode.current;

    unsigned int ProfileBit=0;

    if(G_NA_PROFILE_FLAG==true)
    {
        ProfileBit |= DE_NAPROFILE_BIT;
    }
    if(G_UF_PROFILE_FLAG==true)
    {
        ProfileBit |= DE_UFPROFILE_BIT;
    }
    if(G_DIA_PROFILE_FLAG==true)
    {
        ProfileBit |= DE_DIAPROFILE_BIT;
    }
    if(G_HEP_PROFILE_FLAG==true)
    {
        ProfileBit |= DE_HEPPROFILE_BIT;
    }
    if(G_HEP_PROFILE_FLAG==true)
    {
        ProfileBit |= DE_HEPPROFILE_BIT;
    }
    if(G_COND_PROFILE_FLAG==true)
    {
        ProfileBit |= DE_CONDPROFILE_BIT;
    }
    if(G_TEMP_PROFILE_FLAG==true)
    {
        ProfileBit |= DE_TEMPPROFILE_BIT;
    }
    if(G_HCO3_PROFILE_FLAG==true)
    {
        ProfileBit |= DE_HCO3PROFILE_BIT;
    }

    infor.profilemode=ProfileBit;

    //往病人信息数据表里面插入一条数据  插入数据库缓冲列表#/
    DataList_Insertpatientinfor(infor);

    showYesDialog(DE_TR_PROMPT,DE_TR_WRITEINFORMATIONSUCESS);

}

void PatientMessage::on_Choose_clicked(void)
{
    stpatientinfor infor;
    QList<int> list;

    list.clear();

    L_idnum=editIDNum.text().toInt();
    if(L_idnum<10)
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_IDCANNOTLESS10);
        return;
    }

    list=DataSql_patientinforGetNumByIdnum(L_idnum);

    if(list.size()<=0) //尚未写入病人的治疗信息
    {
        showYesDialog(DE_TR_PROMPT,DE_TR_PLEASEWRITEPATIENTFIRST);
        return;
    }

    if(list.size()>0)
    {
        //从病人信息数据表里面读取一条数据#/
        infor=DataSql_Readpatientinfor(L_idnum,list.last());

        if(infor.timesec==0)
        {

            L_curenum_now=list.last();
        }
        else
            L_curenum_now=list.size()+1;
    }

    G_PATIENT_IDNUM_NOW=L_idnum; //当前病人ID号
    G_PATIENT_CUREID_NOW= L_curenum_now;  //当前病人治疗次号
}

void PatientMessage::on_ShowTable_clicked(void)
{
    int tmp;
    showtable_flag = !showtable_flag;

    if(showtable_flag==true)
    {

        L_page_now=1;  //数据当前页

        tmp=G_MAP_CUREDATA.size();

        L_page_sum=tmp/DE_DATAPAGE_ROWNUM;

        tmp=tmp%DE_DATAPAGE_ROWNUM;

        if(tmp>0)
        {
            L_page_sum += 1;
        }


        labelPageInfor.setText(QString::number(L_page_now)+"/"+QString::number(L_page_sum));
        showDataTable(L_page_now);   //根据页数来刷新数据表格

        tableWidget.setVisible(true);
        buttonFirstPage.setVisible(true);
        buttonPrePage.setVisible(true);
        labelPageInfor.setVisible(true);
        buttonNxtPage.setVisible(true);
        buttonLastPage.setVisible(true);
        buttonDataAnalysis.setVisible(true);

        buttonTimesPrePage.setVisible(false);
        labelTimesInfor.setVisible(false);
        buttonTimesNxtPage.setVisible(false);
        buttonWrite.setVisible(false);
        buttonRead.setVisible(false);
        buttonChoose.setVisible(false);
        labelCureProfile.setVisible(false);
        labelInformationPrompt.setVisible(false);
    }
    else
    {
        tableWidget.setVisible(false);
        buttonFirstPage.setVisible(false);
        buttonPrePage.setVisible(false);
        labelPageInfor.setVisible(false);
        buttonNxtPage.setVisible(false);
        buttonLastPage.setVisible(false);
        buttonDataAnalysis.setVisible(false);
        labelDataAnalysis.setVisible(false);

        buttonTimesPrePage.setVisible(true);
        labelTimesInfor.setVisible(true);
        buttonTimesNxtPage.setVisible(true);
        buttonWrite.setVisible(true);
        buttonRead.setVisible(true);
        buttonChoose.setVisible(true);
        labelCureProfile.setVisible(true);
        labelInformationPrompt.setVisible(true);
    }

}

void PatientMessage::on_TimesPrePage_clicked(void)
{

    L_curenum_now--;

    if(L_curenum_now<1)
    {
        L_curenum_now=L_curesum;
    }

    labelTimesInfor.setText(QString::number(L_curenum_now)+"/"+QString::number(L_curesum));
    G_MAP_CUREDATA.clear(); //病人治疗记录数据
    //根据ID号和治疗次号将该次治疗的所有数据从病人信息数据表读取出来#/
    G_MAP_CUREDATA=DataSql_GetcuredatainforByIdnum(L_idnum,L_curenum_now);
    UpdatePatientInfor(G_MAP_PATIENT.value(L_curenum_now-1));

}

void PatientMessage::on_TimesNxtPage_clicked(void)
{
    L_curenum_now++;

    if(L_curenum_now>L_curesum)
    {
        L_curenum_now=1;
    }

    labelTimesInfor.setText(QString::number(L_curenum_now)+"/"+QString::number(L_curesum));
    G_MAP_CUREDATA.clear(); //病人治疗记录数据
    //根据ID号和治疗次号将该次治疗的所有数据从病人信息数据表读取出来#/
    G_MAP_CUREDATA=DataSql_GetcuredatainforByIdnum(L_idnum,L_curenum_now);
    UpdatePatientInfor(G_MAP_PATIENT.value(L_curenum_now-1));
}



void PatientMessage::on_FirstPage_clicked(void)
{
    L_page_now=1;
    showDataTable(L_page_now);   //根据页数来刷新数据表格
    labelPageInfor.setText(QString::number(L_page_now)+"/"+QString::number(L_page_sum));
}

void PatientMessage::on_PrePage_clicked(void)
{
    L_page_now--;
    if(L_page_now<1)
        L_page_now=L_page_sum;
    showDataTable(L_page_now);   //根据页数来刷新数据表格
    labelPageInfor.setText(QString::number(L_page_now)+"/"+QString::number(L_page_sum));

}

void PatientMessage::on_NxtPage_clicked(void)
{
    L_page_now++;
    if(L_page_now>L_page_sum)
        L_page_now=1;
    showDataTable(L_page_now);   //根据页数来刷新数据表格
    labelPageInfor.setText(QString::number(L_page_now)+"/"+QString::number(L_page_sum));

}

void PatientMessage::on_LastPage_clicked(void)
{
    L_page_now=L_page_sum;
    showDataTable(L_page_now);   //根据页数来刷新数据表格
    labelPageInfor.setText(QString::number(L_page_now)+"/"+QString::number(L_page_sum));
}

void PatientMessage::on_DataAnalysis_clicked(void)
{
    QString str;
    int i=0,num=0;
    int AVGt204=0,AVGc702=0,AVGpulse=0,AVGsys=0,AVGdia=0,AVGtmp=0;
    int maxt204=0,maxc702=0,maxpulse=0,maxsys=0,maxdia=0,maxtmp=0;
    int mint204=0,minc702=0,minpulse=0,minsys=0,mindia=0,mintmp=0;

    num=G_MAP_CUREDATA.size();
    if(num>0)
    {
         mint204=G_MAP_CUREDATA.value(0).t204;
         minc702=G_MAP_CUREDATA.value(0).c702;
         minpulse=G_MAP_CUREDATA.value(0).pulse;
         minsys=G_MAP_CUREDATA.value(0).sys;
         mindia=G_MAP_CUREDATA.value(0).dia;
         mintmp=G_MAP_CUREDATA.value(0).tmp;

        for(i=0;i<num;i++)
        {
            AVGt204 += G_MAP_CUREDATA.value(i).t204;
            AVGc702 += G_MAP_CUREDATA.value(i).c702;
            AVGpulse += G_MAP_CUREDATA.value(i).pulse;
            AVGsys += G_MAP_CUREDATA.value(i).sys;
            AVGdia += G_MAP_CUREDATA.value(i).dia;
            AVGtmp += G_MAP_CUREDATA.value(i).tmp;

            if(G_MAP_CUREDATA.value(i).t204<mint204)
                mint204=G_MAP_CUREDATA.value(i).t204;
            if(G_MAP_CUREDATA.value(i).c702<minc702)
                minc702=G_MAP_CUREDATA.value(i).c702;
            if(G_MAP_CUREDATA.value(i).pulse<minpulse)
                minpulse=G_MAP_CUREDATA.value(i).pulse;
            if(G_MAP_CUREDATA.value(i).sys<minsys)
                minsys=G_MAP_CUREDATA.value(i).sys;
            if(G_MAP_CUREDATA.value(i).dia<mindia)
                mindia=G_MAP_CUREDATA.value(i).dia;
            if(G_MAP_CUREDATA.value(i).tmp<mintmp)
                mintmp=G_MAP_CUREDATA.value(i).tmp;

            if(G_MAP_CUREDATA.value(i).t204>maxt204)
                maxt204=G_MAP_CUREDATA.value(i).t204;
            if(G_MAP_CUREDATA.value(i).c702>maxc702)
                maxc702=G_MAP_CUREDATA.value(i).c702;
            if(G_MAP_CUREDATA.value(i).pulse>maxpulse)
                maxpulse=G_MAP_CUREDATA.value(i).pulse;
            if(G_MAP_CUREDATA.value(i).sys>maxsys)
                maxsys=G_MAP_CUREDATA.value(i).sys;
            if(G_MAP_CUREDATA.value(i).dia>maxdia)
                maxdia=G_MAP_CUREDATA.value(i).dia;
            if(G_MAP_CUREDATA.value(i).tmp>maxtmp)
                maxtmp=G_MAP_CUREDATA.value(i).tmp;
        }

        AVGt204=AVGt204/num;
        AVGc702=AVGc702/num;
        AVGpulse=AVGpulse/num;
        AVGsys=AVGsys/num;
        AVGdia=AVGdia/num;
        AVGtmp=AVGtmp/num;

    }



    str=DE_TR_TEMP+" :";
    str.append(DE_TR_MAXVALUE+" "+QString::number(maxt204)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(mint204)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(AVGt204)+" ");
    str.append("  ||  "+DE_TR_COND+" :");
    str.append(DE_TR_MAXVALUE+" "+QString::number(maxc702)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(minc702)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(AVGc702)+" ");

    str.append("\n");
    str.append(DE_TR_PULSE+" :");
    str.append(DE_TR_MAXVALUE+" "+QString::number(maxpulse)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(minpulse)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(AVGpulse)+" ");
    str.append("  ||  "+DE_TR_SYS+" :");
    str.append(DE_TR_MAXVALUE+" "+QString::number(maxsys)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(minsys)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(AVGsys)+" ");

    str.append("\n");
    str.append(DE_TR_DIA+" :");
    str.append(DE_TR_MAXVALUE+" "+QString::number(maxdia)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(mindia)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(AVGdia)+" ");

    labelDataAnalysis.setText(str);

    labelDataAnalysis.setVisible(true);

}


void PatientMessage::CustomSetText()
{
    labelID.setText(tr("ID :"));
    labelCureSel.setText(DE_TR_CURETIMESSELECT+" : ");
//    labelCureTimes.setText(DE_TR_CURETIMES+" : "+QString::number(0));
//    labelSelNow.setText(DE_TR_THISIDNUMBERFORCURRENTLYTREATMENT+QString(" : <font color = red>%1</font>").arg(QString::number(1000)));
    labelName.setText(tr("Name & Surname :"));
    labelSex.setText(tr("Gender :"));
    labelAge.setText(tr("Age :"));
 //   labelFlag.setText(tr("Flag :"));
    labelDepartment.setText(tr("Depart :"));
    labelDryWeight.setText(tr("Dry weight(KG):"));
    labelDiagnose.setText(tr("Diagnose :"));


    labelTime.setText(tr("Time(H):"));
    labelTemperature.setText(tr("Temp :"));
    labelNa.setText(tr("Na+ :"));
    labelHco3.setText(tr("Hco3 :"));
    labelUltrafiltrationVol.setText(tr("UF goal :"));
    labelBloodPumpSpeed.setText(tr("Blood Flow :"));
    labelSubVol.setText(DE_TR_SUBVOLUME+":");
    labelHeparinVol.setText(DE_TR_HEPARINVOLUME+":");
    labelCureMode.setText(DE_TR_TREATMENTMODE+":");
    labelCureProfile.setText(DE_TR_PROFILEMODE+":");

    labelInformationPrompt.setText(" ");

    buttonWrite.setText(DE_TR_WRITE);
    buttonRead.setText(DE_TR_READ);
    buttonChoose.setText(DE_TR_CHOOSE);
    buttonShowTable.setText(DE_TR_DATATABLE);
    buttonTimesPrePage.setText(DE_TR_PRETIMES);
    buttonTimesNxtPage.setText(DE_TR_NXTTIMES);
    buttonFirstPage.setText(DE_TR_FIRSTPAGE);
    buttonPrePage.setText(DE_TR_PREPAGE);
    buttonNxtPage.setText(DE_TR_NXTPAGE);
    buttonLastPage.setText(DE_TR_LASTPAGE);

    buttonDataAnalysis.setText(DE_TR_DATAANALYSIS);
    labelTimesInfor.setText("0/0");
    labelPageInfor.setText("0/0");


    /***********************************Data analysis*********************************/
  /*
    QString str;
    str=DE_TR_TEMP+" :";
    str.append(DE_TR_MAXVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(0)+" ");
    str.append("  ||  "+DE_TR_COND+" :");
    str.append(DE_TR_MAXVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(0)+" ");

    str.append("\n");
    str.append(DE_TR_PULSE+" :");
    str.append(DE_TR_MAXVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(0)+" ");
    str.append("  ||  "+DE_TR_SYS+" :");
    str.append(DE_TR_MAXVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(0)+" ");

    str.append("\n");
    str.append(DE_TR_DIA+" :");
    str.append(DE_TR_MAXVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_MINVALUE+" "+QString::number(0)+" ");
    str.append(DE_TR_AVGVALUE+" "+QString::number(0)+" ");

    labelDataAnalysis.setText(str);
   */

    /***********************************Data analysis*********************************/

    /***********************************storage items*********************************/
    labelPulse.setText(DE_TR_PULSEAVG+QString(": <font color = red>%1</font>").arg("---"));
    labelSYS.setText(DE_TR_SYSAVG+QString(": <font color = red>%1</font>").arg("---"));
    labelDIA.setText(DE_TR_DIAAVG+QString(": <font color = red>%1</font>").arg("---"));
    labelAP.setText(DE_TR_APAVG+QString(": <font color = red>%1</font>").arg("---"));
    labelVP.setText(DE_TR_VPAVG+QString(": <font color = red>%1</font>").arg("---"));
    labelTMP.setText(DE_TR_TMPAVG+QString(": <font color = red>%1</font>").arg("---"));

}


void PatientMessage::paintEvent(QPaintEvent *event)
{
    event = event;
}


void PatientMessage::setCusVisible(bool vis, char* para)
{
    static bool onceflag=false;

    para = para;
    QString strtmp;
   //this->setVisible(vis);

    if(onceflag==false&&vis==true)
    {
        comboBoxSex.insertItem(0,DE_TR_MALE);
        comboBoxSex.insertItem(1,DE_TR_FEMALE);

        comboBoxDepartment.insertItem(0,DE_TR_RENALMEDICINE);
        comboBoxDepartment.insertItem(1,DE_TR_ENDOCRINE);

        comboBoxDiagnose.insertItem(0,DE_TR_RENALFAILURE);
        comboBoxDiagnose.insertItem(1,DE_TR_UREMIA);

        tableWidget.setHorizontalHeaderLabels(QStringList()<<tr("ID")<<DE_TR_DATE<<DE_TR_TEMP<<DE_TR_COND<<DE_TR_AP<<DE_TR_VP<<DE_TR_TMP<<DE_TR_PULSE<<DE_TR_SYSDIA);

        tableWidget.setColumnWidth(0, 50);
        tableWidget.setColumnWidth(1, 110);
        tableWidget.setColumnWidth(2, 60);
        tableWidget.setColumnWidth(3, 60);
        tableWidget.setColumnWidth(4, 70);
        tableWidget.setColumnWidth(5, 70);
        tableWidget.setColumnWidth(6, 70);
        tableWidget.setColumnWidth(7, 60);
        tableWidget.setColumnWidth(8, 105);

        onceflag=true;
    }

    if(vis==false)
    {
        if(labelPageInfor.isVisible()==true)
        {
            on_ShowTable_clicked();
        }
    }

    if((vis==true)&&(G_PATIENT_IDNUM_NOW==1)&&(G_PATIENT_CURESUM_NOW==0)&&(L_idnum==1)&&(L_curesum==0)) //开机默认没有选定ID号，则病人ID号默认为1号
    {
        editIDNum.setText("1");
        on_Read_clicked();
    }


    if(vis==true&&(G_PATIENT_IDNUM_NOW==L_idnum))
    {
        labelCureTimes.setText(DE_TR_CURETIMES+" : "+QString::number(L_curesum));
        labelTimesInfor.setText(QString::number(L_curenum_now)+"/"+QString::number(L_curesum));
        labelSelNow.setText(DE_TR_THISIDNUMBERFORCURRENTLYTREATMENT+QString(" : <font color = red>%1</font>").arg(QString::number(G_PATIENT_IDNUM_NOW)));
        labelName.setText(tr("Name & Surname :"));
        labelSex.setText(tr("Gender :"));
        labelAge.setText(tr("Age :"));
     //   labelFlag.setText(tr("Flag :"));
        labelDepartment.setText(tr("Depart :"));
        labelDryWeight.setText(tr("Dry weight(KG):"));
        labelDiagnose.setText(tr("Diagnose :"));

        labelTime.setText(DE_TR_TIMEH+QString(": <font color = red>%1</font>").arg(QString::number(total_secCount/3600)));

        PacketFluid packet;
        dataRead(PACKET_NAME_DIALYSATE,&packet);
        labelTemperature.setText(DE_TR_TEMP+QString("(C): <font color = red>%1</font>").arg(QString::number(packet.tempTarget)));

        FormulaSet formula;
        dataRead(PACKET_FORMULA_TARGET,&formula);
        labelNa.setText(DE_TR_NA+QString("(mmol/L): <font color = red>%1</font>").arg(QString::number(formula.tempBuffer.na)));
        labelHco3.setText(DE_TR_HCO3+QString("(mmol/L): <font color = red>%1</font>").arg(QString::number(formula.tempBuffer.hco3)));

        PacketUF packetUF;
        dataRead(PACKET_NAME_UF, &packetUF);
        labelUltrafiltrationVol.setText(DE_TR_UFGOAL+QString("(ml): <font color = red>%1</font>").arg(QString::number(packetUF.ufVolAim)));

        labelBloodPumpSpeed.setText(DE_TR_BLOODFLOW+QString("(ml/min): <font color = red>%1</font>").arg(QString::number(G_SET_BPSPEED)));

        labelSubVol.setText(DE_TR_SUBVOLUME+QString("(ml): <font color = red>%1</font>").arg(QString::number(SubsPump_totalvolume)));

        PacketPDOHeparinPump heparinPDO;
        dataRead(PACKET_PDO_HEPARINPUMP,&heparinPDO);
        labelHeparinVol.setText(DE_TR_HEPARINRATE+QString("(ml/h): <font color = red>%1</font>").arg(QString::number(heparinPDO.speed)));

        PacketWorkMode workModeTMP;
        dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
        switch(workModeTMP.treatmode.current)
        {
        case HD_double:
            strtmp=DE_TR_HDDOUBLENEEDLE;
            break;
        case HD_SNDP:
            strtmp=DE_TR_HDSNDP;
            break;
        case HD_SNSP:
            strtmp=DE_TR_HDSNSP;
            break;
        case HDF_pre_dilute://前置换 透析滤过
            strtmp=DE_TR_HDFONLINE;
            break;
        case HDF_post_dilute://后置换 透析滤过
            strtmp=DE_TR_HDFONLINE;
            break;
        case HF_pre_dilute://
            strtmp=DE_TR_HFONLINE;
            break;
        case HF_post_dilute://
            strtmp=DE_TR_HFONLINE;
            break;
        case ISOUF_DNDP://
            strtmp=DE_TR_ISODOUBLENEEDLE;
            break;
        case ISOUF_SNDP://
            strtmp=DE_TR_ISOSNDP;
            break;
        case ISOUF_SNSP://
            strtmp=DE_TR_ISOSNSP;
            break;
        case PE ://
            strtmp=DE_TR_PEPH;
            break;
        }
        labelCureMode.setText(DE_TR_TREATMENTMODE+QString(": <font color = red>%1</font>").arg(strtmp));

        unsigned int ProfileBit=0;

        strtmp.clear();
        if(G_NA_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_NAPROFILE_BIT;
            strtmp.append(DE_TR_NAPROFILE+"  ");
        }
        if(G_UF_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_UFPROFILE_BIT;
            strtmp.append(DE_TR_UFPROFILE+"  ");
        }
        if(G_DIA_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_DIAPROFILE_BIT;
            strtmp.append(DE_TR_DIAFLOWPROFILE+"  ");
        }
        if(G_HEP_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_HEPPROFILE_BIT;
            strtmp.append(DE_TR_HEPARINPROFILEE+"  ");
        }
        if(G_HEP_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_HEPPROFILE_BIT;
            strtmp.append(DE_TR_HEPARINPROFILEE+"  ");
        }
        if(G_COND_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_CONDPROFILE_BIT;
            strtmp.append(DE_TR_CONDPROFILE+"  ");
        }
        if(G_TEMP_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_TEMPPROFILE_BIT;
            strtmp.append(DE_TR_TEMPPROFILE+"  ");
        }
        if(G_HCO3_PROFILE_FLAG==true)
        {
            ProfileBit |= DE_HCO3PROFILE_BIT;
            strtmp.append(DE_TR_HCO3PROFILE+"  ");
        }

        if(ProfileBit==0)
        {
           strtmp=DE_TR_TRNONE;
        }

        labelCureProfile.setText(DE_TR_PROFILEMODE+QString(": <font color = red>%1</font>").arg(strtmp));
    }

    buttonWrite.setVisible(vis);
    buttonRead.setVisible(vis);
    buttonChoose.setVisible(vis);

    labelID.setVisible(vis);
    editIDNum.setVisible(vis);
    labelCureSel.setVisible(vis);
    labelCureTimes.setVisible(vis);
    buttonTimesPrePage.setVisible(vis);
    labelTimesInfor.setVisible(vis);
    buttonTimesNxtPage.setVisible(vis);
    labelName.setVisible(vis);
    editName.setVisible(vis);

    labelSex.setVisible(vis);
    comboBoxSex.setVisible(vis);

    labelAge.setVisible(vis);
    editAge.setVisible(vis);
 //   labelFlag.setVisible(vis);
    labelDepartment.setVisible(vis);
    comboBoxDepartment.setVisible(vis);

    labelDryWeight.setVisible(vis);
    editDryWeight.setVisible(vis);
    labelDiagnose.setVisible(vis);
    comboBoxDiagnose.setVisible(vis);

    labelTime.setVisible(vis);
    labelTemperature.setVisible(vis);
    labelNa.setVisible(vis);
    labelHco3.setVisible(vis);
    labelUltrafiltrationVol.setVisible(vis);
    labelBloodPumpSpeed.setVisible(vis);
    labelSubVol.setVisible(vis);
    labelHeparinVol.setVisible(vis);
    labelCureMode.setVisible(vis);
    labelCureProfile.setVisible(vis);

    labelInformationPrompt.setVisible(vis);

    labelSelNow.setVisible(vis);

    editTime.setVisible(false);
    editTemperature.setVisible(false);
    editNa.setVisible(false);
    editHco3.setVisible(false);
    editUltrafiltrationVol.setVisible(false);
    editBloodPumpSpeed.setVisible(false);


    labelPulse.setVisible(vis);
    labelSYS.setVisible(vis);
    labelDIA.setVisible(vis);
    labelAP.setVisible(vis);
    labelVP.setVisible(vis);
    labelTMP.setVisible(vis);

    buttonShowTable.setVisible(vis);

 //   tableWidget.setVisible(vis);

}

void PatientMessage::lineEdit_init()
{

}

void PatientMessage::lineEdit_close(const char *name)
{
    name=name;
}

void PatientMessage::lineEdit_data_set(const char *name, const char *data)
{
    name=name;
    data=data;
}


 void PatientMessage::UpdatePatientInfor(stpatientinfor infor)  //根据读取的数据更新页面
 {
     QString strtmp;

     editName.setText(infor.name);
     comboBoxSex.setCurrentIndex(infor.gender);
     editAge.setText(QString::number(infor.age));

     comboBoxDepartment.setEditText(comboBoxDepartment.itemText(infor.depart));
     editDryWeight.setText(QString::number(infor.weight));
     comboBoxDiagnose.setEditText(comboBoxDiagnose.itemText(infor.diagno));

     int i=0,num=0,pulse=0,sys=0,dia=0,ap=0,vp=0,tmp=0;

     num=G_MAP_CUREDATA.size();
     if(num>0)
     {

         for(i=0;i<num;i++)
         {
             pulse += G_MAP_CUREDATA.value(i).pulse;
             sys += G_MAP_CUREDATA.value(i).sys;
             dia += G_MAP_CUREDATA.value(i).dia;
             ap += G_MAP_CUREDATA.value(i).ap;
             vp += G_MAP_CUREDATA.value(i).vp;
             tmp += G_MAP_CUREDATA.value(i).tmp;
         }

         pulse=pulse/num;
         sys=sys/num;
         dia=dia/num;
         ap=ap/num;
         vp=vp/num;
         tmp=tmp/num;


         labelPulse.setText(DE_TR_PULSEAVG+QString(": <font color = red>%1</font>").arg(QString::number(pulse)));
         labelSYS.setText(DE_TR_SYSAVG+QString(": <font color = red>%1</font>").arg(QString::number(sys)));
         labelDIA.setText(DE_TR_DIAAVG+QString(": <font color = red>%1</font>").arg(QString::number(dia)));
         labelAP.setText(DE_TR_APAVG+QString(": <font color = red>%1</font>").arg(QString::number(ap)));
         labelVP.setText(DE_TR_VPAVG+QString(": <font color = red>%1</font>").arg(QString::number(vp)));
         labelTMP.setText(DE_TR_TMPAVG+QString(": <font color = red>%1</font>").arg(QString::number(tmp)));

     }
     else
     {
         labelPulse.setText(DE_TR_PULSEAVG+QString(": <font color = red>%1</font>").arg("---"));
         labelSYS.setText(DE_TR_SYSAVG+QString(": <font color = red>%1</font>").arg("---"));
         labelDIA.setText(DE_TR_DIAAVG+QString(": <font color = red>%1</font>").arg("---"));
         labelAP.setText(DE_TR_APAVG+QString(": <font color = red>%1</font>").arg("---"));
         labelVP.setText(DE_TR_VPAVG+QString(": <font color = red>%1</font>").arg("---"));
         labelTMP.setText(DE_TR_TMPAVG+QString(": <font color = red>%1</font>").arg("---"));
     }


     labelTime.setText(DE_TR_TIMEH+QString(": <font color = red>%1</font>").arg(QString::number(infor.time)));
     labelTemperature.setText(DE_TR_TEMP+QString("(C): <font color = red>%1</font>").arg(QString::number(infor.temp/10)));

     labelNa.setText(DE_TR_NA+QString("(mmol/L): <font color = red>%1</font>").arg(QString::number(infor.na/10)));
     labelHco3.setText(DE_TR_HCO3+QString("(mmol/L): <font color = red>%1</font>").arg(QString::number(infor.hco3/10)));
     labelUltrafiltrationVol.setText(DE_TR_UFGOAL+QString("(ml): <font color = red>%1</font>").arg(QString::number(infor.ufgoal)));
     labelBloodPumpSpeed.setText(DE_TR_BLOODFLOW+QString("(ml/min): <font color = red>%1</font>").arg(QString::number(infor.bldflow)));
     labelSubVol.setText(DE_TR_SUBVOLUME+QString("(ml): <font color = red>%1</font>").arg(QString::number(infor.subvol)));
     labelHeparinVol.setText(DE_TR_HEPARINRATE+QString("(ml/h): <font color = red>%1</font>").arg(QString::number(infor.heprate)));
     switch(infor.treatmode)
     {
     case HD_double:
         strtmp=DE_TR_HDDOUBLENEEDLE;
         break;
     case HD_SNDP:
         strtmp=DE_TR_HDSNDP;
         break;
     case HD_SNSP:
         strtmp=DE_TR_HDSNSP;
         break;
     case HDF_pre_dilute://前置换 透析滤过
         strtmp=DE_TR_HDFONLINE;
         break;
     case HDF_post_dilute://后置换 透析滤过
         strtmp=DE_TR_HDFONLINE;
         break;
     case HF_pre_dilute://
         strtmp=DE_TR_HFONLINE;
         break;
     case HF_post_dilute://
         strtmp=DE_TR_HFONLINE;
         break;
     case ISOUF_DNDP://
         strtmp=DE_TR_ISODOUBLENEEDLE;
         break;
     case ISOUF_SNDP://
         strtmp=DE_TR_ISOSNDP;
         break;
     case ISOUF_SNSP://
         strtmp=DE_TR_ISOSNSP;
         break;
     case PE ://
         strtmp=DE_TR_PEPH;
         break;
     }
     labelCureMode.setText(DE_TR_TREATMENTMODE+QString(": <font color = red>%1</font>").arg(strtmp));

     unsigned int ProfileBit=0;
     ProfileBit=infor.profilemode;
     strtmp.clear();

     if(ProfileBit & DE_NAPROFILE_BIT)
     {
         strtmp.append(DE_TR_NAPROFILE+"  ");
     }
     if(ProfileBit & DE_UFPROFILE_BIT)
     {
         strtmp.append(DE_TR_UFPROFILE+"  ");
     }
     if(ProfileBit & DE_DIAPROFILE_BIT)
     {
         strtmp.append(DE_TR_DIAFLOWPROFILE+"  ");
     }
     if(ProfileBit & DE_HEPPROFILE_BIT)
     {
         strtmp.append(DE_TR_HEPARINPROFILEE+"  ");
     }
     if(ProfileBit & DE_HEPPROFILE_BIT)
     {
         strtmp.append(DE_TR_HEPARINPROFILEE+"  ");
     }
     if(ProfileBit & DE_CONDPROFILE_BIT)
     {
         strtmp.append(DE_TR_CONDPROFILE+"  ");
     }
     if(ProfileBit & DE_TEMPPROFILE_BIT)
     {
         strtmp.append(DE_TR_TEMPPROFILE+"  ");
     }
     if(ProfileBit & DE_HCO3PROFILE_BIT)
     {
         strtmp.append(DE_TR_HCO3PROFILE+"  ");
     }

     if(ProfileBit==0)
     {
        strtmp=DE_TR_TRNONE;
     }

     labelCureProfile.setText(DE_TR_PROFILEMODE+QString(": <font color = red>%1</font>").arg(strtmp));

 }


void PatientMessage::showDataTable(int page)   //根据页数来刷新数据表格
{

    int i=0,j=0;
    int begin=(page-1)*12;
    int end=page*12;

    if(end>G_MAP_CUREDATA.size())
    {
        end=G_MAP_CUREDATA.size();
    }


    int rownum,colnum;
    QTableWidgetItem *item=0;
    QDateTime timeobj;

    rownum=tableWidget.rowCount();
    colnum=tableWidget.columnCount();

    for(i=rownum-1;i>=0;i--)
    {
        for(j=colnum-1;j>=0;j--)
        {
            item=tableWidget.item(i,j);
            if(item!=0)
                delete item;
            item=0;
        }
 //       table->removeRow(i);
    }

    j=0;
    for(i=begin;i<end;i++)
    {
        if(j<DE_DATAPAGE_ROWNUM)
        {
            timeobj.setTime_t(G_MAP_CUREDATA.value(i).timesec);
            tableWidget.setItem(j,0,new QTableWidgetItem(QString::number(i)));
            tableWidget.setItem(j,1,new QTableWidgetItem(timeobj.toString("yyyy-MM-dd hh:mm")));
            tableWidget.setItem(j,2,new QTableWidgetItem(QString::number(G_MAP_CUREDATA.value(i).t204)));
            tableWidget.setItem(j,3,new QTableWidgetItem(QString::number(G_MAP_CUREDATA.value(i).c702)));
            tableWidget.setItem(j,4,new QTableWidgetItem(QString::number(G_MAP_CUREDATA.value(i).ap)));
            tableWidget.setItem(j,5,new QTableWidgetItem(QString::number(G_MAP_CUREDATA.value(i).vp)));
            tableWidget.setItem(j,6,new QTableWidgetItem(QString::number(G_MAP_CUREDATA.value(i).tmp)));
            tableWidget.setItem(j,7,new QTableWidgetItem(QString::number(G_MAP_CUREDATA.value(i).pulse)));
            tableWidget.setItem(j,8,new QTableWidgetItem(QString::number(G_MAP_CUREDATA.value(i).sys)+"/"+QString::number(G_MAP_CUREDATA.value(i).dia)));
        }
        j++;
    }

}




