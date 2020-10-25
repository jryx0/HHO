#include "macrodef.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "wResource.h"
#include "hhosvga.h"
#include "list.h"
#include "hglobal.h"

#include "htextbox.h"
#include "hbutton.h"
#include "data.h"
#include "drugpage.h"
#include "hchkbox.h"

#define COL1 120
#define COL2 (COL1 + 150)
#define COL3 (COL2 + 100)
#define COL4 (COL3 + 65)
#define COL5 (COL4 + 65)
#define COL6 (COL5 + 100)
#define COL7 (COL6 + 100)
#define COL8 (COL7 + 120)

hbasewinAttr *CreateDrugpage(hbasewinAttr *parent, int winID, int userid)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, NULL);
  page->onPaint = OnPaint_Drugpage;
  Createhyperlink(page, 20, 10, 65, 25, ID_POST_RETURN, "[�� ҳ]");
  page->EventHandler = Eventhandler_drugpage;

  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);

  page->title = malloc(10);
  if (userid == -1)
    strcpy(page->title, "ҩ����ҩ");
  else
    strcpy(page->title, "ҩ��ȡҩ");
  Createhyperlink(page, 20, 10, 65, 25, ID_DRUG_RETURN, "[�� ҳ]");
  ((WinStyle *)page->style)->type = userid;

  createDrugList(page, userid);

  return page;
}

//���������ؼ�
void createPrescription(hbasewinAttr *win, int userid)
{
  hbasewinAttr *lnk;
  // list_t *ps, *patient, *doc;
  int i;
  int x = 20, y = 110;

  TESTNULLVOID(win);
  // ps = ReadPrescription(DATAPATH "database\\ps\\psinfo.txt");
  // patient = ReadPatientInfo(DATAPATH "database\\painfo.txt");
  // doc = ReadDoctorInfo(DATAPATH "database\\docinfo.txt");

  // TESTNULLVOID(ps);
  // TESTNULLVOID(patient);
  // TESTNULLVOID(doc);

  //��󲻳���7��
  for (i = 0; i < 7; i++)
  {
    if (userid == -1)
    { //ҩ����Ա���Բ鿴�����˵�
      hbasewinAttr *lnk = Createhyperlink(win, x, y + 30 * i, PAGE_W, 25, ID_POST_LINK + i, "test");
      lnk->wintype = HYPERLINK_BK;
    }
    // else if (userid == pi->userid)
    // {
    // lnk = Createhyperlink(parent, x, y + 30 * i, PAGE_W, 25, ID_POST_LINK + i, info);
    // lnk->data = pi->postid;
    // lnk->wintype = HYPERLINK_BK;
    // }
  }
}

//���ƴ���
void drawPrescription(int x, int y)
{
}

//ˢ�¿ؼ�����
void refreshPrescription(int page)
{
}

void OnPaint_Drugpage(hbasewinAttr *win, void *val)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  style = win->style;
  _h = getFont(SIMHEI, 24, style->fontcolor);
  printTextLineXY(x + 450, y + 25, win->title, _h);
  freeFont(_h);

  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- ������Ϣ", _h);
  rectangleEx(x, y + 65, PAGE_W, PAGE_H / 2 - 50, 0x6BAF, 1, 1);

  printTextLineXY(x + 10, y + 70, " ��������         ��  ��         ��  ��    �Ա�   ����      ҽ��      ����          ���      ״̬", _h);
  freeFont(_h);
  lineyEx(x + COL1, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL2, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL3, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL4, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL5, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL6, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL7, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL8, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  linex_styleEx(x, y + 90, PAGE_W, 0x6BAF, 1, 1);

  //fillDrugList(win, style->type);

  repaintChildren(win, val);
}

void Eventhandler_drugpage(hbasewinAttr *win, int type, void *val)
{
  globaldef *_g;
  hbasewinAttr *hitwin;
  _g = (globaldef *)val;
  //ȷ���Ƿ����Ӵ����¼������Ӵ����¼����ر���
  hitwin = checkmousewin(win, &_g->mouse);

  switch (hitwin->winID)
  {                 //������Ǹ��Ӵ��ڻ���
  case ID_DRUGPAGE: //����
    if (type == EVENT_MOUSE)
    {                                                                              //homepage ��������ƶ�������ƶ�
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
    }
    break;
  }
}
