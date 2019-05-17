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
	dp_mode_f	equ	r_flage.1

	reg0		ds	1
	reg1		ds	1
	reg2		ds	1
	num_main	ds	1
	num_main1	ds	1
	num_main2	ds	1
	mode		ds	1
	mode1		ds	1

	pwm_num0	ds	1

	pwm_num1	ds	1
	pwm_num2	ds	1
	pwm_num3	ds	1
	pwm_num4	ds	1

	count		ds	1
	count3		ds	1
	count31		ds	1

/*I/O口定义*/
	d1		equ	p2.2	;O
	d2		equ	p2.1	;O	
	d3		equ	p2.0	;O
	d4		equ	p1.3	;O
	d5		equ	p1.2	;O
	d6		equ	p5.4	;O
	d7		equ	p1.0	;O
	d8		equ	p0.0	;O
	d9		equ	p2.5	;O
	d10		equ	p2.4	;O
	d11		equ	p2.3	;O
	select		equ	p1.1	;I

	out1		equ	p1.3
	out2		equ	p1.2
	out3		equ	p5.4
	out4		equ	p1.0
	r_flage1	ds	1
	turn		equ	r_flage1.1
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
	mov	a,#100
	sub	a,num_main
	b0bts0	fc
	jmp	main_loop
	clr	num_main

	incms	num_main1
	mov	a,#10
	sub	a,num_main1
	b0bts0	fc
	jmp	main_loop
	clr	num_main1

	b0bts1	select
	jmp	main_loop00

	call	LED_C
	call	dlay_10ms
call	mode_0_1_1
	call	display_mode0_1
	bclr	dp_mode_f
	
	;incms   count31
	;mov     a,#3
	;sub     a,count31;	bobts0  fc

	

	;incms   count3
	;mov     a,#3
	;sub     a,count3
;	bobts0  fc

	;clr	count3
	;clr	count31
	jmp	main_loop

main_loop00:
	call	display_mode1
	clr	mode
	jmp	main_loop

	org	051h
;;;;;;;;;;;;;
display_mode0:
	mov	a,mode
	b0add	pcl,a
	jmp	mode_0
	jmp	mode_1
	jmp	mode_2
	jmp	mode_3
	jmp	mode_4
	jmp	mode_5
	jmp	mode_6
	jmp	mode_7
	jmp	mode_8
	jmp	mode_9
	jmp	mode_10
	jmp	mode_11
	jmp	mode_12
	jmp	mode_13
	jmp	mode_14
	jmp	mode_15
	ret

display_mode0_1:
	mov	a,mode
	b0add	pcl,a
	jmp	mode_0
	jmp	mode_1
	jmp	mode_2
	jmp	mode_3
	jmp	mode_4
	jmp	mode_5
	jmp	mode_6
	jmp	mode_7
	jmp	mode_8
	jmp	mode_9
	jmp	mode_15
	ret

mode_0_1_1:
	b0bts0	turn
	jmp	mode_0_1_1_1
	bset	out1
	bset	out2
	bset	out3
	bset	out4
	incms	pwm_num0
	mov	a,#180		;范围：0-255,调节亮灯的时间长度
	sub	a,pwm_num0
	b0bts0	fc
	ret
	clr	pwm_num0
	bset	turn
	ret

mode_0_1_1_1:
	bclr	out1
	bclr	out2
	bclr	out3
	bclr	out4
	incms	pwm_num0
	mov	a,#91		;范围：0-255,调节灭灯的时间长度
	sub	a,pwm_num0

	b0bts0	fc

incms count3
mov a,#3
sub a,count3
;bobts0 fc


	ret


clr count3

	clr	pwm_num0
	bclr	turn
	ret

;;;;;;;;;;;;;
display_mode1:	
	b0bts0	dp_mode_f            ;标志位
	jmp	display_mode10       ;函数
	bset	d1                   ;拉高点亮 
	bset	d2
	bset	d3
	bset	d4
	bset	d5
	bset	d6
	bset	d7
	bset	d8
	bset	d9			
	bset	d10
	bset	d11

	incms	count   
	mov	a,#1
	sub	a,count
	b0bts0	fc
	ret
	clr	count

	bset	dp_mode_f
	ret                

display_mode10:            ;函数
	call	LED_C	   ;
	incms	count      ;变量灭2次
	mov	a,#2
	sub	a,count
	b0bts0	fc         ;FC清0数据手册可查 
	ret
	clr	count
	bclr	dp_mode_f  ;清楚标志位
	ret                ;返回

;;;;;;;;;;;;;;;;;;;;;;;;;
mode_0:
	bset	d2
	incms	mode	
	ret

mode_1:
	bset	d3
	incms	mode	
	ret

mode_2:
	bset	d4
	incms	mode	
	ret

mode_3:
	bset	d5
	incms	mode	
	ret

mode_4:
	bset	d6
	incms	mode	
	ret
mode_5:
	bset	d7
	incms	mode	
	ret

mode_6:
	bset	d8
	incms	mode	
	ret

mode_7:
	bset	d9
	incms	mode	
	ret

mode_8:
	bset	d10
	incms	mode	
	ret

mode_9:
	bset	d9
	incms	mode	
	ret

mode_10:
	bset	d8
	incms	mode	
	ret

mode_11:
	bset	d7
	incms	mode	
	ret

mode_12:
	bset	d6
	incms	mode	
	ret

mode_13:
	bset	d5
	incms	mode	
	ret

mode_14:
	bset	d4
	incms	mode	
	ret

mode_15:
	bset	d3
	clr	mode	
	ret

LED_C:                    ;灭灯拉低
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8
	bclr	d9
	bclr	d10
	bclr	d11
	ret              ;返回语句到CALL LED-C

dlay_10ms:
	mov	a,#1
	mov	reg0,a
dlay_10ms_1:
	mov	a,#50
	mov	reg1,a
dlay_10ms_2:
	mov	a,#254
	mov	reg2,a
dlay_10ms_3:
	mov	a,#5ah		//清看门口
	b0mov	wdtr,a
	decms	reg2
	jmp	dlay_10ms_3
	decms	reg1
	jmp	dlay_10ms_2
	decms	reg0
	jmp	dlay_10ms_1
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
	mov	a,#11111111b		;r=0.5	g=0.7	b=0.6	w=0.4
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

sleep_init:
	mov	a,#11111111b		;r=0.5	g=0.7	b=0.6	w=0.4
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
						
	mov	a,#20h	//Fcpu/32	
	b0mov	t0m,a								
	mov	a,#64h	//10ms	interrupt Once 					
	b0mov	t0c,a					
							
	b0bset	ft0enb	// Enable T0 timer

	b0bset	ft0ien  // Enable T0 interrupt
	b0bclr	ft0irq	// Clean T0 interrupt request flag

	b0bset	fgie	// all interrupt operation

	ret
endp