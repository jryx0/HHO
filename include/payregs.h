#ifndef __PAYREGS_H__
#define __PAYREGS_H__

#include "HBaseWin.h"

hbasewinAttr *CreatePayRegspage(hbasewinAttr *parent, int winID, int userid);
void OnPaint_PayRegspage(hbasewinAttr *win, void *value);
void EventHandler_payregspage(hbasewinAttr *win, int type, void *value);
 
#endif