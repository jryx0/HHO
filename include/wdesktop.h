#ifndef __WDESKTOP_H__
#define __WDESKTOP_H__

#include "Resource.h"
#include "HBaseWin.h"
#include "mouse.h"

/**
 * 创建主窗口
 * 
 */
hbasewinAttr *CreateDesktop(void);
void eventhandlerdesktop(hbasewinAttr *win, int type, void *value);
hbasewinAttr *creatmouseevent(hbasewinAttr *win, mousestatus *mouse);
hbasewinAttr *checkmousewin(hbasewinAttr *win, mousestatus *mouse);
void closePY(hbasewinAttr *win, globaldef *_g);
#endif