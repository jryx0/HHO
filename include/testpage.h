#ifndef __TESTPAGE_H__
#define __TESTPAGE_H__
#include "macrodef.h"
#include "wResource.h"
#include "hhosvga.h"
#include "HBaseWin.h"

hbasewinAttr *CreateTestPage(hbasewinAttr *parent, int winID, char *title);
void OnPaint_TestPage(hbasewinAttr *win, void *value);
void OnDestroy_TestPage(hbasewinAttr *win, void *value);
#endif