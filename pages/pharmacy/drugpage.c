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
  {
    strcpy(page->title, "ҩ����ҩ");
    CreateButton(page, page->x + 660, page->y + 280, 100, 32, ID_DRUG_STOCK, "�鿴�ֿ�");
    CreateButton(page, page->x + 825, page->y + 280, 100, 32, ID_DRUG_SHIP, "���ɷ�����");
  }
  else
  {
    strcpy(page->title, "ҩ��ȡҩ");
    CreateButton(page, page->x + 660, page->y + 280, 100, 32, ID_DRUG_PAY, "ȷ�Ͻɷ�");
  }

  Createhyperlink(page, 20, 10, 65, 25, ID_DRUG_RETURN, "[�� ҳ]");
  ((WinStyle *)page->style)->type = userid; //����id����

  Createhyperlink(page, page->x + 860, page->y + 180, 55, 25, ID_DRUG_PREV, "��һҳ");
  Createhyperlink(page, page->x + 925, page->y + 180, 55, 25, ID_DRUG_NEXT, "��һҳ");

  return page;
}

//���ƴ���
//onpaint�е���,��������ʾ�����
void drawPrescription(int x, int y)
{
  rectangleEx(x, y + 65, PAGE_W, PAGE_H / 2 - 50, 0x6BAF, 1, 1);

  //printTextLineXY(x + 10, y + 70, " ��������         ��  ��         ��  ��    �Ա�   ����      ҽ��      ����          ���      ״̬", _h);

  lineyEx(x + COL1, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL2, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL3, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL4, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL5, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL6, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL7, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  lineyEx(x + COL8, y + 65, PAGE_H / 2 - 50, 0x6BAF);
  linex_styleEx(x, y + 90, PAGE_W, 0x6BAF, 1, 1);
}

//��䴦������
//��drawPrescriptionǰ����
//�����û����Ͳ�ͬ,��ʾ��ͬ�Ĵ��� style->type
//page ҳ�Ŵ�0��ʼ
void fillPrescription(hbasewinAttr *win, int page)
{
  hbasewinAttr *lnk;
  list_t *ps, *patient, *doc;
  PatientInfo *pi;
  Prescription *p;
  DoctorInfo *d;
  //char infostr[256];

  list_node_t *node;
  int i;

  TESTNULLVOID(win);

  ps = ReadPrescription(DATAPATH "database\\ps\\psinfo.txt");
  TESTNULLVOID(ps);

  patient = ReadPatientInfo(DATAPATH "database\\painfo.txt");
  doc = ReadDoctorInfo(DATAPATH "database\\docinfo.txt");

  if (patient && doc)
    for (i = 0; i < ps->len && i < 8; i++)
    { //һ�����8��, page������ʾ�Ǹ�8��
      hbasewinAttr *lnk = findWinByID(win, ID_DRUG_PSLINK + i);

      if (lnk == NULL)
      { //����lnk
        lnk = Createhyperlink(win, 10 + 15, 95 + 25 * i, PAGE_W, 25, ID_DRUG_PSLINK + i, NULL);
        lnk->wintype = HYPERLINK_BK;
        CreateCheckBox(win, 10, 95 + 8 + 25 * i, 10, 10, ID_DRUG_PSCHK + i, NULL);
      }

      if (lnk->title)
        free(lnk->title);
      lnk->title = malloc(256);
      memset(lnk->title, 0, 256);

      node = list_at(ps, i);

      p = (Prescription *)node->val;            //����
      pi = FindPatientInfo(patient, p->userid); //����
      d = FindDoctorInfo(doc, p->doctorid);     //ҽ��
      if (p && pi && d)
      {
        sprintf(lnk->title, "%-12d%-20s%-5s%8s%7d%8s%12s%15.2f%12s", p->id, p->date, pi->name,
                pi->sex ? "��" : "Ů", 2020 - pi->year, d->name, p->dept, (float)(p->amount / 100), p->status);
        lnk->data = p->id; //����id
        //TRACE(("%s\n", lnk->title));
      }
    }

  if (ps)
    list_destroy(ps);
  if (patient)
    list_destroy(patient);
  if (doc)
    list_destroy(doc);
}

//����ҩƷ�嵥
void drawDruglist(int x, int y)
{
  rectangleEx(x, y + 325, 540, 205, 0x6BAF, 1, 1);

  lineyEx(x + 170, y + 325, 205, 0x6BAF);
  lineyEx(x + 220, y + 325, 205, 0x6BAF);
  lineyEx(x + 330, y + 325, 205, 0x6BAF);
  lineyEx(x + 370, y + 325, 205, 0x6BAF);
  lineyEx(x + 470, y + 325, 205, 0x6BAF);
  linex_styleEx(x, y + 350, 540, 0x6BAF, 1, 1);
}

//���ҩƷ
//Ĭ�ϲ��ᳬ��
void fillDruglist(hbasewinAttr *win, int psid)
{
  FILE *fp;
  char info[256];
  PrescriptionDrugItem *pdi;
  DrugItem *di;
  hfont *_h;
  int i = 0;
  TESTNULLVOID(win);
  //TRACE(("linkdata:%d\n",link->data));

  //����ҩƷ�嵥�ļ���
  sprintf(info, "%sdatabase\\ps\\%d.txt", DATAPATH, psid);
  fp = fopen(info, "r");
  if (fp == NULL)
  {
    TRACE(("%s(%d):�޷����ļ�%s\n", __FILE__, __LINE__, info));
    return;
  }
  //����ҩƷ
  pdi = malloc(sizeof(PrescriptionDrugItem));
  _h = getFont(SIMHEI, 16, 0x0000);
  while (fgets(info, 256, fp))
  {
    if (info[0] == '#')
      continue;
    sscanf(info, "%d\t%d", &pdi->drugItemid, &pdi->amount);
    di = fFindDrugItem(DATAPATH "database\\drug\\druglist.txt", pdi->drugItemid);

    if (di)
    {
      sprintf(info, "%-20s%-5s%-14s%-4d%7.2f%8.2f", di->name, di->unit, di->kind, pdi->amount,
              di->price / 100.0, (float)(pdi->amount * di->price) / 100.00);
      printTextLineXY(win->x + 10, win->y + 360 + i * 20, info, _h);
      i++;
      TRACE(("%s\n", info));
    }
  }

  free(pdi);

  fclose(fp);
  freeFont(_h);
  return NULL;
}

void OnPaint_Drugpage(hbasewinAttr *win, void *val)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);

  //��䴦������
  fillPrescription(win, 0);
  //����
  repaintChildren(win, val);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  style = win->style;
  _h = getFont(SIMHEI, 24, style->fontcolor);
  printTextLineXY(x + 450, y + 25, win->title, _h);
  freeFont(_h);

  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- ������Ϣ", _h);
  //����ͷ
  printTextLineXY(x + 10, y + 70, " ��������         ��  ��         ��  ��    �Ա�   ����      ҽ��      ����          ���      ״̬", _h);
  //�������߿�
  drawPrescription(x, y);

  //ҩƷͷ
  printTextLineXY(x + 10, y + 330, "    ҩƷ����       ��λ   ���       ����    ����     С��", _h);
  //��ҩƷ�嵥��
  drawDruglist(x, y);

  freeFont(_h);
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
  default:
    if (hitwin->winID >= ID_DRUG_PSLINK && hitwin->winID < ID_DRUG_PSLINK + 8)
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

        // hbasewinAttr *pslink;
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        //��������
        drawPrescription(getAbsoluteX(win), getAbsoluteY(win));

        //���ҩƷ hitwin->data  ��������
        fillDruglist(win, hitwin->data);

        //pslink = findWinByID(win, ID_POST_UPDATEINFO);

        // //������ǰ������˵��ŵ�"����������Ϣ��ť"
        // info->data = hitwin->data;
        // //��ʾ��ϸ
        // showPostInfo(win, hitwin->data);
      }
    }
    else if (hitwin->winID >= ID_DRUG_PSCHK && hitwin->winID < ID_DRUG_PSCHK + 8)
    {                                                                             //checkbox
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��label1���ڲ�����ʾ�������
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
        hitwin->onPaint(hitwin, NULL);
      }
    }

    break;
  }
}
