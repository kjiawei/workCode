#include "data_set.h"
#include <math.h>
#include <QString>
#include <QDebug>
#include "packet.h"

#include "scene_message.h"

//#include "scene_note.h"

#include "data_custom.h"

#include "global_var.h"



void updateFaisonFormulaCond(struct FormulaSet *formula)
{
    //(nacl, kcl, cacl, mgcl, ac, anaac, bnacl, bnaac, nahco, na, hco)
    //int id=formula->use_formula;
    float hco = formula->tempBuffer.hco3;//
    float anaac = formula->tempBuffer.naac;//
    float na = formula->tempBuffer.na;//

    float nahco = 0;
    float bnaac = 0;
    float nacl = 0;
    float bi_a = 0;

    float ac = (float)(1 + formula->tempBuffer.scale_b + formula->tempBuffer.scale_water ) * formula->tempBuffer.ace;  //0.03102
    float bnacl = (float)formula->tempBuffer.bnacl/1*1000/58.443;//402.6;

    nahco=((formula->tempBuffer.stand_hco3+formula->tempBuffer.ace)) * (1+(formula->tempBuffer.stand_b_workmode)+(formula->tempBuffer.stand_water_workmode) ) / (formula->tempBuffer.stand_b_workmode);
    anaac=(float)(formula->tempBuffer.naac)/1*1000/82.034;
    ac=ac-anaac;
    nahco=nahco-anaac/(formula->tempBuffer.stand_b_workmode);

    bnaac = 0;
    nacl=(1+(formula->tempBuffer.stand_b_workmode)+(formula->tempBuffer.stand_water_workmode)) *
         ((formula->tempBuffer.stand_na-formula->tempBuffer.stand_hco3-formula->tempBuffer.ace) -
         (bnacl/(1+(formula->tempBuffer.stand_b_workmode)+(formula->tempBuffer.stand_water_workmode))*(formula->tempBuffer.stand_b_workmode)));

    bi_a = 1;


    float kcl=(1 + formula->tempBuffer.scale_b + formula->tempBuffer.scale_water) * formula->tempBuffer.k;//73.66;
    float cacl=(1 + formula->tempBuffer.scale_b + formula->tempBuffer.scale_water) * formula->tempBuffer.ca;//128.9;
    float mgcl=(1 + formula->tempBuffer.scale_b + formula->tempBuffer.scale_water) * formula->tempBuffer.mg;//36.83;

    float bi_b = (hco * (nacl + ac + anaac) / (na - hco) + ac) / (nahco - hco * (bnaac + bnacl) / (na - hco));
    float bi_all = (nacl + ac + anaac) / (na - hco) + (bnaac + bnacl) * bi_b / (na - hco);
    float c_nahco3 = (nahco * bi_b) / (bi_all - bi_a) / 1000;

    float k_nacl_bcond=0;
    float c_nacl_bcond=0;
    if(bnacl>0)
    {
      c_nacl_bcond=( bnacl * bi_b) / (bi_all- bi_a) / 1000.0 ;
      k_nacl_bcond=(12.645 - 5.472 * sqrt(c_nahco3 + c_nacl_bcond)) * c_nacl_bcond * 10;
     }

    float b_nahco3 = (double)(9.461 - 5.456 * sqrt(c_nahco3+c_nacl_bcond)) * c_nahco3 * 10;

    float c_nacl = (nacl + bnacl * bi_b) / (bi_all) / 1000 ;//
    float c_kcl = kcl / (bi_all) / 1000;
    float c_mgcl = mgcl * 2 / (bi_all) / 1000;
    float c_cacl = cacl * 2 / (bi_all) / 1000;
    float c_naac2 = (anaac + bnaac * bi_b) / (bi_all) / 1000;
    float c_naac = (ac) / (bi_all) / 1000;
    float c_nahco = nahco * bi_b / (bi_all) / 1000;//


    float k_nacl = (12.645 - 5.472 * sqrt(c_nacl + c_kcl + c_mgcl + c_cacl + c_naac + c_nahco + c_naac2)) * c_nacl * 10;
    float k_kcl = (14.986 - 6.14 * sqrt(c_nacl + c_kcl + c_mgcl + c_cacl + c_naac + c_nahco + c_naac2)) * c_kcl * 10;
    float k_cacl = (13.584 - 9.71 * sqrt(c_nacl + c_kcl + c_mgcl + c_cacl + c_naac + c_nahco + c_naac2)) * c_cacl * 10;
    float k_mgcl = (12.94 - 7.5 * sqrt(c_nacl + c_kcl + c_mgcl + c_cacl + c_naac + c_nahco + c_naac2)) * c_mgcl * 10;
    float k_nahco = (9.461 - 5.456 * sqrt(c_nacl + c_kcl + c_mgcl + c_cacl + c_naac + c_nahco + c_naac2)) * c_nahco * 10;
    float k_naac = (9.101 - 21.316 * sqrt(c_nacl + c_kcl + c_mgcl + c_cacl + c_naac + c_nahco + c_naac2)) * (c_naac) * 10;
    float k_naac2 = (9.101 + 0.2 * sqrt(c_nacl + c_kcl + c_mgcl + c_cacl + c_naac + c_nahco + c_naac2)) * c_naac2 * 10;

    float m_nahco3 = k_nacl + k_kcl + k_cacl + k_mgcl + k_nahco + k_naac + k_naac2;

    QString number ;

    //formula->tempBuffer.scale_b = number.setNum(bi_b, 'f', 3).toFloat();
    //formula->tempBuffer.scale_water = number.setNum(bi_all - bi_b - 1, 'f', 3).toFloat();
    formula->outTarget.scale_b = number.setNum(bi_b, 'f', 2).toFloat() * 100;
    formula->outTarget.scale_water = number.setNum(bi_all - bi_b - 1, 'f', 2).toFloat() * 100;
    //
    formula->outTarget.target_b = number.setNum(b_nahco3+k_nacl_bcond, 'f', 2).toFloat() * 100;
    formula->outTarget.target_mix = number.setNum(m_nahco3, 'f', 2).toFloat() * 100;

    number.clear();
//    qDebug()<< "scale_b" << formula->outTarget.scale_b
//            << "scale_water" << formula->outTarget.scale_water
//            << "target_b"  << formula->outTarget.target_b
//            << "target_mix" << formula->outTarget.target_mix;

    dataNodeUpdate(PACKET_FORMULA_TARGET, formula);
}


//时|分 转为小时
float hMinToHour(float hMin)
{
    int time = hMin * 100;
    int hour = time / 100;
    int min = time % 100;

    return (float)(hour + (float)min/60);
}

//y = ax^3 + bx^2 + cx + d
float getThreeCurveParam(float treatTime, float water_value, float initValue,
    float maxValue, float maxTime, int min)
{
    float a = 0, b = 0, c = 0, d = 0, y = 0 , total_avalue =0, total_bvalue = 0
           , total_cvalue = 0, total_dvalue = 0;

    int i = 0;

    float xMaxTime = hMinToHour(maxTime);
    float hminute = (float)min / 60;

    d = initValue;
    int temp = treatTime * 100;
    int count = (temp / 100 * 60 + temp %100) /15;
    //处理总脱水量
    for(i = 0; i < count; i ++) {
        total_avalue += 1.0 * i * 1/4 * i * 1/4 * i * 1/4;
        total_bvalue += 1.0 * i * 1/4 * i * 1/4;
        total_cvalue += 1.0 * i * 1/4;
        total_dvalue += 1;
    }

    float maxDInit = maxValue - initValue;
    float tempY = water_value * 4 - d * total_dvalue;


    a = (tempY + maxDInit / (xMaxTime * xMaxTime) * total_bvalue
            - maxDInit *2 /xMaxTime * total_cvalue) /
            (total_avalue - 2 * xMaxTime * total_bvalue + xMaxTime * xMaxTime * total_cvalue);

    b = -2 * a * xMaxTime - maxDInit/(xMaxTime * xMaxTime);

    c = a * xMaxTime * xMaxTime + maxDInit * 2 /xMaxTime;

    y = a * hminute * hminute * hminute + b * hminute * hminute  + c * hminute + d;

    return y;
}

//y= kx + b
float getLineCurveParam(float treatTime, float maxTreatValue, float initValue, int min) {
    float k = 0, b = 0, y = 0;
    float total_k = 0, total_b = 0;

    float hminute = (float)min / 60;

    int temp = treatTime * 100;
    int count = (temp / 100 * 60 + temp %100) /15;

    b = initValue;
    //处理总脱水量
    for(int i = 0; i < count; i ++) {
        total_k += i/4;
        total_b += 1;
    }
    k = 1.0 * (maxTreatValue * 4 - total_b * b)/total_k;

    y = 1.0 * k * hminute + b;

    return y;
}

//y = a(x - h)^2 + k
float getPaoWuLineCurveParam(float treatTime, float maxTreatValue, float maxValue, float maxValueTime, int min) {
    float a = 0, h = 0, k = 0, y = 0;
    float total_a = 0, total_k = 0;

    float hminute = (float)min / 60;
    float xMaxTime = hMinToHour(maxValueTime);

    int temp = treatTime * 100;
    int count = (temp / 100 * 60 + temp %100) /15;

    h = xMaxTime;
    k = maxValue;

    for(int i = 0; i < count; i ++) {
        total_a += 1.0 * (i/4 - h) * (i/4 - h);
        total_k += 1;
    }

    a = 1.0 * (maxTreatValue * 4 - k * total_k) / total_a;

    y = 1.0 * a * (hminute - h) * (hminute - h) + k;

    return y;
}

//y = a^x + b这个怎么定
float getZhiShuCurveParam(float treatTime, float maxTreatValue, float initValue, int min) {

    treatTime = treatTime;
    maxTreatValue = maxTreatValue;
    initValue = initValue;
    min = min;
    //    float a = 0, y = 0;

    //    float total_a = 1;

    //    float hminute = (float)min / 60;

    //    int temp = treatTime * 100;
    //    int count = (temp / 100 * 60 + temp %100) /15;


    ////    for(int i = 0; i < count; i ++) {
    ////        if(i != 0)
    ////            total_a *= 1.0 * i/4;
    ////        total_b += 1;
    ////    }

    //    //maxTreatValue * 4 = 1 + pow(a, total_a) + total_b * b;
    ////    float pow_x = 1.0* maxTreatValue * 4 - 1 - 1.0 * total_b * b ;
    ////    float pow_y = 1.0 / total_a;
    ////    a = pow(pow_x, pow_y);

    ////    qDebug() << "a=:" << a << "pow_x" << pow_x <<"pow_y" << pow_y
    ////               << "total_a" << total_a << "total_b" << total_b;
    //    y = pow(1.0 * 1/3, 1.0 * hminute);

    return 0;
}

//提示性消息框
Window_Message *messageDialog = NULL;
void showMessagesDialog() {
    if(messageDialog == NULL) {
//        messageDialog = new Window_Message(0, QString("Messages"), ":/resource/images/warn/message_bk.png");
        messageDialog = new Window_Message(window_parent, QString("Messages"), ":/resource/images/warn/message_bk.png");
    }
    if(messageDialog->isCanShow())
    {
        messageDialog ->showDialog(true);
    }
}

void closeMessagesDialog() {
    if(messageDialog != NULL)
        messageDialog -> showDialog(false);
}

void clearMessagesDialogInfo() {
    if(messageDialog != NULL)
        messageDialog->removeAllRow();
}

void removeOneMessages(int key)
{
    if(messageDialog != NULL)
        messageDialog->removeOneAlarm(key);
}

void updateMessageDialog(QMap<int, AlarmInfor> mapValue) {
    showMessagesDialog();
    messageDialog -> showItem(mapValue);
}

bool isMessageDialogShow() {
    if(messageDialog != NULL && messageDialog->isVisible())
        return true;
    return false;
}

bool isMessageInfoNull()
{
    if(messageDialog != NULL)
        return messageDialog->isInfoNull();
    else
        return true;
}

//报警提示框
Window_Message *alarmDialog = NULL;
void showAlarmDialog() {
    if(alarmDialog == NULL) {
//        alarmDialog = new Window_Message(0, QString("Alarm"), ":/resource/images/warn/alarm_bk.png");
        alarmDialog = new Window_Message(window_parent, QString("Alarm"), ":/resource/images/warn/alarm_bk.png");
    }
    if(alarmDialog->isCanShow())
    {
        alarmDialog->showDialog(true);
    }
}

void closeAlarmDialog() {
    if(alarmDialog != NULL)
        alarmDialog->showDialog(false);
}

void clearAlarmDialogInfo() {
    if(alarmDialog != NULL)
        alarmDialog->removeAllRow();
}

void removeOneAlarm(int key)
{
    if(alarmDialog != NULL)
        alarmDialog->removeOneAlarm(key);
}

bool isAlarmDialogShow() {
    if(alarmDialog != NULL && alarmDialog->isVisible())
        return true;
    return false;
}

bool isAlarmInfoNull()
{
    if(alarmDialog != NULL)
        return alarmDialog->isInfoNull();
    else
        return true;
}

void updateAlarmDialog(QMap<int, AlarmInfor> mapValue) {
    showAlarmDialog();
    alarmDialog -> showItem(mapValue);
}

//警告提示框
Window_Message *warnDialog = NULL;
void showWarnDialog() {
    if(warnDialog == NULL) {
//        warnDialog = new Window_Message(0, QString("Warn"), ":/resource/images/warn/warn_bk.png");
         warnDialog = new Window_Message(window_parent, QString("Warn"), ":/resource/images/warn/warn_bk.png");
    }
    if(warnDialog->isCanShow())
    {
        warnDialog->showDialog(true);
    }
}

void closeWarnDialog() {
    if(warnDialog != NULL)
        warnDialog->showDialog(false);
}

void clearWarnDialogInfo() {
    if(warnDialog != NULL)
        warnDialog->removeAllRow();
}

void removeOneWarn(int key)
{
    if(warnDialog != NULL)
        warnDialog->removeOneAlarm(key);
}

void updateWarnDialog(QMap<int, AlarmInfor> mapValue) {
    showWarnDialog();
    warnDialog -> showItem(mapValue);
}

bool isWarnInfoNull()
{
    if(warnDialog != NULL)
        return warnDialog->isInfoNull();
    else
        return true;
}

void closeAllDialog()
{
    closeMessagesDialog();
    closeAlarmDialog();
    closeWarnDialog();
}

void showAllDialog()
{
    if(!isMessageInfoNull())
        showMessagesDialog();
    if(!isWarnInfoNull())
        showWarnDialog();
    if(!isAlarmInfoNull())
        showAlarmDialog();
}

//
//Window_Note *NoteDialog = NULL;
void showNoteDialog(const QString title)
{
    if(title.compare("Cure Mode") == 0 )
     {
        if(NoteCureMode == NULL) {
//            NoteCureMode = new Window_Note(0, QObject::tr("Cure Mode"), ":/resource/test.png");
            NoteCureMode = new Window_Note(window_parent, QObject::tr("Cure Mode"), ":/resource/test.png");
        }
        NoteCureMode->CustomSetText();
        NoteCureMode->showNote(true);
    }

    if(title.compare("Return to ready") == 0 )
     {
        if(NoteReturnReady == NULL) {
//            NoteReturnReady = new Window_Note(0, QObject::tr("Return to ready"), ":/resource/test.png");
            NoteReturnReady = new Window_Note(window_parent, QObject::tr("Return to ready"), ":/resource/test.png");
        }
        NoteReturnReady->CustomSetText();
        NoteReturnReady->showNote(true);
    }

    if(title.compare("WorkModel") == 0 )
     {
        if(NoteDialog == NULL) {
//            NoteDialog = new Window_Note(0, QObject::tr("WorkModel"), ":/resource/test.png");
            NoteDialog = new Window_Note(window_parent, QObject::tr("WorkModel"), ":/resource/test.png");
        }
        NoteDialog->showNote(true);
    }

   if(title.compare("CureFinish") == 0 )
   {
        if(CureFinishDlg == NULL) {
//            CureFinishDlg = new Window_Note(0, QObject::tr("CureFinish"), ":/resource/test.png");
            CureFinishDlg = new Window_Note(window_parent, QObject::tr("CureFinish"), ":/resource/test.png");
        }
        CureFinishDlg->showNote(true);
   }

   if(title.compare("CureLeave") == 0 )
   {
        if(CureLeaveDlg == NULL) {
//            CureLeaveDlg = new Window_Note(0, QObject::tr("CureLeave"), ":/resource/test.png");
            CureLeaveDlg = new Window_Note(window_parent, QObject::tr("CureLeave"), ":/resource/test.png");
        }
        CureLeaveDlg->showNote(true);
   }

   if(title.compare("Confirm AP") == 0 )
    {
       if(ConfirmDlgAP == NULL) {
//           ConfirmDlgAP = new Window_Confirm(0, QObject::tr("Confirm AP"), ":/resource/test.png");
           ConfirmDlgAP = new Window_Confirm(window_parent, QObject::tr("Confirm AP"), ":/resource/test.png");
       }
       ConfirmDlgAP->showNote(true);
   }
   if(title.compare("Confirm VP") == 0 )
    {
       if(ConfirmDlgVP == NULL) {
//           ConfirmDlgVP = new Window_Confirm(0, QObject::tr("Confirm VP"), ":/resource/test.png");
           ConfirmDlgVP = new Window_Confirm(window_parent, QObject::tr("Confirm VP"), ":/resource/test.png");
       }
       ConfirmDlgVP->showNote(true);
   }
   if(title.compare("Confirm TMP") == 0 )
    {
       if(ConfirmDlgTMP == NULL) {
//           ConfirmDlgTMP = new Window_Confirm(0, QObject::tr("Confirm TMP"), ":/resource/test.png");
            ConfirmDlgTMP = new Window_Confirm(window_parent, QObject::tr("Confirm TMP"), ":/resource/test.png");
       }
       ConfirmDlgTMP->showNote(true);
   }
}

void closeNoteDialog() {
    if(NoteDialog != NULL)
        NoteDialog->showNote(false);
}

bool IsNoteDialogShow() {
    if(NoteDialog == NULL) {
        return false;
    }
    return NoteDialog->isShow()|| CureFinishDlg->isShow()|| IsYesNoDialogShow();
}
//

void showYesNoDialog(QString title,QString info)
{
    if(YesNoDialog ==  NULL)
    {
        YesNoDialog = new Window_YESNO(0);
//        YesNoDialog = new Window_YESNO(window_parent);
        YesNoDialog->showMessage(title,info);
    }
    else
    {
       YesNoDialog->showMessage(title,info);
    }
}

void closeYesNoDialog()
{
    if(YesNoDialog != NULL)
        YesNoDialog->showNote(false);
}

bool IsYesNoDialogShow()
{
    if(YesNoDialog == NULL)
        return false;
    else
        return YesNoDialog->isShow();
}


void showYesDialog(QString title,QString info)
{
    if(YesDialog ==  NULL)
    {
        YesDialog = new Window_YES(0);
//        YesDialog = new Window_YES(window_parent);
        YesDialog->showMessage(title,info);
    }
    else
    {
        YesDialog->showMessage(title,info);
    }
}

void showYesDialogNoYes(QString title,QString info)
{
    if(YesDialog ==  NULL)
    {
        YesDialog = new Window_YES(0);
//        YesDialog = new Window_YES(window_parent);
        YesDialog->showMessageNoYes(title,info);
    }
    else
    {
        YesDialog->showMessageNoYes(title,info);
    }
}

void closeYesDialog()
{
    if(YesDialog != NULL)
        YesDialog->showNote(false);
}

bool IsYesDialogShow()
{
    if(YesDialog == NULL)
        return false;
    else
        return YesDialog->isShow();
}

void setAlarmDialogOnTop() {

    if(warnDialog == NULL) {
//        warnDialog = new Window_Message(0, QObject::tr("Warn"), ":/resource/images/warn/warn_bk.png");
        warnDialog = new Window_Message(window_parent, QObject::tr("Warn"), ":/resource/images/warn/warn_bk.png");
        warnDialog -> setOnTop();
        warnDialog -> raise();
    } else {
        warnDialog -> setOnTop();
        warnDialog->showDialog(true);
        warnDialog -> raise();
    }

    if(alarmDialog != NULL) {
        alarmDialog ->setOnTop();
        alarmDialog->showDialog(true);
        alarmDialog -> raise();
    } else {
//        alarmDialog = new Window_Message(0, QObject::tr("Alarm"), ":/resource/images/warn/alarm_bk.png");
        alarmDialog = new Window_Message(window_parent, QObject::tr("Alarm"), ":/resource/images/warn/alarm_bk.png");
        alarmDialog ->setOnTop();
        alarmDialog -> raise();
    }

    if(messageDialog != NULL) {
        messageDialog -> setOnTop();
        messageDialog -> showDialog(true);
        messageDialog -> raise();
    } else {
//        messageDialog = new Window_Message(0, QObject::tr("Messages"), ":/resource/images/warn/message_bk.png");
        messageDialog = new Window_Message(window_parent, QObject::tr("Messages"), ":/resource/images/warn/message_bk.png");
        messageDialog -> setOnTop();
        messageDialog -> raise();
    }

//    if(NoteDialog != NULL) {

//        NoteDialog -> showNote(true);
//        NoteDialog -> raise();
//    } else {
//        NoteDialog = new Window_Note(0, QObject::tr("Note"), ":/resource/test.png");
//        NoteDialog -> raise();
//    }
}

//超净滤器对话框
void showFilterDialog(QString title,QString info)
{
    if(ChangeFilterDlg ==  NULL)
    {
        ChangeFilterDlg = new Window_ChangeFilter(0);//window_parent
        ChangeFilterDlg->showMessage(title,info);
    }
    else
    {
        ChangeFilterDlg->showMessage(title,info);
    }
}





