#ifndef __LOGIPAGE_H__
#define __LOGIPAGE_H__

#include "HBaseWin.h"


hbasewinAttr *CreatePostpage(hbasewinAttr *parent, int winID, int userid);
void OnPaint_Post(hbasewinAttr *win, void *val);
void EventHandler_postpage(hbasewinAttr *win, int type, void *val);

#endif