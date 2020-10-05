#ifndef __WDESKTOP_H__
#define __WDESKTOP_H__

#include "wResource.h"
#include "HBaseWin.h"
#include "mouse.h"

hbasewinAttr *CreateDesktop();
void eventhandlerdesktop(hbasewinAttr *win, int type, void *value);
hbasewinAttr *creatmouseevent(hbasewinAttr *win, mousestatus *mouse);
hbasewinAttr *checkmousewin(hbasewinAttr *win, mousestatus *mouse);
#endif