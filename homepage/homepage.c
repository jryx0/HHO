#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "hglobal.h"
#include "homepage.h"
#include "wResource.h"

void EventHandler_homepage(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;

  TESTNULLVOID(win);
  TESTNULLVOID(value);
  _g = (globaldef *)value;
  switch (type)
  {
  case MOUSE_EVENT:
    if (_g->mouse.currentCur != _g->cursor_arrow)
      _g->mouse.currentCur = _g->cursor_arrow;
    break;
  case MOUSE_BUTTON_UP:
    switch (win->winID)
    {
    case ID_LABEL_1:
      //win->title = "c:\\hho\\data\\news\\1.txt";
      if (win && win->onPaint)
        win->onPaint(win, "c:\\hho\\data\\news\\1.txt");
      break;
    case ID_LABEL_2:
      if (win && win->onPaint)
        win->onPaint(win, "main.c");
    default:
      break;
    }
    break;
  default:
    break;
  }
}

hbasewinAttr *CreateHomepage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "homepage");
  TESTNULL(page, NULL);

  CreateLabel(page, 15, 90, 300, 150, ID_LABEL_1, "readme");
  CreateLabel(page, 15 + 350, 90, 300, 150, ID_LABEL_2, "c:\\hho\\data\\news\\1.txt");

  page->onPaint = OnPaint_homepage;
  page->onDestroy = OnDestory_homepage;
  page->EventHandler = EventHandler_homepage;
}

void OnPaint_homepage(hbasewinAttr *win, void *value)
{
  TESTNULLVOID(win);

  //²âÊÔÓÃ
  rectangleEx(win->x, win->y, win->nWidth, win->nHeight, 0xF801, 1, 3);

  repaintChildren(win);
}

void OnDestory_homepage(hbasewinAttr *win, void *value)
{
  TESTNULLVOID(win);
  destoryChildren(win);
  OnDestory(win, NULL);
  clearRegion(PAGE_X, PAGE_Y, PAGE_W, PAGE_H, 0xffff);
}
