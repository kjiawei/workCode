/*****************************************************
** 作者： Huangjunxian
** 创始时间：2014-11-18
** 修改人：Huangjunxian
** 修改时间：2014-11-20
** 描述：
**    曲线数据文件保存读取 《源文件》
*****************************************************/
#include "curve_datahandle.h"
#include <qdebug.h>

CurveDataHandle curveDataObject;

Curve_NaPara naParaArray[10]={{0,0}};
Curve_UfPara ufParaArray[10]={{0,0,0}};

void CurveDataHandle::initNaCurve()
{
    char filename[20]={0};

    for(int i=0;i<16;i++)
    {
        getfilename(NACURVETYPE,i,filename);
        if(QFile::exists(filename)==true)
            nacurvearray[i]=true;
        else
            nacurvearray[i]=false;
    }
}

void CurveDataHandle::initUfCurve()
{
    char filename[20]={0};

    for(int i=0;i<16;i++)
    {
        getfilename(UFCURVETYPE,i,filename);
        if(QFile::exists(filename)==true)
            ufcurvearray[i]=true;
        else
            ufcurvearray[i]=false;
    }
}

void CurveDataHandle::getfilename(int curvetype,int curvenum,char *filename)
{
    int i=0;

    strcpy(filename,"./db/");//曲线数据文件保存目录#/

    if(curvetype==NACURVETYPE)
    {
        i=5+sizeof(NADATAFILE)-1;
        strcpy(&filename[5],NADATAFILE);
        filename[i++]=0x30+(curvenum/10)%10;
        filename[i++]=0x30+curvenum%10;
    }
    else if(curvetype==UFCURVETYPE)
    {
        i=5+sizeof(UFDATAFILE)-1;
        strcpy(&filename[5],UFDATAFILE);
        filename[i++]=0x30+(curvenum/10)%10;
        filename[i++]=0x30+curvenum%10;
    }
    strcpy(&filename[i],".dat");
}

bool CurveDataHandle::saveCurve(int curvetype,int curvenum,char *pdata,int len)
{
    char filename[20]={0};

    if(curvenum<0||curvenum>=16)
        return false;

    getfilename(curvetype,curvenum,filename);

    QFile file(filename);

    //判断该文件是否存在，如果不存在则新建#/
    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open file fail1";
        return false;
    }

    if(file.write(pdata,len)!=len)
        return false;

    file.flush();

    if(curvetype==NACURVETYPE)
        nacurvearray[curvenum]=true;
    else if(curvetype==UFCURVETYPE)
        ufcurvearray[curvenum]=true;

    return true;
}

bool CurveDataHandle::readCurve(int curvetype,int curvenum,char *pdata,int len)
{
    char filename[20]={0};

    if(curvenum<0||curvenum>=16)
        return false;

    getfilename(curvetype,curvenum,filename);//获取对应数据文件名#/

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"open file fail2";
        return false;
    }

    if(file.read(pdata,len)!=len)
        return false;

    return true;
}

bool CurveDataHandle::checkCurve(int curvetype,int curvenum)
{
    if(curvenum<0||curvenum>15)
        return false;

    if(curvetype==NACURVETYPE)
        return nacurvearray[curvenum];
    else if(curvetype==UFCURVETYPE)
        return ufcurvearray[curvenum];
    else
        return false;
}

QByteArray CurveDataHandle::checkPngFile(int curvetype,int curvenum)
{
    QString file("./db/");
    if(curvetype==NACURVETYPE)
        file += DE_NAPNGFILE_STR;
    else if(curvetype==UFCURVETYPE)
        file += DE_UFPNGFILE_STR;

    file += QString().setNum(curvenum);
    file += ".png";

    QByteArray ba;

    if(QFile::exists(file)==true)
        ba=file.toLatin1();
    else
        ba.clear();

    return ba;
}







