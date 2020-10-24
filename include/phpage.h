#ifndef __PHPAGE_H__
#define __PHPAGE_H__

#include "HBaseWin.h"

hbasewinAttr *CreatePharmacypage(hbasewinAttr *parent, int winID, int userid);
void OnPaint_Pharmacy(hbasewinAttr *win, void *val);
void EventHandler_pharmacypage(hbasewinAttr *win, int type, void *val);

#endif