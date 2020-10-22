#ifndef __HNEWS_H__
#define __HNEWS_H__

#include "macrodef.h"

#define NEWSPATH DATAPATH "news"

hbasewinAttr *CreateNewspage(hbasewinAttr *parent, int winID, char *title);
void OnPaint_newpage(hbasewinAttr *win, void *val);
void EventHandler_newspage(hbasewinAttr *win, int type, void *val);

#endif