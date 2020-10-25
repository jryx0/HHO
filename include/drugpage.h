#ifndef __DRUGPAGE_H__
#define __DRUGPAGE_H__

#include "HBaseWin.h"

hbasewinAttr *CreateDrugpage(hbasewinAttr *parent, int winID, int userid);
void OnPaint_Drugpage(hbasewinAttr *win, void *val);
void Eventhandler_drugpage(hbasewinAttr *win, int type, void *val);
void createDrugList(hbasewinAttr *win, int id);

#endif