;#include  "sn8p2501d.h";INCLUDE<SN8P2501D>   INCLUDE 'DOSMACS.ASM'
chip	sn8p2501D
//{{SONIX_CODE_OPTION   ϵͳ��Ӳ�����ã������������ͣ����Ź���ʱ���Ĳ�����LVD ѡ���λ����
//ѡ���Լ� OTP ROM �İ�ȫ����
	.Code_Option	LVD		LVD_M		; VDD ���� 2.0V ʱ��LVD ��λϵͳ��PFLAG �Ĵ����� LVD24 λ��Ϊ 2.4V �͵�ѹ����� 2.0V Reset Enable LVD24 bit of PFLAG for 2.4V Low Voltage Indicator
	.Code_Option	Reset_Pin	P11		;P1.1 Ϊ�����������ţ�����������
	.Code_Option	Watch_Dog	Enable		; Normal mode: Enable Watchdog Green and Sleep mode: Stop Watchdog
	.Code_Option	High_Clk	IHRC_16M	; ����ʱ�Ӳ����ڲ� 16MHz RC �񵴵�·��XIN/XOUT Ϊ GPIO ���š� Internal 16M RC Oscillator
	.Code_Option	Fcpu		#1     ; Fcpu = Fosc/2 ָ������ = 2 ��ʱ������
	.Code_Option	Security	Enable ;ROM �������
	.Code_Option	Noise_Filter	Disable ;�ر���Ѷ�˲����ܣ�Fcpu = Fosc/1~Fosc/16
//}}SONIX_CODE_OPTION


.data ;��ʼ�����ݶ�
	r_flage		ds	1
	r_100us		equ	r_flage.0  ;�ȼ� #define ����
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

/*I/O�ڶ���*/
	d1		equ	p2.2	;O  ��Ӧԭ��ͼ�ϵĵ�
	d2		equ	p2.1	;O	
	d3		equ	p2.0	;O
	d4		equ	p1.2	;O

	d5		equ	p5.4	;O
	d6		equ	p1.0	;O
	d7		equ	p0.0	;O
	d8		equ	p2.5	;O

	d9		equ	p2.4	;O
	
.code 
	org	00h ;ָ��һ����ַ,����ĳ�������ݴ������ֵַ��ʼ����
	jmp	reset
	ORG	08H
	JMP	int_service ;��������תָ��

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
	push					; ���� ACC �� PFLAG
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
	reti					; �˳��ж�
RESET:
	MOV	A,#7FH				; A = 7FH��RAM �������Ԫ
	MOV	STKP,A				;STKPΪ��ջָ��
	CALL	SYS_INIT
	call	io_init
	nop
	nop
	nop

;14(P2.3)�Ŷ�Ӧ1,2��Ӧ8.9.10.12(P2.5)��3��Ӧ1(P2.2)236
main_loop:
	mov	a,#5ah		//�忴�Ź�-��ʼ��
	b0mov	wdtr,a
	b0bts1	r_100us //��ʱ100΢��
	jmp	main_loop
	b0bclr	r_100us

	incms	num_main	;��ת
	mov	a,#46         ;������(�ƶ���ʱ��ԭ51����24�Ƚ�����)
	sub	a,num_main	  ;a->acc
	b0bts0	fc			;��ת
	jmp	main_loop
	clr	num_main

	;call	display_mode0	;��ʾģʽ0:������
	;call	display_mode1	;��ʾģʽ1:������
	call	display_mode2	;28��
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
	
mode_3_1:;ȫ����һ��һ���Ĵ���3
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8
    incms	pwm_num2;pwm_num2ȫ��24
	mov	a,#24            
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bclr	turn

	incms	count
	mov	a,#3         ;һ��һ��3��
	sub	a,count
	b0bts0	fc
	ret
	clr	count
	bclr	turn
	bclr	turn1

	incms	count1
	mov	a,#7        ;�ӳ�ʱ��7
	sub	a,count1
	b0bts0	fc
    ret
	clr	count1
    ret

mode_3_2:;�м䲿�ֵƿ�����3��
	bset	d5;������4��
	bset	d6
	bset	d3
	bset	d4
	incms	pwm_num1;pwm_num1���ҵ�49
	mov	a,#49          
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bset	turn
    b0bts0	turn
	;jmp	mode_0_1;��ȫ�� Ȼ����3��
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8
	incms	pwm_num2
	mov	a,#24       ;ȫ���ʱ�䳤��(D1��D2��D3��D4��D5��D6��D7��D8)ԭ24
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bclr	turn
    ret
    
mode_3_3:;���ҽ�����
	jmp mode_0
    ret
    
mode_3_4:;����λ������3��
	bclr	d1
	;bclr	d2
	bclr	d3
	;bclr	d4
	bclr	d5
	;bclr	d6
	bclr	d7
	;bclr	d8
    incms	pwm_num2;pwm_num2ȫ��24
	mov	a,#24            
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bclr	turn

	incms	count
	mov	a,#3         ;һ��һ��3��
	sub	a,count
	b0bts0	fc
	ret
	clr	count
	bclr	turn
	bclr	turn1

	incms	count1
	mov	a,#7        ;�ӳ�ʱ��7
	sub	a,count1
	b0bts0	fc
    ret
	clr	count1
    
	mov	a,#4          ;���¸�ģʽ
	mov	mode,a	
	ret

;������
display_mode0:
	mov	a,mode
	b0add	pcl,a ;pcl��ת�����һ��ֵ-����
	jmp	mode_0	;������ �εεε� �εεε�
	jmp	mode_1  ;������1�� ���������� [�������л���������:������Ȼ��һ��һ��14;Ȼ��ȫ��]
	jmp	mode_2  ;һֱ���� ����[����4���������4���� һ��һ��12]
	ret


;������
display_mode1:	
	mov	a,mode1
	b0add	pcl,a
	jmp	dp_mode_0;d9��
	jmp	dp_mode_1;d9��
	ret

;
mode_0:                          
	b0bts0	turn1;P2.1��Ӧ�ĵ� ״̬��־
	jmp	mode_0_2;��ȫ�� Ȼ����3 �� ������4��
	b0bts0	turn;λ��1��0
	jmp	mode_0_1;��ȫ�� Ȼ����3��

	bset	d1 ;������4�� ��0��1 ����EFLAGS�Ĵ����е�״̬��ʶ����Ŀ���������ֵΪ0��1
	bset	d2
	bset	d3
	bset	d4
	incms	pwm_num1;��ַ��ת
	mov	a,#49          ;�������ʱ�䳤��(D1,D2,D3,D4)ԭ49
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
	mov	a,#24;ȫ���ʱ�䳤��(D1��D2��D3��D4��D5��D6��D7��D8)ԭ24
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2

	bclr	turn

	incms	count
	mov	a,#3       ;һ��һ���Ĵ���ԭ3
	sub	a,count
	b0bts0	fc
	ret
	clr	count

	bclr	turn
	bset	turn1
	
	ret

mode_0_2:                        
	b0bts0	turn
	jmp	mode_0_3;ȫ��
	bset	d5;������4��
	bset	d6
	bset	d7
	bset	d8
	incms	pwm_num1
	mov	a,#49          ;������ʱ�䣨D5��D6��D7,D8��ԭ49
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
	mov	a,#24         ;ȫ���ʱ�䳤��(D1��D2��D3��D4��D5��D6��D7��D8)ԭ24   
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2

	bclr	turn

	incms	count
	mov	a,#3         ;һ��һ���Ĵ���3
	sub	a,count
	b0bts0	fc
	ret
	clr	count
	bclr	turn
	bclr	turn1

	incms	count1
	mov	a,#7        ;�ӳ�ʱ��7
	sub	a,count1
	b0bts0	fc
	ret
	clr	count1

	mov	a,#1      ;��#1��a �ڴ�����һģʽ
	mov	mode,a
	ret

mode_1:
	b0bts0	turn
	jmp	mode_1_1

	bset	d1;ȫ��
	bset	d2
	bset	d3
	bset	d4

	bset	d5
	bset	d6
	bset	d7
	bset	d8

	incms	pwm_num1
	mov	a,#24      ;����ʱ�䳤��(D1��D2��D3��D4��D5��D6��D7��D8)ԭ49
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1

	bset	turn	
	ret

;
mode_2:                      
	b0bts0	turn;��turn������
	jmp	mode_2_1

	bset	d1
	bset	d2
	bset	d3
	bset	d4

	incms	pwm_num1
	mov	a,#49            ;����ʱ�䳤��(D1,D2,D3,D4)ԭ49         
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

	incms	pwm_num3 	;��תָ��
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

	bset	d9            ;����ʱ�䣨������
	bset	turn3	
	ret

dp_mode_11:
	incms	pwm_num3
	mov	a,#18
	sub	a,pwm_num3
	b0bts0	fc
	ret
	clr	pwm_num3

	bclr	d9           ;���ʱ�䣨������
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
	mov	a,#10       ;���ʱ�䳤��(D1��D2��D3��D4��D5��D6��D7��D8)ԭ24
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2

	bclr	turn

	incms	count
   	mov	a,#18       ;һ��һ˵�Ĵ���ԭ14
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
	
	mov	a,#2          ;���¸�ģʽ
	mov	mode,a	
	ret

mode_2_1:
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4

	incms	pwm_num1
	mov	a,#100             ;���ʱ�䣨D1��D2��D3,D4�� ����Խ�����ʱ��Խ�� �൱�ڵ�����ʱ     
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
	mov	a,#12              ;һ��һ���Ĵ���ԭ7
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

	incms	pwm_num2      ;(�Լ�1)
	mov	a,#24
	sub	a,pwm_num2    ;(����)
	b0bts0	fc            ;FC=0����2�䣬��FC=1��ret
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

	bclr	d5 //��/��
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
	mov	a,#5ah		//�忴�Ź�
	b0mov	wdtr,a
	decms	r_acc
	jmp	dlay_50ms_3
	decms	reg1
	jmp	dlay_50ms_2
	ret

;=========================================���ڴ�	
SYS_INIT:
	CLR	OSCM ;����ģʽ�Ĵ���  ϵͳʱ��,�Ĵ��� OSCM ����������״̬��ϵͳ�Ĺ���ģʽ
	CLR	PFLAG ;ROM?ҳ������Ĵ���
	CLR	INTEN ;�ж�ʹ�ܼĴ���
	CLR	INTRQ	;�ж�ʹ�ܼĴ���

CLR_RAM:
	B0MOV	Y,#00H			;clear	bank0 ;Y ָ�� RAM bank 0
	B0MOV	Z,#5FH			; Z ָ�� 55H

CLR_RAM10:
	CLR	@YZ
	DECMS	Z
	JMP	CLR_RAM10
	CLR	@YZ ;���Ѱַ�Ĵ���
	ret

//=========================================��ʼ��IO��
io_init:
	mov	a,#11110111b		;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p2m,a
	mov	a,#00000000b		;����  1Ϊ����	0Ϊ��ֹ
	b0mov	p2ur,a

	mov	a,#11111101b		;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p1m,a
	mov	a,#00000000b		;����  1Ϊ����	0Ϊ��ֹ
	b0mov	p1ur,a

	mov	a,#11111111b		;0Ϊ���� 1Ϊ���  
	b0mov	p5m,a
	mov	a,#11111111b		;0Ϊ���� 1Ϊ���   ���ж˿���Ϊ���ģʽ
	b0mov	p0m,a

sleep_init:
	mov	a,#11110111b		;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p2m,a
	mov	a,#00000000b		;����  1Ϊ����	0Ϊ��ֹ
	b0mov	p2ur,a

	mov	a,#11111101b		;r=0.5	g=0.7	b=0.6	w=0.4
	b0mov	p1m,a
	mov	a,#00000000b		;����  1Ϊ����	0Ϊ��ֹ
	b0mov	p1ur,a

	mov	a,#11111111b		;0Ϊ���� 1Ϊ���  
	b0mov	p5m,a
	mov	a,#11111111b		;0Ϊ���� 1Ϊ���  
	b0mov	p0m,a

	b0bclr	ft0enb
						
	mov	a,#30h	//Fcpu/32	#��ʾ������������
	b0mov	t0m,a								
	mov	a,#64h	//10ms	interrupt Once 					
	b0mov	t0c,a					
							
	b0bset	ft0enb	// Enable T0 timer

	b0bset	ft0ien  // Enable T0 interrupt ʹ��T0�ж�
	b0bclr	ft0irq	// Clean T0 interrupt request flag

	b0bset	fgie	// all interrupt operation  ȫ���жϿ���λ 1ʹ��


	ret
endp