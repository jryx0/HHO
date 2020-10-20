#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "hglobal.h"
#include "homepage.h"
#include "wResource.h"
#include "hyperlnk.h"
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

      if (_g->mouse.rightClickState == MOUSE_BUTTON_UP)
      {
        // hbasewinAttr *child = CreateLabel(hitwin, random(800), random(600), 300, 150, ID_LABEL_3, NULL);
        // if (child)
        //   child->onPaint(child, NULL);
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
  //hbasewinAttr *label;
  TESTNULL(page, NULL);

  // CreateLabel(page, 15, 90, 300, 150, ID_LABEL_1, "readme");
  // CreateLabel(page, 15 + 350, 90, 300, 150, ID_LABEL_2, "c:\\hho\\data\\news\\1.txt");
  // label = CreateLabel(page, 15 + 700, 90, 300, 150, ID_LABEL_3, NULL);
  // label->value = malloc(10);
  // strcpy((char *)label->value, "test");
  Createhyperlink(page, 310, 40, 450, 30, ID_HOMEPAGE_LINK1, "УҽԺְ�����ǿ������������ ���ڹ� ������ ����ɫ");
  Createhyperlink(page, 310, 40 + 24, 450, 30, ID_HOMEPAGE_LINK2, "�������¹ڷ��� ���д����ж���УҽԺ��ʱ�����Ŷ�Ϊһ����Ա�ṩ��ʵ����");
  Createhyperlink(page, 310, 40 + 24 * 2, 450, 30, ID_HOMEPAGE_LINK3, "��ս�߼���һ�ߡ�УҽԺ����������ҩ���ң���ʦ������������");
  Createhyperlink(page, 310, 40 + 24 * 3, 450, 30, ID_HOMEPAGE_LINK4, "��ս�߼���һ�ߡ�УҽԺΪ��ְ����ȫ���ڱ��ݻ���");
  Createhyperlink(page, 310, 40 + 24 * 4, 450, 30, ID_HOMEPAGE_LINK5, "УҽԺ���Ͻ�������������ѧ���¸���");
  Createhyperlink(page, 310, 40 + 25 * 5, 450, 30, ID_HOMEPAGE_LINK6, "УҽԺů�ķ���߿��ľ�");
  Createhyperlink(page, 310, 40 + 25 * 6, 450, 30, ID_HOMEPAGE_LINK7, "��������ҵ����УҽԺΪ��ҵ����У���ݻ���");
  Createhyperlink(page, 310, 40 + 25 * 7, 450, 30, ID_HOMEPAGE_LINK8, "���ߡ���ǰ �������Ե���");
 // Createhyperlink(page, 310, 40 + 25 * 8, 450, 30, ID_HOMEPAGE_LINK9, "�������¹ڷ��� ���д����ж���УҽԺ�ܷ�������ʦ��������������");

  page->onPaint = OnPaint_homepage;
  page->EventHandler = EventHandler_homepage;

  return page;
}

void OnPaint_homepage(hbasewinAttr *win, void *value)
{
  FILE *fpfile;
  int x, y;

  hfont *_font;

  TESTNULLVOID(win);

  _font = getFont(SIMHEI, 24, 0x3244);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  printTextLineXY(x, y, "ҽԺ����", _font);
  linex_styleEx(x, y + 30, 770, 0x3244, 2, 1);

  printTextLineXY(x, y + 280, "��������", _font);
  linex_styleEx(x, y + 310, 1000, 0x3244, 2, 1);

  printTextLineXY(x, y + 480, "����ҽ��", _font);
  linex_styleEx(x, y + 510, 1000, 0x3244, 2, 1);
  freeFont(_font);

  Putbmp64k(x, y + 40, "data\\news\\newspic2.bmp");

  _font = getFont(SIMHEI, 16, 0x3244);
  //printTextLineXY(x + 60, y + 250, "УҽԺְ�����ǿ��...", _font);

  Putbmp64k(x + 40, y + 325, "data\\bmp\\flow-11.bmp");
  printTextLineXY(x + 20, y + 420, "ѡ��ҽ���Һ�", _font);
  Putbmp64k(x + 190, y + 325, "data\\bmp\\flow-12.bmp");
  printTextLineXY(x + 180, y + 420, "����ɷ�", _font);

  Putbmp64k(x + 360, y + 325, "data\\bmp\\flow-13.bmp");
  printTextLineXY(x + 340, y + 420, "ҽ������", _font);

  Putbmp64k(x + 530, y + 325, "data\\bmp\\flow-14.bmp");
  printTextLineXY(x + 520, y + 420, "��Ͻ��", _font);

  Putbmp64k(x + 670, y + 325, "data\\bmp\\flow-12.bmp");
  printTextLineXY(x + 650, y + 420, "�����ɷ�", _font);

  Putbmp64k(x + 810, y + 325, "data\\bmp\\flow-15.bmp");
  printTextLineXY(x + 810, y + 420, "ҩ����ҩ", _font);

   Putbmp64k(x + 910, y + 325, "data\\bmp\\flow-16.bmp");
  printTextLineXY(x + 910, y + 420, "�����Ϣ", _font);

   

  repaintChildren(win, value);

  rectangleEx(x + 810, y + 30, 200, 50, 0xF483, 1, 1);
  printTextLineXY(x + 850, y + 40, "������֪", _font);

  rectangleEx(x + 810, y + 90, 200, 50, 0xF483, 1, 1);
  printTextLineXY(x + 850, y + 100, "����ָ��", _font);

  rectangleEx(x + 810, y + 150, 200, 50, 0xF483, 1, 1);
  printTextLineXY(x + 850, y + 160, "Ͷ�ߵ绰", _font);

  rectangleEx(x + 810, y + 210, 200, 50, 0xF483, 1, 1);
  printTextLineXY(x + 850, y + 220, "��ͨλ��", _font);

  freeFont(_font);
  (void)value;
}
