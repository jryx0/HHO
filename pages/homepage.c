#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "hglobal.h"
#include "homepage.h"
#include "wResource.h"
#include "hyperlnk.h"
#include "htextbox.h"
#include "hbutton.h"
#include <string.h>
#include <stdio.h>

void Homepage_MouseHandler(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  //ȷ���Ƿ����Ӵ����¼������Ӵ����¼����ر���
  hitwin = checkmousewin(win, &_g->mouse);

  switch (hitwin->winID)
  {                 //������Ǹ��Ӵ��ڻ���
  case ID_HOMEPAGE: //����
    if (type == EVENT_MOUSE)
    {                                                                              //homepage ��������ƶ�������ƶ�
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
    }
    break;
  case ID_HOMEPAGE_BUTTON1:
  case ID_HOMEPAGE_BUTTON2:
  case ID_HOMEPAGE_BUTTON3:
  case ID_HOMEPAGE_BUTTON4:
  case ID_HOMEPAGE_LINK1:
  case ID_HOMEPAGE_LINK2:
  case ID_HOMEPAGE_LINK3:
  case ID_HOMEPAGE_LINK4:
  case ID_HOMEPAGE_LINK5:
  case ID_HOMEPAGE_LINK6:
  case ID_HOMEPAGE_LINK7:
  case ID_HOMEPAGE_LINK8:
    /*���Ź���*/
    if (type == EVENT_MOUSE)
    {                                                                             //homepage ��������ƶ�������ƶ�
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��homepage���ڲ�����ʾ��׼���
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
          _g->activePageID = ID_NEWSPAGE;
          _g->data = hitwin->winID; //ҳ��֮�����ݽ���
          win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
        }
      }
    }
    break;
  case ID_HOMEPAGE_AISEARCH:
    if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      if (hitwin->onActivate)
        hitwin->onActivate(hitwin, _g);
    break;
  case ID_HOMEPAGE_CHARGE1:
  case ID_HOMEPAGE_CHARGE2:
  case ID_HOMEPAGE_DIAGNOSE:
  case ID_HOMEPAGE_RESULT:
  case ID_HOMEPAGE_DRUG:
  case ID_HOMEPAGE_LOGISTICS:
  case ID_HOMEPAGE_REGS:
    /*��������*/
    if (type == EVENT_MOUSE)
    {                                                                             //homepage ��������ƶ�������ƶ�
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��homepage���ڲ�����ʾ��׼���
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

        if (_g->isLogin == 1)
        {
          /*ת��ҳ��*/
          //homepage��˳�� 50 - 55
          if (win->parent && win->parent->winID == ID_DESKTOP) //�ҵ�desktop
          {
            _g->activePageID = ID_HOMEPAGE;
            if (hitwin->winID == ID_HOMEPAGE_LOGISTICS)
              _g->activePageID = ID_POSTPAGE; //����ҳ�� ֻ���� �����Ͳ��˲鿴
            else if (hitwin->winID == ID_HOMEPAGE_DRUG)
              _g->activePageID = ID_DRUGPAGE; //ҩ��ҳ�� ֻ���� ���˺�ҩ���鿴
            else if (hitwin->winID == ID_HOMEPAGE_CHARGE2)
              _g->activePageID = ID_PAYPSPAGE; //�����ɷ�ҳ�� ֻ�����˲鿴
            else if (hitwin->winID == ID_HOMEPAGE_DIAGNOSE)
              _g->activePageID = ID_DOCPAGE; //ֻ����ҽ���鿴
            else if (hitwin->winID == ID_HOMEPAGE_REGS)
              _g->activePageID = ID_FINDDOCPAGE;

            win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
          }
        }
        else
        {
          /*ת����¼ҳ��*/
          if (win->parent && win->parent->winID == ID_DESKTOP) //�ҵ�desktop
          {
            _g->activePageID = ID_LOGINPAGE;
            win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
          }
        }
      }
    }
    break;

    /*
  case ID_LABEL_1: //label1
    if (type == EVENT_MOUSE)
    {                                                                             //label1 ��������ƶ�������ƶ�
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��label1���ڲ�����ʾ�������
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //�ı���ʾ �������ͷ�
        if (hitwin && hitwin->onPaint)
          hitwin->onPaint(hitwin, "c:\\hho\\data\\news\\1.txt");
      }
    }
    break;

  case ID_LABEL_2: //label2
    if (type == EVENT_MOUSE)
    {                                                                             //label1 ��������ƶ�������ƶ�
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��label1���ڲ�����ʾ�������
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //�ı���ʾ
        if (hitwin && hitwin->onPaint)
          hitwin->onPaint(hitwin, "main.c");
      }
    }
    break;
  case ID_LABEL_3: //label3
    if (type == EVENT_MOUSE)
    { //���ı������״
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        list_node_t *n;
        //ɾ��label
        int x = getAbsoluteX(hitwin);
        int y = getAbsoluteY(hitwin);
        fillRegionEx(x, y, hitwin->nWidth + 1, hitwin->nHeight + 1, 0xFFFF); //����Ӵ�������

        TRACE(("%s(%d): ɾ��label%u\n", __FILE__, __LINE__, hitwin->winID));
        hitwin->onDestroy(hitwin, NULL);
      }
    }
    break;*/
  default:
    break;
  }
}

void EventHandler_homepage(hbasewinAttr *win, int type, void *value)
{
  //globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);
  //_g = (globaldef *)value;

  switch (type)
  {
  case EVENT_MOUSE:
    Homepage_MouseHandler(win, type, value);
    break;
  case EVENT_KEYBORAD:
    break;
  default:
    break;
  }
}

hbasewinAttr *CreateHomepage(hbasewinAttr *parent, int winID)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "homepage");
  hbasewinAttr *ctrl;
  TESTNULL(page, NULL);

  ctrl = Createhyperlink(page, 310, 40, 450, 30, ID_HOMEPAGE_LINK1, "УҽԺְ�����ǿ������������ ���ڹ� ������ ����ɫ");
  ctrl->data = ID_HOMEPAGE_LINK1;
  ctrl = Createhyperlink(page, 310, 40 + 26, 450, 30, ID_HOMEPAGE_LINK2, "�������¹ڷ��� ���д����ж���УҽԺ��ʱ�����Ŷ�Ϊһ����Ա�ṩ��ʵ����");
  ctrl->data = ID_HOMEPAGE_LINK2;
  ctrl = Createhyperlink(page, 310, 40 + 26 * 2, 450, 30, ID_HOMEPAGE_LINK3, "��ս�߼���һ�ߡ�УҽԺ����������ҩ���ң���ʦ������������");
  ctrl->data = ID_HOMEPAGE_LINK3;
  ctrl = Createhyperlink(page, 310, 40 + 26 * 3, 450, 30, ID_HOMEPAGE_LINK4, "��ս�߼���һ�ߡ�УҽԺΪ��ְ����ȫ���ڱ��ݻ���");
  ctrl->data = ID_HOMEPAGE_LINK4;
  ctrl = Createhyperlink(page, 310, 40 + 26 * 4, 450, 30, ID_HOMEPAGE_LINK5, "УҽԺ���Ͻ�������������ѧ���¸���");
  ctrl->data = ID_HOMEPAGE_LINK5;
  ctrl = Createhyperlink(page, 310, 40 + 26 * 5, 450, 30, ID_HOMEPAGE_LINK6, "УҽԺů�ķ���߿��ľ�");
  ctrl->data = ID_HOMEPAGE_LINK6;
  ctrl = Createhyperlink(page, 310, 40 + 26 * 6, 450, 30, ID_HOMEPAGE_LINK7, "��������ҵ����УҽԺΪ��ҵ����У���ݻ���");
  ctrl->data = ID_HOMEPAGE_LINK7;
  ctrl = Createhyperlink(page, 310, 40 + 26 * 7, 450, 30, ID_HOMEPAGE_LINK8, "���ߡ���ǰ �������Ե���");
  ctrl->data = ID_HOMEPAGE_LINK8;

  CreateTextBox(page, 90, 330, 779, 32, ID_HOMEPAGE_AISEARCH, "", 1);
  CreateButton(page, 870, 330, 130, 32, ID_HOMEPAGE_AIBUTTON, "������ѯ");

  CreateButton(page, 810, 30, 200, 45, ID_HOMEPAGE_BUTTON1, "������֪");
  CreateButton(page, 810, 85, 200, 45, ID_HOMEPAGE_BUTTON2, "����ָ��");
  CreateButton(page, 810, 140, 200, 45, ID_HOMEPAGE_BUTTON3, "�绰Ͷ��");
  CreateButton(page, 810, 196, 200, 45, ID_HOMEPAGE_BUTTON4, "��������");

  CreateWindowsEx(page, 45, 445, 64, 64, ID_HOMEPAGE_REGS, NULL);
  CreateWindowsEx(page, 189, 445, 64, 64, ID_HOMEPAGE_CHARGE1, NULL);
  CreateWindowsEx(page, 333, 445, 64, 64, ID_HOMEPAGE_DIAGNOSE, NULL);
  CreateWindowsEx(page, 477, 445, 64, 64, ID_HOMEPAGE_RESULT, NULL);
  CreateWindowsEx(page, 621, 445, 64, 64, ID_HOMEPAGE_CHARGE2, NULL);
  CreateWindowsEx(page, 765, 445, 64, 64, ID_HOMEPAGE_DRUG, NULL);
  CreateWindowsEx(page, 909, 445, 64, 64, ID_HOMEPAGE_LOGISTICS, NULL);

  page->onPaint = OnPaint_homepage;
  page->EventHandler = EventHandler_homepage;

  page->style = malloc(sizeof(WinStyle));
  memset(page->style, 0, sizeof(WinStyle));
  getWinTheme(page->style, 1);

  return page;
}

void OnPaint_homepage(hbasewinAttr *win, void *value)
{
  WinStyle *winstyle = NULL;
  FILE *fpfile;
  int x, y;

  hfont *_font;

  TESTNULLVOID(win);
  winstyle = win->style;

  _font = getFont(SIMKAI, 24, 0x3244);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  Putbmp565(x + 50, y + 270, "data\\bmp\\ai.565");

  Putbmp565(x, y + 40, "data\\news\\newspic2.565");

  printTextLineXY(x, y, "ҽԺ����", _font);
  linex_styleEx(x, y + 30, 770, winstyle->bkcolor, 2, 1);

  printTextLineXY(x, y + 395, "��������", _font);
  linex_styleEx(x, y + 425, 1000, winstyle->bkcolor, 2, 1);
  freeFont(_font);

  _font = getFont(SIMHEI, 16, 0x3244);

  printTextLineXY(x + 15, y + 340, "����ҽ��", _font);
  rectangleEx(x, y + 255, 1010, 130, 0x6BAF, 1, 1);

  Putbmp565(x + 45, y + 445, "data\\bmp\\flow00.565");
  printTextLineXY(x + 20, y + 525, "ѡ��ҽ���Һ�", _font);

  //Putbmp64k(x + 126, y + 477, "data\\bmp\\arraw.bmp");

  //Putbmp64k(x + 189, y + 445, "data\\bmp\\flow-12.bmp");
  printTextLineXY(x + 185, y + 525, "����ɷ�", _font);

  //Putbmp64k(x + 270, y + 477, "data\\bmp\\arraw.bmp");

  //Putbmp64k(x + 333, y + 445, "data\\bmp\\flow-13.bmp");
  printTextLineXY(x + 330, y + 525, "ҽ������", _font);

  //Putbmp64k(x + 415, y + 477, "data\\bmp\\arraw.bmp");

  //Putbmp64k(x + 477, y + 445, "data\\bmp\\flow-14.bmp");
  printTextLineXY(x + 475, y + 525, "��Ͻ��", _font);

  //Putbmp64k(x + 555, y + 477, "data\\bmp\\arraw.bmp");

  //Putbmp64k(x + 621, y + 445, "data\\bmp\\flow-12.bmp");
  printTextLineXY(x + 620, y + 525, "�����ɷ�", _font);

  //Putbmp64k(x + 700, y + 477, "data\\bmp\\arraw.bmp");

  //Putbmp64k(x + 765, y + 445, "data\\bmp\\flow-15.bmp");
  printTextLineXY(x + 765, y + 525, "ҩ����ҩ", _font);

  //Putbmp64k(x + 845, y + 477, "data\\bmp\\arraw.bmp");

  //Putbmp64k(x + 909, y + 445, "data\\bmp\\flow-16.bmp");
  printTextLineXY(x + 909, y + 525, "����ǩ��", _font);

  repaintChildren(win, value);

  freeFont(_font);
  (void)value;
}
