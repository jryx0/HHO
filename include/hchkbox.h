#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "HBaseWin.h"

hbasewinAttr *CreateCheckBox(hbasewinAttr *parent, int x, int y, int nWidth,
                             int nHeight, int winID, const char *title);
void OnPaintCheckBoxNone(hbasewinAttr *checkbox, void *value);
void OnPaintCheckBoxRight(hbasewinAttr *checkbox, void *value);
void OnPaintCheckBoxCross(hbasewinAttr *checkbox, void *value);
void OnPaintCheckBox(hbasewinAttr *checkbox, void *value);
void OnClickCheckBox(hbasewinAttr *checkbox, void *value);

#endif