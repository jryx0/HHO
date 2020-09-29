#ifndef __HBUTTON_H__
#define __HBUTTON_H__

#include "HBaseWin.h"

hbasewinAttr *CreateButton(hbasewinAttr *parent, int x, int y, int nWidth,
                              int nHeight, int winID, const char *title);
void OnPaintButtonUp(hbasewinAttr *btn, void *value);
void OnPaintButtonDown(hbasewinAttr *btn, void *value);
void OnLeftUpButton(hbasewinAttr *btn, void *value);
void OnLeftDownButton(hbasewinAttr *btn, void *value);

#endif