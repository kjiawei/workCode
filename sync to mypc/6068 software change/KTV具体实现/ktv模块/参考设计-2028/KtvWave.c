#include "../gui/gui/core/GUI.h"
#include	"../ucos-ii/includes.h"               /* uC/OS interface */
#include "../gui/gui/Widget/BUTTON.H"
#include "../gui/gui/Widget/PROGBar.h"
#include "../app/mygui.h"
#include "../mygui/jihua.h"
#include "../mygui/extern_var.h"

#if WAVE&&KTV
#include "../app/wave/wave.h"

#define ARAEHCOX1   50
#define ARAEHCOX2   (ARAEHCOX1+380)
#define ARAEHCOY1   214-100+24//214-100
#define ARAEHCOY2   (ARAEHCOY1+210+100)
#define SCALESTARTX  (ARAEHCOX1)
#define SCALESTARTY  (ARAEHCOY2)  
#define SCALEHIGH    10
#define SCALELONG    60

static INT8U save_disktv; 
static INT16U GetX(INT16U xData)
    {
    INT16U ui_temp_data ;
    ui_temp_data = xData;
    ui_temp_data = ui_temp_data * 15;
    ui_temp_data = ui_temp_data + SCALESTARTX;
    return ui_temp_data;
    }
	
static INT16U GetY(INT16U yData)
    {
    INT16U ui_temp_data;
    ui_temp_data = SCALESTARTY;
    ui_temp_data = ui_temp_data - yData;
    return ui_temp_data;
    }
	
static void draw(INT8U Dstart)
    {
    INT8U i;
    INT16U x1,x2,y1,y2;
    GUI_SetColor(0x00ffff); 
    for(i=Dstart; i<mG_KTVDisp.DisNum; i++)
        {
        x1 = GetX(i);
        y1 = GetY(mG_KTVDisp.DisKtv[i]);
        x2 = GetX(i+1);
        y2 = GetY(mG_KTVDisp.DisKtv[i+1]);
        GUI_DrawLine(x1,y1, x2,y2); 
        } 
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font32B_1); 
    GUI_SetColor(GUI_GREEN);
    GUI_SetTextMode(GUI_TM_NORMAL);  
 
    GUI_GotoXY(ARAEHCOX1+10 , ARAEHCOY1+10);
    i = mG_KTVDisp.DisKtv[i]/10;
    GUI_DispDecShift(i,3, 1);
    }
	
void DisWaveInit_KTV(void)
    {
    INT8U i;
    GUI_MEMDEV_Handle hMem;
    hMem = GUI_MEMDEV_Create(ARAE1X1,ARAE1Y1,ARAE1X2,ARAE1Y2);
    GUI_MEMDEV_Select(hMem);
    InitDraw(LINEY_KTV,1);
    if(b_language_select)
        {
        #if COUNTRY == WZBKST
        GUI_DisHzAt("(§¹) ",SCALESTARTX+6*SCALELONG+10, ARAEHCOY2+10);
        GUI_DisHzAt("Kt/V", ARAEHCOX1-30, ARAEHCOY1-20);   
        GUI_SetColor(GUI_WHITE);
        GUI_DisHzAt("§¦§ã§Ý§Ú §â§Ö§Ù§å§Ý§î§ä§Ñ§ä §á§à§ã§Ý§Ö §Õ§Ú§Ñ§Ý§Ú§Ù§Ñ-> 1.2, §ï§ä§à §à§Ù§ß§Ñ§é§Ñ§Ö§ä §Ñ§Õ§Ö§Ü§Ó§Ñ§ä§ß§à§ã§ä§î §Õ§Ú§Ñ§Ý§Ú§Ù§Ñ.", EXPCHARX2, EXPCHARY1);    
    
        #elif COUNTRY == COUNTRY_SA
        GUI_DispStringAt("(h) ",SCALESTARTX+6*SCALELONG+10, ARAEHCOY2+10);
        GUI_DispStringAt("Kt/V", ARAEHCOX1-30, ARAEHCOY1-20);   
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font16_1);
        GUI_DispStringAt("Si el resultado después de diálisis es >1.2, significa diálisis ideal.", EXPCHARX2, EXPCHARY1);        
        
        #elif COUNTRY == FARMARIN
        GUI_DispStringAt("(h) ",SCALESTARTX+6*SCALELONG+10, ARAEHCOY2+10);
        GUI_DispStringAt("Kt/V", ARAEHCOX1-30, ARAEHCOY1-20);   
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font16_1);
        GUI_DispStringAt("Se resultado após tratamento é > 1,2 siginfica dose diálise adequada.", EXPCHARX2, EXPCHARY1);
    
        #elif COUNTRY == COUNTRY_TNS
        GUI_DispStringAt("(h) ",SCALESTARTX+6*SCALELONG+10, ARAEHCOY2+10);
        GUI_DispStringAt("Kt/V", ARAEHCOX1-30, ARAEHCOY1-20);   
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font16_1);
        GUI_DispStringAt("Si le résultat après la séance de dialyse soit >1.2, ça veut dire que la dialyse est adéquate.", EXPCHARX2-30, EXPCHARY1);
           
        #else
        GUI_DisHzAt("Ð¡Ê± ",SCALESTARTX+6*SCALELONG+5, ARAEHCOY2+10);
        GUI_DispStringAt("Kt/V", ARAEHCOX1-30, ARAEHCOY1-20);
        GUI_SetColor(GUI_WHITE);
        GUI_DisHzAt("Kt/VÇúÏß·´Ó¦ÁËÍ¸ÎöµÄÐ§¹û, Í¸ÎöÍê½á¹û >1.2 ±íÊ¾Í¸Îö³ä·Ö",EXPCHARX2, EXPCHARY1);
        #endif
        }
    else
        {
        GUI_DispStringAt("(h) ",SCALESTARTX+6*SCALELONG+10, ARAEHCOY2+10);
        GUI_DispStringAt("Kt/V", ARAEHCOX1-30, ARAEHCOY1-20);   
        GUI_SetColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font16_1);
        GUI_DispStringAt("If the result after dialysis is >1.2,it measn dialysis adequacy.", EXPCHARX2, EXPCHARY1);
        }
 
    GUI_SetColor(WAVEBKCOLOR);
    GUI_FillRect(ARAEHCOX1,ARAEHCOY1,ARAEHCOX2,ARAEHCOY2);
    GUI_SetColor(GUI_WHITE);
    GUI_DrawLine(ARAEHCOX1-1,ARAEHCOY1-1, ARAEHCOX2+1,ARAEHCOY1-1); 
    GUI_DrawLine(ARAEHCOX1-1,ARAEHCOY1-1, ARAEHCOX1-1,ARAEHCOY2+1); 
    GUI_DrawLine(ARAEHCOX1-1,ARAEHCOY2+1, ARAEHCOX2+1,ARAEHCOY2+1); 
    GUI_DrawLine(ARAEHCOX2+1,ARAEHCOY2+1, ARAEHCOX2+1,ARAEHCOY1-1); 

    for(i=1;i <30; i++)
        {
        GUI_DrawLine(ARAEHCOX1-3, SCALESTARTY-i*SCALEHIGH, ARAEHCOX1-1, SCALESTARTY-i*SCALEHIGH);
        }
    for(i=1;i <7; i++)
        {
        GUI_DrawLine(ARAEHCOX1-7, SCALESTARTY-5*i*SCALEHIGH, ARAEHCOX1-1, SCALESTARTY-5*i*SCALEHIGH);
        }
  
   for(i=1;i <7; i++)
        {
        GUI_DrawLine(SCALESTARTX+i*SCALELONG, ARAEHCOY2+7, SCALESTARTX+i*SCALELONG, ARAEHCOY2+1);
        }
   GUI_SetFont(&GUI_Font16_1);
   GUI_DispStringAt("0.0", ARAEHCOX1-30, SCALESTARTY-8);
   GUI_DispStringAt("0.5", ARAEHCOX1-30, SCALESTARTY-5*SCALEHIGH-8);
   GUI_DispStringAt("1.0", ARAEHCOX1-30, SCALESTARTY-10*SCALEHIGH-8);
   GUI_DispStringAt("1.5", ARAEHCOX1-30, SCALESTARTY-15*SCALEHIGH-8);
   GUI_DispStringAt("2.0", ARAEHCOX1-30, SCALESTARTY-20*SCALEHIGH-8);
   GUI_DispStringAt("2.5", ARAEHCOX1-30, SCALESTARTY-25*SCALEHIGH-8); 
   GUI_DispStringAt("3.0", ARAEHCOX1-30, SCALESTARTY-30*SCALEHIGH-8); 
 
   GUI_DispStringAt("0", SCALESTARTX-4, ARAEHCOY2+10);
   GUI_DispStringAt("1", SCALESTARTX+SCALELONG-4, ARAEHCOY2+10);
   GUI_DispStringAt("2", SCALESTARTX+2*SCALELONG-4, ARAEHCOY2+10);
   GUI_DispStringAt("3", SCALESTARTX+3*SCALELONG-4, ARAEHCOY2+10);
   GUI_DispStringAt("4", SCALESTARTX+4*SCALELONG-4, ARAEHCOY2+10);
   GUI_DispStringAt("5", SCALESTARTX+5*SCALELONG-4, ARAEHCOY2+10);
   GUI_DispStringAt("6", SCALESTARTX+6*SCALELONG-4, ARAEHCOY2+10);

   GUI_SetColor(WAVELINECOL);
   for(i=0;i <30; i++)
       {
       GUI_DrawLine(ARAEHCOX1, SCALESTARTY-(i+1)*SCALEHIGH, ARAEHCOX2, SCALESTARTY-(i+1)*SCALEHIGH);
       }
 
   for(i=1;i <7; i++)
       {
       GUI_DrawLine(SCALESTARTX+i*SCALELONG, ARAEHCOY2, SCALESTARTX+i*SCALELONG, ARAEHCOY1);
       }
   draw(0);  
   GUI_MEMDEV_CopyToLCD(hMem);
   GUI_MEMDEV_Select(0);
   GUI_MEMDEV_Delete(hMem);
   }
   
void DisWave_KTV(int Key, INT8U mode)
   {
   Key = Key;
   mode = mode;
   if(save_disktv != mG_KTVDisp.DisNum)
       {
       draw(save_disktv);
       save_disktv = mG_KTVDisp.DisNum;
       } 
   }
   
#endif