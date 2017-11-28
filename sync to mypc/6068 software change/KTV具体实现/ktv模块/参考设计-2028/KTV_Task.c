#include "KTV_Task.h"

#if KTV

#define  STANDBLOODFLOW   250
#define  STANDHDFLOW      500 
#undef DEBUG

#ifdef DEBUG
const INT8U ddlin[30] = {141,142,141,142,143,138,139,144,143,140,
                         141,142,141,142,143,138,139,144,143,140,
                         141,142,141,142,143,138,139,144,143,140,
                        };
const INT8U ddlout[30] = {140,141,140,141,142,137,138,142,141,138,
                          140,141,140,141,142,137,138,142,141,138,
                          140,141,140,141,142,137,138,142,141,138,
                        };
#endif

#define KTV_MAX   300
void * malloc(unsigned nbyte);
void free(void *pt);

void KTVWork_Task(void *Id)
{
 INT8U err;
 INT16U i;
 INT16U temp_old_cl;
 INT16U temp_cll;
 Id = Id;
 temp_old_cl = mG_Total_CL;
 
 mG_KTVDisp.DisKtv[0]  = 0;
 mG_KTVDisp.DisNum     = 0;
 mG_KTVHDInfo.Counter  = 0;
 mG_KTVHDInfo.Flag     = 1;
 
 while(1)
  {
  
#ifdef DEBUG
  for(mG_KTVHDInfo.Counter = 0; mG_KTVHDInfo.Counter<30; mG_KTVHDInfo.Counter++)
    {
     mG_KTVHDInfo.ddlIn[mG_KTVHDInfo.Counter] = ddlin[mG_KTVHDInfo.Counter];
     mG_KTVHDInfo.ddlOut[mG_KTVHDInfo.Counter] = ddlout[mG_KTVHDInfo.Counter];
    }
    mG_KTVHDInfo.Counter = 0;
    temp_cll = mG_Total_CL - temp_old_cl;
    temp_old_cl = mG_Total_CL;
    if(mG_KTVDisp.DisNum < KTVDISPNUM-1)
      {
       mG_KTVDisp.DisNum++;
       i = GetDtv(300) + mG_KTVDisp.DisKtv[mG_KTVDisp.DisNum-1];
       if(i > KTV_MAX)         i = KTV_MAX;
       mG_KTVDisp.DisKtv[mG_KTVDisp.DisNum] = i;
      } 

    if(mG_KTVHDInfo.Flag && uc_mg_blood_work_flag && (mG_TX_Mode == TXMODE_TX) && (!(ui_mg_already_work_second%30)))  
      {
       CommQBuf[Q_COMM_KTVDDL - Q_COMM_START] = Q_COMM_KTVDDL;
       err = OSQPost(CommQ,(void *)&CommQBuf[Q_COMM_KTVDDL - Q_COMM_START]);
      }  
    
    OSTimeDly(10000);
 
#else
    if(mG_KTVHDInfo.Counter >= KTVDDLNUM) //取到了30个值
      {
        mG_KTVHDInfo.Counter = 0; //归0
        temp_cll = mG_Total_CL - temp_old_cl;//计算超率量有多大（此15分钟内）
        temp_old_cl = mG_Total_CL;
        if(mG_KTVDisp.DisNum < KTVDISPNUM-1)
         {
          mG_KTVDisp.DisNum++;
          i = GetDtv(temp_cll) + mG_KTVDisp.DisKtv[mG_KTVDisp.DisNum-1];
          if(i > KTV_MAX)  i = KTV_MAX;
          mG_KTVDisp.DisKtv[mG_KTVDisp.DisNum] = i;
         } 
      }
    if(mG_KTVHDInfo.Flag && uc_mg_blood_work_flag && (mG_TX_Mode == TXMODE_TX) && (!(ui_mg_already_work_second%30)))  
      {
       CommQBuf[Q_COMM_KTVDDL - Q_COMM_START] = Q_COMM_KTVDDL;
       err = OSQPost(CommQ,(void *)&CommQBuf[Q_COMM_KTVDDL - Q_COMM_START]);
       OSTimeDly(1000);
      }  
    
    OSTimeDly(800);
#endif  
  }
}

#define KTV_ADD_PARA    1
//#define KTV_PARA        20
/****************************************************
*
*Dt/V = Q|Cdi-Cdo|/|Cdimax-Cdomin|
//Q = [Do/Di*Bo/Bi*(0.58W+UF)/(0.58W-UF)]*1.73/P
*Q = [Do/Di*Bo/Bi*(0.58W+UF)/(0.58W-UF)]*KTV_ADD_PARA/P
*****************************************************/
static INT8U GetDtv(INT16U uf)
    {
    float q,UF;
    INT8U i;
    INT16U Cdi,Cdo,CdiMax,CdoMin,ui_temp_data;
    UF = uf;
    UF /= 1000;
    ////有点问题0    ui_mg_xb_flow/STANDHDFLOW 应该为STANDBLOODFLOW
    // q = ((mG_Flow_TXY/STANDHDFLOW)*(ui_mg_xb_flow/STANDHDFLOW)*((0.58*mG_ManInfo.weight+UF)/(0.58*mG_ManInfo.weight-UF))); //*1.73/mG_ManInfo.bodyarea;
    q = 0.58*mG_ManInfo.weight+UF;
    q = q/(0.58*mG_ManInfo.weight-UF);
    
    q = q * ( ui_mg_xb_flow + 0.2*ui_mg_xb_flow ) / ( STANDBLOODFLOW + ui_mg_xb_flow ) ;
    q = q * mG_Flow_TXY /STANDHDFLOW; 
    if(mG_ManInfo.bodyarea == 0)   mG_ManInfo.bodyarea = 1.73;
    q = q/mG_ManInfo.bodyarea;
    
    JiaoDDLOut(KTVDDLNUM);
 
    Cdi = GetP(mG_KTVHDInfo.ddlIn, KTVDDLNUM);   
    Cdo = GetP(mG_KTVHDInfo.ddlOut, KTVDDLNUM);  
    for(i=0; i<KTVDDLNUM; i++)
        {
        if((mG_KTVHDInfo.ddlIn[i] > (Cdi+3)) || ((mG_KTVHDInfo.ddlIn[i]+3) < Cdi))     mG_KTVHDInfo.ddlIn[i] = Cdi;
        if((mG_KTVHDInfo.ddlOut[i] > (Cdo+3)) || ((mG_KTVHDInfo.ddlOut[i]+3) < Cdo))   mG_KTVHDInfo.ddlOut[i] = Cdo;  
        }
    GetOrder(mG_KTVHDInfo.ddlIn, KTVDDLNUM);  
    GetOrder(mG_KTVHDInfo.ddlOut, KTVDDLNUM);
    
    CdiMax = GetMax(mG_KTVHDInfo.ddlIn, KTVDDLNUM);
    CdoMin = GetMin(mG_KTVHDInfo.ddlOut, KTVDDLNUM);
    ui_temp_data = Cdi;
    if(CdiMax>CdoMin)   CdiMax -= CdoMin;
    else                CdiMax = CdoMin - CdiMax;
    
    q = 10*q*ui_temp_data/CdiMax/uc_ktv_para;//KTV_PARA;
    i = (INT8U) q;
    q -= i;
    if(q >= 0.5)  i++;           
    return i;
    }
	
static INT16U GetP(INT16U *ddl, INT8U num)
{ 
 INT8U i;
 INT16U temp;
 temp = 0;
 for(i=0; i<num; i++)
   temp += *(ddl+i);
 return (mDIV_45(temp,num));
}

#define   PINNUM   10

static INT16U GetMax(INT16U *ddl, INT8U num)               //将最大的10个数据取平均值
    { 
    INT8U i;
    INT16U temp;
    temp = *(ddl+num-1);
    for(i=1; i<PINNUM; i++)   temp += *(ddl+num-1-i);
    temp = mDIV_45(temp, PINNUM);
    return temp;
    }
	
static INT16U GetMin(INT16U *ddl, INT8U num)
    { 
    INT8U i;
    INT16U temp;
    temp = *ddl;
    for(i=1; i<PINNUM; i++)   temp += *(ddl+i);
    temp = mDIV_45(temp, PINNUM);  
    return temp;
    }
	
//////冒泡法排序从小到大排序////////
static void GetOrder(INT16U *ui_list_point, INT8U uc_number)
    {
    INT8U uc_temp1_data;
    INT8U uc_temp2_data;
    INT16U  ui_temp_data;
    for(uc_temp1_data = 0 ; uc_temp1_data < (uc_number -1) ; uc_temp1_data++)
        {
        for(uc_temp2_data = uc_temp1_data + 1 ; uc_temp2_data < uc_number  ; uc_temp2_data++)
             {
             if(ui_list_point[uc_temp1_data] > ui_list_point[uc_temp2_data])
                   {
                   ui_temp_data                 = ui_list_point[uc_temp1_data] ;
                   ui_list_point[uc_temp1_data] = ui_list_point[uc_temp2_data];
                   ui_list_point[uc_temp2_data] = ui_temp_data;
                   }
             }
        }
    }

/*****************************************************
*             JiaoDDLOut
*校正输出电导
*
******************************************************/
static void JiaoDDLOut(INT8U num)
    {
    INT8U i;
    INT16U temp;
    for(i=0; i<num; i++)
        {
        temp = mG_KTVHDInfo.ddlOut[i];
        temp *= mG_KTVHDInfo.DDL_para;
        mG_KTVHDInfo.ddlOut[i] = mDIV_45(temp,100);
        }
    }
#endif