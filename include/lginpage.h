#ifndef __LGINPAGE_H__
#define __LGINPAGE_H__

#include "HBaseWin.h"

hbasewinAttr *Createloginpage(hbasewinAttr *parent, int winID);
void OnPaint_loginpage(hbasewinAttr *win, void *value);
void EventHandler_loginpage(hbasewinAttr *win, int type, void *value);

#endif