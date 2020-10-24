#include "macrodef.h"
#include "wResource.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "hglobal.h"
#include "hnews.h"
#include "hlabel.h"

hbasewinAttr *CreateNewspage(hbasewinAttr *parent, int winID, char *title)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, NULL);
  hbasewinAttr *label;

  TESTNULL(page, NULL);

  page->onPaint = OnPaint_newpage;
  page->EventHandler = EventHandler_newspage;
  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);

  Createhyperlink(page, 20, 10, 65, 25, ID_NEWS_RETURN, "[首 页]");
  label = CreateLabel(page, 20, 30, PAGE_W - 40, PAGE_H - 5, ID_NEWS_TEXT, title);
  label->wintype = LABEL_FILE_TXT; //设置label 从文件中读取文本,并显示

  return page;
}

void OnPaint_newpage(hbasewinAttr *win, void *val)
{
  WinStyle *style;
  hfont *_h;
  int x, y;
  TESTNULLVOID(win);
  TESTNULLVOID(win->style);

  repaintChildren(win, val);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- 新闻公告", _h);
  freeFont(_h);
}

void EventHandler_newspage(hbasewinAttr *win, int type, void *val)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(val);
  _g = (globaldef *)val;
  hitwin = checkmousewin(win, &_g->mouse);

  switch (type)
  {
  case EVENT_MOUSE:
    switch (hitwin->winID)
    {
    case ID_NEWSPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //无激活窗口，原有textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;
    case ID_NEWS_RETURN:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标释放
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        //转跳homepage
        if (win->parent && win->parent->winID == ID_DESKTOP) //找到desktop
        {
          _g->activePageID = ID_HOMEPAGE;
          win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
        }
      }
      break;
    case ID_NEWS_TEXT:
      //label
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
      break;
    }
    break;
  }
}
