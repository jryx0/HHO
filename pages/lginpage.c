#include "lginpage.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "Resource.h"
#include "htextbox.h"
#include "hbutton.h"
#include "hyperlnk.h"
#include "hglobal.h"
#include "macrodef.h"
#include "data.h"

hbasewinAttr *Createloginpage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "loginpage");
  hbasewinAttr *pwd;
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_loginpage;
  page->EventHandler = EventHandler_loginpage;

  CreateTextBox(page, PAGE_W / 2 - 100, PAGE_H / 2 - 67, 240, 40, ID_LOGIN_USERNAME, "", 1);
  pwd = CreateTextBox(page, PAGE_W / 2 - 100, PAGE_H / 2 + 3, 240, 40, ID_LOGIN_KEY, "", 1);
  pwd->wintype = TEXTBOX_PASSWORD;
  Createhyperlink(page, 20, 10, 65, 25, ID_LOGIN_RETURN, "[首 页]");
  CreateButton(page, PAGE_W / 2 + 30, PAGE_H - 200, 120, 40, ID_LOGIN_LOGIN, "登 录");
  CreateButton(page, PAGE_W / 2 - 120, PAGE_H - 200, 120, 40, ID_LOGIN_REGISTER, "注 册");

  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);
  return page;
}

void OnPaint_loginpage(hbasewinAttr *win, void *value)
{
  //hfont *_h = getFont(SIMSUN, 24, 0);
  WinStyle *style;
  hfont *_h;
  int x, y;
  TESTNULLVOID(win);
  TESTNULLVOID(win->style);

  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  Putbmp565(x - 4, y + 30, DATAPATH "bmp\\login.565");

  linex_styleEx(x + PAGE_W / 2 - 200 - 30, y + PAGE_H / 2 - 100, 450, 0x6BAF, 1, 1);
  rectangleEx(x + PAGE_W / 2 - 200 - 30 - 1, y + PAGE_H / 2 - 100 - 40 - 1, 451, 301, 0x6BAF, 1, 1);
  fillRegionEx(x + PAGE_W / 2 - 200 - 30, y + PAGE_H / 2 - 100 - 40, 450, 300, 0xFFFF);
  fillRegionEx(x + PAGE_W / 2 - 200 - 30 + 1, y + PAGE_H / 2 - 100 - 40 + 1, 449, 39, style->bkcolor2);

  printTextLineXY(x + PAGE_W / 2 - 200, y + PAGE_H / 2 - 60, "用户名：", _h);
  printTextLineXY(x + PAGE_W / 2 - 200, y + PAGE_H / 2 + 10, "密  码：", _h);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- 用户登录", _h);

  _h->fontcolor = 0xFFFF;
  printTextLineXY(x + PAGE_W / 2 - 200 - 30 + 5, y + PAGE_H / 2 - 100 - 40 + 13, "请输入用户名和密码:", _h);

  repaintChildren(win, value);
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
      { //无激活窗口，原有textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;

    case ID_LOGIN_USERNAME:
    case ID_LOGIN_KEY:
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
      break;

    case ID_LOGIN_RETURN:
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

    case ID_LOGIN_LOGIN:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标释放
        userInfo *infotemp;
        hbasewinAttr *username, *password;
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);
        username = findWinByID(win, ID_LOGIN_USERNAME);
        password = findWinByID(win, ID_LOGIN_KEY);
        if (username && password)
        {
          infotemp = fFindUserInfo(USERINFOFILE, username->title, password->title);
          if (infotemp != NULL)
          {
            _g->userid = infotemp->userID;
            _g->usertype = infotemp->userType;
            _g->isLogin = 1;
            switch (_g->usertype)
            {
            case PATIENT:
              _g->activePageID = _g->beforeloginPageID;
              break;
            case DOCTOR:
              _g->activePageID = ID_DOCPAGE;
              break;
            case PHARMCY:
              _g->activePageID = ID_DRUGPAGE;
              break;
            case LOGISTIC:
              _g->activePageID = ID_POSTPAGE;
              break;
            default:
              _g->activePageID = ID_HOMEPAGE;
              break;
            }
            win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
            free(infotemp);
          }
          else
          {
            hfont *_h = getFont(SIMSUN, 16, 0xF801);
            printTextLineXY(PAGE_W / 2 - 60, PAGE_H - 100, "用户名或密码错误", _h);
            freeFont(_h);
          }
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
}