#include "macrodef.h"
#include "mouse.h"
#include "hglobal.h"
#include "hhosvga.h"
#include "wdesktop.h"
#include "HBaseWin.h"
#include "wResource.h"
#include "hlabel.h"
#include "hyperlnk.h"
#include "hbutton.h"
#include "hinput.h"

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
  linex_styleEx(0, HEADER_HEIGHT - 44, SCR_WIDTH, 0x4A96, 3, 1);

  repaintChildren(win);

  //draw footer
  linex_styleEx(0, SCR_HEIGHT - FOOTER_HEIGHT, SCR_WIDTH, 0x4A96, 3, 1);
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
  CreateButton(desktop, 450, HEADER_HEIGHT - 46, 120, 44, ID_MENU_HOMEPAGE, "首页");
  CreateButton(desktop, 600, HEADER_HEIGHT - 46, 150, 44, ID_MENU_TESTPAGE, "测试页");
  CreateButton(desktop, 850, HEADER_HEIGHT - 46, 150, 44, ID_MENU_EXIT, "退出");

  Createhyperlink(desktop, 900, SCR_HEIGHT - FOOTER_HEIGHT / 2, 100, 30, ID_MENU_PY, "中文");

  //创建首页
  CreateHomepage(desktop, ID_HOMEPAGE);

  desktop->onPaint = OnPaint_Desktop;
  desktop->EventHandler = eventhandlerdesktop;

  return desktop;
}

/**
 * Desktop专用函数，为desktop创建页面
 * 
 */
hbasewinAttr *pageFactory(hbasewinAttr *desktop, int winID)
{ //page的创建函数名称不一样、参数不一样，但返回值一样，本函数对外统一函数，创建page
  hbasewinAttr *newpage = NULL;

  TESTNULL(desktop, NULL);
  switch (winID)
  {
  case ID_HOMEPAGE:
    newpage = CreateHomepage(desktop, ID_HOMEPAGE);
    break;

  case ID_TESTPAGE:
    newpage = CreateTestPage(desktop, ID_TESTPAGE, "测试页面-建设中.....");
    break;
  /*
   * case :
   * case : 
   */
  default:
    break;
  }
  return newpage;
}
/**
 * Desktop专用函数，为desktop切换页面
 * 
 */
void Desktop_changePage(hbasewinAttr *desktop, int pageID, hbasewinAttr *activePage)
{
  globaldef *_g;
  list_node_t *node;
  hbasewinAttr *py;

  TESTNULLVOID(desktop);
  if (activePage && activePage->winID != pageID)
  { //当前活动page不是pageID页面，则删除其他页面
    clearWinRegion(activePage, 0xFFFF);
    if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
      _g->foucsedTextBox->onActivate(NULL, _g);

    if (activePage->onDestroy)
    {
      activePage->onDestroy(activePage, NULL);
    }
    activePage = NULL;
  }

  if (activePage == NULL)
  { //并创建homepage
    activePage = pageFactory(desktop, pageID);
    if (activePage == NULL)
    {
      TRACE(("%s(%d):Desktop_changePage(), 创建窗口失败！\n", __FILE__, __LINE__));
      return;
    }
    if (activePage->onPaint)
      activePage->onPaint(activePage, NULL);
  }

  // py = findWinByID(desktop, ID_MENU_PY);
  // if(py)
  //   hidePYInput(py);
}

/**
 * 桌面消息处理函数
 * 
 * @param win 处理消息窗口
 * @param type 事件类型
 * @param value 参数
 */
void eventhandlerdesktop(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  switch (type)
  {
  case EVENT_PAGE_CHANGE:
    //发送到desktop窗口
    Desktop_changePage(win, _g->activePageID, _g->activePage); //使用win
    break;
  case EVENT_MOUSE:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

    switch (win->winID)
    {
    case ID_MENU_TESTPAGE:

      /* 改变鼠标形状、改变背景颜色 */
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下,改变按钮样式
        if (win->onClick)
          win->onClick(win, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标弹起
        //切换页面
        if (win->onLeave)
          win->onLeave(win, NULL);
        Desktop_changePage(win->parent, ID_TESTPAGE, _g->activePage); //使用win->parent
      }

      break;
    case ID_MENU_HOMEPAGE:
      /* 改变鼠标形状、改变背景颜色 */
      //.....
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (win->onClick)
          win->onClick(win, NULL);       
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标弹起
        //切换页面
        if (win->onLeave)
          win->onLeave(win, NULL);
        Desktop_changePage(win->parent, ID_HOMEPAGE, _g->activePage);
      }
      break;
    case ID_MENU_EXIT:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标按下
        if (win->onClick)
          win->onClick(win, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标弹起
        //切换页面
        _g->isExit = TRUE;
      }
      break;
    default:
      break;
    }
  default:
    break;
  }
}

/*
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
      //点击首页按钮
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
    case ID_MENU_TESTPAGE: //点击测试页按钮
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
*/