#include "signpage.h"
#include "HBaseWin.h"
#include "macrodef.h"
#include "hhosvga.h"
#include "wResource.h"
#include "hglobal.h"

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
    case ID_SIGNINPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //无激活窗口，原有textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;

    default:
      break;
    }
    break;
  default:
    break;
  }
  (void)type;
}