#ifndef COMMONVAR_H
#define COMMONVAR_H

#include "commonUtil.h"

#define DEBUG_OTHER_FLAG 0 //原config_ui 其它调试(未具体定义的)标志
#define DEBUG_LAMP 0  //报警灯调试标志
#define DEBUG_AUDIO 0 //报警音量调试表示
#define LANGUAGE_ENGLISH 1 //多国语言宏

bool IS_SOUND_SHIELDING = false; //屏蔽声音(报警)


buttonsIrq buttonsIrqApp;

#endif