#include "macrodef.h"
#include "mouse.h"
#include "hglobal.h"
#include "hhosvga.h"
#include "wdesktop.h"
#include "HBaseWin.h"
#include "wResource.h"
#include "hlabel.h"

#include "testpage.h"
#include "homepage.h"

/**
 * 窗口绘制程序
 * 
 */
void OnPaint_Desktop(hbasewinAttr *win, void *val)
{
  hfont *_font;

  if (win == NULL)
    return;

  clearScreen(COLOR_white);
  //draw header
  Putbmp64k(0, 2, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linex_styleEx(0, HEADER_HEIGHT, SCR_WIDTH, 0x2104, 2, 1);

  repaintChildren(win);

  //draw footer
  linex_styleEx(0, SCR_HEIGHT - FOOTER_HEIGHT, SCR_WIDTH, 0x2104, 2, 1);
  _font = getFont(SIMSUN, 16, 0x00);
  _font->fontcolor = 0x4228;
  printTextLineXY(215, SCR_HEIGHT - FOOTER_HEIGHT + 15, "地址：湖北省武汉市洪山区珞喻路1037号      咨询电话:027-87543469", _font);
  freeFont(_font);

  (void)val;
}

/**
 * 创建主窗口
 * 
 */
hbasewinAttr *CreateDesktop(void)
{
  hbasewinAttr *desktop;

  desktop = CreateWindowsEx(NULL, 0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1, ID_DESKTOP, "desktop");

  //创建菜单,切换页面,临时使用
  CreateLabel(desktop, 450, HEADER_HEIGHT - 26, 70, 25, ID_MENU_HOMEPAGE, NULL);
  CreateLabel(desktop, 570, HEADER_HEIGHT - 26, 70, 25, ID_MENU_TESTPAGE, NULL);

  //创建首页
  CreateHomepage(desktop, ID_HOMEPAGE);

  desktop->onPaint = OnPaint_Desktop;
  desktop->EventHandler = eventhandlerdesktop;

  return desktop;
}

void Desktop_MouseHandler(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  if (win->winID == ID_DESKTOP) //desktop本身无其他处理
    return;

  if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
  { //鼠标按键弹起
    switch (win->winID)
    {
    case ID_MENU_HOMEPAGE:

      if (_g->activePage && _g->activePage->winID != ID_HOMEPAGE)
      { //当前活动page不是homepage页面，则删除其他页面
        clearWinRegion(_g->activePage, 0xFFFF);
        _g->activePage->onDestroy(_g->activePage, NULL);

        _g->activePage = NULL;
      }

      if (_g->activePage == NULL)
      { //并创建homepage
        _g->activePage = CreateHomepage(win->parent, ID_HOMEPAGE);
        if (_g->activePage == NULL)
        {
          TRACE(("%s(%d):eventhandlerdesktop(), 创建窗口失败！\n"));
          return NULL;
        }
        if (_g->activePage->onPaint)
          _g->activePage->onPaint(_g->activePage, NULL);
      }

      break;
    case ID_MENU_TESTPAGE:
      if (_g->activePage && _g->activePage->winID != ID_TESTPAGE)
      { //当前活动page不是homepage页面，则删除其他页面
        clearWinRegion(_g->activePage, 0xFFFF);
        _g->activePage->onDestroy(_g->activePage, NULL);
        _g->activePage = NULL;
      }

      if (_g->activePage == NULL)
      { //创建homepage

        _g->activePage = CreateTestPage(win->parent, ID_TESTPAGE, "测试页面-建设中.....");
        if (_g->activePage == NULL)
        {
          TRACE(("%s(%d):eventhandlerdesktop(), 创建窗口失败！\n"));
          return NULL;
        }

        if (_g->activePage->onPaint)
          _g->activePage->onPaint(_g->activePage, NULL);
      }
      break;
    default:
      break;
    }
  }
}

void eventhandlerdesktop(hbasewinAttr *win, int type, void *value)
{
  switch (type)
  {
  case EVENT_MOUSE:
    Desktop_MouseHandler(win, type, value);
  default:
    break;
  }
}
