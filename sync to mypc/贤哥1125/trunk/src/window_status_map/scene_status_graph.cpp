#include "config_ui.h"
#include "global_var.h"
#include "scene_status_graph.h"

#define LABEL_STATUS_FONT(p,x,y)        \
    p.setGeometry(QRect(x, y, 40, 13)); \
    p.setFont(QFont(QString(CONFIG_FONT_STYLE), \
            12, CONFIG_FONT_NORMAL)); \
    p.setStyleSheet(CONFIG_COLOR_YELLOW_CHARS)



inline void SceneStatusGraph::setUi()
{
    QPalette palette;
    palette.setBrush(QPalette::Background,
            QBrush(QPixmap(":/resource/images/status/water_botton.bmp")));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    setGeometry(QRect(0, 70, 1024, 479));

    LABEL_STATUS_FONT(labelP2Power, 480, 12);
    LABEL_STATUS_FONT(labelT204Temp, 815, 140);
    LABEL_STATUS_FONT(labelC702Cond, 815, 153);

    LABEL_STATUS_FONT(labelP3Power, 728, 315);
    LABEL_STATUS_FONT(labelT201Temp, 370, 8);
    LABEL_STATUS_FONT(labelC701Cond, 370, 25);

    LABEL_STATUS_FONT(labelT203Temp, 816, 75);
    LABEL_STATUS_FONT(labelC709Cond, 816, 90);
    LABEL_STATUS_FONT(labelC709Target, 816, 105);

    LABEL_STATUS_FONT(labelT205Temp, 800, 440);
    LABEL_STATUS_FONT(labelC704Cond, 800, 455);

    LABEL_STATUS_FONT(labelCP1WP, 890, 420);

    LABEL_STATUS_FONT(labelBloodSpeed, 980, 380);
    LABEL_STATUS_FONT(labelP1Power, 144, 130);
    LABEL_STATUS_FONT(labelCP2Pre, 137, 188);
    LABEL_STATUS_FONT(labelJP1Rate, 465, 430);
    LABEL_STATUS_FONT(labelJP1Now, 465, 445);
    //LABEL_STATUS_FONT(labelHeatPower, 231, 73);
    LABEL_STATUS_FONT(labelT202Power, 237, 69);
    LABEL_STATUS_FONT(labelT202Temp, 237, 10);
    LABEL_STATUS_FONT(labelT202Target, 237, 23);
    LABEL_STATUS_FONT(labelJP3Stand, 319, 129);
    LABEL_STATUS_FONT(labelJP3Now, 319, 142);
    LABEL_STATUS_FONT(labelJP2Stand, 440, 127);
    LABEL_STATUS_FONT(labelJP2Now, 440, 140);

    LABEL_STATUS_FONT(labelDB501Leak, 462, 288);
    LABEL_STATUS_FONT(labelAP, 960, 190);
    LABEL_STATUS_FONT(labelVP, 960, 205);
    LABEL_STATUS_FONT(labelTMP, 960, 220);

    point[0].setX(596);
    point[0].setY(305);
    point[1].setX(840);
    point[1].setY(287);
    //point[2].setX();
    //point[2].setY();
    point[3].setX(82);
    point[3].setY(99);
    point[4].setX(47);
    point[4].setY(409);
    point[5].setX(840);
    point[5].setY(255);
    point[6].setX(47);
    point[6].setY(34);
    //point[7].setX();
    //point[7].setY();
    //point[8].setX();
    //point[8].setY();
    point[9].setX(770);
    point[9].setY(280);
    point[10].setX(644);
    point[10].setY(205);
    point[11].setX(644);
    point[11].setY(48);
    point[12].setX(644);
    point[12].setY(223);
    point[13].setX(644);
    point[13].setY(32);
    point[14].setX(554);
    point[14].setY(205);
    point[15].setX(554);
    point[15].setY(50);
    point[16].setX(554);
    point[16].setY(223);
    point[17].setX(554);
    point[17].setY(32);
    //point[18].setX();
    //point[18].setY();
    //point[19].setX();
    //point[19].setY();
    //point[20].setX();
    //point[20].setY();
    point[21].setX(735);
    point[21].setY(368);
    point[22].setX(596);
    point[22].setY(364);
    point[23].setX(839);
    point[23].setY(322);
    point[27].setX(922);
    point[27].setY(386);
    point[31].setX(189);
    point[31].setY(208);

    valve.valve32 = 0x0;
    update();
}

SceneStatusGraph::SceneStatusGraph(QWidget *parent) :
    QWidget(parent),
    labelP2Power(this),
    labelT204Temp(this),
    labelC702Cond(this),
    labelP3Power(this),
    labelT201Temp(this),
    labelC701Cond(this),
    //labelC701CondTarget(this),
    labelT203Temp(this),
    labelC709Cond(this),
    labelC709Target(this),
    labelT205Temp(this),
    labelC704Cond(this),
    labelCP1WP(this),
    labelBloodSpeed(this),
    labelP1Power(this),
    labelCP2Pre(this),
    labelJP1Rate(this),
    labelJP1Now(this),
    //labelHeatPower(this),
    labelT202Power(this),
    labelT202Temp(this),
    labelT202Target(this),
    labelJP3Stand(this),
    labelJP3Now(this),
    labelJP2Stand(this),
    labelJP2Now(this),
    labelDB501Leak(this),
    labelAP(this),
    labelVP(this),
    labelTMP(this),
    labelP2Speed(this),
    labelD6Speed(this)
{
    setUi();
    dataNodeAddObject(PACKET_MASTERCTRL_VALVE, this);
    dataNodeAddObject(PACKET_MASTERCTRL_OUTLET, this);
    dataNodeAddObject(PACKET_MASTERCTRL_MIX, this);
    dataNodeAddObject(PACKET_MASTERCTRL_FLUIDB, this);
    dataNodeAddObject(PACKET_MASTERCTRL_FLUIDAFTER, this);
    //dataNodeAddObject(PACKET_MASTERCTRL_PRESS, this);
    dataNodeAddObject(PACKET_MASTERCTRL_UFPUMP, this);
    dataNodeAddObject(PACKET_MASTERCTRL_HEAT, this);
    dataNodeAddObject(PACKET_MASTERCTRL_BPUMP, this);
    dataNodeAddObject(PACKET_MASTERCTRL_MIXPUMP, this);
    dataNodeAddObject(PACKET_NAME_BLOODPUMP, this);
    dataNodeAddObject(PACKET_MONITOR_WATER, this);
    dataNodeAddObject(PACKET_MONITOR_PRESS, this);
    PageAppendPiece("Status Graph", this);
}

void SceneStatusGraph::CustomSetText()
{
}

void SceneStatusGraph::setCusVisible(bool vis, char* para)
{
    para = para;
    if(vis==true)
    {
        PacketValve datav;
        dataRead(PACKET_MASTERCTRL_VALVE,&datav);
        valve.valve32=datav.valve32;

        PacketOutlet dataout;
        dataRead(PACKET_MASTERCTRL_OUTLET,&dataout);
        labelP2Power.setNum(dataout.N3_P2_Power);
        labelT204Temp.setNum((double(dataout.N3_out_temp))/10);
        labelC702Cond.setNum((double(dataout.N3_out_cond))/100);
        labelCP1WP.setNum(dataout.N3_WP-700);

        PacketFluidB datafluidB;
        dataRead(PACKET_MASTERCTRL_FLUIDB,&datafluidB);
        labelP3Power.setNum(datafluidB.N3_P3_Power);
        labelT201Temp.setNum((double(datafluidB.N3_B_temp))/10);
        labelC701Cond.setNum((double(datafluidB.N3_B_cond))/100);

        PacketMix datamix;
        dataRead(PACKET_MASTERCTRL_MIX,&datamix);
        labelT203Temp.setNum((double(datamix.N3_Mix_temp))/10);
        labelC709Cond.setNum((double(datamix.N3_Mix_cond))/100);
        labelC709Target.setNum((double(datamix.N3_mix_Target))/10);

        PacketFluidAfter dataAfter;
        dataRead(PACKET_MASTERCTRL_FLUIDAFTER,&dataAfter);
        labelT205Temp.setNum((double(dataAfter.N3_after_temp))/10);
        labelC704Cond.setNum((double(dataAfter.N3_after_cond))/100);

        PacketUfPump datauf;
        dataRead(PACKET_MASTERCTRL_UFPUMP,&datauf);
        labelP1Power.setNum(datauf.N3_P1_Power);
        labelCP2Pre.setNum(datauf.N3_Degass_P-700);
        labelJP1Rate.setNum(datauf.N3_UFP_Rate);
        labelJP1Now.setNum(datauf.N3_UFP_Feedback);

        PacketHeat dataheat;
        dataRead(PACKET_MASTERCTRL_HEAT,&dataheat);
        labelT202Power.setNum(dataheat.N3_heat_Power);
        labelT202Temp.setNum(dataheat.N3_heat_temp);
        labelT202Target.setNum(dataheat.N3_heat_Target);

        PacketBPump databPump;
        dataRead(PACKET_MASTERCTRL_BPUMP,&databPump);
        labelJP3Stand.setNum(databPump.N3_BP_Stand);
        labelJP3Now.setNum(databPump.N3_BP_Now);
        labelJP2Stand.setNum(databPump.N3_MixP_Stand);

        PacketMixPump datamixPump;
        dataRead(PACKET_MASTERCTRL_MIXPUMP,&datamixPump);
        labelJP2Now.setNum(datamixPump.N3_MixP_Now);

        PacketBloodPump datablood;
        dataRead(PACKET_NAME_BLOODPUMP,&datablood);
        labelBloodSpeed.setNum(datablood.N9_speed);

        PacketWaterOut datawater;
        dataRead(PACKET_MONITOR_WATER,&datawater);
        labelDB501Leak.setNum(datawater.N5_leak);

        PacketPress datapre;
        dataRead(PACKET_MONITOR_PRESS,&datapre);
        labelAP.setNum(datapre.N5_AP-700);
        labelVP.setNum(datapre.N5_VP-700);
        labelTMP.setNum(datapre.N5_TMP-700);

        update();
    }
    setVisible(vis);
}

void SceneStatusGraph::paintEvent(QPaintEvent *event)
{
    event = event;
    unsigned int i, mask;
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    for (i=0, mask=1; i<32; i++) {
        if ((valve.valve32&mask) == 0) {
            painter.setBrush(QColor(CONFIG_COLOR_RED));
            painter.drawEllipse(point[i], 4, 4);
        } else {
            painter.setBrush(QColor(CONFIG_COLOR_GREEN));
            painter.drawEllipse(point[i], 4, 4);
        }
        mask <<=1;
    }
}

void SceneStatusGraph::ActionHandle(const void* dataPtr,int name)
{
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneStatusGraph-ActionHandle-in");
#endif

    if(workstate.tipState.current==DEBUG)
        return;

    if(!this->isVisible())
    {
        return;
    }

    if ((dataPtr == NULL) || (name == 0)) {
        return;
    }

    if(name==PACKET_MASTERCTRL_VALVE) {
        PacketValve *v = (PacketValve*)dataPtr;
        valve.valve32 = v->valve32;
        update();
    } else if (name==PACKET_MASTERCTRL_OUTLET) {
        PacketOutlet *outlet = (PacketOutlet*)dataPtr;
        labelP2Power.setNum(outlet->N3_P2_Power);
        labelT204Temp.setNum((double(outlet->N3_out_temp))/10);
        labelC702Cond.setNum((double(outlet->N3_out_cond))/100);
        labelCP1WP.setNum(((PacketOutlet*)dataPtr)->N3_WP-700);
    } else if (name==PACKET_MASTERCTRL_FLUIDB) {
        PacketFluidB *fluidB = (PacketFluidB*)dataPtr;
        labelP3Power.setNum(fluidB->N3_P3_Power);
        labelT201Temp.setNum((double(fluidB->N3_B_temp))/10);
        labelC701Cond.setNum((double(fluidB->N3_B_cond))/100);
    } else if (name==PACKET_MASTERCTRL_MIX) {
        PacketMix *mix = (PacketMix*)dataPtr;
        labelT203Temp.setNum((double(mix->N3_Mix_temp))/10);
        labelC709Cond.setNum((double(mix->N3_Mix_cond))/100);
        labelC709Target.setNum((double(mix->N3_mix_Target))/10);
    } else if (name==PACKET_MASTERCTRL_FLUIDAFTER) {
        PacketFluidAfter *fluidAfter = (PacketFluidAfter*)dataPtr;
        labelT205Temp.setNum((double(fluidAfter->N3_after_temp))/10);
        labelC704Cond.setNum((double(fluidAfter->N3_after_cond))/100);
    }
    //else if (name==PACKET_MASTERCTRL_OUTLET) {//PACKET_MASTERCTRL_PRESS
        //labelCP1WP.setNum(((PacketOutlet*)dataPtr)->N3_WP-700);//PacketWayPress
    //}
    else if (name==PACKET_MASTERCTRL_UFPUMP) {
        PacketUfPump *ufPump = (PacketUfPump*)dataPtr;
        labelP1Power.setNum(ufPump->N3_P1_Power);
        labelCP2Pre.setNum(ufPump->N3_Degass_P-700);
        labelJP1Rate.setNum(ufPump->N3_UFP_Rate);
        labelJP1Now.setNum(ufPump->N3_UFP_Feedback);
    } else if (name==PACKET_MASTERCTRL_HEAT) {
        PacketHeat *heat = (PacketHeat*)dataPtr;
        labelT202Power.setNum(heat->N3_heat_Power);
        labelT202Temp.setNum(heat->N3_heat_temp);
        labelT202Target.setNum(heat->N3_heat_Target);
    } else if (name==PACKET_MASTERCTRL_BPUMP) {
        PacketBPump * bPump = (PacketBPump*)dataPtr;
        labelJP3Stand.setNum(bPump->N3_BP_Stand);
        labelJP3Now.setNum(bPump->N3_BP_Now);
        labelJP2Stand.setNum(bPump->N3_MixP_Stand);
    } else if (name==PACKET_MASTERCTRL_MIXPUMP) {
        PacketMixPump *mixPump = (PacketMixPump*)dataPtr;
        labelJP2Now.setNum(mixPump->N3_MixP_Now);
    } else if (name==PACKET_NAME_BLOODPUMP) {
        PacketBloodPump *blood = (PacketBloodPump*)dataPtr;
        labelBloodSpeed.setNum(blood->N9_speed);
    } else if (name==PACKET_MONITOR_WATER) {
        labelDB501Leak.setNum(((PacketWaterOut*)dataPtr)->N5_leak);
    } else if (name==PACKET_MONITOR_PRESS) {
        PacketPress *pre = (PacketPress *)dataPtr;
        labelAP.setNum(pre->N5_AP-700);
        labelVP.setNum(pre->N5_VP-700);
        labelTMP.setNum(pre->N5_TMP-700);
    }
#if SYSTEM_DEBUG_FLAG
    Debug_insert("SceneStatusGraph-ActionHandle-out");
#endif
}
