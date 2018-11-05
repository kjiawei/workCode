struct PacketColdRinse { //冷冲洗参数 全局变量
	int id;//权限
    int rinseTime; //时间 min
    int rinseSpeed; //透析液流速
};

数据包
#define PACKET_DISINFECT 103

//读取
PacketColdRinse coldRinse;
dataRead(PACKET_DISINFECT, &coldRinse);
coldRinseSpeed->setNum(coldRinse.rinseSpeed);

//添加
dataNodeAddObject(PacketColdRinse, this);

//更新
PacketColdRinse coldRinse;
coldRinse.rinseSpeed = coldRinseSpeed.text().toFloat();//将消毒参数设置页面上手工输入的值保存到结构体
coldRinse.rinseTime = coldRinseTime.text().toFloat();//(lineEditTemp.text().toFloat()*10);  //精确到小数点后一位
dataNodeUpdate(PacketColdRinse,&coldRinse);//更新透析液数据包

//UI
QLabel coldRinseSpeed;
    QLineEdit lineEditColdRinseSpeed;
    QLabel coldRinseSpeedUnit;
	
	.text().toFloat();