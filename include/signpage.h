#ifndef __SIGNPAGE_H__
#define __SIGNPAGE_H__

#include "HBaseWin.h"

hbasewinAttr *Createsignpage(hbasewinAttr *parent, int winID);
void OnPaint_signpage(hbasewinAttr *win, void *value);
void EventHandler_signpage(hbasewinAttr *win, int type, void *value);

int id_num_judge(char *id_num);

#endif