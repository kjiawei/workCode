#ifndef  __KTVTASK_H
#define  __KTVTASK_H 

#include "../../ucos-ii/includes.h"
#include "../mygui/jihua.h"
#include "../mygui/extern_var.h"

#if KTV
static INT8U GetDtv(INT16U uf);
static INT16U GetP(INT16U *ddl, INT8U num);
static INT16U GetMax(INT16U *ddl, INT8U num);
static INT16U GetMin(INT16U *ddl, INT8U num);
static void JiaoDDLOut(INT8U num);
static void GetOrder(INT16U *ui_list_point, INT8U uc_number);
#endif

#endif
