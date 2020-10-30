#ifndef __RESULT_H__
#define __RESULT_H__

#include "HBaseWin.h"

hbasewinAttr *CreateResultPage(hbasewinAttr *parent, int winID, int userid);
void OnPaint_ResultPage(hbasewinAttr *win, void *val);
void EventHandler_resultpage(hbasewinAttr *win, int type, void *val);

#endif