#ifndef __HLABEL_H__
#define __HLABEL_H__

#include "HBaseWin.h"

hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title);
void OnPaint_Label(hbasewinAttr *label, void *value);

/**
 * 删除自身 
 */
void OnDestoy_Lable(hbasewinAttr *label, void *value);

/**
 * 事件处理
 */
void EventHandler_Label(hbasewinAttr *win, int type, void *value);
#endif