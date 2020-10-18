#include "lginpage.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "wResource.h"
#include "htextbox.h"
#include "hbutton.h"

hbasewinAttr *Createloginpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "loginpage");
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_loginpage;

  CreateTextBox(page, PAGE_W / 2 - 100, PAGE_H / 2 - 107, 240, 40, ID_LOGIN_USERNAME, NULL);
  CreateTextBox(page, PAGE_W / 2 - 100, PAGE_H / 2 - 7, 240, 40, ID_LOGIN_KEY, NULL);
  CreateButton(page, PAGE_W / 2 + 40, PAGE_H - 70, 120, 40, ID_LOGIN_LOGIN, "µÇ Â¼");
  CreateButton(page, PAGE_W / 2 - 110, PAGE_H - 70, 120, 40, ID_LOGIN_REGISTER, "×¢ ²á");

  return page;
}

void OnPaint_loginpage(hbasewinAttr *win, void *value)
{
  hfont *_h = getFont(SIMSUN, 24, 0);
  int x, y;
  TESTNULLVOID(win);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  printTextLineXY(x + PAGE_W / 2 - 200, y + PAGE_H / 2 - 100, "ÓÃ»§Ãû£º", _h);
  printTextLineXY(x + PAGE_W / 2 - 200, y + PAGE_H / 2, "ÃÜ  Âë£º", _h);

  repaintChildren(win);
}