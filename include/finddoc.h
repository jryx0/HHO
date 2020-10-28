#ifndef __FINDDOC_H__
#define __FINDDOC_H__

#include "HBaseWin.h"
hbasewinAttr *CreateFindDocpage(hbasewinAttr *parent, int winID, int userid);
void OnPaint_FindDoc(hbasewinAttr *win, void *val);
void EventHandler_FindDoc(hbasewinAttr *win, int type, void *val);

#endif