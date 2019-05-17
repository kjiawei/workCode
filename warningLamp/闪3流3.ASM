chip	sn8p2501D
//{{SONIX_CODE_OPTION
	.Code_Option	LVD		LVD_M		; 2.0V Reset Enable LVD24 bit of PFLAG for 2.4V Low Voltage Indicator
	.Code_Option	Reset_Pin	P11
	.Code_Option	Watch_Dog	Enable		; Normal mode: Enable Watchdog Green and Sleep mode: Stop Watchdog
	.Code_Option	High_Clk	IHRC_16M	; Internal 16M RC Oscillator
	.Code_Option	Fcpu		#1     ; Fcpu = Fosc/2
	.Code_Option	Security	Enable
	.Code_Option	Noise_Filter	Disable
//}}SONIX_CODE_OPTION


.data
	r_flage		ds	1
	r_100us		equ	r_flage.0
	r_syn		equ	r_flage.1
	r_on_off_ok	equ	r_flage.2
	r_17		equ	r_flage.3
	r_key_up	equ	r_flage.4
	r_sleep_ok	equ	r_flage.5
	turn7		equ	r_flage.6
	r_flage1	ds	1
	r_c2		equ	r_flage1.0
	turn		equ	r_flage1.1
	turn1		equ	r_flage1.2
	turn2		equ	r_flage1.3
	turn3		equ	r_flage1.4
	turn4		equ	r_flage1.5
	turn5		equ	r_flage1.6
	turn6		equ	r_flage1.7
	r_acc		ds	1
	r_acc4		ds	1
	r_acc5		ds	1
	reg0		ds	1
	reg1		ds	1
	num_main	ds	1
	mode		ds	1
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
	mode_inc	ds	1
	mode_del	ds	1
	mode_del1	ds	1	
	recoder		ds	1
	num_key		ds	1
	on_num		ds	1
	on_num1		ds	1

	pwm_num		ds	1	
	pwm_num1	ds	1
	pwm_num2	ds	1
	pwm_num3	ds	1
	pwm_num4	ds	1
	pwm_num5	ds	1
	pwm_num6	ds	1
	pwm_num7	ds	1

	r_cc		ds	1

	count		ds	1
	count1		ds	1
	count2		ds	1
//	on_key		equ	p1.1

	out1		equ	p1.3
	out2		equ	p1.2
	out3		equ	p5.4
	out4		equ	p1.0
    out5		equ	p2.2
    out6		equ	p2.1
    out7		equ	p2.0
    out8		equ	p2.4
    out9		equ	p2.3
	
.code 
	org	00h
	jmp	reset
	ORG	08H
	JMP	int_service

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
	push
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
	reti
RESET:
	MOV	A,#7FH
	MOV	STKP,A
	CALL	SYS_INIT
	call	io_init
	nop
	nop
	nop

main_loop:
	mov	a,#5ah		//清看门口
	b0mov	wdtr,a
	b0bts1	r_100us
	jmp	main_loop
	b0bclr	r_100us

	incms	num_main
	mov	a,#51
	sub	a,num_main
	b0bts0	fc
	jmp	main_loop
	clr	num_main
	call	mode_10
	call	mode_12
	jmp	main_loop
	org	050h
    
display_mode:
	mov	a,mode
	b0add	pcl,a
	jmp	mode_0
	jmp	mode_2
	ret

mode_10:
	b0bts0	turn2
	jmp	mode_0_2
	b0bts0	turn1
	jmp	mode_0_4
	b0bts0	turn
	jmp	mode_0_1
	incms	pwm_num1
	mov	a,#23
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bclr	out4
	bset	turn	
	ret
    
mode_0:
	b0bts0	turn2
	jmp	mode_0_2
	b0bts0	turn1
	jmp	mode_0_4
	b0bts0	turn
	jmp	mode_0_1
	incms	pwm_num1
	mov	a,#23
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bclr	out4;pwm使能位 拉低 关闭
	bset	turn;某种模式标志位	
	ret
    
mode_0_1:
	incms	pwm_num2
	mov	a,#15
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bset	out4
	bclr	turn

	incms	count
	mov	a,#31
	sub	a,count
	b0bts0	fc
	ret
	clr	count

	bset	turn1	
	ret

mode_0_2:;turn2==0
	b0bts0	turn;turn没set过,此时=0,所以进入0_3
	jmp	mode_0_3
	incms	pwm_num1
	mov	a,#120
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bclr	out4
	bset	turn	
	ret
    
mode_0_3:
	incms	pwm_num2
	mov	a,#120
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
    
	incms	count
	mov	a,#3;闪3次
	sub	a,count
	b0bts1	fc
	jmp	mode_0_3_1	
	bset	out4;P5.4 输出 拉高
	bclr	turn;?
	ret
    
mode_0_3_1:
	clr	count
	bclr	turn
	bclr	turn2
	ret

mode_0_4:
	incms	pwm_num2
	mov	a,#70
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bset	out4
	bclr	turn
	bclr	turn1
	bset	turn2
	ret

mode_2:
	b0bts0	turn6
	jmp	mode_1
	b0bts0	turn7
	jmp	mode_2_1
	bset	out1
	bset	out2
	bset	out3
	incms	pwm_num3
	mov	a,#91
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3
	bset	turn7
	ret

mode_2_1:
	bclr	out1
	bclr	out2
	bclr	out3

	incms	pwm_num3
	mov	a,#91
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3
	bclr	turn7

	incms	count1
	mov	a,#5
	sub	a,count1
	b0bts0	fc
	ret
	clr	count1
	bset	turn6
	ret

mode_1:
	b0bts0	turn5
	jmp	mode_1_4

	b0bts0	turn4
	jmp	mode_1_2

	b0bts0	turn3
	jmp	mode_1_1

	bset	out1
	incms	pwm_num3
	mov	a,#28
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3
	bset	turn3	
	ret
    
mode_1_1:
	bclr	out1
	incms	pwm_num4
	mov	a,#28
	sub	a,pwm_num4
	b0bts0	fc
	ret
	clr	pwm_num4
	bclr	turn3
	incms	count1
	mov	a,#4
	sub	a,count1
	b0bts0	fc
	ret
	clr	count1
	bset	turn4	
	ret
    
mode_1_2:
	bclr	out1
	b0bts0	turn3
	jmp	mode_1_3
	bset	out2
	incms	pwm_num3
	mov	a,#28
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3
	bset	turn3	
	ret
    
mode_1_3:
	bclr	out2
	incms	pwm_num4
	mov	a,#28
	sub	a,pwm_num4
	b0bts0	fc
	ret
	clr	pwm_num4
	bclr	turn3
	incms	count1
	mov	a,#4
	sub	a,count1
	b0bts0	fc
	ret
	clr	count1
	bset	turn5
	ret

mode_1_4:
	b0bts0	turn3
	jmp	mode_1_5
	bset	out3
	incms	pwm_num3
	mov	a,#28
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3
	bset	turn3	
	ret
    
mode_1_5:
	bclr	out3
	incms	pwm_num4
	mov	a,#28
	sub	a,pwm_num4
	b0bts0	fc
	ret
	clr	pwm_num4
	bclr	turn3
	incms	count1
	mov	a,#4
	sub	a,count1
	b0bts0	fc
	ret
	clr	count1
	bclr	turn3
	bclr	turn4
	bclr	turn5
	incms	count2
	mov	a,#2
	sub	a,count2
	b0bts0	fc
	ret
	clr	count2
	bclr	turn6
	ret

dlay_50ms_2:
	mov	a,#1
	mov	r_acc,a
    
dlay_50ms_3:
	mov	a,#5ah		//清看门口
	b0mov	wdtr,a
	decms	r_acc
	jmp	dlay_50ms_3
	decms	reg1
	jmp	dlay_50ms_2
	ret
;=========================================清内存	
SYS_INIT:
	CLR	OSCM
	CLR	PFLAG
	CLR	INTEN
	CLR	INTRQ	
CLR_RAM:
	B0MOV	Y,#00H			;clear	bank0
	B0MOV	Z,#5FH	
CLR_RAM10:
	CLR	@YZ
	DECMS	Z
	JMP	CLR_RAM10
	CLR	@YZ
	ret
//=========================================初始化IO口
io_init:
	mov	a,#11111101b			;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p1m,a
	mov	a,#00000000b		;上啦  1为上啦	0为禁止
	b0mov	p1ur,a

	mov	a,#11111111b		;0为输入 1为输出  
	b0mov	p5m,a
	mov	a,#11111110b		;0为输入 1为输出  
	b0mov	p0m,a
	bset	out4

sleep_init:
	mov	a,#11111101b			;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p1m,a
	mov	a,#00000000b		;上啦  1为上啦	0为禁止
	b0mov	p1ur,a

	mov	a,#11111111b		;0为输入 1为输出  
	b0mov	p5m,a
	mov	a,#11111110b		;0为输入 1为输出  
	b0mov	p0m,a

	b0bclr	ft0enb
						
	mov	a,#30h	//Fcpu/32	
	b0mov	t0m,a								
	mov	a,#64h	//10ms	interrupt Once 					
	b0mov	t0c,a					
							
	b0bset	ft0enb	// Enable T0 timer

	b0bset	ft0ien  // Enable T0 interrupt
	b0bclr	ft0irq	// Clean T0 interrupt request flag

	b0bset	fgie	// all interrupt operation


	ret
endp