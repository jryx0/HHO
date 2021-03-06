
#include "macrodef.h"
#include "wResource.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "hglobal.h"
#include "htextbox.h"
#include "hchkbox.h"

#include "testpage.h"
void EventHandler_testpage(hbasewinAttr *win, int type, void *value);

hbasewinAttr *CreateTestPage(hbasewinAttr *parent, int winID, char *title)
{
  hbasewinAttr *testPage = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, title);
  hbasewinAttr *pwd;
  TESTNULL(testPage, NULL);
  testPage->onPaint = OnPaint_TestPage;
  testPage->EventHandler = EventHandler_testpage;
  testPage->onDestroy = OnDestroy_TestPage;

  Createhyperlink(testPage, 500, 20, 350, 30, ID_TEST_HYPERLINK, "超链接，转跳--->Homepage!");
  CreateTextBox(testPage, 500, 100, 240, 35, ID_TEST_TEXTBOX, "测a试", 1);
  CreateTextBox(testPage, 500, 140, 240, 150, ID_TEST_TEXTBOX2, "超a23链35接华中科技4545454大小超链接华中科技大小超链接华中科技大小超链接华中科技大小超链接华中科技大小", 0);

  pwd = CreateTextBox(testPage, 500, 290, 240, 100, ID_TEST_TEXTBOX2, "password", 1);
  pwd->wintype = TEXTBOX_PASSWORD;

  CreateCheckBox(testPage, 300, 50, 48, 48, ID_TEST_CHECKBOX, NULL);
  return testPage;
}

void OnDestroy_TestPage(hbasewinAttr *win, void *value)
{
  //TRACE(("%s(%d):OnDestroy_TestPage, %d\n", __FILE__, __LINE__, win->winID));

  OnDestory(win, NULL);
  (void)value;
}

void OnPaint_TestPage(hbasewinAttr *win, void *value)
{
  if (win->title)
  {
    hfont *_h = getFont(SIMKAI, 24, 0x403F);
    printTextLineXY(getAbsoluteX(win), getAbsoluteY(win) + 100, win->title, _h);
    freeFont(_h);
  }

  repaintChildren(win, value);

  //Putbmp565(win->x, win->y, "c:\\hho\\data\\bmp\\000.565");
  //Putbmp64k(win->x, win->y, "c:\\hho\\data\\bmp\\000.bmp");
  (void)value;
}

void EventHandler_testpage(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  //确定是否是子窗口事件，非子窗口事件返回本身
  hitwin = checkmousewin(win, &_g->mouse);

  switch (type)
  {
  case EVENT_MOUSE:
    switch (hitwin->winID)
    { //鼠标在那个子窗口或本身
    case ID_TESTPAGE:
    {
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        TRACE(("winID:%d\n", hitwin->winID));
        //无激活窗口，原有textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
    }
    break;
    case ID_TEST_HYPERLINK:
    {
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
    }
    break;
    case ID_TEST_TEXTBOX:
    case ID_TEST_TEXTBOX2:
    {
      TRACE(("winID:%d\n", hitwin->winID));
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //鼠标释放

        // if (_g->foucsedTextBox == NULL)
        //   TRACE(("%s(%d)ID_TEST_TEXTBOX.......\n", __FILE__, __LINE__));
        // else
        // {
        //   TRACE(("%s(%d)type= %d, id = %u.......\n", __FILE__, __LINE__,
        //          _g->foucsedTextBox->wintype, _g->foucsedTextBox->winID));
        // }

        // if (_g->foucsedTextBox == NULL //首次激活
        //     || _g->foucsedTextBox->winID != hitwin->winID)
        // {
        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
        // }
      }
    }
    break;
    case ID_TEST_CHECKBOX:
    {
      TRACE(("winID:%d\n", hitwin->winID));
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
        hitwin->onPaint(hitwin, NULL);
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