#ifndef _BUTTONS_H
#define _BUTTONS_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int ButtonsInit(void);
extern int fd_button;
extern int btnMute;
extern int btnAlarmReset;
void buttons(int key);

#ifdef __cplusplus
}
#endif
void sigroutine_buttons(int dunno);
#endif

