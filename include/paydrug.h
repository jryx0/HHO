#ifndef __PAYDRUG_H__
#define __PAYDRUG_H__

#include "HBaseWin.h"
void OnPaint_PayDrugpage(hbasewinAttr *win, void *val);
void Eventhandler_paydrugpage(hbasewinAttr *win, int type, void *val);
hbasewinAttr *CreatePayDrugpage(hbasewinAttr *parent, int winID, int userid);
#endif