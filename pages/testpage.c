
#include "macrodef.h"
#include "wResource.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "hglobal.h"
#include "htextbox.h"

#include "testpage.h"
void EventHandler_testpage(hbasewinAttr *win, int type, void *value);

hbasewinAttr *CreateTestPage(hbasewinAttr *parent, int winID, char *title)
{
  hbasewinAttr *testPage = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, title);

  TESTNULL(testPage, NULL);
  testPage->onPaint = OnPaint_TestPage;
  testPage->EventHandler = EventHandler_testpage;
  testPage->onDestroy = OnDestroy_TestPage;

  Createhyperlink(testPage, 500, 20, 350, 30, ID_TEST_HYPERLINK, "�����ӣ�ת��--->Hompage!");
  CreateTextBox(testPage, 500, 100, 240, 35, ID_TEST_TEXTBOX, NULL);
  CreateTextBox(testPage, 500, 140, 240, 35, ID_TEST_TEXTBOX2, NULL);

  //CreateCheckBox(testPage, 500, 50, 100,100, ID_TEST_CHECKBOX, NULL);

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

  repaintChildren(win);
  (void)value;
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

    switch (hitwin->winID)
    { //������Ǹ��Ӵ��ڻ���
    case ID_TESTPAGE:
    {
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        //�޼���ڣ�ԭ��textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
    }
    break;
    case ID_TEST_HYPERLINK:
    {
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��label1���ڲ�����ʾ�������
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //��갴��
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //����ͷ�
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        //ת��homepage
        if (win->parent && win->parent->winID == ID_DESKTOP) //�ҵ�desktop
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
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //����ͷ�

        if (_g->foucsedTextBox == NULL)
          TRACE(("%s(%d)ID_TEST_TEXTBOX.......\n", __FILE__, __LINE__));
        else
        {
          TRACE(("%s(%d)type= %d, id = %u.......\n", __FILE__, __LINE__,
                 _g->foucsedTextBox->wintype, _g->foucsedTextBox->winID));
        }

        // if (_g->foucsedTextBox == NULL //�״μ���
        //     || _g->foucsedTextBox->winID != hitwin->winID)
        // {

        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
        // }
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