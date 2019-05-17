;14(P2.3)脚对应1,2对应8.9.10.12(P2.5)，3对应1(P2.2)236
main_loop:
	mov	a,#5ah		//清看门狗-初始化
	b0mov	wdtr,a
	b0bts1	r_100us //延时100微秒
	jmp	main_loop
	b0bclr	r_100us

	incms	num_main	;声明/取数
	mov	a,#46         ;主函数(移动的时间原51，用24比较理想)
	sub	a,num_main	  ;a->acc
	b0bts0	fc			;跳转
	jmp	main_loop
	clr	num_main

	call	display_mode0	;显示模式0:左右闪 ;	call	display_mode1	;显示模式1:独立闪
	jmp	main_loop
	org	050h
	
;左右闪
display_mode0:
	mov	a,mode
	b0add	pcl,a ;pcl跳转表里的一个值-常量
	jmp	mode_0	;
	jmp	mode_1
	jmp	mode_2
	ret
	
;模式0 滴滴滴滴 滴滴滴滴
mode_0:                          
	b0bts0	turn1;P2.1对应的灯
	jmp	mode_0_2
	b0bts0	turn;位置1或0
	jmp	mode_0_1

	bset	d1 ;清0或1 根据EFLAGS寄存器中的状态标识设置目标操作数的值为0或1
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
	
;一直跳动 中闪
mode_2:                      
	b0bts0	turn
	jmp	mode_2_1

	bset	d1;灭 bclr亮
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
	
mode_1:;快速闪1秒 接着跳动闪
	b0bts0	turn
	jmp	mode_1_1

	bset	d1
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
	
;独立闪
display_mode1:	
	mov	a,mode1
	b0add	pcl,a
	jmp	dp_mode_0
	jmp	dp_mode_1
	ret
	
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