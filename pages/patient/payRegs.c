#include "hhosvga.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "hbutton.h"
#include "wResource.h"
#include "macrodef.h"
#include "hglobal.h"
#include "hyperlnk.h"
#include "htextbox.h"
#include "payregs.h"
#include "hchkbox.h"
#include "data.h"

#include <string.h>

hbasewinAttr *CreatePayRegspage(hbasewinAttr *parent, int winID, int userid)
{
  WinStyle *style;
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "registerpage");
  hbasewinAttr *label;
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_PayRegspage;
  page->EventHandler = EventHandler_payregspage;
  style = malloc(sizeof(WinStyle));
  getWinTheme(style, 1);
  page->style = style;
  style->type = userid;
  style->height = 0;

  Createhyperlink(page, 20, 10, 65, 25, ID_PAYREGS_RETURN, "[�� ҳ]");
  label = CreateLabel(page, 388, 250, 348, 250, ID_PAYREGS_DISEASE, NULL);
  label->wintype = LABEL_TITLE;
  CreateButton(page, 638, 510, 100, 32, ID_PAYREGS_OK, "ȷ�Ͻɷ�");

  CreateCheckBox(page, 40, 250, 10, 10, ID_ID_PAYREGS_ALLCHK, NULL);
  //TRACE(("%s(%d):userid = %d\n", __FILE__, __LINE__, userid));
  return page;
}

//���ƹҺ��б���
void drawPayRegister(int x, int y)
{
  rectangleEx(x + 20, y + 40, 340, 200, 0x6BAF, 1, 1);
  //fillRegionEx(x + 2, y + 320, 320, 200, 0x0000);

  lineyEx(x + 110, y + 40, 200, 0x6BAF);
  lineyEx(x + 180, y + 40, 200, 0x6BAF);
  lineyEx(x + 235, y + 40, 200, 0x6BAF);
  lineyEx(x + 300, y + 40, 200, 0x6BAF);
  linex_styleEx(x + 20, y + 70, 340, 0x6BAF, 1, 1);
}
//���Һŵ��б�
void fillPayRegisterList(hbasewinAttr *win, int userid)
{
  hbasewinAttr *lnk;
  int i, j;
  list_t *rgslist, *palist, *doclist;
  list_node_t *node;
  RegisterInfo *regs;
  PatientInfo *pa;
  DoctorInfo *doc;
  WinStyle *style;

  char *status[] = {"δ�ɷ�", "�ѽɷ�", "�����"};

  TESTNULLVOID(win);
  style = (WinStyle *)win->style;
  // x = getAbsoluteX(win);
  // y = getAbsoluteY(win);

  rgslist = ReadRegistration(REGISTRATIONFILE);
  TESTNULLVOID(rgslist);
  palist = ReadPatientInfo(PATIENTINFOFILE);
  doclist = ReadDoctorInfo(DOCTORINFOFILE);

  //���link����
  for (i = ID_PAYREGS_LINK; i < ID_PAYREGS_LINK + 8; i++)
  {
    lnk = findWinByID(win, i);
    if (lnk && lnk->title)
    {
      free(lnk->title);
      lnk->title = NULL;
    }
  }

  for (i = 0, j = 0; i < rgslist->len && i < 8; i++)
  { //һ�����8��, page������ʾ�Ǹ�8��
    hbasewinAttr *lnk = findWinByID(win, ID_PAYREGS_LINK + j);
    node = list_at(rgslist, i);
    regs = (RegisterInfo *)node->val;

    //ֻ��ʾ����
    if (regs->userid != userid)
      continue;

    //������ʾȫ����δ�ɷѵ���
    if (style->height == 0 && regs->status != 0)
      continue;

    if (lnk == NULL)
    { //����lnk
      lnk = Createhyperlink(win, 23, 75 + 25 * j, 340, 25, ID_PAYREGS_LINK + j, NULL);
      lnk->wintype = HYPERLINK_BK;
      //CreateCheckBox(win, 10, 95 + 8 + 25 * j, 10, 10, ID_PAYREGS_CHK + j, NULL);
    }

    if (lnk->title)
      free(lnk->title);
    lnk->title = malloc(256);
    memset(lnk->title, 0, 256);

    pa = FindPatientInfo(palist, regs->userid);    //����
    doc = FindDoctorInfo(doclist, regs->doctorid); //ҽ��

    if (pa && doc)
    {
      sprintf(lnk->title, " %-10d%-8s%-7s%-6s%s", regs->id, pa->name, regs->dept, doc->name, status[regs->status]);
      lnk->data = regs->id; //�Һ�id
      j++;
    }
  }

  if (rgslist)
    list_destroy(rgslist);
  if (palist)
    list_destroy(palist);
  if (doclist)
    list_destroy(doclist);
}

void drawPayRegisterInfo(hbasewinAttr *win)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  style = (WinStyle *)win->style;
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  _h = getFont(style->fonttype, style->fontsize, 0x0);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- �ҺŽɷ�", _h);

  //���ƹҺŴ���ͷ
  printTextLineXY(x + 20, y + 45, "   ����     ����    ����   ҽ��  ״̬", _h);
  drawPayRegister(x, y);

  //��ȡ��ǰ����
  printTextLineXY(x + 388, y + 100, "�Һ�ʱ��:", _h);
  // _strdate(temp);
  // printTextLineXY(x + 470, y + 100, temp, _h);
  // _strtime(temp);
  // printTextLineXY(x + 550, y + 100, temp, _h);
  printTextLineXY(x + 388, y + 125, "��������:             ���߱��:", _h);
  printTextLineXY(x + 388, y + 150, "����ҽ��:             ����:", _h);
  printTextLineXY(x + 388, y + 175, "���ҵص�: ", _h);
  printTextLineXY(x + 388, y + 200, "�Һŷ�:               ���:", _h);
  printTextLineXY(x + 388, y + 225, "�˻����:", _h);
  printTextLineXY(x + 388, y + 250, "��������(������ʷ):", _h);

  freeFont(_h);
}

void fillPayRegisterInfo(hbasewinAttr *win, int regsid)
{
  int x, y;
  RegisterInfo *rgs;
  PatientInfo *paInfo;
  DoctorInfo *docInfo;
  //WinStyle *style;
  char info[32];
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  //style = (WinStyle *)win->style;

  _h = getFont(SIMHEI, 16, 0x0000);
  //��ȡ�Һ���Ϣ
  rgs = fFindRegisterInfo(REGISTRATIONFILE, regsid);
  if (rgs)
  {
    paInfo = fFindPatientInfo(PATIENTINFOFILE, rgs->userid);
    docInfo = fFindDoctorInfo(DOCTORINFOFILE, rgs->doctorid);

    if (paInfo && docInfo)
    {
      hregion _r;
      printTextLineXY(x + 470, y + 100, rgs->datetime, _h);
      sprintf(info, "%d", paInfo->id);
      printTextLineXY(x + 670, y + 125, info, _h);
      printTextLineXY(x + 470, y + 125, paInfo->name, _h);

      //ҽ��
      printTextLineXY(x + 470, y + 150, docInfo->name, _h);
      //����
      printTextLineXY(x + 650, y + 150, rgs->dept, _h);
      printTextLineXY(x + 470, y + 175, "���пƼ���ѧУҽԺ��¥����", _h);

      //�Һŷ�
      printTextLineXY(x + 470, y + 200, "1.50 Ԫ", _h);
      printTextLineXY(x + 650, y + 200, "5.00 Ԫ", _h);
      sprintf(info, "%.2f", paInfo->money / 100.00);
      printTextLineXY(x + 470, y + 225, info, _h);

      _r.left_top.x = x + 388 + 5;
      _r.left_top.y = y + 275 + 5;
      _r.right_bottom.x = _r.left_top.x + 349 - 10;
      _r.right_bottom.y = _r.left_top.y + 160 - 10;
      rectangleEx(x + 388, y + 275, 349, 160, 0x6BAF, 1, 1);
      //�ָ��س�
      //strrpl(rgs->disease, '#', '\r');
      printTextEx(&_r, rgs->disease, _h, 0);
    }

    if (paInfo)
      free(paInfo);
    if (docInfo)
      free(docInfo);
    free(rgs);
  }
  freeFont(_h);
}

void OnPaint_PayRegspage(hbasewinAttr *win, void *value)
{
  WinStyle *style;
  hfont *_h;
  int x, y;
  char temp[32];
  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  style = (WinStyle *)win->style;
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  //���Һ��б���Ϣ
  fillPayRegisterList(win, style->type);
  repaintChildren(win, value);

  rectangleEx(x + 378, y + 40, 368, 520, 0x0000, 1, 1);
  //TRACE(("%s(%d):docid = %d\n", __FILE__, __LINE__, win->data));
  _h = getFont(SIMHEI, 16, 0x0000);
  printTextLineXY(x + 468, y + 60, "���пƼ���ѧУҽԺ�Һŵ�", _h);
  linex_styleEx(x + 378, y + 90, 368, 0x0000, 1, 1);
  freeFont(_h);

  _h = getFont(style->fonttype, style->fontsize, 0x0);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- �ҺŽɷ�", _h);
  printTextLineXY(x + 55, y + 246, "��ʾȫ���Һŵ�", _h);

  //���ƹҺŴ���ͷ
  printTextLineXY(x + 20, y + 45, "   ����     ����    ����   ҽ��  ״̬", _h);
  drawPayRegister(x, y);

  //�����ʾ�Һŵ���Ϣ
  fillPayRegisterInfo(win, -1);
  drawPayRegisterInfo(win);
  freeFont(_h);

  // //��ȡ��ǰ����
  // printTextLineXY(x + 388, y + 100, "�Һ�ʱ��:", _h);
  // // _strdate(temp);
  // // printTextLineXY(x + 470, y + 100, temp, _h);
  // // _strtime(temp);
  // // printTextLineXY(x + 550, y + 100, temp, _h);
  // printTextLineXY(x + 388, y + 125, "��������:             ���߱��:", _h);
  // printTextLineXY(x + 388, y + 150, "����ҽ��:             ����:", _h);
  // printTextLineXY(x + 388, y + 175, "���ҵص�: ", _h);
  // printTextLineXY(x + 388, y + 200, "�Һŷ�:               ���:", _h);
  // printTextLineXY(x + 388, y + 225, "��������(������ʷ):", _h);
}

void UpdateRegiserBill(hbasewinAttr *win)
{
  list_t *rgsilist, *palist;
  RegisterInfo *rgsi;
  PatientInfo *pa;
  TESTNULLVOID(win);

  rgsilist = ReadRegistration(REGISTRATIONFILE);
  TESTNULLVOID(rgsilist);

  rgsi = FindRegisterInfo(rgsilist, win->data);
  if (rgsi && rgsi->status == 0)
  {
    rgsi->status = 1;
    palist = ReadPatientInfo(PATIENTINFOFILE);
    pa = FindPatientInfo(palist, rgsi->userid);
    if (pa)
    {
      pa->money = pa->money - 650;
      SaveRegistration(REGISTRATIONFILE, rgsilist);
      SavePatientInfo(PATIENTINFOFILE, palist);
    }
    list_destroy(palist);
  }
  list_destroy(rgsilist);
}

void EventHandler_payregspage(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  hitwin = checkmousewin(win, &_g->mouse);

  if (type == EVENT_MOUSE)
    switch (hitwin->winID)
    {
    case ID_PAYREGSPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //�޼���ڣ�ԭ��textbox inactive
        if (_g->foucsedTextBox && _g->foucsedTextBox->onActivate)
          _g->foucsedTextBox->onActivate(NULL, _g);
      }
      break;
    case ID_PAYREGS_RETURN:
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
    case ID_PAYREGS_DISEASE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //�ڴ��ڲ�����ʾ�������
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      break;
    case ID_PAYREGS_OK:
      //ȷ�ϹҺ�
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //�ڴ��ڲ�����ʾ�������
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
      { //��갴��
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
      }
      else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //����ͷ�
        int x = getAbsoluteX(win);
        int y = getAbsoluteY(win);
        WinStyle *style = (WinStyle *)win->style;
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);

        //TRACE(("�Һŵ��ɷ�\n"));
        UpdateRegiserBill(win);

        //���¹Һ��嵥��Ϣ
        fillPayRegisterList(win, style->type);
        repaintChildren(win, NULL);
        drawPayRegister(getAbsoluteX(win), getAbsoluteY(win));

        //���¹Һ���Ϣ
        fillRegionEx(x + 388, y + 100, 350, 400, 0xFFFF);
        fillPayRegisterInfo(win, win->data);
        drawPayRegisterInfo(win);
      }
      break;
    case ID_ID_PAYREGS_ALLCHK:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��label1���ڲ�����ʾ�������
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        WinStyle *style = (WinStyle *)win->style;
        int x, y;
        if (hitwin->onClick)
          hitwin->onClick(hitwin, NULL);
        hitwin->onPaint(hitwin, NULL);

        x = getAbsoluteX(win);
        y = getAbsoluteY(win);

        //�������
        //rectangleEx(x + 20, y + 40, 340, 200, 0x6BAF, 1, 1);
        fillRegionEx(x + 20, y + 71, 340, 170, 0xFFFF);

        style->height = hitwin->data;
        fillPayRegisterList(win, style->type);
        repaintChildren(win, NULL);

        drawPayRegister(x, y);

        //�����Ϣ
        fillRegionEx(x + 388, y + 100, 350, 400, 0xFFFF);
        fillPayRegisterInfo(win, -1);
        drawPayRegisterInfo(win);
        //���õ�ǰ�Һŵ�,���ڸ��¹Һŵ�״̬
        win->data = -1;
      }
      break;
    default:
      if (hitwin->winID >= ID_PAYREGS_LINK && hitwin->winID < ID_PAYREGS_LINK + 8)
      {
        if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��label1���ڲ�����ʾ�������
          _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

        if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
        { //��갴��
          if (hitwin->onClick)
            hitwin->onClick(hitwin, NULL);
        }
        else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        { //����Һ������ͷ�
          int x = getAbsoluteX(win);
          int y = getAbsoluteY(win);
          if (hitwin->onLeave)
            hitwin->onLeave(hitwin, NULL);

          //�����Ϣ
          fillRegionEx(x + 388, y + 100, 350, 400, 0xFFFF);
          fillPayRegisterInfo(win, hitwin->data);
          drawPayRegisterInfo(win);
          //���õ�ǰ�Һŵ�,���ڸ��¹Һŵ�״̬
          win->data = hitwin->data;
        }
      }
      break;
    }
}