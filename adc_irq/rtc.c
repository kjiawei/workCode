#include "main.h"
#include "stdio.h"
#include "int.h"
extern unsigned int m;
#define 	RTC_BASE	 (0xE2800000)
#define		rINTP      	 (*((volatile unsigned long *)(RTC_BASE + 0x30)))
#define		rRTCCON    	 (*((volatile unsigned long *)(RTC_BASE + 0x40)))
#define		rTICCNT    	 (*((volatile unsigned long *)(RTC_BASE + 0x44)))
#define		rRTCALM    	 (*((volatile unsigned long *)(RTC_BASE + 0x50)))
#define		rALMSEC    	 (*((volatile unsigned long *)(RTC_BASE + 0x54)))
#define		rALMMIN    	 (*((volatile unsigned long *)(RTC_BASE + 0x58)))
#define		rALMHOUR  	 (*((volatile unsigned long *)(RTC_BASE + 0x5c)))
#define		rALMDATE     (*((volatile unsigned long *)(RTC_BASE + 0x60)))
#define		rALMMON    	 (*((volatile unsigned long *)(RTC_BASE + 0x64)))
#define		rALMYEAR  	 (*((volatile unsigned long *)(RTC_BASE + 0x68)))
#define		rRTCRST      (*((volatile unsigned long *)(RTC_BASE + 0x6c)))
#define		rBCDSEC    	 (*((volatile unsigned long *)(RTC_BASE + 0x70)))
#define		rBCDMIN   	 (*((volatile unsigned long *)(RTC_BASE + 0x74)))
#define		rBCDHOUR     (*((volatile unsigned long *)(RTC_BASE + 0x78)))
#define		rBCDDATE     (*((volatile unsigned long *)(RTC_BASE + 0x7c)))
#define		rBCDDAY      (*((volatile unsigned long *)(RTC_BASE + 0x80)))
#define		rBCDMON      (*((volatile unsigned long *)(RTC_BASE + 0x84)))
#define		rBCDYEAR     (*((volatile unsigned long *)(RTC_BASE + 0x88)))
#define		rCURTICCNT   (*((volatile unsigned long *)(RTC_BASE + 0x90)))
#define		rRTCLVD    	 (*((volatile unsigned long *)(RTC_BASE + 0x94)))


unsigned int alarm_flag = 1; //1---alarm没有工作； 
          
// 函数功能：把十进制num转成bcd码，譬如把56转成0x56，num-->dec
static unsigned int num_2_bcd(unsigned int num)
{
	// 第一步，把56拆分成5和6 
	// 第二步，把5和6组合成0x56
	return (((num / 10)<<4) | (num % 10));
}
#if 1
// 函数功能：把bcd码bcd转成十进制，譬如把0x56转成56
static unsigned int bcd_2_num(unsigned int bcd)
{
	// 第一步，把0x56拆分成5和6 
	// 第二步，把5和6组合成56
	return (((bcd & 0xf0)>>4)*10 + (bcd & (0x0f)));
}
#endif
void rtc_alarm_init(void)
{
	rALMMIN = num_2_bcd(1);  //闹钟的分是1
	rRTCALM |= 1<<1;//分钟匹配成功，产生闹钟中断
	rRTCALM |= 1<<6; //打开闹钟中断功能
}

void isr_rtc_alarm(void) //闹钟的ISR<--当RTC的分和ALARM分一致
{
	static int i = -1; 
	printf("rtc alarm, i = %d...\r\n", i++); //debug
	GPD0DAT |= (1<<0); //beep 响
	alarm_flag = 0;
	rINTP |= (1<<1); //清中断
	intc_clearvectaddr();
}

void rtc_tick_init(void)
{
	
	rRTCCON &= ~(0xf<<4); //clock = 32768
	rTICCNT = 26214;//32768/(1.25);
	printf("---rtc write time test  2016-9-13 2 15:59:03 ---\n\r");
	struct rtc_time tWrite = 
	{
		.year = 2016,
		.month = 9,
		.date = 13,
		.hour = 15,
		.minute = 59,
		.second = 3,
		.day = 2,
	};
	rtc_set_time(&tWrite);
	rRTCCON |= (1<<8); //enable tick timer
}

void rtc_set_time(const struct rtc_time *p)
{
	// 第一步，打开RTC读写开关
	rRTCCON |= (1<<0);
	
	// 第二步，写RTC时间寄存器
	rBCDYEAR = num_2_bcd(p->year - 2000);
	rBCDMON = num_2_bcd(p->month);
	rBCDDATE = num_2_bcd(p->date);
	rBCDHOUR = num_2_bcd(p->hour);
	rBCDMIN = num_2_bcd(p->minute);
	rBCDSEC = num_2_bcd(p->second);
	rBCDDAY = num_2_bcd(p->day);
	
	// 最后一步，关上RTC的读写开关
	rRTCCON &= ~(1<<0);
}

void rtc_get_time(struct rtc_time *p)
{
	// 第一步，打开RTC读写开关
	rRTCCON |= (1<<0);
	
	// 第二步，读RTC时间寄存器
	p->year = bcd_2_num(rBCDYEAR) + 2000;
	p->month = bcd_2_num(rBCDMON);
	p->date = bcd_2_num(rBCDDATE);
	p->hour = bcd_2_num(rBCDHOUR);
	p->minute = bcd_2_num(rBCDMIN);
	p->second = bcd_2_num(rBCDSEC);
	p->day = bcd_2_num(rBCDDAY);
	
	// 最后一步，关上RTC的读写开关
	rRTCCON &= ~(1<<0);
}

void isr_rtc_tick(void) //Tick timer 的ISR （800ms）
{
	if(alarm_flag == 0)
		m++;  //每800ms 加一次1
	if(m == 12) //10s
	{
		GPD0DAT &= ~(1<<0);  //beep不响
		m = 0;
		alarm_flag = 1;
	}
	struct rtc_time tRead;
	rtc_get_time(&tRead);
	printf("The time read is: %d:%d:%d:%d:%d:%d:%d.\r\n", 
			tRead.year, tRead.month, tRead.date, tRead.hour, 
			tRead.minute, tRead.second, tRead.day);
	rINTP |= (1<<0); //清中断
	intc_clearvectaddr();
}
