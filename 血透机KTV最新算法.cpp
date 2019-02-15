/*头文件相关配置说明
/*函数作用:获取血泵值来计算KTV并进行显示(带宏参数 默认值) 
/*[大致实现:根据分段贡献率对不同血流量进行累积计算  KTV是一个(血流量)条件方程组]
/*作者:jkCodic
/*2018.1.26 V2.05.17(与软件主版本一致)
/*联系方式:523090538@qq.com
/*
/*算法:void timerUpdateValue()//定时器更新值
/*
/*
*/

//①功能函数相关宏及前置声明(系统变量-结构体)
#define FIVE_SECOND 5  //更新周期
#define DefaultUreaExcValue_A 180.00
#define DefaultUreaExcValue_B 230.00
#define defaultHeight 168.00
#define defaultWeight 76.00
#define defaultAge 48

struct stpatientinfor infor;
WorkState  stateTMP = GetWorkStateManage();

//获取系统属性值
    PacketSdoBlood sdoBlood;
    dataRead(PACKET_SDO_BLOOD,&sdoBlood);
    disBloodFlowRate->setNum( sdoBlood.N3_set_BPspeed );

    PacketUF UF;
    dataRead(PACKET_NAME_UF, &UF);
    disUF->setNum(UF.ufVolAim);
    disTime->setNum(total_secCount/60);

    PacketFluid Fluid;
    dataRead(PACKET_NAME_DIALYSATE, &Fluid);
    disDialysateFlowRate->setNum(Fluid.Wspeed);

//一些使用的控件示例(主题相关顺序)
    QWidget *Ktv;
    QLabel labelTitle;
    QTabWidget KtVTabWidget;
    QMessageBox reConfirmMsgBox;
    QLineEdit *lineEditKtvHeight;//ureaExc->text().isEmpty() != true
    QButtonGroup *radioButtonSex;
    QPushButton *btnPatientApply;
    QRadioButton *radioButtonSelectMale;
    //QString labelString[] = {labelCaliText,labelAttentionText};//KTV标签数组
    float actualKTV,targetKTV;
    float actulBloodSpeed_a,actulBloodSpeed_b;
    float Ctbv_a,Ctbv_b,Ctbv_c,Ctbv_d,Ctbv_e;
    float TBV_a,TBV_b,TBV_c,TBV_d,TBV_e;
    float ctbv_all;//last minute TBV_all
    float TBV_all;//当前流速下(a+b+c+d+e)段的比例总和值
    float temp_all;//加权平均
    float current_v;
    int updateTime,dA,lastDa,realCureTime;//用来记录更改次数
    float dH,lastDh,dW,lastDw;
    bool isConfirmClick;
    float adpProDefVV;

//②常用函数(qt编程)
updateAddQuickTimer = new QTimer();
updateAddQuickTimer->setInterval(1000*FIVE_SECOND);//超时时间 5秒刷新一次已补液量理论值
connect(updateAddQuickTimer, SIGNAL(timeout()),this, SLOT(timerUpdateAddQuick()));
updateAddQuickTimer->start();//stop()停定时器

//调用参数说明:传入系统值 自定义值  默认值
//要求:不超过20行 主要逻辑-算法 主循环判断
void SceneKTV2::timerUpdateActualKTV(PacketSdoBlood sdoBlood,TBV_a,Ctbv_a,)  
{
    if( sdoBlood.N3_set_BPspeed <= 200 ){//将各段血量保存
        TBV_a = sdoBlood.N3_set_BPspeed;
    }else if( sdoBlood.N3_set_BPspeed <= 300 ){
        TBV_a = 200.00;
        TBV_b = sdoBlood.N3_set_BPspeed - 200;
    }else if( sdoBlood.N3_set_BPspeed <= 400 ){
        TBV_a = 200.00;
        TBV_b = 100.00;
        TBV_c = sdoBlood.N3_set_BPspeed - 300;
    }else if( sdoBlood.N3_set_BPspeed <= 500 ){
        TBV_a = 200.00;
        TBV_b = 100.00;
        TBV_c = 100.00;
        TBV_d = sdoBlood.N3_set_BPspeed - 400;
    }else{
        TBV_a = 200.00;
        TBV_b = 100.00;
        TBV_c = 100.00;
        TBV_d = 100.00;
        TBV_e = sdoBlood.N3_set_BPspeed - 500;
    }

    //清除率计算
    Ctbv_a = DefaultUreaExcValue_A / 100;
    if(ureaExc->text().isEmpty() != true)
        Ctbv_a = ureaExc->text().toFloat() / 100;

    Ctbv_b = DefaultUreaExcValue_B / 100;
    if(ureaExc2->text().isEmpty() != true)
        Ctbv_b = ureaExc2->text().toFloat() / 100;

    //调整比例
    adpProDefVV = 1.0;
    if(adpProDefV->text().isEmpty() != true)
        adpProDefVV = adpProDefV->text().toFloat();

    Ctbv_c = 0.15 * Ctbv_b;
    Ctbv_d = 0.08 * Ctbv_b;
    Ctbv_e = 0.04 * Ctbv_b;
    Ctbv_b = Ctbv_b - Ctbv_a;

    //计算KTV值
    TBV_all = ( TBV_a * Ctbv_a / 200 + TBV_b * Ctbv_b / 100 + TBV_c * Ctbv_c / 100 +  TBV_d * Ctbv_d / 100 + TBV_e * Ctbv_e / 100);
    TBV_a = 0.00;
    TBV_b = 0.00;
    TBV_c = 0.00;
    TBV_d = 0.00;
    TBV_e = 0.00;

    //改变默认值个数判断
//    dH = defaultHeight;
//    if(!lineEditKtvHeight->text().isEmpty())//如果身高值赋过就更新
//        dH = lineEditKtvHeight->text().toFloat();

//    dW = defaultWeight;
//    if(!lineEditKtvWeight->text().isEmpty())//如果体重值赋过就更新
//        dW = lineEditKtvWeight->text().toFloat();

//    dA = defaultAge;
//    if(!lineEditKtvAge->text().isEmpty())
//        dA = lineEditKtvAge->text().toInt();
    //disUpTo->setText("0.000");
    disCurrentKtV->setText("0.000");

    if(isConfirmClick == true){
        dA = defaultAge;
        if(!lineEditKtvAge->text().isEmpty())
        {
            dA = lineEditKtvAge->text().toInt();
            lastDa = dA;
        }

        dW = defaultWeight;
        if(!lineEditKtvWeight->text().isEmpty())
        {
            dW = lineEditKtvWeight->text().toFloat();
            lastDw = dW;
        }

        dH = defaultHeight;
        if(!lineEditKtvHeight->text().isEmpty())
        {
            dH = lineEditKtvHeight->text().toFloat();
            lastDh = dH;
        }
    }else{
        dA = defaultAge;
        if(lastDa != 0)
            dA = lastDa;

        dW = defaultWeight;
        if(lastDw > 0.00001)
            dW = lastDw;

        dH = defaultHeight;
        if(lastDh > 0.00001)
            dH = lastDh;
    }

    current_v = V_MALE(dA,dH,dW);
    if(radioButtonSelectFemale->isChecked())
        current_v = V_FEMALE(dH, dW);

    targetKTV = ( TBV_all * (total_secCount/60) / 10 ) / current_v * adpProDefVV;//没开始治疗也可查看目标值
    if(stateTMP.tipState.current == TREAT_FINISH)//治疗结束
    {
        updateTime = 0;//记录值变化次数
        ctbv_all = 0.00;
        temp_all = 0.00;
        updateTime = 0;
        actualKTV = 0.000;
        targetKTV = 0.00;
        targetKTV = ( TBV_all * (total_secCount/60) / 10 ) / current_v * adpProDefVV;
    }else if(stateTMP.tipState.current == TREAT_RUNNING){//正在治疗
        updateTime = (updateTime >= (total_secCount/FIVE_SECOND)) ? (total_secCount/FIVE_SECOND) : ++updateTime;
        actualKTV += ( TBV_all / 10 ) / current_v  * (float(FIVE_SECOND)/60);//默认值的计算
        // if(current_v != 0)
        //     actualKTV += ( TBV_all / 10 ) / current_v ;
        // else{
        //     actualKTV += ( TBV_all / 10 );//开始治疗后就有算值,待有病人信息再显示出来
        // }
        //血泵流速发生速变
        QString ca = QString("%1").arg(ctbv_all);
        QString ta = QString("%1").arg(TBV_all);
        if((ca.compare(ta) != 0) && (ctbv_all > 0.00001)){
            temp_all = ctbv_all * ((updateTime - (float(FIVE_SECOND)/60))/ (total_secCount/(float)FIVE_SECOND)) + TBV_all * ((total_secCount/float(FIVE_SECOND)) - updateTime + (float(FIVE_SECOND)/60)) / (total_secCount/float(FIVE_SECOND));
            targetKTV = ( temp_all * (total_secCount/60) / 10 ) / current_v  * adpProDefVV;
        }else{
            //QString tea = QString("%1").arg(temp_all);
            if(temp_all > 0.00001){//血泵流速有更新过就用最新的替换
                targetKTV = ( temp_all * (total_secCount/60) / 10 ) / current_v  * adpProDefVV;//3 2 2的情况
            }else{
                targetKTV = ( TBV_all * (total_secCount/60) / 10 ) / current_v  * adpProDefVV;
            }
        }
    }else if(stateTMP.tipState.current == TREAT_READY){//准备状态
        actualKTV = 0.000;
    }
    ctbv_all = TBV_all;//防止治疗中改变血泵值
    disUpTo->setText(QString::number(targetKTV,'f',3));
    disCurrentKtV->setText(QString::number(actualKTV,'f',3));
    isConfirmClick = false;
}