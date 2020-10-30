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
#include "lginpage.h"
#include "hnews.h"
#include "regspage.h"
#include "payregs.h"
#include "hdept.h"
#include "postpage.h"
#include "drugpage.h"
#include "paydrug.h"
#include "docpage.h"
#include "finddoc.h"
#include "payregs.h"
#include "result.h"
#include <memory.h>
#include <string.h>

#define INPUTWINHEIGHT 38

#define PYFONTSIZE 24
#define PYFONTTYPE SIMKAI
#define PYFONTCOLOR 0x7A96
#define PYZONEX 70
#define PYZONEY (SCR_HEIGHT - PYFONTSIZE - 10)
#define PYZONEWIDTH (PYFONTSIZE * 6 + 10)
#define PYZONEHEIGHT (PYFONTSIZE + 6)

#define HZZONEX PYZONEWIDTH + 40
#define HZZONEY PYZONEY + 2
#define HZZONEWIDTH (SCR_WIDTH - HZZONEX) - 1
#define HZZONEHEIGHT PYZONEHEIGHT

/**
 * 输入法专用汉字显示函数,在每个前10个汉字前加序号
 * 
 * 
 */
void printCandiateHZ(int x, int y, char *text, hfont *_font);

/**
 * 窗口绘制程序
 * 
 */
void OnPaint_Desktop(hbasewinAttr *win, void *val)
{
  hfont *_font;
  WinStyle *dskStyle = NULL;
  int footY = SCR_HEIGHT - FOOTER_HEIGHT;

  if (win == NULL)
    return;

  dskStyle = (WinStyle *)win->style;

  if (val == NULL)
  {
    clearScreen(COLOR_white);
    //draw header
    //Putbmp64k(0, 2, "c:\\hho\\data\\bmp\\hhologo.bmp");
    Putbmp565(0, 2, "c:\\hho\\data\\bmp\\hhologo.565");
    // linex_styleEx(0, HEADER_HEIGHT - 44, SCR_WIDTH, 0x4A96, 3, 1);
    fillRegionEx(0, HEADER_HEIGHT - 44, SCR_WIDTH, 44, dskStyle->bkcolor);
    repaintChildren(win, val);
  }
  else
  { //输入法窗口
    globaldef *_g = (globaldef *)val;
    fillRegion(0, footY - INPUTWINHEIGHT, 1024, 768, 0xFFFF);
    if (_g->InputMode == CHINESE)
    {
      footY -= INPUTWINHEIGHT; //foot 高度增加35

      fillRegion(0, footY + INPUTWINHEIGHT + 3, 1024, 768, 0xCE59);
      rectangle(0, footY + INPUTWINHEIGHT + 3, 1024, 768, 0x7A96, 2, 1);
      linexEx(2, footY + INPUTWINHEIGHT + 3 + 2, 1023, 0xFFFF);
      lineyEx(2, footY + INPUTWINHEIGHT + 3 + 2, INPUTWINHEIGHT, 0xFFFF);

      liney_styleEx(2 + 50, footY + INPUTWINHEIGHT + 3 + 3, INPUTWINHEIGHT, 0xCE59, 1, 1);
      liney_styleEx(2 + 51, footY + INPUTWINHEIGHT + 3 + 3, INPUTWINHEIGHT, 0x8410, 1, 1);
      liney_styleEx(2 + 52, footY + INPUTWINHEIGHT + 3 + 3, INPUTWINHEIGHT, 0xFFFF, 1, 1);
      liney_styleEx(2 + 53, footY + INPUTWINHEIGHT + 3 + 3, INPUTWINHEIGHT, 0xDEDB, 1, 1);
      liney_styleEx(2 + 54, footY + INPUTWINHEIGHT + 3 + 3, INPUTWINHEIGHT, 0xCE59, 1, 1);

      _font = getFont(SIMHEI, 16, 0x0000);
      printTextLineXY(10, footY + INPUTWINHEIGHT + 3 + 1 + 12, "中文", _font);
      freeFont(_font);

      // _font = getFont(SIMKAI, 24, 0x0000);
      // printTextLineXY(10, footY + INPUTWINHEIGHT + 5 + 1 + 12, _g->pystring, _font);
      // freeFont(_font);
    }
  }

  //draw footer
  linex_styleEx(0, footY, SCR_WIDTH, dskStyle->bkcolor, 3, 1);
  _font = getFont(dskStyle->fontsize, 16, 0x0000);
  //_font->fontcolor = 0x4228;
  printTextLineXY(215, footY + 15, "地址：湖北省武汉市洪山区珞喻路1037号      咨询电话:027-87543469", _font);
  freeFont(_font);

  (void)val;
}

void OnKeyPress_Desktop(hbasewinAttr *win, void *val)
{
  globaldef *_g;
  hfont *_font;
  //int footY = SCR_HEIGHT - FOOTER_HEIGHT;

  TESTNULLVOID(win);
  TESTNULLVOID(val);

  _g = (globaldef *)val;

  if (_g->InputMode == CHINESE)
  {

    fillRegionEx(PYZONEX, PYZONEY, PYZONEWIDTH, PYZONEHEIGHT, 0xCE59);

    _font = getFont(PYFONTTYPE, PYFONTSIZE, PYFONTCOLOR);
    printTextLineXY(PYZONEX, PYZONEY, _g->pystring, _font);

    fillRegionEx(HZZONEX, HZZONEY, SCR_WIDTH - HZZONEX, PYZONEHEIGHT, 0xCE59);
    if (_g->hzstring)
      printCandiateHZ(HZZONEX, HZZONEY, _g->hzstring, _font);

    freeFont(_font);
  }
}

/**
 * 输入法专用汉字显示函数,在每个前10个汉字前加序号
 * 
 * 
 */
void printCandiateHZ(int x, int y, char *text, hfont *_font)
{
  int x0, y0;                //每个汉字起始点
  unsigned char quma, weima; //定义汉字的区码和位码
  unsigned long offset;
  unsigned char *buffer;
  char index = '1';
  int i = 0;
  //int linenum = 0;
  //char isNewLine = FALSE;

  TESTNULLVOID(_font);

  buffer = (unsigned char *)malloc(_font->totalbytes);
  TESTNULL(buffer, );

  x0 = x;
  while (*text)
  {
    y0 = y;

    if (i < 9)
    {
      printASC(x0, y0 - _font->ascy, (char)(index + i), _font); //输出单个字符,非打印字符用空格替代
      x0 += _font->ascSize + _font->xgap;                       //偏移一个字符宽度+字间距
    }
    else if (i == 9)
    {
      printASC(x0, y0 - _font->ascy, '0', _font); //输出单个字符,非打印字符用空格替代
      x0 += _font->ascSize + _font->xgap;
    }
    else
    {
      break;
    }

    printASC(x0, y0 - _font->ascy, '.', _font); //输出单个字符,非打印字符用空格替代
    x0 += _font->ascSize + _font->xgap;
    i++;

    if (((unsigned char)text[0] >= 0xa0) &&
        ((unsigned char)text[1] >= 0xa0))
    {                                                            //打印中文                                                            //汉字
      quma = text[0] - 0xa1;                                     //求出区码
      weima = text[1] - 0xa1;                                    //求出位码
      offset = (94L * (quma) + (weima)) * _font->totalbytes;     //求出要显示的汉字在字库文件中的偏移
      fseek(_font->fpCurrentFont, offset, SEEK_SET);             //重定位文件指针
      fread(buffer, _font->totalbytes, 1, _font->fpCurrentFont); //读出该汉字的具体点阵数据,1为要读入的项数

      printHZWord(x0, y0, buffer, _font);         //输出单个汉字
      x0 += _font->currentFontSize + _font->xgap; //偏移一个汉字宽度+字间距
      text += 2;                                  //下一个汉字
    }
    x0 += 5;
  }
  free(buffer);
}

/**
 * 创建主窗口
 * 
 */
hbasewinAttr *CreateDesktop(void)
{
  hbasewinAttr *desktop;
  hbasewinAttr *child;
  WinStyle *dskStyle;

  desktop = CreateWindowsEx(NULL, 0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1, ID_DESKTOP, "desktop");
  desktop->wintype = DESKTOP;

  //登陆状态
  Createhyperlink(desktop, 900, (HEADER_HEIGHT - 44) / 2 - 10, 100, 25, ID_MENU_LOGIN, "[登  录]");
  //创建菜单,切换页面,临时使用
  CreateButton(desktop, 400, HEADER_HEIGHT - 44, 120, 44, ID_MENU_HOMEPAGE, "首页");
  CreateButton(desktop, 520, HEADER_HEIGHT - 44, 150, 44, ID_MENU_TESTPAGE, "挂号");
  CreateButton(desktop, 670, HEADER_HEIGHT - 44, 150, 44, ID_MENU_DEPTPAGE, "科室简介");
  CreateButton(desktop, 900, HEADER_HEIGHT - 44, 120, 44, ID_MENU_EXIT, "退出");

  //创建首页
  CreateHomepage(desktop, ID_HOMEPAGE);

  desktop->onPaint = OnPaint_Desktop;
  desktop->EventHandler = eventhandlerdesktop;
  desktop->onKeyPress = OnKeyPress_Desktop;

  dskStyle = malloc(sizeof(WinStyle));
  dskStyle = getWinTheme(dskStyle, 1);
  desktop->style = dskStyle;

  return desktop;
}

/**
 * Desktop专用函数，为desktop创建页面
 * 
 */
hbasewinAttr *pageFactory(hbasewinAttr *desktop, int winID, globaldef *_g)
{ //page的创建函数名称不一样、参数不一样，但返回值一样，本函数对外统一函数，创建page
  hbasewinAttr *newpage = NULL;

  TESTNULL(desktop, NULL);
  switch (winID)
  {
  case ID_HOMEPAGE:
    newpage = CreateHomepage(desktop, ID_HOMEPAGE);
    break;

  case ID_TESTPAGE:
    //newpage = CreateTestPage(desktop, ID_TESTPAGE, "测试页面-建设中.....");
    newpage = Createregisterpage(desktop, ID_REGISTERPAGE, _g->userid);
    break;

  case ID_LOGINPAGE:
    newpage = Createloginpage(desktop, ID_LOGINPAGE);
    break;
  case ID_NEWSPAGE:
  {
    char title[40];
    sprintf(title, "%s\\news%02d.txt", NEWSPATH, _g->data);
    newpage = CreateNewspage(desktop, ID_NEWSPAGE, title);
  }
  break;
  case ID_DEPTPAGE:
    newpage = CreateDeptpage(desktop, ID_DEPTPAGE, "科室简介");
    break;
  case ID_REGISTERPAGE:
    newpage = Createregisterpage(desktop, ID_REGISTERPAGE, _g->userid);
    break;
  case ID_POSTPAGE:
    if (_g->isLogin)
    {
      if (_g->usertype == LOGISTIC)
        newpage = CreatePostpage(desktop, ID_POSTPAGE, -1); //物流任意
      else if (_g->usertype == PATIENT)
        newpage = CreatePostpage(desktop, ID_POSTPAGE, _g->userid); //普通患者
      else
        newpage = CreateHomepage(desktop, ID_HOMEPAGE); //其他人转跳homepage   //CreateWindowsEx(desktop, ID_DENYPAGE, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, "未授权登录");
    }
    break;
  case ID_DRUGPAGE:
    if (_g->isLogin)
    {
      if (_g->usertype == PHARMCY)
        newpage = CreateDrugpage(desktop, ID_DRUGPAGE, -1); //进入发药管理 id = -1  可以看到全部药方
      else if (_g->usertype == PATIENT)
        newpage = CreateDrugpage(desktop, ID_DRUGPAGE, _g->userid); //进入药房取药 id = userid
      else
        newpage = CreateHomepage(desktop, ID_HOMEPAGE); //CreateWindowsEx(desktop, ID_DENYPAGE, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, "未授权登录");
    }
    break;
  case ID_PAYPSPAGE:
    if (_g->isLogin)
    {
      if (_g->usertype == PATIENT)
        newpage = CreatePayDrugpage(desktop, ID_PAYPSPAGE, _g->userid);
      else
        newpage = CreateHomepage(desktop, ID_HOMEPAGE);
    }
    break;
  case ID_DOCPAGE:
    if (_g->isLogin)
    {
      if (_g->usertype == DOCTOR)
        newpage = CreateDocPage(desktop, ID_DOCPAGE, _g->userid);
      else
        newpage = CreateHomepage(desktop, ID_HOMEPAGE);
    }
    break;
  case ID_PAYREGSPAGE:
    newpage = CreatePayRegspage(desktop, ID_PAYREGSPAGE, _g->userid);
    break;
  case ID_RESULTPAGE:
    newpage = CreateResultPage(desktop, ID_RESULTPAGE, _g->userid);
    break;

  case ID_FINDDOCPAGE:
    newpage = CreateFindDocpage(desktop, ID_FINDDOCPAGE, _g->userid);
    break;
  default:
    break;
  }
  return newpage;
}
/**
 * Desktop专用函数，为desktop切换页面
 * 
 */
void Desktop_changePage(hbasewinAttr *desktop, int pageID, globaldef *_g)
{
  //globaldef *_g;
  hbasewinAttr *activePage = _g->activePage;
  list_node_t *node;
  hbasewinAttr *login;

  TESTNULLVOID(desktop);
  if (activePage && activePage->winID != pageID)
  { //当前活动page不是pageID页面，则删除其他页面
    //clearWinRegion(activePage, 0xFFFF);
    clearRegion(0, PAGE_Y, 1023, PAGE_H + HEADER_HEIGHT, 0xFFFF);
    // if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
    //   _g->foucsedTextBox->onActivate(NULL, _g);

    if (activePage->onDestroy)
    {
      activePage->onDestroy(activePage, NULL);
    }
    activePage = NULL;
  }

  if (activePage == NULL)
  { //并创建homepage
    activePage = pageFactory(desktop, pageID, _g);
    //页面间数据传递
    activePage->data = _g->data;
    if (activePage == NULL)
    {
      TRACE(("%s(%d):Desktop_changePage(), 创建窗口失败！\n", __FILE__, __LINE__));
      return;
    }
    if (activePage->onPaint)
      activePage->onPaint(activePage, &_g->theme);

    _g->activePage = activePage;
    activePage->onTheme(activePage, &_g->theme);
  }
  _g->foucsedTextBox = NULL;

  if (_g->InputMode == CHINESE)
  {
    _g->InputMode = ENGLISH;
    closePY(desktop, _g);
  }

  login = findWinByID(desktop, ID_MENU_LOGIN);
  if (_g->isLogin == 1)
    strcpy(login->title, "[注  销]");
  else
    strcpy(login->title, "[登  录]");
  login->onPaint(login, NULL);
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
    //焦点textbox 取消焦点
    Desktop_changePage(win, _g->activePageID, _g);
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
        Desktop_changePage(win->parent, ID_REGISTERPAGE, _g);
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

        Desktop_changePage(win->parent, ID_HOMEPAGE, _g); //->activePage);
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

    case ID_MENU_LOGIN:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //鼠标弹起
        //切换页面
        if (win->onClick)
          win->onClick(win, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标弹起
        //切换页面
        if (win->onLeave)
          win->onLeave(win, NULL);

        if (_g->isLogin == 0)
          Desktop_changePage(win->parent, ID_LOGINPAGE, _g); //->activePage);
        else
        {
          _g->isLogin = 0;
          Desktop_changePage(win->parent, ID_HOMEPAGE, _g);
        }
      }
      break;
    case ID_MENU_DEPTPAGE:
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

        Desktop_changePage(win->parent, ID_DEPTPAGE, _g); //->activePage);
      }
      break;
    default:
      break;
    }
  default:
    break;
  }
}

void closePY(hbasewinAttr *win, globaldef *_g)
{
  if (_g->InputMode == ENGLISH)
  {
    if (win->winID == ID_DESKTOP)
      win->onPaint(win, _g);

    memset(_g->pystring, 0, 6);
    _g->hzstring = NULL;
    _g->pyNum = 0;
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