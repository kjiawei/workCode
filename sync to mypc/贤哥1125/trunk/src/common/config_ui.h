#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#define CONFIG_SCREEN_WIDTH             1024
#define CONFIG_SCREEN_HIGH              768

/* DroidSansFallback Courier ... */
#define CONFIG_FONT_STYLE               "Droid Sans Fallback"

#define CONFIG_FONT_POINTSIZE_BIG       40
#define CONFIG_FONT_POINTSIZE_NORMAL    28
#define CONFIG_FONT_POINTSIZE_SMALL     18
#define CONFIG_FONT_POINTSIZE_SMALL2    16
#define CONFIG_FONT_POINTSIZE_SMALL3    12

#define CFG_FONT_SIZE_TITLE             21
#define CFG_FONT_SIZE_FIRST_1           40
#define CFG_FONT_SIZE_FIRST_2           20
#define CFG_FONT_SIZE_FIRST_3           18
#define CFG_FONT_SIZE_SECOND            20
#define CFG_FONT_SIZE_THIRD             17
#define CFG_FONT_SIZE_FOURTH            13
#define CFG_FONT_SIZE_TEXT              15
/* QFont::Light-25, QFont::Normal-50, QFont::DemiBold-63, QFont::Bold-75 QFont::Black-87 */
#define CONFIG_FONT_WEIGHT              QFont::Bold 
#define CONFIG_FONT_NORMAL              QFont::Normal

/* QFont: color */
#define CONFIG_COLOR_WHITE              0xFFdce6f0
#define CONFIG_COLOR_BLUE               0xFF031e37
#define CONFIG_COLOR_BLACK              0xff484848
#define CONFIG_COLOR_GREEN              0xff237d3a
#define CONFIG_COLOR_RED                0xffc6181e
#define CONFIG_COLOR_YELLOW             0xFFd1bd51

#define CONFIG_COLOR_WHITE_CHARS        "color:#dce6f0"
#define CONFIG_COLOR_BLUE_CHARS         "color:#031e37"
#define CONFIG_COLOR_BLACK_CHARS        "color:#484848"
#define CONFIG_COLOR_GREEN_CHARS        "color:#237d3a"
#define CONFIG_COLOR_RED_CHARS          "color:#c6181e"
#define CONFIG_COLOR_YELLOW_CHARS       "color:#d1bd51"

#define CONFIG_COLOR_LABEL              CONFIG_COLOR_WHITE_CHARS

/* Work mode, not more than 128 */
#define MODE_UNIVERSAL                  0
#define MODE_HD                         1
#define MODE_HDF                        2
#define MODE_NUM                        3
#define MODE_MAX                        128

/* Lang family, not more than 128 */
#define LANG_UNIVERSAL                  0
#define LANG_ENGLISH                    1
#define LANG_ZHCN                       2
#define LANG_NUM                        3
#define LANG_MAX                        128


//HJX调试开关大全
#define   Widget_Data_Curve_Switch  1    //数据曲线控件开关  0-关  1-开

#define Input_Box_Fault_Problem 0  //输入框输入黑屏现象

#define HJX_DEBUG_FLAG  1    //HJX调式标志

#define SYSTEM_DEBUG_FLAG 1

#define DEBUG_TIMER_HANDLE 1

#define DEBUG_XML_CONFIG 1

#define DEBUG_DATA_SQL  1

#define DEBUG_OTHER_FLAG 0

#define DEBUG_MUTEXDIE_FLAG 1

#define HJX_COMMUNICATION_SWITCH  2   //HJX新实现的CANOPEN通信方式  0：最原始版本  1：HJX1.0版本有偶发PDO错误和同时重复发送PDO的问题  2：为解决HJX1.0问题而实现

#define EEDA_VERSION_OPEN   1     //英文EEDA版本开关   1：开启  0：关闭

#define HJX_DELAYTIME_100000   100000 //延时时间 us#/
#define HJX_DELAYTIME_80000   80000 //延时时间 us#/
#define HJX_DELAYTIME_50000   50000
#define HJX_DELAYTIME_10000   10000

#endif // CONFIG_H
