#ifndef __REGSPAGE_H__
#define __REGSPAGE_H__

#include "HBaseWin.h"

hbasewinAttr *Createregisterpage(hbasewinAttr *parent, int winID, int userid);
void OnPaint_registerpage(hbasewinAttr *win, void *value);
void EventHandler_registerpage(hbasewinAttr *win, int type, void *value);
 
#endif