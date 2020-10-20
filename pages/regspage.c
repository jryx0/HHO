#include "regspage.h"
#include "HBaseWin.h"
#include "macrodef.h"
#include "hhosvga.h"

hbasewinAttr *Createregisterpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "registerpage");
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_registerpage;
  page->EventHandler = EventHandler_registerpage;

  

  return page;
}

void OnPaint_registerpage(hbasewinAttr *win, void *value)
{
  
  
  repaintChildren(win, value);
}
void EventHandler_registerpage(hbasewinAttr *win, int type, void *value)
{
  TESTNULLVOID(win);
  TESTNULLVOID(value);
}