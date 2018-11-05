#include "stdio.h"
#include "int.h"
#include "main.h"
unsigned int m;


int main(void)
{
    int i;
    m = 0;
	uart_init(); //uart0的初始化
	//GPD0CON配置蜂鸣器为输出模式
	GPD0CON &= ~(0xf);
	GPD0CON |= 0X1; //初始化GPD0_0--->output
	GPD0DAT &= ~(1<<0);
	
	for(i=0;i<0x100;i++);
	printf("\n\r the test of rtc on GEC210\n\r");
	
	int_init(); //中断控制器初始化
	rtc_alarm_init(); 
	rtc_tick_init();
	intc_setvectaddr(NUM_RTC_ALARM, isr_rtc_alarm);
	intc_setvectaddr(NUM_RTC_TICK, isr_rtc_tick);
	intc_enable(NUM_RTC_ALARM);
	intc_enable(NUM_RTC_TICK);
	while(1)
	{
		;
	}

	return 0;
}
