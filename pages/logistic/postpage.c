#include "macrodef.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "wResource.h"
#include "hhosvga.h"
#include "list.h"
#include "hglobal.h"
#include "postpage.h"
#include "htextbox.h"
#include "hbutton.h"

#define COL1 100
#define COL2 (COL1 + 90)
#define COL3 (COL2 + 180)
#define COL4 (COL3 + 120)
#define COL5 (COL4 + 80)
#define COL6 (COL5 + 180)
#define COL7 (COL6 + 150)
#define COL8 120

hbasewinAttr *CreatePostpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, NULL);
  page->onPaint = OnPaint_Post;
  Createhyperlink(page, 20, 10, 65, 25, ID_POST_RETURN, "[首 页]");
  page->EventHandler = EventHandler_postpage;

  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);

  Createhyperlink(page, page->x, page->y + 200, 55, 25, ID_POST_PREV, "上一页");
  Createhyperlink(page, page->x + 65, page->y + 200, 55, 25, ID_POST_NEXT, "下一页");

  CreateTextBox(page, 72, 43, 120, 20, ID_POST_QUERY_BILLNUM, NULL, 1);
  CreateTextBox(page, 280, 43, 100, 20, ID_POST_QUERY_SHIPPER, NULL, 1);
  CreateTextBox(page, 500, 43, 150, 20, ID_POST_QUERY_SHIPPERTEL, NULL, 1);
  CreateTextBox(page, 730, 43, 150, 20, ID_POST_QUERY_RECEIVER, NULL, 1);

  CreateButton(page, 890, 43, 120, 30, ID_POST_QUERY_RECEIVER, "查  询");

  return page;
}

void OnPaint_Post(hbasewinAttr *win, void *val)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  Putbmp565(PAGE_W - 250, y, DATAPATH "database\\logistic\\postlogo.565");
  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- 物流信息", _h);

  rectangleEx(x, y + 80, PAGE_W, PAGE_H / 2 - 50, 0x6BAF, 1, 1);
  //rectangleEx(x, y + 35 + PAGE_H / 2 + 3, PAGE_W, 30, 0x6BAF, 1, 1);

  printTextLineXY(x + 10, y + 84, " 运单号      收货人       收货地址         收货人电话    发货人      发货地址           发货时间         状态", _h);
  lineyEx(x + COL1, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL2, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL3, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL4, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL5, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL6, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL7, y + 80, PAGE_H / 2 - 50, 0x6BAF);
  linex_styleEx(x, y + 104, PAGE_W, 0x6BAF, 1, 1);

  printTextLineXY(x + 10, y + 50, "运单号:", _h);
  printTextLineXY(x + 210, y + 48, "收货人:", _h);
  printTextLineXY(x + 400, y + 48, "收货人电话:", _h);
  printTextLineXY(x + 665, y + 48, "发货人:", _h);

  printTextLineXY(x + 510, y + 380, "运单号:", _h);
  printTextLineXY(x + 680, y + 380, "订单号:", _h);
  printTextLineXY(x + 850, y + 380, "发货人:", _h);
  printTextLineXY(x + 510, y + 420, "收货人:", _h);
  printTextLineXY(x + 680, y + 420, "收货人电话:", _h);
  printTextLineXY(x + 510, y + 460, "收货人地址:", _h);

  printTextLineXY(x + 510, y + 500, "配送站:", _h);

  freeFont(_h);
  repaintChildren(win, val);

  // rectangleEx(x + 15, y + 35, 140, PAGE_H - 10, 0x0000, 1, 2);
  // rectangleEx(x + 160, y + 25, PAGE_W - 250, PAGE_H - 100, 0x0000, 1, 2);
}

void EventHandler_postpage(hbasewinAttr *win, int type, void *val)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(val);
  _g = (globaldef *)val;
  hitwin = checkmousewin(win, &_g->mouse);

  if (type == EVENT_MOUSE)
  {
    switch (hitwin->winID)
    {
    case ID_POSTPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
      break;
    case ID_POST_RETURN:
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
    }
  }
}