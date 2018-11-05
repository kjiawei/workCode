#ifndef _MAIN_H_
#define _MAIN_H_

// rtc.c
struct rtc_time
{
	unsigned int year;
	unsigned int month;
	unsigned int date;			// 几号
	unsigned int hour;			
	unsigned int minute;
	unsigned int second;
	unsigned int day;			// 星期几
};
//rtc相关函数
void rtc_alarm_init(void);
void isr_rtc_alarm(void);
void rtc_set_time(const struct rtc_time *p);
void rtc_tick_init(void);
void rtc_get_time(struct rtc_time *p);
void isr_rtc_tick(void);
//串口相关函数
void uart_init();

#endif