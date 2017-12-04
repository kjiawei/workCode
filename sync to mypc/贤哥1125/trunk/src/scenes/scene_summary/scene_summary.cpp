#include "config_ui.h"
#include "widget_common.h"
#include "scene_summary.h"
#include "version.h"
#include "../../common/global_var.h"
#include "common_tr_string.h"
#include "data_sqllite.h"

#define SCENE_SUMMARY_WIDGET_HIGH       30
#define SCENE_SUMMARY_ROW_0             65
#define SCENE_SUMMARY_ROW_1             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_0+5)
#define SCENE_SUMMARY_ROW_2             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_1+5)
#define SCENE_SUMMARY_ROW_3             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_2+5)
#define SCENE_SUMMARY_ROW_4             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_3+5)
#define SCENE_SUMMARY_ROW_5             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_4+5)
#define SCENE_SUMMARY_ROW_6             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_5+5)
#define SCENE_SUMMARY_ROW_7             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_6+5)
#define SCENE_SUMMARY_ROW_8             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_7+5)
#define SCENE_SUMMARY_ROW_9             (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_8+5)
#define SCENE_SUMMARY_ROW_10            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_9+5)
#define SCENE_SUMMARY_ROW_11            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_10+5)
#define SCENE_SUMMARY_ROW_12            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_11+5)
#define SCENE_SUMMARY_ROW_13            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_12+5)
#define SCENE_SUMMARY_ROW_14            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_13+5)
#define SCENE_SUMMARY_ROW_15            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_14+5)
#define SCENE_SUMMARY_ROW_16            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_15+5)
#define SCENE_SUMMARY_ROW_17            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_16+5)
#define SCENE_SUMMARY_ROW_18            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_17+5)
#define SCENE_SUMMARY_ROW_19            (SCENE_SUMMARY_WIDGET_HIGH+SCENE_SUMMARY_ROW_18+5)

#define SCENE_SUMMARY_TABLE(x,y)                        \
    do {                                                \
        x.setGeometry(QRect(20, SCENE_SUMMARY_ROW_##y,  \
                    145, SCENE_SUMMARY_WIDGET_HIGH));   \
        x.setAlignment( Qt::AlignVCenter | Qt::AlignRight); \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        x.setStyleSheet(CONFIG_COLOR_LABEL);            \
    } while(0)

#define SCENE_SUMMARY_VALUE(x,y)                        \
    do {                                                \
        x.setGeometry(QRect(122+35, SCENE_SUMMARY_ROW_##y+3,\
                    60, SCENE_SUMMARY_WIDGET_HIGH-6));    \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        x.setAlignment(Qt::AlignVCenter | Qt::AlignCenter);    \
        x.setStyleSheet("color:#7171f8");       \
    } while(0)

#define SCENE_SUMMARY_UNIT(x,y)                        \
    do {                                                \
        x.setGeometry(QRect(185+35, SCENE_SUMMARY_ROW_##y, \
                    70, SCENE_SUMMARY_WIDGET_HIGH));    \
        x.setAlignment( Qt::AlignVCenter | Qt::AlignLeft); \
        x.setFont(QFont(QString(CONFIG_FONT_STYLE),     \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
        x.setStyleSheet(CONFIG_COLOR_LABEL);    \
    } while(0)

inline void SceneSummary::setUi()
{
    labelTitle.setGeometry(QRect(34, 21, 650, 30));
    LABEL_TITLE_STYLE(labelTitle);

    rebirthinfo.setGeometry(QRect(680,60,150,30));
    rebirthinfo.setAlignment(Qt::AlignLeft);
    rebirthinfo.setFont(QFont(QString(CONFIG_FONT_STYLE),\
                CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    rebirthinfo.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);


    SCENE_SUMMARY_TABLE(labelCurveType, 0);
//    SCENE_SUMMARY_VALUE(labelCurveValue,0);
    labelCurveValue.setGeometry(QRect(122+45, SCENE_SUMMARY_ROW_0+3,
                150, SCENE_SUMMARY_WIDGET_HIGH-6));
    labelCurveValue.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelCurveValue.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelCurveValue.setStyleSheet("color:#7171f8");
    labelCurveValue.setNum(0);

    SCENE_SUMMARY_TABLE(labelDilutionType, 1);
//    SCENE_SUMMARY_VALUE(labelDilutionValue,1);
    labelDilutionValue.setGeometry(QRect(122+45, SCENE_SUMMARY_ROW_1+3,
                150, SCENE_SUMMARY_WIDGET_HIGH-6));
    labelDilutionValue.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelDilutionValue.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelDilutionValue.setStyleSheet("color:#7171f8");
    labelDilutionValue.setNum(0);

    SCENE_SUMMARY_TABLE(labelTimeLeft, 2);
    //SCENE_SUMMARY_VALUE(labelTimeLeftValue, 2);
    labelTimeLeftValue.setGeometry(QRect(122+45, SCENE_SUMMARY_ROW_2+3,
                70, SCENE_SUMMARY_WIDGET_HIGH-6));
    labelTimeLeftValue.setFont(QFont(QString(CONFIG_FONT_STYLE),
                CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL));
    labelTimeLeftValue.setAlignment(Qt::AlignVCenter | Qt::AlignCenter);
    labelTimeLeftValue.setStyleSheet("color:#7171f8");
    labelTimeLeftValue.setNum(0);

    SCENE_SUMMARY_TABLE(labelUfRate, 3);
    SCENE_SUMMARY_VALUE(labelUfRateValue, 3);
    SCENE_SUMMARY_UNIT(labelUfRateUnit, 3);
    labelUfRateValue.setNum(2000);
    labelUfRateUnit.setText("ml/h");

    SCENE_SUMMARY_TABLE(labelUfGoal, 4);
    SCENE_SUMMARY_VALUE(labelUfGoalValue, 4);
    SCENE_SUMMARY_UNIT(labelUfGoalUnit, 4);
    labelUfGoalValue.setNum(0);
    labelUfGoalUnit.setText("ml");
    /*
    SCENE_SUMMARY_TABLE(labelUfVolume, 4);
    SCENE_SUMMARY_VALUE(labelUfVolumeValue, 4);
    SCENE_SUMMARY_UNIT(labelUfVolumeUnit, 4);
    labelUfVolumeValue.setNum(0);
    labelUfVolumeUnit.setText("ml");
    */

    SCENE_SUMMARY_TABLE(labelAP, 5);
    SCENE_SUMMARY_VALUE(labelAPValue, 5);
    SCENE_SUMMARY_UNIT(labelAPUnit, 5);
    labelAPValue.setNum(30);
    labelAPUnit.setText("mmHg");

    SCENE_SUMMARY_TABLE(labelVP, 6);
    SCENE_SUMMARY_VALUE(labelVPValue, 6);
    SCENE_SUMMARY_UNIT(labelVPUnit, 6);
    labelVPValue.setNum(3);
    labelVPUnit.setText("mmHg");

    SCENE_SUMMARY_TABLE(labelTMP, 7);
    SCENE_SUMMARY_VALUE(labelTMPValue, 7);
    SCENE_SUMMARY_UNIT(labelTMPUnit, 7);
    labelTMPValue.setNum(-100);
    labelTMPUnit.setText("mmHg");

    SCENE_SUMMARY_TABLE(labelNa, 8);
    SCENE_SUMMARY_VALUE(labelNaValue, 8);
    SCENE_SUMMARY_UNIT(labelNaUnit, 8);
    labelNa.setText("Na+ :");
    labelNaValue.setNum(140);
    labelNaUnit.setText("mmol/l");

    SCENE_SUMMARY_TABLE(labelCond, 9);
    SCENE_SUMMARY_VALUE(labelCondValue, 9);
    SCENE_SUMMARY_UNIT(labelCondUnit, 9);
    labelCondValue.setNum(0.0);
    labelCondUnit.setText("mS/cm");

    SCENE_SUMMARY_TABLE(labelTemp, 10);
    SCENE_SUMMARY_VALUE(labelTempValue, 10);
    SCENE_SUMMARY_UNIT(labelTempUnit, 10);
    labelTempValue.setNum(0.0);
    labelTempUnit.setText("'C");

    SCENE_SUMMARY_TABLE(labelHeparinFlux, 11);
    SCENE_SUMMARY_VALUE(labelHeparinFluxValue, 11);
    SCENE_SUMMARY_UNIT(labelHeparinFluxUnit, 11);
    labelHeparinFluxValue.setNum(10);
    labelHeparinFluxUnit.setText("ml/h");

    SCENE_SUMMARY_TABLE(labelHeparinVol, 12);
    SCENE_SUMMARY_VALUE(labelHeparinVolValue, 12);
    SCENE_SUMMARY_UNIT(labelHeparinVolUnit, 12);
    labelHeparinVolValue.setNum(0);
    labelHeparinVolUnit.setText("ml");

    SCENE_SUMMARY_TABLE(labelBPSpeed, 13);
    SCENE_SUMMARY_VALUE(labelBPSpeedValue, 13);
    SCENE_SUMMARY_UNIT(labelBPSpeedUnit, 13);
    labelBPSpeedValue.setNum(12);
    labelBPSpeedUnit.setText("ml/min");

    //SUB泵目标量#/
    SCENE_SUMMARY_TABLE(labelSubsTotal, 14);
    SCENE_SUMMARY_VALUE(labelSubsTotalValue, 14);
    SCENE_SUMMARY_UNIT(labelSubsTotalUnit, 14);
    labelSubsTotalUnit.setText("ml");

    //SUB泵累计量#/
    SCENE_SUMMARY_TABLE(labelSubsAcc, 15);
    SCENE_SUMMARY_VALUE(labelSubsAccValue, 15);
    SCENE_SUMMARY_UNIT(labelSubsAccUnit, 15);
    labelSubsAccUnit.setText("ml");

    SCENE_SUMMARY_TABLE(labelDialysisFlus, 16);
    SCENE_SUMMARY_VALUE(labelDialysisFlusValue, 16);
    SCENE_SUMMARY_UNIT(labelDialysisFlusUnit, 16);
    labelDialysisFlusValue.setNum(0);
    labelDialysisFlusUnit.setText("ml/min");

    SCENE_SUMMARY_TABLE(labelHCO3, 17);
    SCENE_SUMMARY_VALUE(labelHCO3Value, 17);
    SCENE_SUMMARY_UNIT(labelHCO3Unit, 17);
    labelHCO3.setText("HCO3- :");
    labelHCO3Value.setNum(0);
    labelHCO3Unit.setText("mmol/l");

    SCENE_SUMMARY_TABLE(labelBloodTEMP, 18);
    SCENE_SUMMARY_VALUE(labelBloodTEMPValue, 18);
    SCENE_SUMMARY_UNIT(labelBloodTEMPUnit, 18);
    labelBloodTEMPValue.setNum(0);
    labelBloodTEMPUnit.setText("'C");

    labelCondTitle.setGeometry(QRect(280+10, 70, 200, 35));
    LABEL_TITLE_STYLE(labelCondTitle);        
    labelCondText.setGeometry(QRect(280+180, 70, 350, 35));
    labelCondText.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelCondText.setFont(QFont(QString(CONFIG_FONT_STYLE),\
                CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelCondText.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    curveCond.setGeometry(QRect(280+15, 110, 450, 270));
    curveCond.setUnit("mmol/L");
    curveCond.setValueZoom(1);
    curveCond.setHorizontalScale(10);
    curveCond.setVerticalScale(8);
    curveCond.setMode(1);
    curveCond.setMinValue(125);
    curveCond.setMaxValue(151);
    curveCond.setTestTime(60*60*6);
    curveCond.setUnitX(Hour);

    labelUFTitle.setGeometry(QRect(280+10, 420, 200, 35));
    LABEL_TITLE_STYLE(labelUFTitle);
    labelUFText.setGeometry(QRect(280+180, 420, 350, 35));
    labelUFText.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    labelUFText.setFont(QFont(QString(CONFIG_FONT_STYLE),\
                CFG_FONT_SIZE_TEXT, CONFIG_FONT_NORMAL));
    labelUFText.setStyleSheet(CONFIG_COLOR_WHITE_CHARS);

    curveUF.setGeometry(QRect(280+15, 460, 450, 270));
    curveUF.setUnit("ml/h");
    curveUF.setValueZoom(1);
    curveUF.setHorizontalScale(10);
    curveUF.setVerticalScale(5);
    curveUF.setMode(1);
    curveUF.setMinValue(0);
    curveUF.setMaxValue(2000);
    curveUF.setTestTime(60*60*6);
    curveUF.setUnitX(Hour);

    CustomSetText();
}

SceneSummary::SceneSummary(QWidget *parent) :
    QObject(parent),
    labelCurveType(parent),
    labelCurveValue(parent), //治疗类型#/
    labelDilutionType(parent),
    labelDilutionValue(parent),  //稀释类型#/
    labelTimeLeft(parent),
    labelTimeLeftValue(parent),
    labelUfRate(parent),
    labelUfRateValue(parent),
    labelUfRateUnit(parent),
    labelUfGoal(parent),
    labelUfGoalValue(parent),
    labelUfGoalUnit(parent),
    labelUfVolume(parent),
    labelUfVolumeValue(parent),
    labelUfVolumeUnit(parent),
    labelAP(parent),
    labelAPValue(parent),
    labelAPUnit(parent),
    labelVP(parent),
    labelVPValue(parent),
    labelVPUnit(parent),
    labelTMP(parent),
    labelTMPValue(parent),
    labelTMPUnit(parent),
    labelNa(parent),
    labelNaValue(parent),
    labelNaUnit(parent),
    labelCond(parent),
    labelCondValue(parent),
    labelCondUnit(parent),
    labelTemp(parent),
    labelTempValue(parent),
    labelTempUnit(parent),
    labelHeparinFlux(parent),
    labelHeparinFluxValue(parent),
    labelHeparinFluxUnit(parent),
    labelHeparinVol(parent),
    labelHeparinVolValue(parent),
    labelHeparinVolUnit(parent),
    labelBPSpeed(parent),
    labelBPSpeedValue(parent),
    labelBPSpeedUnit(parent),
    labelSubsTotal(parent),
    labelSubsTotalValue(parent),  //SUB泵目标量#/
    labelSubsTotalUnit(parent),
    labelSubsAcc(parent),
    labelSubsAccValue(parent),  //SUB泵累计量#/
    labelSubsAccUnit(parent),
    labelDialysisFlus(parent),
    labelDialysisFlusValue(parent),
    labelDialysisFlusUnit(parent),
    labelHCO3(parent),
    labelHCO3Value(parent),
    labelHCO3Unit(parent),
    labelBloodTEMP(parent),
    labelBloodTEMPValue(parent),
    labelBloodTEMPUnit(parent),

    labelCondTitle(parent),
    labelCondText(parent),
    curveCond(parent),
    labelUFTitle(parent),
    labelUFText(parent),
    curveUF(parent),
    labelTitle(parent),
    rebirthinfo(parent)
{
    setUi();
    PageAppendPiece("Summary", this);

    dataNodeAddObject(PACKET_NAME_UPDATE_AP, this);
    dataNodeAddObject(PACKET_NAME_UPDATE_VP, this);
    dataNodeAddObject(PACKET_NAME_UPDATE_TMP, this);
    dataNodeAddObject(PACKET_NAME_UF, this);
    dataNodeAddObject(PACKET_NAME_COND, this);
    dataNodeAddObject(PACKET_NAME_NOW_UF_VOLUME, this);
    dataNodeAddObject(PACKET_NAME_BLOODTEMP, this);
    dataNodeAddObject(PACKET_FORMULA_TARGET, this);
    dataNodeAddObject(PACKET_NAME_DIALYSATE, this);
    dataNodeAddObject(PACKET_SDO_BLOOD, this);
    dataNodeAddObject(PACKET_NAME_HEPARINDATA, this);
    dataNodeAddObject(PACKET_PDO_HEPARINPUMP, this);

}

void SceneSummary::CustomSetText()
{
    QString VerString;
//    VerString.sprintf("  [ %s REV %d Build %s ]", VER, REV, BUILD_TIME);
//    VerString = tr("Summary Info") + VerString;
    VerString = tr("Summary Info");
    labelTitle.setText(VerString);
    //labelTitle.setText(tr("Summary Info"));
    labelCurveType.setText(tr("Curve Type :"));
    labelDilutionType.setText(tr("Dilution Type :"));
    labelTimeLeft.setText(tr("Remaining Time :"));
    labelUfRate.setText(tr("UF Rate :"));
    labelUfGoal.setText(tr("UF Goal :"));
    labelUfVolume.setText(tr("Acc UF Volume :"));
    labelAP.setText(tr("A.P. :"));
    labelVP.setText(tr("V.P. :"));
    labelTMP.setText(tr("TMP :"));
    labelCond.setText(tr("Cond. :"));
    labelTemp.setText(tr("Temp :"));
    labelHeparinFlux.setText(tr("Heparin Flux:"));
    labelHeparinVol.setText(tr("Heparin Vol :"));
    labelBPSpeed.setText(tr("BP Flow :"));
    labelSubsTotal.setText(tr("""Sub. Goal :"));
    labelSubsAcc.setText(tr("Sub. Vol :"));
    labelDialysisFlus.setText(tr("Dialysate Flow :"));
    labelBloodTEMP.setText(tr("Blood Temp :"));
    labelCondTitle.setText(tr("Cond Profile:"));
    labelUFTitle.setText(tr("UF Profile:"));
}

void SceneSummary::setCusVisible(bool vis, char* para)
{
    para = para;

    if(vis==true)  //曲线同步处理#/
    {
        unsigned char i=0;
        int persec=0;
        WorkState  stateTMP=GetWorkStateManage();
        QString str;
        //NA曲线#/
        curveCond.setTestTime(total_secCount);
        curveCond.delAllTargetCurve();
        curveCond.delAllTestPoint();
        persec=total_secCount/10;
        if(G_NA_PROFILE_FLAG==true)  //setCusVisible(bool vis, char* para)#/
        {                                               
            for(i=0;i<10;i++)
            {
                if(A_NA_PROFILE[i].flag==true)
                {
         //           curveCond.addTargetCurve(0+i*persec,(i+1)*persec ,(int)A_NA_PROFILE[i].navalue);
                      curveCond.addTargetCurve(0+i*persec, persec ,(int)A_NA_PROFILE[i].navalue);
                }
                else
                    break;
            }

            if(stateTMP.tipState.current==TREAT_RUNNING)
            {
                str=DE_TR_PROFILERUNING;
                str += QString::number(G_NA_SELECTCURVE_NUM);
            }
            else
            {
                str=DE_TR_HASSELECTPROFILE;
                str += QString::number(G_NA_SELECTCURVE_NUM);
            }
            labelCondText.setText(str);
        }
        else
        {
            FormulaSet formula;
            dataRead(PACKET_FORMULA_TARGET,&formula);
            curveCond.addTargetCurve(0,total_secCount,(int)formula.tempBuffer.na);
            labelCondText.setText(DE_TR_NOTSELECTPROFILECURE);
        }

        //UF曲线#/
        curveUF.delAllTargetCurve();
        curveUF.delAllTestPoint();

        curveUF.setTestTime(total_secCount);
        curveUF.setMinValue((int)usr_params.Params_UF.UF_Rate.attribute.min_value);
        curveUF.setMaxValue((int)usr_params.Params_UF.UF_Rate.attribute.max_value);
        if(G_UF_PROFILE_FLAG==true)
        {

            persec=(G_UF_PROFILE_END-G_UF_PROFILE_START)/10;

            for(i=0;i<10;i++)
            {
                if(A_UF_PROFILE[i].flag==true)
                {
 //                   curveUF.addTargetCurve(G_UF_PROFILE_START-G_CURE_STARTTIME+i*persec,(i+1)*persec ,(int)(A_UF_PROFILE[i].ufvol/persec));
                     curveUF.addTargetCurve((G_UF_PROFILE_START-G_CURE_STARTTIME)+i*persec,persec ,(int)((A_UF_PROFILE[i].ufvol/persec)*3600));
                }
                else
                    break;
            }

            if(stateTMP.tipState.current==TREAT_RUNNING)
            {
                str=DE_TR_PROFILERUNING;
                str += QString::number(G_UF_SELECTCURVE_NUM);
            }
            else
            {
                str=DE_TR_HASSELECTPROFILE;
                str += QString::number(G_UF_SELECTCURVE_NUM);
            }
            labelUFText.setText(str);
        }
        else
        {
            PacketUF packetTmp;
            dataRead(PACKET_NAME_UF,&packetTmp);
            curveUF.addTargetCurve(0,total_secCount,(int)(packetTmp.ufRate));
            labelUFText.setText(DE_TR_NOTSELECTPROFILECURE);
        }
    }

    if(vis==true)   //显示死机黑屏信息#/
    {
        QString strtmp;
        strtmp.clear();
        strtmp.append("Die:");
        strtmp.append(QString::number(dietimes));
        strtmp.append(" Bir:");
        strtmp.append(QString::number(rebirthtimes));
        strtmp.append(" Sql:");
        strtmp.append(QString::number(ListSqlDat.size())); //数据库缓冲队列#/

        rebirthinfo.setText(strtmp);

        QTime timeLeave;
        labelTimeLeftValue.setText((timeLeave.addSecs(residue_secCount)).toString(QString("hh:mm")));

        PacketWorkMode workModeTMP;
        dataRead(PACKET_NAME_WORKMODE,&workModeTMP);
        if(workModeTMP.treatmode.current==HDF_pre_dilute||workModeTMP.treatmode.current==HF_pre_dilute)
        {
            labelDilutionValue.setText(tr("Pre Dilute"));
        }
        else if(workModeTMP.treatmode.current==HDF_post_dilute||workModeTMP.treatmode.current==HF_post_dilute)
        {
            labelDilutionValue.setText(tr("Post Dilute"));
        }
        else
        {
            labelDilutionValue.setText(DE_TR_TRNONE);
        }

        switch(workModeTMP.treatmode.current)
        {
        case HD_double:
            labelCurveValue.setText(DE_TR_HDDOUBLENEEDLE);
            break;
        case HD_SNDP:
            labelCurveValue.setText(DE_TR_HDSNDP);
            break;
        case HD_SNSP:
            labelCurveValue.setText(DE_TR_HDSNSP);
            break;
        case HDF_pre_dilute://前置换 透析滤过
            labelCurveValue.setText(DE_TR_HDFONLINE);
            break;
        case HDF_post_dilute://后置换 透析滤过
            labelCurveValue.setText(DE_TR_HDFONLINE);
            break;
        case HF_pre_dilute://
            labelCurveValue.setText(DE_TR_HFONLINE);
            break;
        case HF_post_dilute://
            labelCurveValue.setText(DE_TR_HFONLINE);
            break;
        case ISOUF_DNDP://
            labelCurveValue.setText(DE_TR_ISODOUBLENEEDLE);
            break;
        case ISOUF_SNDP://
            labelCurveValue.setText(DE_TR_ISOSNDP);
            break;
        case ISOUF_SNSP://
            labelCurveValue.setText(DE_TR_ISOSNSP);
            break;
        case PE ://
            labelCurveValue.setText(DE_TR_PEPH);
            break;
        }

        labelSubsTotalValue.setText(QString::number(SubsPump_totalvolume));

    }
    labelCurveType.setVisible(vis);
    labelCurveValue.setVisible(vis); //治疗类型#/
    labelDilutionType.setVisible(vis);
    labelDilutionValue.setVisible(vis);  //稀释类型#/
    labelTitle.setVisible(vis);
    labelTimeLeft.setVisible(vis);
    labelTimeLeftValue.setVisible(vis);

    labelUfRate.setVisible(vis);
    labelUfRateValue.setVisible(vis);
    labelUfRateUnit.setVisible(vis);
    labelUfGoal.setVisible(vis);
    labelUfGoalValue.setVisible(vis);
    labelUfGoalUnit.setVisible(vis);
    labelUfVolume.setVisible(false);
    labelUfVolumeValue.setVisible(false);
    labelUfVolumeUnit.setVisible(false);
    labelAP.setVisible(vis);
    labelAPValue.setVisible(vis);
    labelAPUnit.setVisible(vis);
    labelVP.setVisible(vis);
    labelVPValue.setVisible(vis);
    labelVPUnit.setVisible(vis);
    labelTMP.setVisible(vis);
    labelTMPValue.setVisible(vis);
    labelTMPUnit.setVisible(vis);
    labelNa.setVisible(vis);
    labelNaValue.setVisible(vis);
    labelNaUnit.setVisible(vis);
    labelCond.setVisible(vis);
    labelCondValue.setVisible(vis);
    labelCondUnit.setVisible(vis);
    labelTemp.setVisible(vis);
    labelTempValue.setVisible(vis);
    labelTempUnit.setVisible(vis);
    labelHeparinFlux.setVisible(vis);
    labelHeparinFluxValue.setVisible(vis);
    labelHeparinFluxUnit.setVisible(vis);
    labelHeparinVol.setVisible(vis);
    labelHeparinVolValue.setVisible(vis);
    labelHeparinVolUnit.setVisible(vis);
    labelBPSpeed.setVisible(vis);
    labelBPSpeedValue.setVisible(vis);
    labelBPSpeedUnit.setVisible(vis);
    labelSubsTotal.setVisible(vis);
    labelSubsTotalValue.setVisible(vis);  //SUB泵目标量#/
    labelSubsTotalUnit.setVisible(vis);
    labelSubsAcc.setVisible(vis);
    labelSubsAccValue.setVisible(vis);  //SUB泵累计量#/
    labelSubsAccUnit.setVisible(vis);
    labelDialysisFlus.setVisible(vis);
    labelDialysisFlusValue.setVisible(vis);
    labelDialysisFlusUnit.setVisible(vis);
    labelHCO3.setVisible(vis);
    labelHCO3Value.setVisible(vis);
    labelHCO3Unit.setVisible(vis);
    labelBloodTEMP.setVisible(vis);
    labelBloodTEMPValue.setVisible(vis);
    labelBloodTEMPUnit.setVisible(vis);
    labelCondTitle.setVisible(vis);
    labelCondText.setVisible(vis);
    curveCond.setVisible(vis);
    labelUFTitle.setVisible(vis);
    labelUFText.setVisible(vis);
    curveUF.setVisible(vis);
    rebirthinfo.setVisible(vis);
}

void SceneSummary::timeRemain(QString tm)
{

   labelTimeLeftValue.setText(tm);

}

void SceneSummary::ActionHandle(const void* dataPtr,int name)
{

    if ((dataPtr == NULL) || (name == 0)){
        return;
    }

    if(labelTimeLeftValue.isVisible())
    {
        QTime timeLeave;
        labelTimeLeftValue.setText((timeLeave.addSecs(residue_secCount)).toString(QString("hh:mm")));

        labelSubsAccValue.setText(QString::number(SubsPump_finishvolume));
    }

    if (name==PACKET_NAME_UF) {
        struct PacketUF *pData = (struct PacketUF*)dataPtr;
        if(packetUF.ufRate !=pData->ufRate)
        {
          packetUF.ufRate =pData->ufRate;
          labelUfRateValue.setNum(packetUF.ufRate);
        }
        if(packetUF.ufVolAim !=pData->ufVolAim)
        {
          packetUF.ufVolAim =pData->ufVolAim;
          labelUfGoalValue.setNum(packetUF.ufVolAim);
        }
        if(packetUF.ufVolNow !=pData->ufVolNow)
        {
          packetUF.ufVolNow =pData->ufVolNow;
          labelUfGoalValue.setNum(packetUF.ufVolNow);
        }

    }

    if (name==PACKET_FORMULA_TARGET) {
        struct FormulaSet *pData = (struct FormulaSet*)dataPtr;
        if(formulaSet.tempBuffer.na !=pData->tempBuffer.na)
        {
          formulaSet.tempBuffer.na =pData->tempBuffer.na;
          labelNaValue.setNum(formulaSet.tempBuffer.na);
        }

        if(formulaSet.tempBuffer.hco3 !=pData->tempBuffer.hco3)
        {
          formulaSet.tempBuffer.hco3 =pData->tempBuffer.hco3;
          labelHCO3Value.setNum(formulaSet.tempBuffer.hco3);
        }

        if(formulaSet.outTarget.target_mix !=pData->outTarget.target_mix)
        {
          formulaSet.outTarget.target_mix =pData->outTarget.target_mix;
          QString str;
          str.setNum((float)formulaSet.outTarget.target_mix/100, 'f', 1);
          labelCondValue.setText(str);
        }

    }

    if (name==PACKET_NAME_UPDATE_AP){
        int AP_value = *(int *)dataPtr;
        if(packetAP.ap !=AP_value)
        {
          packetAP.ap =AP_value;
          labelAPValue.setNum(AP_value);
        }
    }

    if (name==PACKET_NAME_UPDATE_VP) {
        int VP_value = *(int *)dataPtr;
        if(packetVP.vp !=VP_value)
        {
          packetVP.vp =VP_value;
          labelVPValue.setNum(packetVP.vp);
        }
    }

    if (name==PACKET_NAME_UPDATE_TMP) {
        int TMP_value = *(int *)dataPtr;
        if(packetTMP.tmp !=TMP_value)
        {
          packetTMP.tmp =TMP_value;
          labelTMPValue.setNum(packetTMP.tmp);
        }
    }

    if (name==PACKET_NAME_DIALYSATE) {
        struct PacketFluid *pData = (struct PacketFluid*)dataPtr;
        if(packetFluid.Wspeed !=pData->Wspeed)
        {
          packetFluid.Wspeed =pData->Wspeed;
          labelDialysisFlusValue.setNum(packetFluid.Wspeed);
        }

        if(packetFluid.tempTarget!=pData->tempTarget)
        {
            packetFluid.tempTarget=pData->tempTarget;

            labelTempValue.setText(QString::number(packetFluid.tempTarget/10.0,'f',1));
        }
    }

    if (name==PACKET_SDO_BLOOD) {
        struct PacketSdoBlood *pData = (struct PacketSdoBlood*)dataPtr;
        if(packetSdoBlood.N3_set_BPspeed !=pData->N3_set_BPspeed)
        {
          packetSdoBlood.N3_set_BPspeed =pData->N3_set_BPspeed;
          labelBPSpeedValue.setNum(packetSdoBlood.N3_set_BPspeed);
        }
    }

    if (name==PACKET_PDO_HEPARINPUMP) {
        struct PacketPDOHeparinPump *pData = (struct PacketPDOHeparinPump*)dataPtr;
        if(packetPDOHeparinPump.speed !=pData->speed)
        {
          packetPDOHeparinPump.speed =pData->speed;
          labelHeparinFluxValue.setNum((float)packetPDOHeparinPump.speed/10);
        }
    }

    if (name==PACKET_NAME_HEPARINDATA) {
        PackN15PDO *heparnPtr = (PackN15PDO*)dataPtr;
        if(packetHeparinPump.inputAllValue !=heparnPtr->inputAllValue)//累计注入量#/
        {
          packetHeparinPump.inputAllValue =heparnPtr->inputAllValue;
          labelHeparinVolValue.setNum((float)packetHeparinPump.inputAllValue/10);
        }               
    }
}

