#ifndef __DOCPAGE_H__
#define __DOCPAGE_H__


#include "HBaseWin.h"
hbasewinAttr *CreateDocPage(hbasewinAttr *parent, int winID, int docid);
void OnPaint_Docpage(hbasewinAttr *win, void *val);

void Eventhandler_docpage(hbasewinAttr *win, int type, void *val);
#endif