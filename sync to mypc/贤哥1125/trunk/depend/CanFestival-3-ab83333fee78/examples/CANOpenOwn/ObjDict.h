
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef OBJDICT_H
#define OBJDICT_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 Master_valueRangeTest (UNS8 typeValue, void * value);
const indextable * Master_scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **callbacks);

/* Master node data struct */
extern CO_Data Master_Data;
extern UNS16 N15_all_value;		/* Mapped at index 0x2000, subindex 0x00*/
extern UNS16 N15_dose_speed;		/* Mapped at index 0x2001, subindex 0x00*/
extern UNS8 N15_mode;		/* Mapped at index 0x2002, subindex 0x00*/
extern UNS8 N15_sub_mode;		/* Mapped at index 0x2003, subindex 0x00*/
extern UNS16 N3_out_temp;		/* Mapped at index 0x2004, subindex 0x00*/
extern UNS16 N3_out_cond;		/* Mapped at index 0x2005, subindex 0x00*/

#endif // OBJDICT_H
