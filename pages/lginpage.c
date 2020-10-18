#include "lginpage.h"
#include "hhosvga.h"
#include "HBaseWin.h"

hbasewinAttr *Createloginpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "loginpage");
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_loginpage;

  return page;
}

void OnPaint_loginpage(hbasewinAttr *win, void *value)
{
  hfont *_h = getFont(SIMSUN, 24, 0);
  int x, y;
  TESTNULLVOID(win);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  printTextLineXY(x+PAGE_W/2-200, y+PAGE_H/2-100, "ÓÃ»§Ãû", _h);
  printTextLineXY(x+PAGE_W/2-200, y+PAGE_H/2, "ÃÜ Âë", _h);

  repaintChildren(win);
}