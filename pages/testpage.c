
#include "macrodef.h"
#include "wResource.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "hglobal.h"

#include "testpage.h"
void EventHandler_testpage(hbasewinAttr *win, int type, void *value);

hbasewinAttr *CreateTestPage(hbasewinAttr *parent, int winID, char *title)
{
  hbasewinAttr *testPage = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, title);

  TESTNULL(testPage, NULL);
  testPage->onPaint = OnPaint_TestPage;
  testPage->EventHandler = EventHandler_testpage;

  Createhyperlink(testPage, 500, 20, 350, 30, ID_TEST_HYPERLINK, "�����ӣ�ת��--->Hompage!");

  return testPage;
}

void OnPaint_TestPage(hbasewinAttr *win, void *value)
{
  if (win->title)
  {
    hfont *_h = getFont(SIMKAI, 24, 0x403F);
    printTextLineXY(getAbsoluteX(win), getAbsoluteY(win) + 100, win->title, _h);
    freeFont(_h);
  }

  repaintChildren(win);
}

void EventHandler_testpage(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  //ȷ���Ƿ����Ӵ����¼������Ӵ����¼����ر���
  hitwin = checkmousewin(win, &_g->mouse);

  switch (type)
  {
  case EVENT_MOUSE:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

    switch (hitwin->winID)
    { //������Ǹ��Ӵ��ڻ���
    case ID_TEST_HYPERLINK:

      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��label1���ڲ�����ʾ�������
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //��갴��
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //��굯��
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        //ת��homepage
        if (win->parent && win->parent->winID == ID_DESKTOP) //�ҵ�desktop
        {
          _g->activePageID = ID_HOMEPAGE;
          win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
        }
      }
      break;
    default:
      break;
    }

    break;
  case EVENT_KEYBORAD:
    break;
  default:
    break;
  }
}