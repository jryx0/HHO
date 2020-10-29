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

#include <string.h>

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
  // Createhyperlink(page, 20, 10, 65, 25, ID_POST_RETURN, "[�� ҳ]");
  page->EventHandler = Eventhandler_drugpage;

  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);

  page->title = malloc(10);
  if (userid == -1)
  {
    strcpy(page->title, "ҩ����ҩ");
    CreateButton(page, page->x + 660, page->y + 365, 100, 32, ID_DRUG_STOCK, "�鿴�ֿ�");
    CreateButton(page, page->x + 825, page->y + 365, 100, 32, ID_DRUG_SHIP, "���ɷ�����");
  }
  else
  {
    // strcpy(page->title, "ҩ��ȡҩ");
    //CreateButton(page, page->x + 660, page->y + 365, 100, 32, ID_DRUG_CONFIRM, "ȷ�϶���");
  }

  Createhyperlink(page, 20, 10, 65, 25, ID_DRUG_RETURN, "[�� ҳ]");
  ((WinStyle *)page->style)->type = userid; //����id����

  Createhyperlink(page, page->x + 860, page->y + 175, 55, 25, ID_DRUG_PREV, "��һҳ");
  Createhyperlink(page, page->x + 925, page->y + 175, 55, 25, ID_DRUG_NEXT, "��һҳ");

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
  WinStyle *style;
  //char infostr[256];

  list_node_t *node;
  int i, j = 0;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  style = win->style;

  //ps = ReadPrescription(DATAPATH "database\\ps\\psinfo.txt");
  ps = ReadPrescription(PRESCRITIONFILE);
  TESTNULLVOID(ps);

  // patient = ReadPatientInfo(DATAPATH "database\\painfo.txt");
  // doc = ReadDoctorInfo(DATAPATH "database\\docinfo.txt");
  patient = ReadPatientInfo(PATIENTINFOFILE);
  doc = ReadDoctorInfo(DOCTORINFOFILE);

  if (patient && doc)
    for (i = 0, j = 0; i < ps->len && i < 8; i++)
    { //һ�����8��, page������ʾ�Ǹ�8��
      hbasewinAttr *lnk = findWinByID(win, ID_DRUG_PSLINK + j);
      node = list_at(ps, i);
      p = (Prescription *)node->val;

      if (style->type != -1)
      {
        if (p->userid != style->type)
          continue;
      }

      if (lnk == NULL)
      { //����lnk
        lnk = Createhyperlink(win, 10 + 15, 95 + 25 * j, PAGE_W, 25, ID_DRUG_PSLINK + j, NULL);
        lnk->wintype = HYPERLINK_BK;
        CreateCheckBox(win, 10, 95 + 8 + 25 * j, 10, 10, ID_DRUG_PSCHK + j, NULL);
        j++;
      }

      if (lnk->title)
        free(lnk->title);
      lnk->title = malloc(256);
      memset(lnk->title, 0, 256);

      // node = list_at(ps, i);

      // p = (Prescription *)node->val;            //����
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

  (void)page;
}

//����ҩƷ�嵥
void drawDruglist(int x, int y)
{
  fillRegionEx(x + 1, y + 360, 540, 205, 0xFFFF);
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
      //TRACE(("%s\n", info));
      free(di);
    }
  }

  free(pdi);

  fclose(fp);
  freeFont(_h);
  return;
}

//�����˵���ʾ��Ϣ
/**
 * 
 * @param psid ����id
 */
void drawPostInfo(hbasewinAttr *win, int psid)
{
  int x, y;
  //WinStyle *style;
  hfont *_h;
  Prescription *ps;
  postInfo *po;
  char info[64];

  TESTNULLVOID(win);
  //TESTNULLVOID(win->style);
  //style = win->style;
  _h = getFont(SIMSUN, 16, 0x0000);

  fillRegionEx(win->x + 580, win->y + 325, 420, 160, 0xFFFF);

  printTextLineXY(win->x + 585, win->y + 335, "�˵���:                ������:", _h);
  printTextLineXY(win->x + 585, win->y + 32 + 335, "�ջ���:                �����˵绰:", _h);
  printTextLineXY(win->x + 585, win->y + 64 + 335, "�ջ���ַ:", _h);

  ps = fFindPrescription(PRESCRITIONFILE, psid);
  if (ps)
  {
    po = fFindPostInfo(POSTFILE, ps->postid);
    if (po)
    {
      sprintf(info, "%d", po->postid);
      printTextLineXY(win->x + 650, win->y + 334, info, _h);
      sprintf(info, "%s", po->shipper);
      printTextLineXY(win->x + 850, win->y + 334, info, _h);
      sprintf(info, "%s", po->receiver);
      printTextLineXY(win->x + 650, win->y + 32 + 334, info, _h);
      sprintf(info, "%s", po->tel);
      printTextLineXY(win->x + 890, win->y + 32 + 334, info, _h);
      sprintf(info, "%s", po->receiveraddr);
      printTextLineXY(win->x + 690, win->y + 64 + 334, info, _h);
      free(po);
    }
    free(ps);
  }
  freeFont(_h);
}

void generatePostInfo(hbasewinAttr *win, int psid)
{
  hbasewinAttr *lnk;
  Prescription *ps;
  list_t *polist, *pslist;
  postInfo *po, *newpo;
  PatientInfo *patient;
  list_node_t *node;
  char datebuf[9];
  char timebuf[9];
  int i, maxPostid = 0;

  TESTNULLVOID(win);

  //��ѯ�����ļ�,��ѯ�����Ƿ����ѽɷ�
  pslist = ReadPrescription(PRESCRITIONFILE);
  TESTNULLVOID(pslist);

  ps = FindPrescription(pslist, psid);

  //fFind ��Ҫ�ͷ� Find��Ҫ�ͷ�
  patient = fFindPatientInfo(PATIENTINFOFILE, ps->userid);
  TESTNULLVOID(patient);
  TESTNULLVOID(ps);

  if (patient && ps && strcmp(ps->status, "�ѽɷ�") == 0)
  {
    polist = ReadPostInfo(POSTFILE);
    for (i = 0; i < polist->len; i++)
    {
      node = list_at(polist, i);
      if (node && node->val)
      {
        po = (postInfo *)node->val;
        if (maxPostid <= po->postid)
          maxPostid = po->postid;
      }
    }

    newpo = malloc(sizeof(postInfo));
    if (newpo == NULL)
    {
      TRACE(("%s(%d):ursrid = %d\n", __FILE__, __LINE__, ps->userid));
    }
    else
    {
      newpo->postid = maxPostid + 1;
      newpo->userid = ps->userid;
      strcpy(newpo->shipper, "ҩ��");
      strcpy(newpo->shipperaddr, "���пƼ���ѧУҽԺ");
      strcpy(newpo->receiver, patient->name);
      strcpy(newpo->receiveraddr, patient->addr);
      strcpy(newpo->tel, patient->telenum);
      strcpy(newpo->status, "���ջ�");
      _strdate(datebuf);
      _strtime(timebuf);
      sprintf(newpo->shiptime, "%sT%s", datebuf, timebuf);

      strcpy(ps->status, "���ջ�");
      ps->postid = maxPostid + 1;
      SavePrescription(PRESCRITIONFILE, pslist);

      list_lpush(polist, list_node_new(newpo));
      SavePostInfo(POSTFILE, polist);
    }
    list_destroy(polist);
  }

  list_destroy(pslist);

  free(patient);
  TRACE(("%s(%d): save2\n", __FILE__, __LINE__));
}

//����checkboxѡ���hyperlink
hbasewinAttr *getcheckedlink(hbasewinAttr *win)
{
  hbasewinAttr *lnk = NULL, *chk = NULL;
  int i;
  TESTNULL(win, NULL);

  for (i = 0; i < ID_DRUG_PSCHK + 8; i++)
  {
    chk = findWinByID(win, ID_DRUG_PSCHK + i);

    if (chk && chk->data == 1)
    { //��һ�� checbox ѡ��
      //����i�ҵ���Ӧ��link
      lnk = findWinByID(win, ID_DRUG_PSLINK + i);
      return lnk;
    }
  }
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
  case ID_DRUG_SHIP:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��homepage���ڲ�����ʾ��׼���
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //��갴��
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //����ͷ�,�����˵���

      hbasewinAttr *pslink;
      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      //��ѯcheckbox��ѡ��link
      pslink = getcheckedlink(win);
      //�����˵���Ϣ,����ʾ
      if (pslink)
      {
        generatePostInfo(win, pslink->data);
        fillPrescription(win, 0);
        repaintChildren(win, NULL);

        drawPrescription(win->x, win->y);
        drawPostInfo(win, pslink->data);
      }
    }
    break;
  case ID_DRUG_RETURN:
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
  case ID_DRUG_CONFIRM:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��homepage���ڲ�����ʾ��׼���
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

    if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
    { //��갴��
      if (hitwin->onClick)
        hitwin->onClick(hitwin, NULL);
    }
    else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
    { //����ͷ�,�����˵���

      hbasewinAttr *pslink;
      if (hitwin->onLeave)
        hitwin->onLeave(hitwin, NULL);

      //�ѽɷ�->��ȷ��
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
        drawDruglist(win->x, win->y);
        fillDruglist(win, hitwin->data);

        //�˵���Ϣ
        drawPostInfo(win, hitwin->data);
        //fillPostInfo(win, hitwin->data);
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
