#ifndef __HLABEL_H__
#define __HLABEL_H__

#include "HBaseWin.h"

hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                              int nHeight, int winID, const char *title);

#endif