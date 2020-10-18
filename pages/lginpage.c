#include "lginpage.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "wResource.h"
#include "htextbox.h"
#include "hbutton.h"
#include "hyperlnk.h"
#include "hglobal.h"
#include "macrodef.h"

hbasewinAttr *Createloginpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "loginpage");
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_loginpage;

  CreateTextBox(page, PAGE_W / 2 - 100, PAGE_H / 2 - 107, 240, 40, ID_LOGIN_USERNAME, NULL);
  CreateTextBox(page, PAGE_W / 2 - 100, PAGE_H / 2 - 7, 240, 40, ID_LOGIN_KEY, NULL);
  Createhyperlink(page, 20, 10, 55, 25, ID_LOGIN_RETURN, "返 回");
  CreateButton(page, PAGE_W / 2 + 40, PAGE_H - 70, 120, 40, ID_LOGIN_LOGIN, "登 录");
  CreateButton(page, PAGE_W / 2 - 110, PAGE_H - 70, 120, 40, ID_LOGIN_REGISTER, "注 册");

  return page;
}

void OnPaint_loginpage(hbasewinAttr *win, void *value)
{
  hfont *_h = getFont(SIMSUN, 24, 0);
  int x, y;
  TESTNULLVOID(win);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  printTextLineXY(x + PAGE_W / 2 - 200, y + PAGE_H / 2 - 100, "用户名：", _h);
  printTextLineXY(x + PAGE_W / 2 - 200, y + PAGE_H / 2, "密  码：", _h);

  repaintChildren(win);
  freeFont(_h);
}

//原理同EventHandler_testpage
void EventHandler_loginpage(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  hitwin = checkmousewin(win, &_g->mouse);

  switch (type)
  {
  case EVENT_MOUSE:
    switch (hitwin->winID)
    {
    case ID_LOGINPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {//无激活窗口，原有textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;
    case ID_LOGIN_USERNAME:
      break;
    default:
      break;
    }
    break;

  default:
    break;
  }
}