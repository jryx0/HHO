#include "signpage.h"
#include "HBaseWin.h"
#include "macrodef.h"
#include "hhosvga.h"

hbasewinAttr *Createsignpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "signpage");
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_signpage;
  page->EventHandler = EventHandler_signpage;

  

  return page;
}

void OnPaint_signpage(hbasewinAttr *win, void *value)
{
  
  
  repaintChildren(win, value);
}
void EventHandler_signpage(hbasewinAttr *win, int type, void *value)
{
  TESTNULLVOID(win);
  TESTNULLVOID(value);
}