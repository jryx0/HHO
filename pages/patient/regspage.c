#include "hhosvga.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "hbutton.h"
#include "Resource.h"
#include "macrodef.h"
#include "hglobal.h"
#include "hyperlnk.h"
#include "htextbox.h"
#include "regspage.h"
#include "data.h"

#include <string.h>

hbasewinAttr *Createregisterpage(hbasewinAttr *parent, int winID, int userid)
{
  WinStyle *style;
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, "registerpage");
  TESTNULL(page, NULL);

  page->onPaint = OnPaint_registerpage;
  page->EventHandler = EventHandler_registerpage;
  style = malloc(sizeof(WinStyle));
  getWinTheme(style, 1);
  page->style = style;
  style->type = userid;

  Createhyperlink(page, 20, 10, 65, 25, ID_REGISTER_RETURN, "[�� ҳ]");
  CreateTextBox(page, 388, 250, 348, 250, ID_REGISTER_DISEASE, "", 0);
  CreateButton(page, 638, 510, 100, 32, ID_REGISTER_OK, "ȷ�ϹҺ�");

  //TRACE(("%s(%d):userid = %d\n", __FILE__, __LINE__, userid));
  return page;
}

void fillRegisterInfo(hbasewinAttr *win)
{
  int x, y;
  DeptInfo *deptInfo;
  PatientInfo *paInfo;
  DoctorInfo *docInfo;
  WinStyle *style;
  char info[32];
  hfont *_h;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);
  style = (WinStyle *)win->style;

  _h = getFont(SIMHEI, 16, 0x0000);
  //��ȡ������Ϣ
  paInfo = fFindPatientInfo(PATIENTINFOFILE, style->type);
  if (paInfo)
  {
    //��ȡҽ��
    docInfo = fFindDoctorInfo(DOCTORINFOFILE, win->data);
    if (docInfo)
    {
      //��ȡ����
      deptInfo = fFindDeptInfo(DEPTINFOFILE, docInfo->deptid);
      if (deptInfo)
      {
        sprintf(info, "%d", paInfo->id);
        printTextLineXY(x + 670, y + 125, info, _h);
        printTextLineXY(x + 470, y + 125, paInfo->name, _h);

        //ҽ��
        printTextLineXY(x + 470, y + 150, docInfo->name, _h);
        //����
        printTextLineXY(x + 650, y + 150, deptInfo->deptname, _h);
        printTextLineXY(x + 470, y + 175, "���пƼ���ѧУҽԺ��¥����", _h);
        free(deptInfo);
      }
      free(docInfo);
    }
    free(paInfo);
  }

  freeFont(_h);
}

void OnPaint_registerpage(hbasewinAttr *win, void *value)
{
  WinStyle *style;
  hfont *_h;
  int x, y;
  char temp[32];
  TESTNULLVOID(win);
  TESTNULLVOID(win->style);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  rectangleEx(x + 378, y + 40, 368, 510, 0x0000, 1, 1);
  //TRACE(("%s(%d):docid = %d\n", __FILE__, __LINE__, win->data));
  _h = getFont(SIMHEI, 16, 0x0000);
  printTextLineXY(x + 468, y + 60, "���пƼ���ѧУҽԺ�Һŵ�", _h);
  freeFont(_h);
  linex_styleEx(x + 378, y + 90, 368, 0x0000, 1, 1);

  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- �Һ�", _h);

  //��ȡ��ǰ����
  printTextLineXY(x + 388, y + 100, "�Һ�ʱ��:", _h);
  _strdate(temp);
  printTextLineXY(x + 470, y + 100, temp, _h);
  _strtime(temp);
  printTextLineXY(x + 550, y + 100, temp, _h);
  printTextLineXY(x + 388, y + 125, "��������:             ���߱��:", _h);
  printTextLineXY(x + 388, y + 150, "����ҽ��:             ����:", _h);
  printTextLineXY(x + 388, y + 175, "���ҵص�: ", _h);
  printTextLineXY(x + 388, y + 200, "�Һŷ�: 1.50 Ԫ         ���:  5.00 Ԫ", _h);
  printTextLineXY(x + 388, y + 225, "��������(������ʷ):", _h);
  freeFont(_h);
  //�������
  fillRegisterInfo(win);

  repaintChildren(win, value);
}

void SaveRegiserBill(hbasewinAttr *win, char *disease)
{
  int userid, docid;
  WinStyle *style;
  list_t *rgslist;
  list_node_t *node;
  int i;
  int maxid = 0;
  RegisterInfo *rgsi;
  DeptInfo *dept;
  DoctorInfo *doc;

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  TESTNULLVOID(disease);
  style = (WinStyle *)win->style;

  userid = style->type;
  docid = win->data;
  rgslist = ReadRegistration(REGISTRATIONFILE);
  if (!rgslist)
    return;

  for (i = 0; i < rgslist->len; i++)
  {
    node = list_at(rgslist, i);
    if (node && node->val)
    {
      rgsi = (RegisterInfo *)node->val;
      if (maxid < rgsi->id)
        maxid = rgsi->id;
    }
  }
  doc = fFindDoctorInfo(DOCTORINFOFILE, docid);
  if (doc)
  {
    dept = fFindDeptInfo(DEPTINFOFILE, doc->deptid);
    if (dept)
    {
      char datebuf[9], timebuf[9];
      rgsi = malloc(sizeof(RegisterInfo));
      rgsi->id = maxid + 1;
      rgsi->userid = userid;
      rgsi->doctorid = docid;
      strcpy(rgsi->dept, dept->deptname);
      _strdate(datebuf);
      _strtime(timebuf);
      sprintf(rgsi->datetime, "%sT%s", datebuf, timebuf);
      rgsi->serial = 0;
      rgsi->status = 0;

      //strrpl(disease, '\r', '#');
      strcpy(rgsi->disease, disease);

      list_rpush(rgslist, list_node_new(rgsi));
      SaveRegistration(REGISTRATIONFILE, rgslist);
    }
  }
  list_destroy(rgslist);
}

void EventHandler_registerpage(hbasewinAttr *win, int type, void *value)
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
    case ID_REGISTER_DISEASE:
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        if (hitwin->onActivate)
          hitwin->onActivate(hitwin, _g);
      break;
    case ID_REGISTER_OK:
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
        hbasewinAttr *disease;
        if (hitwin->onLeave)
          hitwin->onLeave(hitwin, NULL);
        disease = findWinByID(win, ID_REGISTER_DISEASE);
        if (disease && disease->title && strlen(disease->title) > 1 && strlen(disease->title) < 255)
        { //����Һŵ�
          TRACE(("����Һŵ�\n"));
          SaveRegiserBill(win, disease->title);

          //ת��homepage
          if (win->parent && win->parent->winID == ID_DESKTOP) //�ҵ�desktop
          {
            _g->activePageID = ID_HOMEPAGE;
            win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
          }
        }
        else
        { //��ʾ���벡������
          hfont *_h = getFont(SIMSUN, 16, 0xF801);
          printTextLineXY(390, 645, "�����벡��������", _h);
          printTextLineXY(390, 665, "(����1С��255���ַ�)", _h);
          freeFont(_h);
        }
      }
      break;
    }
}