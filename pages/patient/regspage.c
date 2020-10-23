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

  Createhyperlink(page, 20, 10, 65, 25, ID_REGISTER_RETURN, "[�� ҳ]");
  CreateButton(page, 20, 70, 100, 35, ID_REGISTER_DEPT_MEDICINE, "�ڿ�"); //�ڿơ���ơ����ơ����ơ��ۿơ����Ǻ�ơ���ǻ�ơ�Ƥ����
  CreateButton(page, 150, 70, 100, 35, ID_REGISTER_DEPT_SURGERY, "���");
  CreateButton(page, 20, 120, 100, 35, ID_REGISTER_DEPT_PAEDIATRICS, "����");
  CreateButton(page, 150, 120, 100, 35, ID_REGISTER_DEPT_GYNAECOLOGY, "����");
  CreateButton(page, 20, 170, 100, 35, ID_REGISTER_DEPT_OPHTHALMOLOGY, "�ۿ�");
  CreateButton(page, 150, 170, 100, 35, ID_REGISTER_DEPT_ENT, "���Ǻ��");
  CreateButton(page, 20, 220, 100, 35, ID_REGISTER_DEPT_STOMATOLOGY, "��ǻ��");
  CreateButton(page, 150, 220, 100, 35, ID_REGISTER_DEPT_DERMATOLOGY, "Ƥ����");

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
  printTextLineXY(x + style->fontsize * 6, y + 10, "- �Һ�", _h);
  printTextLineXY(x + 16, y + 45, "��ѡ����ң�", _h);

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
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //�޼���ڣ�ԭ��textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;
    case ID_REGISTER_RETURN:
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
      break;

    case ID_REGISTER_DEPT_MEDICINE:
    case ID_REGISTER_DEPT_SURGERY:
    case ID_REGISTER_DEPT_PAEDIATRICS:
    case ID_REGISTER_DEPT_GYNAECOLOGY:
    case ID_REGISTER_DEPT_OPHTHALMOLOGY:
    case ID_REGISTER_DEPT_ENT:
    case ID_REGISTER_DEPT_STOMATOLOGY:
    case ID_REGISTER_DEPT_DERMATOLOGY:
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
      break;

    default:
      break;
    }
  default:
    break;
  }
}