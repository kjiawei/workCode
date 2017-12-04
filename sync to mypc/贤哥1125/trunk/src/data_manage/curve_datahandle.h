/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-18
** 修改人：Huangjunxian
** 修改时间：2014-11-20
** 描述：
**    曲线数据文件保存读取 《头文件》
*****************************************************/
#ifndef CURVE_DATAHANDLE_H
#define CURVE_DATAHANDLE_H

#include <QFile>
#include <string.h>

/*--------------------------------曲线数据处理-------------------------------------*/
#define NADATAFILE "nacurve"
#define UFDATAFILE "ufcurve"
#define DE_NAPNGFILE_STR "napng"
#define DE_UFPNGFILE_STR "ufpng"

#define NACURVETYPE 1
#define UFCURVETYPE 2

struct Curve_NaPara
{
    float navalue;  //钠离子浓度值#/
    float curetime; //该段治疗时间#/
};

struct Curve_UfPara
{
    bool seqmode;
    float ufrate;
    float curetime;
};

struct NA_Profile_Array
{
    float navalue;   //钠离子浓度值#/
    bool flag;       //该段运行标志#/
};

struct UF_Profile_Array
{
    float ufrate;   //每段占平均时段超滤量的百分比#/
    float ufvol;    //每段的超滤目标量#/
    bool seqmode;   //1：单超 0：普通开超滤#/
    bool flag;      //该段运行标志#/
};


struct Cure_Profile_Array
{
    float curevalue;   //治疗曲线值#/
    bool flag;       //该段运行标志#/
};

class CurveDataHandle
{
public:
    void initNaCurve();
    void initUfCurve();
    void getfilename(int curvetype,int curvenum,char *filename);
    bool saveCurve(int curvetype,int curvenum,char *pdata,int len);
    bool readCurve(int curvetype,int curvenum,char *pdata,int len);
    bool checkCurve(int curvetype,int curvenum);//数据文件编号从0开始#/
    QByteArray checkPngFile(int curvetype,int curvenum);

private:
    bool nacurvearray[16];
    bool ufcurvearray[16];
};


extern CurveDataHandle curveDataObject;
extern Curve_NaPara naParaArray[]; //钠曲线数据缓冲区#/
extern Curve_UfPara ufParaArray[]; //UF曲线数据缓冲区#/

#endif

