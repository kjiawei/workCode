;现象对应汇编 100毫秒内

;mode_0:                          
	b0bts0	turn1;P2.1对应的灯 状态标志
	;jmp	mode_0_2;先全灭 然后闪3 再 亮下面4个
	b0bts0	turn
	;jmp	mode_0_3;全灭
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
	mov	a,#1      ;将#1放a 内存跳下一模式
	mov	mode,a
	;
	bset	d5;亮下面4个
	bset	d6
	bset	d7
	bset	d8
	incms	pwm_num1;pwm_num1亮右灯49
	mov	a,#49          
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bset	turn
	;
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
	incms	count
	mov	a,#3       ;pwm_num2亮左灯 一闪一闪3次
	sub	a,count
	b0bts0	fc
	ret
	clr	count
	bclr	turn
	bset	turn1
	;
	bset	d1 ;亮左灯 清0或1 根据EFLAGS寄存器中的状态标识设置目标操作数的值为0或1
	bset	d2
	bset	d3
	bset	d4
	incms	pwm_num1;pwm_num1亮左灯49
	mov	a,#49          
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bset	turn	
;ret

;mode_1:
	b0bts0	turn
	;jmp	mode_1_1
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8
	incms	pwm_num2 ;pwm_num2灭灯24使用模拟pwm让灯快速亮10毫秒
	mov	a,#10       
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2
	bclr	turn

	incms	count	;开始一亮一灭14
   	mov	a,#18       
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
	
	bset	d1;全亮
	bset	d2
	bset	d3
	bset	d4
	bset	d5
	bset	d6
	bset	d7
	bset	d8
	incms	pwm_num1;亮灯49
	mov	a,#24      
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bset	turn	
;ret

;mode_2:                      
	b0bts0	turn
	;jmp	mode_2_1
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4

	incms	pwm_num1
	mov	a,#100             ;灭左灯时间100（D1，D2，D3,D4） 数字越大灭灯时间越长 相当于调节延时     
	sub	a,pwm_num1
	b0bts0	fc
	;jmp	mode_2_2
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
	;
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
	;

	bset	d1
	bset	d2
	bset	d3
	bset	d4
	incms	pwm_num1
	mov	a,#49            ;亮左灯49         
	sub	a,pwm_num1
	b0bts0	fc
	ret
	clr	pwm_num1
	bset	turn	
;ret
