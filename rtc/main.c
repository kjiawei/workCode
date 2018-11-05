#include "stdio.h"
#include "int.h"

void uart_init(void);
void delay(int val);
void set_alrmvalue(void);
void set_rtcvalue(void);
volatile int a;


void isr_tic(void)
{
	unsigned long year, mon, week, day, hour,min, second;

	second  = BCDSEC;
	min		= BCDMIN;
	hour    = BCDHOUR;
	day	    = BCDDAY;
	week    = BCDWEEK;
	mon     = BCDMON;
	year    = BCDYEAR & 0xfff;


	printf("year: %x, mon: %2x, day: %2x, week: %2x, hour: %2x, min: %2x, sec: %2x\r\n", year, mon, day, week, hour, min, second);

	RTC_INTP |= (1<<0);
	intc_clearvectaddr();
	
}

void isr_alarm(void)
{
  	a = 10;
	printf("alarm !!! a = %d \r\n", a);
	RTC_INTP |= (1<<1);
	intc_clearvectaddr();
}



int main(void)
{

	a = 0;
	
	int_init();
    uart_init();
	init_buzzer();
	
	init_rtc();
	set_rtcvalue();
	set_alrmvalue();
	
	intc_setvectaddr(NUM_RTC_TICK, isr_tic);
	intc_enable(NUM_RTC_TICK);
	
	intc_setvectaddr(NUM_RTC_ALARM, isr_alarm);
	intc_enable(NUM_RTC_ALARM);

	while(1)
	{
		while(a)
		{
			printf("a = %d\r\n", a);
			GPD0DAT = 0x1;
			delay(5000);
			a--;
		}	
		GPD0DAT = 0x0;
	}

    return 0;
}

void set_rtcvalue(void)
{
	RTCCON |= (1<<0);

	BCDSEC 	= 0x50;
	BCDMIN  = 0x45;
	BCDHOUR = 0x11;
	BCDDAY  = 0x13;
	BCDWEEK = 0x02;
	BCDMON  = 0x09;
	BCDYEAR = 0x16;

	RTCCON &= ~(1<<0);

	printf("set rtc value successd \r\n");
	
}

void set_alrmvalue(void)
{

	unsigned long year, mon, week, day, hour,min, second;

	ALMSEC 	= 0x00;
	ALMMIN  = 0x46;
	ALMHOUR = 0x11;
	ALMDAY  = 0x13;
	ALMMON  = 0x09;
	ALMYEAR = 0x16;

	RTCALM |= (0xff<<0);

	//RTCALM |= (0x1<<6);
	
	printf("set alrm value successd \r\n");
}



void delay(int val)
{	
	volatile unsigned int i;	
	volatile unsigned char j;	
	for(i=0;i<val;i++)	
	{		
		for(j=0; j<200; j++);		
		for(j=0; j<102; j++);		
	}		
}

