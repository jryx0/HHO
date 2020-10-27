#include "signpage.h"
#include "HBaseWin.h"
#include "macrodef.h"
#include "hhosvga.h"
#include "wResource.h"
#include "hglobal.h"
#include "htextbox.h"
#include "hyperlnk.h"
#include "hbutton.h"
#include <time.h>

hbasewinAttr *Createsignpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "signpage");
  hbasewinAttr *pwd;
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_signpage;
  page->EventHandler = EventHandler_signpage;

  Createhyperlink(page, 20, 10, 65, 25, ID_SIGNIN_RETURN, "[首 页]");
  CreateTextBox(page, 240, 95, 100, 26, ID_SIGNIN_NAME, "", 1);
  CreateTextBox(page, 610, 95, 80, 26, ID_SIGNIN_SEX, "", 1);
  CreateTextBox(page, 240, 166, 80, 26, ID_SIGNIN_MARRIAGESTATUS, "", 1);
  CreateTextBox(page, 610, 166, 230, 26, ID_SIGNIN_IDNUMBER, "", 1);
  CreateTextBox(page, 240, 237, 80, 26, ID_SIGNIN_BIRTH_YEAR, "", 1);
  CreateTextBox(page, 360, 237, 50, 26, ID_SIGNIN_BIRTH_DAY, "", 1);
  CreateTextBox(page, 610, 237, 200, 26, ID_SIGNIN_TELENUM, "", 1);
  CreateTextBox(page, 240, 297, 250, 26, ID_SIGNIN_ADDRESS, "", 1);
  CreateTextBox(page, 610, 297, 150, 26, ID_SIGNIN_ALLERGY, "", 1);
  CreateTextBox(page, 240, 354, 120, 26, ID_SIGNIN_USERNAME, "", 1);
  pwd = CreateTextBox(page, 240, 411, 150, 26, ID_SIGNIN_PASSWORD1, "", 1);
  pwd->wintype = TEXTBOX_PASSWORD;
  pwd = CreateTextBox(page, 610, 411, 150, 26, ID_SIGNIN_PASSWORD2, "", 1);
  pwd->wintype = TEXTBOX_PASSWORD;
  CreateButton(page, PAGE_W / 2 - 60, 485, 120, 35, ID_SIGNIN_SIGNIN, "注册");
  return page;
}

void OnPaint_signpage(hbasewinAttr *win, void *value)
{
  int x, y;
  hfont *_h1 = getFont(SIMSUN, 16, 0);
  hfont *_h2 = getFont(SIMSUN, 16, 0xFFFF);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  Putbmp565(x - 4, y + 30, DATAPATH "bmp\\login.565");

  rectangleEx(x + 120 - 1, y + 40 - 1, 771, 501, 0x6BAF, 1, 1);
  fillRegionEx(x + 120, y + 40, 770, 500, 0xFFFF);
  fillRegionEx(x + 120, y + 40, 770, 40, 0x03DF);
  linex_styleEx(x + 120, y + 80, 770, 0x6BAF, 1, 1);

  printTextLineXY(x + 96, y + 12, "- 用户注册", _h1);
  printTextLineXY(x + 125, y + 53, "请输入基本信息：", _h2);
  printTextLineXY(x + 192, y + 103, "姓名：", _h1);
  printTextLineXY(x + 562, y + 103, "性别：", _h1);
  printTextLineXY(x + 530, y + 138, "（请在此栏中输入“男”或“女”）", _h1);
  printTextLineXY(x + 176, y + 174, "婚姻史：", _h1);
  printTextLineXY(x + 155, y + 209, "（请在此栏中输入“未婚”“已婚”或“离异”）", _h1);
  printTextLineXY(x + 530, y + 174, "身份证号：", _h1);
  printTextLineXY(x + 160, y + 245, "出生年月：", _h1);
  printTextLineXY(x + 332, y + 245, "年", _h1);
  printTextLineXY(x + 422, y + 245, "月", _h1);
  printTextLineXY(x + 530, y + 245, "电话号码：", _h1);
  printTextLineXY(x + 192, y + 307, "住址：", _h1);
  printTextLineXY(x + 546, y + 307, "过敏史：", _h1);
  printTextLineXY(x + 176, y + 364, "用户名：", _h1);
  printTextLineXY(x + 192, y + 421, "密码：", _h1);
  printTextLineXY(x + 530, y + 421, "确认密码：", _h1);
  repaintChildren(win, value);
  freeFont(_h1);
  freeFont(_h2);
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

    case ID_SIGNIN_RETURN:
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

    case ID_SIGNIN_NAME:
    case ID_SIGNIN_SEX:
    case ID_SIGNIN_MARRIAGESTATUS:
    case ID_SIGNIN_IDNUMBER:
    case ID_SIGNIN_BIRTH_YEAR:
    case ID_SIGNIN_BIRTH_DAY:
    case ID_SIGNIN_TELENUM:
    case ID_SIGNIN_ADDRESS:
    case ID_SIGNIN_ALLERGY:
    case ID_SIGNIN_USERNAME:
    case ID_SIGNIN_PASSWORD1:
    case ID_SIGNIN_PASSWORD2:
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
      break;

    case ID_SIGNIN_SIGNIN:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;
      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标释放
        hfont *_h = getFont(SIMSUN, 16, 0);
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        fillRegionEx(win->x + 125, win->y + 445, 760, 38, 0xFFFF);
        printTextLineXY(PAGE_W / 2 - 32, win->y + 455, "注册成功！", _h);
        freeFont(_h);
        delay(1200);
        //转跳homepage
        if (win->parent && win->parent->winID == ID_DESKTOP) //找到desktop
        {
          _g->activePageID = ID_LOGINPAGE;
          win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
        }
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