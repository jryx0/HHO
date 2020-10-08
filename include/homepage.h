
#ifndef __HOMEPAGE_H__
#define __HOMEPAGE_H__
#include "HBaseWin.h"

hbasewinAttr *CreateHomepage(hbasewinAttr *parent, int winID);
void OnPaint_homepage(hbasewinAttr *page, void *val);
void OnDestory_homepage(hbasewinAttr *page, void *val);
void EventHandler_homepage(hbasewinAttr *win, int type, void *value);
#endif
