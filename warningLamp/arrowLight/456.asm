mode_0_1:;全灭
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
	jmp	mode_0_3;用pwm_num2全灭24毫秒
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

mode_0_3:;全灭
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

mode_1_1:
	bclr	d1
	bclr	d2
	bclr	d3
	bclr	d4
	bclr	d5
	bclr	d6
	bclr	d7
	bclr	d8

	incms	pwm_num2 ;使用模拟pwm让灯快速亮10毫秒
	mov	a,#10       ;灭灯时间长度(D1，D2，D3，D4，D5，D6，D7，D8)原24
	sub	a,pwm_num2
	b0bts0	fc
	ret
	clr	pwm_num2

	bclr	turn

	incms	count	;开始一亮一灭
   	mov	a,#18       ;一闪一说的次数原14
	sub	a,count
	b0bts0	fc
	ret;下面代码扔会执行
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