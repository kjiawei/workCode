/*2018.1.16 定时器更新已补液量*/
    totalTime = 0.00;//总次数
    refreshTime = 0;//刷新次数
    theoryAddQuickSpeed = 0.00;//理论补液速度
    theoryAddQuickVol = 0.00;
    tmpAddVol = 0.00;//暂存已补液量
    initVol = 0;//初始补液量
    finishVol = 0.00;//补液总量
/*2018.1.16 定时器更新已补液量*/

    void SceneSetupSubspump::timerUpdateAddQuick()//2018.1.26 jkCodic添加定时器更新已补液量理论值
{
	WorkState  stateTMP=GetWorkStateManage();
	if(stateTMP.tipState.current == TREAT_FINISH){//治疗结束后清0
		finishVol = 0.00;
		tmpAddVol = 0.00;
	}
    //refreshTime += 1;//计数器
    theoryAddQuickVol = 100;
    if(!lineEditAddQuick.text().isEmpty())
		theoryAddQuickVol = lineEditAddQuick.text().toInt();//2018.1.26替换为用定时器更新已补液量理论值
    theoryAddQuickSpeed = 300;
    if(!lineEditAddRate.text().isEmpty())
		theoryAddQuickSpeed = lineEditAddRate.text().toInt();
    totalTime = theoryAddQuickVol / theoryAddQuickSpeed * 60 / FIVE_SECOND;//刷新次数

//    //tmpAddVol = 0;//暂存已补液量
//    //initVol = 0;//初始补液量
    if ((statusAddQuick == true) && (statusAddQuick2 == true)) {//判断是否处于单次快速补液状态
        finishVol +=theoryAddQuickVol / totalTime;//补液总量
		if ( finishVol == theoryAddQuickVol )//下一次补液
		{
			tmpAddVol += finishVol;//暂存已补液量
//        //initVol = 0;//初始补液量
			finishVol = 0.00;//补液总量
			labelAddValueHas.setText(QString::number(tmpAddVol));
			//refreshTime = 0;//重置计数器
			statusAddQuick2 = false;
			buttonAddQuick.setText(DE_TR_KEYSTART);//补液按钮设置为开始
		}else{
			labelAddValueHas.setText(QString::number((int)(tmpAddVol+finishVol)));
		}
    }
}