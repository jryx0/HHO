#include "regspage.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "hbutton.h"
#include "wResource.h"
#include "macrodef.h"
#include "hglobal.h"
#include "hyperlnk.h"

hbasewinAttr *Createregisterpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "registerpage");
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_registerpage;
  page->EventHandler = EventHandler_registerpage;

  Createhyperlink(page, 20, 10, 65, 25, ID_REGISTER_RETURN, "[首 页]");
  CreateButton(page, 20, 70, 100, 35, ID_REGISTER_DEPT_MEDICINE, "内科"); //内科、外科、儿科、妇科、眼科、耳鼻喉科、口腔科、皮肤科
  CreateButton(page, 150, 70, 100, 35, ID_REGISTER_DEPT_SURGERY, "外科");
  CreateButton(page, 20, 120, 100, 35, ID_REGISTER_DEPT_PAEDIATRICS, "儿科");
  CreateButton(page, 150, 120, 100, 35, ID_REGISTER_DEPT_GYNAECOLOGY, "妇科");
  CreateButton(page, 20, 170, 100, 35, ID_REGISTER_DEPT_OPHTHALMOLOGY, "眼科");
  CreateButton(page, 150, 170, 100, 35, ID_REGISTER_DEPT_ENT, "耳鼻喉科");
  CreateButton(page, 20, 220, 100, 35, ID_REGISTER_DEPT_STOMATOLOGY, "口腔科");
  CreateButton(page, 150, 220, 100, 35, ID_REGISTER_DEPT_DERMATOLOGY, "皮肤科");

  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);
  return page;
}
void OnPaint_registerpage(hbasewinAttr *win, void *value)
{
  WinStyle *style;
  hfont *_h;
  int x, y;
  TESTNULLVOID(win);
  TESTNULLVOID(win->style);

  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  printTextLineXY(x + style->fontsize * 6, y + 10, "- 挂号", _h);
  printTextLineXY(x + 16, y + 45, "请选择科室：", _h);

  repaintChildren(win, value);
  freeFont(_h);
}
void EventHandler_registerpage(hbasewinAttr *win, int type, void *value)
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
    case ID_REGISTERPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //无激活窗口，原有textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;
    case ID_REGISTER_RETURN:
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

    case ID_REGISTER_DEPT_MEDICINE:
    case ID_REGISTER_DEPT_SURGERY:
    case ID_REGISTER_DEPT_PAEDIATRICS:
    case ID_REGISTER_DEPT_GYNAECOLOGY:
    case ID_REGISTER_DEPT_OPHTHALMOLOGY:
    case ID_REGISTER_DEPT_ENT:
    case ID_REGISTER_DEPT_STOMATOLOGY:
    case ID_REGISTER_DEPT_DERMATOLOGY:
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

    default:
      break;
    }
  default:
    break;
  }
}