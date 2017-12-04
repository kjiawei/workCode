#ifndef _SOUND_ADJUST_H
#define _SOUND_ADJUST_H

#include <stdio.h>

#define SILENT 0x8000
#define BEEP   0x0c0c//0x1a1a
#define ALARM  0x0b0b

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief SoundAdjustManual
 * \param value
 * \return
 */
extern int SoundAdjustManual(int value);

/*!
 * \brief SoundAdjustMode
 * \param mode
 * \return
 */
extern int SoundAdjustMode(int mode);

extern int Sound_Mute(void);

#ifdef __cplusplus
}
#endif

#endif

