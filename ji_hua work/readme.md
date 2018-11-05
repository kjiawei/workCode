# 6068重构版本 2018.2.24 V1.00.0

# 2018.3.1 待处理问题
1.内核启动图片更换
2.update.pkg后的密码登录出错
3.钠曲线的实现方式可能跟位-底层有关
4.解耦底层和上位机 硬件开发板的搭建
5.超滤误差控制
6.超滤量报警数据处理
7.虚拟机奔溃重设共享目录sudo mount -t vboxsf fShare /mnt/share

## 目标:软件定义清晰 加快编译速度
## 需求获取时需要添加测试计划/项目 及 风险管理 设计方案
## 消除无效的注释 例如:struct还说这是一个结构体 自增减放在变量前 格式化 if的第一块语句只有 一条语句的话不用加花括号
## 版本控制:基于atmel 9G45kers的可移植6068程序
## 开发方案:QDesigner作界面 python C++ pyside  把QtDesigner生成的ui文件转换为Python文件在windows下调试
	http://www.cnblogs.com/ascii0x03/p/5495035.html 额外测试可以
	nfs搭建参考 http://blog.csdn.net/my_way_/article/details/41846657
	调试:工具命令  挂载目录  mount -t vboxsf fShare(windows下对应文件夹) /mnt/shared
## 开发方式:敏捷 原型开发
	UI变动大的部分用Designer来做
	## 测试驱动开发 Gtest QTestLib 变量控制:每添加一个变量做相应的输入输出测试()性别 数据类型 数据范围
	gtest使用教程 https://www.linuxidc.com/Linux/2015-05/116894.htm
## 跨平台调试控制

## 编程规范
注释规范:不用无意义的说明 动词+名词
头文件用到再包含
谨慎使用变量并注明使用原因
if-else
全局变量的使用

模块（模块名对应项目内的一个文件夹 子模块可作为具体的实现文件或数据库）
threadPool线程池 处理整机工作线程流-状态切换
	business业务流程 工作状态与泵的关系
warnModule报警模块 包含文件IO底层操作-lamp audio
dataCenter数据中心 包含测试数据 数据库  数据隔离层 
	config配置表; 上位机与下位机通信-设置监控板开启血压模块  上面模拟进入治疗 用宏)  头文件管理
	commonVar通用全局变量; 治疗总时 已治疗时 state工作状态(治疗开始 暂停 结束;寻找的两条主线-1.血泵渎值 2.widget_switch 230line左右;联动的因素有:血泵 KTV值)
	historyRecord历史记录 出错记录-写入测试记录
commonUI通用控件布局类 包含大量的宏函数
	specialUI特别部件UI类 数据曲线控件 水路状态图 数据表格等; 
commonUtil通用工具类 
	1.文件读写 数据库操作(与推荐系统结合-数据库软件) 泵操作
	2.屏幕操作(冻屏 截屏)
	ai 算法系统-推荐系统
	can通信协议;
	tools第三方工具包;跨平台调试代码
	simulationBoard模拟下位机运行部分
	http等通信协议; 扩展使之成为安卓 网站的服务端
	hardware硬件控制类 备用:泵(与数据操作类似-派生一个对象)等操作可能移至此处-wlan模块 血压计模块 声音控制 等;
managerSystem管理信息系统 包含病人信息系统  后台测试端-服务端(将6068模拟成一个客户端)  工程模式  调试模式
interface接口汇总 数据调用 模拟测试
TDD测试驱动开发 上面模拟下面发送命令上来进入治疗模式 测试数据通过案例
	unitTest 单元测试
	暴力测试 验证数据类型 整形与浮点型的区别 状态判断-没进治疗的KTV值多次预判(加进需求文档)

----------------------------------------------------------------------------------------------------
UI设计 (倒计时控件 自检进度条 输入键盘 数据表格 数据曲线)  
各页面设计(继承 批量设计控件 个别调整 判断循环的简化)
宏函数
出处BPM    行数1   作用:设置血压计控件的位置   优化方法      备注  分析
SCENE_BPM_CREATE_T(x, pos_x, pos_y, pos_w)             \
    x.setGeometry(QRect(pos_x, SCENE_SETUP_ROW_##pos_y,        \
                        pos_w, SCENE_SETUP_WIDGET_HIGH));      \
    x.setAlignment(Qt::AlignVCenter | Qt::AlignRight);         \
    x.setFont(QFont(QString(CONFIG_FONT_STYLE),                \
                    CFG_FONT_SIZE_THIRD, CONFIG_FONT_NORMAL)); \
    x.setStyleSheet(CONFIG_COLOR_LABEL)
优点:统一 自顶向下设计 规范 速度快 
劣势:多重嵌套-不方便维护(源文件过于分散) 代码量大 作用不成正比 移植性不好
保留意见:保留宏,但要精简 取消多重嵌套-减少代码 通用设计-变量名好理解 加UI规划图-像CAD
总结:宏函数-控件(样式 字体 位置 大小 是否居中 宽高)
重新设计:同类的用循环生成(个别特殊的采用覆盖做法)  调用逻辑(1.这是什么控件label button还是其他 2.设置项 3.额外补充  继承关系 开关)
开关按钮-判断变量  用循环缩短代码

can驱动层的使用与6068部件(传感器)的类封装
例:血泵行为(类比其它部件)
自己启动(传一个机器状态)
转速(传一个数值)
class BloodPump{  //调用这个类生成的对象前已经运行can设备,只是取用节点进行读写的问题
	BloodPump bloodPump;

	int BloodPumpInit();//血泵初始化
	bool BloodPumpStart(int MachineState,...);//启动血泵 设置开始使用的参数
	int BloodPumpRun(int speedNum,time sec,...);//设置运行参数:速度 时间
	bool BloodPumpStop();//停止血泵
}

面向对象设计的对象与通信协议之间的关系

数据接口
当我需要启动血泵时
发送一条启动血泵指令给血泵 
BloodPumpInit();
BloodPumpStart(MACHINE_READY,preFlush);//机器处于准备状态 需要预冲

问题:主线 6068是如何启动治疗的  治疗过程如何设计  结束治疗的(判断条件)注意事项
----------------------------------------------------------------------------------------------------

### 使用/配置方法

## 后期优化
*改动大的用lua等脚本语言做:逻辑
系统升级:script
python:变动大部分 界面(主题)
固定的用C/C++来搞:通信 业务