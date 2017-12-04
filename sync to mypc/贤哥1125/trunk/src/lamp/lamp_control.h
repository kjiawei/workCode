#ifndef _LAMP_CONTROL_H
#define _LAMP_CONTROL_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int LampControl(int mode);
extern int lamp_state;
extern int Lampinit(void);
extern int fd_lamp;
#ifdef __cplusplus
}
#endif

#endif

