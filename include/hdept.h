#ifndef __DEPT_H__
#define __DEPT_H__
#include "HBaseWin.h"
hbasewinAttr *CreateDeptpage(hbasewinAttr *parent, int winID, char *title);
void OnPaint_Dept(hbasewinAttr *win, void *val);
void OnDestory_Dept(hbasewinAttr *win, void *val);
void EventHandler_deptpage(hbasewinAttr *win, int type, void *val);

#endif