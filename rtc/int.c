#include "int.h"

void exceptionundef(void)
{
    //printf("undefined instruction exception.\n");
    while(1);
}

void exceptionswi(void)
{
    //printf("swi exception.\n");
    while(1);
}

void exceptionpabort(void)
{
    //printf("pabort exception.\n");
    while(1);
}

void exceptiondabort(void)
{
    //printf("dabort exception.\n");
    while(1);
}

void FIQ_handle(void)
{
    //printf("fiq exception.\n");
    while(1);
}

//exception and interrupt initialize
void int_init( void)
{
    //vector table of exception initialize
    pExceptionUNDEF	  =	(unsigned long)exceptionundef;//undefine
    pExceptionSWI  	  =	(unsigned long)exceptionswi;//software interrupt
    pExceptionPABORT      =	(unsigned long)exceptionpabort;//pabort
    pExceptionDABORT      =	(unsigned long)exceptiondabort;//data abort
    pExceptionIRQ	  =	(unsigned long)IRQ_handle;//IRQ
    pExceptionFIQ	  =	(unsigned long)FIQ_handle;//FIQ

    //interrupt controller  init
    //Disables Interrupt in VICxINTENABLE Register
    VIC0INTENCLEAR = 0xffffffff;
    VIC1INTENCLEAR = 0xffffffff;
    VIC2INTENCLEAR = 0xffffffff;
    VIC3INTENCLEAR = 0xffffffff;

    //Selects interrupt type for interrupt request (IRQ)
    VIC0INTSELECT = 0x0;
    VIC1INTSELECT = 0x0;
    VIC2INTSELECT = 0x0;
    VIC3INTSELECT = 0x0;

    /*Contains the address of the currently active ISR, with reset value 0x00000000.
    A read of this register returns the address of the ISR and sets the current interrupt as being serviced. 
    A read must be performed while there is an active interrupt.
    A write of any value to this register clears the current interrupt. 
    A write must only be performed at the end of an interrupt service routine.*/
    VIC0ADDRESS = 0;
    VIC1ADDRESS = 0;
    VIC2ADDRESS = 0;
    VIC3ADDRESS = 0;
}

//the address resigster of vector interrupt initialize
			//num of int, 	isr of int
void intc_setvectaddr(unsigned long intnum, void (*handler)(void))
{
    //VIC0
    if(intnum<32)
    {
        *( (volatile unsigned long *)(VIC0VECTADDR + 4*intnum) ) = (unsigned)handler;
    }
    //VIC1
    else if(intnum<64)
    {
        *( (volatile unsigned long *)(VIC1VECTADDR + 4*(intnum-32)) ) = (unsigned)handler;
    }
    //VIC2
    else if(intnum<96)
    {
        *( (volatile unsigned long *)(VIC2VECTADDR + 4*(intnum-64)) ) = (unsigned)handler;
    }
    //VIC3
    else
    {
        *( (volatile unsigned long *)(VIC3VECTADDR + 4*(intnum-96)) ) = (unsigned)handler;
    }
    return;
}

//clear vector interrupt address
void intc_clearvectaddr(void)
{
    //the adderss of isr store in VICnADDRESS
    VIC0ADDRESS = 0;
    VIC1ADDRESS = 0;
    VIC2ADDRESS = 0;
    VIC3ADDRESS = 0;
}

//enable the interrupt controller
void intc_enable(unsigned long intnum)
{
    unsigned long temp;
    if(intnum<32)
    {
        temp = VIC0INTENABLE;
        temp |= (1<<intnum);
        VIC0INTENABLE = temp;
    }
    else if(intnum<64)
    {
        temp = VIC1INTENABLE;
        temp |= (1<<(intnum-32));
        VIC1INTENABLE = temp;
    }
    else if(intnum<96)
    {
        temp = VIC2INTENABLE;
        temp |= (1<<(intnum-64));
        VIC2INTENABLE = temp;
    }
    else if(intnum<NUM_ALL)
    {
        temp = VIC3INTENABLE;
        temp |= (1<<(intnum-96));
        VIC3INTENABLE = temp;
    }
    // NUM_ALL : enable all interrupt
    else
    {
        VIC0INTENABLE = 0xFFFFFFFF;
        VIC1INTENABLE = 0xFFFFFFFF;
        VIC2INTENABLE = 0xFFFFFFFF;
        VIC3INTENABLE = 0xFFFFFFFF;
    }
}

//disable interrupt controller
void intc_disable(unsigned long intnum)
{
    unsigned long temp;

    if(intnum<32)
    {
        temp = VIC0INTENCLEAR;
        temp |= (1<<intnum);
        VIC0INTENCLEAR = temp;
    }
    else if(intnum<64)
    {
        temp = VIC1INTENCLEAR;
        temp |= (1<<(intnum-32));
        VIC1INTENCLEAR = temp;
    }
    else if(intnum<96)
    {
        temp = VIC2INTENCLEAR;
        temp |= (1<<(intnum-64));
        VIC2INTENCLEAR = temp;
    }
    else if(intnum<NUM_ALL)
    {
        temp = VIC3INTENCLEAR;
        temp |= (1<<(intnum-96));
        VIC3INTENCLEAR = temp;
    }
    // NUM_ALL : disable all interrupt
    else
    {
        VIC0INTENCLEAR = 0xFFFFFFFF;
        VIC1INTENCLEAR = 0xFFFFFFFF;
        VIC2INTENCLEAR = 0xFFFFFFFF;
        VIC3INTENCLEAR = 0xFFFFFFFF;
    }
    return;
}


//get status of VICn
unsigned long intc_getvicirqstatus(unsigned long ucontroller)
{
    if(ucontroller == 0)
        return	VIC0IRQSTATUS;
    else if(ucontroller == 1)
        return 	VIC1IRQSTATUS;
    else if(ucontroller == 2)
        return 	VIC2IRQSTATUS;
    else if(ucontroller == 3)
        return 	VIC3IRQSTATUS;
    else
    {}
    return 0;
}

//the ISR of IRQ
void Irq_Isr(void)
{
	int i;    
	unsigned long vicaddr[4] = {VIC0ADDRESS,VIC1ADDRESS,VIC2ADDRESS,VIC3ADDRESS};  
    void (*isr)(void) = (void *)0;

    for(i=0; i<4; i++)
    {
        if(intc_getvicirqstatus(i) != 0)
        {
            isr = (void (*)(void)) vicaddr[i];
            break;
        }
    }
    (*isr)();
}

void init_led(void)
{
	GPJ2CON = (GPJ2CON & ~(0xffff<<0)) | (1 << 12) | (1 << 8) | (1 << 4) | (1 << 0);  	
}

void init_buzzer(void)
{

	GPD0CON = (GPD0CON & ~(0xf)) | (0x1 << 0);

}


void init_button(void)
{
	// K2-KP_COL0-ENIT16 
	GPH2CON |= (0xF << 0);							
	EXT_INT_2_CON &= ~(7<<0);
	EXT_INT_2_CON |= (2<<0);

	//k3-KP_COL1-ENIT17 
	GPH2CON |= (0xF << 4);
	EXT_INT_2_CON &= ~(7<<4);
	EXT_INT_2_CON |= (2<<4);

	//k4-KP_COL2-ENIT18 
	GPH2CON |= (0xF << 8);
	EXT_INT_2_CON &= ~(7<8);
	EXT_INT_2_CON |= (2<<8);
	
	//k5-KP_COL3-ENIT19
	GPH2CON |= (0xF << 12);
	EXT_INT_2_CON &= ~(7<<12);
	EXT_INT_2_CON |= (2<<12);
	
	//k6-KP_ROW0-ENIT24
	GPH3CON |= (0xF << 0);
	EXT_INT_3_CON &= ~(0x7 << 0);
	EXT_INT_3_CON |= (0x2<<0);

}


void init_watchdog(void)
{

	//关闭计数功能
	WTCON &= ~(1<<5);

	//关闭复位功能，启动中断方式输出
	WTCON = (WTCON & (~(1 << 0))) | (1 << 2);	

	//设置预分频器，分频输出1MHZ
	WTCON = (WTCON & (~(0xff << 8))) | (0x65 << 8);

	//设置分频器 128 128us 
	WTCON = (WTCON & (~(0x3 << 3))) | (0x3 << 3);

	//赋初值 
	WTCNT = 100;
	WTDAT = 100;

	//启动计数功能
	WTCON |= (1<<5); 
	

}


void adc_init(void)
{

	//channel 0
    ADCMUX = 0;
	
    TSADCCON0 = (1<<16)|(1<<14) | (65<<6); 
	TSADCCON0 &= ~(1<<2);
	
}

void init_rtc(void)
{

	//enable tick timer
	RTCCON |= (1<<8);

	//select 32768hz
	RTCCON &= ~(0xf<<4);

	//800~1.25hz,tint = 32768/1.25 =26214

	TICCNT |= 0x6666;

}

