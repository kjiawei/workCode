#ifndef SCENE_RECORD_CURVE_H
#define SCENE_RECORD_CURVE_H

#include <QtGui>
#include <QLabel>
#include <QSqlTableModel>

#include "lang_switch.h"
#include "page_info.h"
#include "data_custom.h"
#include "data_timer.h"
#include "data_sqllite.h"



#define DE_SEL_CONDPROFILE 0   //选择电导曲线#/
#define DE_SEL_TEMPPROFILE 1   //选择温度曲线#/
#define DE_SEL_CONDDATA    2   //选择电导数据#/
#define DE_SEL_TEMPDATA    3   //选择温度数据#/
#define DE_SEL_PRESSPROFILE 4   //选择压力曲线#/
#define DE_SEL_PRESSDATA 5   //选择压力数据#/
#define DE_SEL_BLOODPRESSPROFILE 6   //选择血压曲线#/
#define DE_SEL_BLOODPRESSDATA 7   //选择血压数据#/



class SceneRecordCurve: public QObject, public PieceVisible, public LangSetText,
        public ActionCustom,public timer_handle
{
    Q_OBJECT
public:
    SceneRecordCurve(QWidget *parent=0);
    void CustomSetText();
    void setCusVisible(bool vis=false, char* para=NULL);

private:
    inline void setUi();
    void ActionHandle(const void* dataPtr=NULL, int name=0);
    void timer_handler(const int timeOut = 0);
    void curve_Handler(int type);  //选择曲线刷新处理#/
    void show_CondCurve();   //显示指定治疗的电导曲线#/
    void show_TempCurve();   //显示指定治疗的温度曲线#/
    void show_PressCurve();   //显示压力曲线#/
    void show_BloodPressCurve();  //显示血压曲线#/
    void show_CondData();    //显示指定治疗的电导数据#/
    void show_TempData();    //显示指定治疗的温度数据#/
    void show_PressData();    //显示压力数据#/
    void show_BloodPressData();//显示血压数据#/

    //清空释放表格#/
    void clearTableContents(QTableWidget *table);
    void showDataTable(int type,int curpage,int sumpage);  //显示指定的数据表#/
    void insertCurrent(int type,int row,stcuredatainfor info); //将一行指定数据插入指定表#/


private slots:
    void buttonCheckSlots();
    void buttonCondSlots();
    void buttonTempSlots();
    void buttonPressSlots();
    void buttonPressDataSlots();
    void buttonBloodPressSlots();
    void buttonBloodPressDataSlots();

    void buttonCondDateSlots();
    void buttonTempDateSlots();
    void buttonPreCureSlots();
    void buttonNxtCureSlots();
    void buttonPreDateSlots();
    void buttonNxtDateSlots();

private:
    int lastsec;

    int curCureNum;   //当前的治疗次数#/
    int sumCureNum;   //总的治疗次数#/
    int curDataPage;      //数据表当前的页数#/
    int sumDataPage;      //总的页数#/

    int curidnum;      //当前治疗进行时的治疗ID号#/

    char curve_select;  //标志选择温度或电导曲线或数据表 #/
    PacketOutlet outlet;
    PacketFluidB fluidB;
    PacketMix mix;
    PacketFluidAfter fluidAfter;
    PacketPress press;
    PacketBPMState BPMstate;
    PacketBPMMisc BPMmisc;

    QList<QPointF> pointC01;
    QList<QPointF> pointC02;
    QList<QPointF> pointC03;
    QList<QPointF> pointC04;
    QList<QPointF> pointC05;

    QLabel labelTitle;

    QLabel labelIDNum;
    QLineEdit editIDNum;
    QPushButton buttonCheck;

    QLabel labelstart;
    QLabel labelstarttime;

    QLabel labelend;
    QLabel labelendtime;

    QLabel labelcurenum;   //治疗次数页码信息
    QLabel labeldatenum;  //数据表格的页码

    QLabel labelcuretext;   //关于选择治疗次数后，信息的显示#/

    QPushButton buttonCond;
    QPushButton buttonTemp;
    QPushButton buttonPress;      //压力曲线
    QPushButton buttonPressData;  //压力数据
    QPushButton buttonBloodPress;      //血压曲线
    QPushButton buttonBloodPressData;  //血压数据
    QPushButton buttonCondDate;
    QPushButton buttonTempDate;
    QPushButton buttonPreCure;
    QPushButton buttonNxtCure;
    QPushButton buttonPreDate;
    QPushButton buttonNxtDate;

    bool buttonCond_flag;
    bool buttonTemp_flag;
    bool buttonPress_flag;
    bool buttonBloodPress_flag;

    int L_idnum;    //当前查询治疗ID号
    int L_curesum;  //该治疗ID号的治疗总次数
    int L_curenum_now; //该ID号的当前查询治疗次数

    QTableWidget * tableWidget;  //记录表对象#/
};

#endif
