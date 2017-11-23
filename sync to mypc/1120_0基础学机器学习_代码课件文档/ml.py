#coding=utf-8
def myinput(oper,meth):
	#meth为true则只判断1，为false则判断1或0
	while True:
		oper[0] = raw_input()
		if meth and oper[0] =='y' :
			break
		else: 
			if oper[0]=='n' or oper[0]=='y':
				break;
		print "输入错误，请重新输入。" 	#判断参数


if __name__ == '__main__':
	ladd=[0]
	radd=[0]
	aprs=['n']
	rcnt =0
	wcnt=0
	#定义输入与结果，正确次数与错误次数
	
	print "主人，我想学习数学题，您可以教我吗？不要惊讶我会学习的"
	print "开始学习吧……"
	print "输入被加数：",
	ladd = input()
	print "输入加数：",
	radd = input()
	i = 0
	while i!=5:
		print ladd,"+",radd,"=",ladd+radd
		print "我算对了吗？（满意输入y，不满意输入n）：",#评价等级
		myinput(aprs,False)
		if aprs[0] == 'y' :		#判断用户评价
			rcnt = rcnt+1
		else:
			wcnt = wcnt +1
		i = i+1
	
	print "正确次数：",rcnt," 错误次数：",wcnt	#错误次数
	if rcnt>wcnt:	#判断学习结果
		print "主人,我知道了",ladd,"+",radd,"=",ladd+radd,"。"
	else:
		if rcnt<wcnt :
			print "主人,我知道了",ladd,"+",radd,"!=",ladd+radd,"。"
		else:
			print "到底",ladd,"+",radd,"=？我不明白主人是什么意思。"

