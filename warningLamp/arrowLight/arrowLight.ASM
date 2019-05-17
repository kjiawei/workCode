;#include  "sn8p2501d.h";INCLUDE<SN8P2501D>   INCLUDE 'DOSMACS.ASM'
chip	sn8p2501D
//{{SONIX_CODE_OPTION   系统的硬件配置，包括振荡器类型，看门狗定时器的操作，LVD 选项，复位引脚
//选项以及 OTP ROM 的安全控制
	.Code_Option	LVD		LVD_M		; VDD 低于 2.0V 时，LVD 复位系统；PFLAG 寄存器的 LVD24 位作为 2.4V 低电压监测器 2.0V Reset Enable LVD24 bit of PFLAG for 2.4V Low Voltage Indicator
	.Code_Option	Reset_Pin	P11		;P1.1 为单向输入引脚，无上拉电阻
	.Code_Option	Watch_Dog	Enable		; Normal mode: Enable Watchdog Green and Sleep mode: Stop Watchdog
	.Code_Option	High_Clk	IHRC_16M	; 高速时钟采用内部 16MHz RC 振荡电路，XIN/XOUT 为 GPIO 引脚。 Internal 16M RC Oscillator
	.Code_Option	Fcpu		#1     ; Fcpu = Fosc/2 指令周期 = 2 个时钟周期
	.Code_Option	Security	Enable ;ROM 代码加密
	.Code_Option	Noise_Filter	Disable ;关闭杂讯滤波功能，Fcpu = Fosc/1~Fosc/16
//}}SONIX_CODE_OPTION


.data ;初始化数据段
	r_flage		ds	1
	r_100us		equ	r_flage.0  ;等价 #define 常量
	r_syn		equ	r_flage.1
	r_on_off_ok	equ	r_flage.2
	r_17		equ	r_flage.3
	r_key_up	equ	r_flage.4
	r_sleep_ok	equ	r_flage.5
	clock_open	equ	r_flage.6

	r_flage1	ds	1
	r_c2		equ	r_flage1.0
	r_pause		equ	r_flage1.1
	time_up		equ	r_flage1.2
	r_smooth	equ	r_flage1.3
	high_low	equ	r_flage1.4


	r_flage2	ds	1
	turn		equ	r_flage2.0
	turn1		equ	r_flage2.1
	turn2		equ	r_flage2.2	
	turn3		equ	r_flage2.3
	turn4		equ	r_flage2.4

	r_acc		ds	1
	r_acc4		ds	1
	r_acc5		ds	1
	reg0		ds	1
	reg1		ds	1
	num_main	ds	1
	mode		ds	1
	mode1		ds	1
	mode2		ds	1
	num_rgb		ds	1
	rgb_value	ds	1
	rx_mode		ds	1
	rx_time		ds	1
	rx_time1	ds	1
	rx_bit		ds	1
	rx_data1	ds	1
	rx_data2	ds	1
	rx_data3	ds	1
	rx_data4	ds	1

	pwm_num1	ds	1
	pwm_num2	ds	1
	pwm_num3	ds	1
	pwm_num4	ds	1

	on_num		ds	1
	on_num1		ds	1

	r_cc		ds	1

	count		ds	1
	count1		ds	1
	count2		ds	1
	count3		ds	1

/*I/O口定义*/
	d1		equ	p2.2	;O  对应原理图上的灯
	d2		equ	p2.1	;O	
	d3		equ	p2.0	;O
	d4		equ	p1.2	;O

	d5		equ	p5.4	;O
	d6		equ	p1.0	;O
	d7		equ	p0.0	;O
	d8		equ	p2.5	;O

	d9		equ	p2.4	;O
	
.code 
	org	00h ;指定一个地址,后面的程序或数据从这个地址值开始分配
	jmp	reset
	ORG	08H
	JMP	int_service ;无条件跳转指令

	org	10h

//===============================================================================================
//    T0 set 10ms interval time for T0 interrupt.   X'tal:4M  Fcpu=Fosc/4  T0RATE=010 (Fcpu/64) 
//===============================================================================================
//
//	T0 initial value = 256-(T0 interrupt interval time*clocl)
//			 = 256-(10ms*4MHz /4 /64)
//			 = 256-(0.01*4*1000000/4/64)
//			 = 100
//			 = 64H
//
int_service:
	push					; 保存 ACC 和 PFLAG
isr_adc:
	b0bts1	ft0ien				// Check t0ien
	jmp	isr90				
	b0bts1	ft0irq				// Check t0irp
	jmp	isr90			
	b0bclr	ft0irq
	b0bset	r_100us
	mov	a,#0fah				//96us	interrupt Once 	0fah				
	b0mov	t0c,a

isr90:
	pop
	reti					; 退出中断
RESET:
	MOV	A,#7FH				; A = 7FH，RAM 区的最后单元
	MOV	STKP,A				;STKP为堆栈指针
	CALL	SYS_INIT
	call	io_init
	nop
	nop
	nop

;14(P2.3)脚对应1,2对应8.9.10.12(P2.5)，3对应1(P2.2)236
main_loop:
	mov	a,#5ah		//清看门狗-初始化
	b0mov	wdtr,a
	b0bts1	r_100us //延时100微秒
	jmp	main_loop
	b0bclr	r_100us

	incms	num_main	;跳转
	mov	a,#46         ;主函数(移动的时间原51，用24比较理想)
	sub	a,num_main	  ;a->acc
	b0bts0	fc			;跳转
	jmp	main_loop
	clr	num_main

	;call	display_mode0	;显示模式0:左右闪
	;call	display_mode1	;显示模式1:独立闪
	call	display_mode2	;28秒
	jmp	main_loop

	org	050h

display_mode2:
	mov	a,mode2
	b0add	pcl,a
	jmp     mode_3
	ret

mode_3:
	b0bts0	turn;

	jmp mode_3_1
	jmp mode_3_2
	jmp mode_3_3
	jmp mode_3_4
	ret
	
mode_3_1:;全部灯一闪一闪的次数3
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8
    incms	pwm_num2;pwm_num2全灭24
	mov	a,#24            
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bclr	turn

	incms	count
	mov	a,#3         ;一闪一闪3次
	sub	a,count
	b0bts0	fc
	ret
	clr	count
	bclr	turn
	bclr	turn1

	incms	count1
	mov	a,#7        ;延长时间7
	sub	a,count1
	b0bts0	fc
    ret
	clr	count1
    ret

mode_3_2:;中间部分灯快速闪3次
	bset	d5;亮下面4个
	bset	d6
	bset	d3
	bset	d4
	incms	pwm_num1;pwm_num1亮右灯49
	mov	a,#49          
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bset	turn
    b0bts0	turn
	;jmp	mode_0_1;先全灭 然后闪3下
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8
	incms	pwm_num2
	mov	a,#24       ;全灭灯时间长度(D1，D2，D3，D4，D5，D6，D7，D8)原24
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bclr	turn
    ret
    
mode_3_3:;左右交替闪
	jmp mode_0
    ret
    
mode_3_4:;奇数位快速闪3次
	bclr	d1
	;bclr	d2
	bclr	d3
	;bclr	d4
	bclr	d5
	;bclr	d6
	bclr	d7
	;bclr	d8
    incms	pwm_num2;pwm_num2全灭24
	mov	a,#24            
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bclr	turn

	incms	count
	mov	a,#3         ;一闪一闪3次
	sub	a,count
	b0bts0	fc
	ret
	clr	count
	bclr	turn
	bclr	turn1

	incms	count1
	mov	a,#7        ;延长时间7
	sub	a,count1
	b0bts0	fc
    ret
	clr	count1
    
	mov	a,#4          ;跳下个模式
	mov	mode,a	
	ret

;左右闪
display_mode0:
	mov	a,mode
	b0add	pcl,a ;pcl跳转表里的一个值-常量
	jmp	mode_0	;快速闪 滴滴滴滴 滴滴滴滴
	jmp	mode_1  ;快速闪1秒 接着跳动闪 [左右闪切换至单边闪:快速灭然后一闪一闪14;然后全亮]
	jmp	mode_2  ;一直跳动 中闪[上面4个灭后到下面4个灭 一闪一闪12]
	ret


;独立闪
display_mode1:	
	mov	a,mode1
	b0add	pcl,a
	jmp	dp_mode_0;d9灭
	jmp	dp_mode_1;d9亮
	ret

;
mode_0:                          
	b0bts0	turn1;P2.1对应的灯 状态标志
	jmp	mode_0_2;先全灭 然后闪3 再 亮下面4个
	b0bts0	turn;位置1或0
	jmp	mode_0_1;先全灭 然后闪3下

	bset	d1 ;亮上面4个 清0或1 根据EFLAGS寄存器中的状态标识设置目标操作数的值为0或1
	bset	d2
	bset	d3
	bset	d4
	incms	pwm_num1;地址跳转
	mov	a,#49          ;左边亮灯时间长度(D1,D2,D3,D4)原49
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bset	turn	
	ret

mode_0_1:
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4

	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8

	incms	pwm_num2
	mov	a,#24;全灭灯时间长度(D1，D2，D3，D4，D5，D6，D7，D8)原24
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2

	bclr	turn

	incms	count
	mov	a,#3       ;一闪一闪的次数原3
	sub	a,count
	b0bts0	fc
	ret
	clr	count

	bclr	turn
	bset	turn1
	
	ret

mode_0_2:                        
	b0bts0	turn
	jmp	mode_0_3;全灭
	bset	d5;亮下面4个
	bset	d6
	bset	d7
	bset	d8
	incms	pwm_num1
	mov	a,#49          ;右亮灯时间（D5，D6，D7,D8）原49
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1

	bset	turn	
	ret

mode_0_3:
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4

	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8

	incms	pwm_num2
	mov	a,#24         ;全灭灯时间长度(D1，D2，D3，D4，D5，D6，D7，D8)原24   
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2

	bclr	turn

	incms	count
	mov	a,#3         ;一闪一闪的次数3
	sub	a,count
	b0bts0	fc
	ret
	clr	count
	bclr	turn
	bclr	turn1

	incms	count1
	mov	a,#7        ;延长时间7
	sub	a,count1
	b0bts0	fc
	ret
	clr	count1

	mov	a,#1      ;将#1放a 内存跳下一模式
	mov	mode,a
	ret

mode_1:
	b0bts0	turn
	jmp	mode_1_1

	bset	d1;全亮
	bset	d2
	bset	d3
	bset	d4

	bset	d5
	bset	d6
	bset	d7
	bset	d8

	incms	pwm_num1
	mov	a,#24      ;亮灯时间长度(D1，D2，D3，D4，D5，D6，D7，D8)原49
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1

	bset	turn	
	ret

;
mode_2:                      
	b0bts0	turn;非turn都长亮
	jmp	mode_2_1

	bset	d1
	bset	d2
	bset	d3
	bset	d4

	incms	pwm_num1
	mov	a,#49            ;亮灯时间长度(D1,D2,D3,D4)原49         
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1

	bset	turn	
	ret

;
dp_mode_0:
	b0bts0	turn3
	jmp	dp_mode_01

	incms	pwm_num3 	;跳转指令
	mov	a,#122
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3

	bset	d9       
	bset	turn3	
	ret

dp_mode_01:
	incms	pwm_num3
	mov	a,#122
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3

	bclr	d9
	bclr	turn3

	incms	count2
	mov	a,#3
	sub	a,count2
	b0bts0	fc
	ret
	clr	count2

	incms	mode1
	ret			

dp_mode_1:
	b0bts0	turn3
	jmp	dp_mode_11

	incms	pwm_num3
	mov	a,#18
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3

	bset	d9            ;亮灯时间（单闪）
	bset	turn3	
	ret

dp_mode_11:
	incms	pwm_num3
	mov	a,#18
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3

	bclr	d9           ;灭灯时间（单闪）
	bclr	turn3

	incms	count2
	mov	a,#31         
	sub	a,count2
	b0bts0	fc
	ret
	clr	count2

	clr	mode1
	ret

mode_1_1:
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8

	incms	pwm_num2
	mov	a,#10       ;灭灯时间长度(D1，D2，D3，D4，D5，D6，D7，D8)原24
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2

	bclr	turn

	incms	count
   	mov	a,#18       ;一闪一说的次数原14
	sub	a,count
	b0bts0	fc
	ret
	clr	count

	bclr	turn
	bclr	turn1

	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8
	
	mov	a,#2          ;跳下个模式
	mov	mode,a	
	ret

mode_2_1:
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4

	incms	pwm_num1
	mov	a,#100             ;灭灯时间（D1，D2，D3,D4） 数字越大灭灯时间越长 相当于调节延时     
	sub	a,pwm_num1
	b0bts0	fc
	jmp	mode_2_2
	clr	pwm_num1

	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8

	bclr	turn
	bclr	turn2

	incms	count
	mov	a,#12              ;一闪一闪的次数原7
	sub	a,count
	b0bts0	fc
	ret
	clr	count

	clr	mode
	ret

mode_2_2:
	b0bts0	turn2
	ret
	b0bts0	turn1
	jmp	mode_2_3

	incms	pwm_num2      ;(自加1)
	mov	a,#24
	sub	a,pwm_num2    ;(减数)
	b0bts0	fc            ;FC=0跳下2句，若FC=1跳ret
	ret
	clr	pwm_num2

	bset	d5
	bset	d6
	bset	d7
	bset	d8
	bset	turn1
	ret

mode_2_3:
	incms	pwm_num2
	mov	a,#49
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2

	bclr	d5 //亮/灭
	bclr	d6
	bclr	d7
	bclr	d8

	bclr	turn1
	bset	turn2
	ret

dlay_50ms_2:
	mov	a,#1
	mov	r_acc,a

dlay_50ms_3:
	mov	a,#5ah		//清看门狗
	b0mov	wdtr,a
	decms	r_acc
	jmp	dlay_50ms_3
	decms	reg1
	jmp	dlay_50ms_2
	ret

;=========================================清内存	
SYS_INIT:
	CLR	OSCM ;振荡器模式寄存器  系统时钟,寄存器 OSCM 控制振荡器的状态和系统的工作模式
	CLR	PFLAG ;ROM?页和特殊寄存器
	CLR	INTEN ;中断使能寄存器
	CLR	INTRQ	;中断使能寄存器

CLR_RAM:
	B0MOV	Y,#00H			;clear	bank0 ;Y 指向 RAM bank 0
	B0MOV	Z,#5FH			; Z 指向 55H

CLR_RAM10:
	CLR	@YZ
	DECMS	Z
	JMP	CLR_RAM10
	CLR	@YZ ;间接寻址寄存器
	ret

//=========================================初始化IO口
io_init:
	mov	a,#11110111b		;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p2m,a
	mov	a,#00000000b		;上啦  1为上啦	0为禁止
	b0mov	p2ur,a

	mov	a,#11111101b		;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p1m,a
	mov	a,#00000000b		;上啦  1为上啦	0为禁止
	b0mov	p1ur,a

	mov	a,#11111111b		;0为输入 1为输出  
	b0mov	p5m,a
	mov	a,#11111111b		;0为输入 1为输出   所有端口设为输出模式
	b0mov	p0m,a

sleep_init:
	mov	a,#11110111b		;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p2m,a
	mov	a,#00000000b		;上啦  1为上啦	0为禁止
	b0mov	p2ur,a

	mov	a,#11111101b		;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p1m,a
	mov	a,#00000000b		;上啦  1为上啦	0为禁止
	b0mov	p1ur,a

	mov	a,#11111111b		;0为输入 1为输出  
	b0mov	p5m,a
	mov	a,#11111111b		;0为输入 1为输出  
	b0mov	p0m,a

	b0bclr	ft0enb
						
	mov	a,#30h	//Fcpu/32	#提示后面是立即数
	b0mov	t0m,a								
	mov	a,#64h	//10ms	interrupt Once 					
	b0mov	t0c,a					
							
	b0bset	ft0enb	// Enable T0 timer

	b0bset	ft0ien  // Enable T0 interrupt 使能T0中断
	b0bclr	ft0irq	// Clean T0 interrupt request flag

	b0bset	fgie	// all interrupt operation  全局中断控制位 1使能


	ret
endp