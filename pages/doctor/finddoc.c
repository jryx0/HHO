#include "macrodef.h"
#include "wResource.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "hglobal.h"
#include "hnews.h"
#include "hlabel.h"
#include "hbutton.h"
#include "finddoc.h"
#include "data.h"
#include "list.h"
#include <string.h>

///��̬����hyperlink
///��Ӧ��̬����hyperlink�¼�
void CreateDocDept(hbasewinAttr *parent)
{
  list_t *deptlist = ReadDeptInfo(DEPTINFOFILE);
  int i, k = 0, j = 0;
  list_node_t *node;
  DeptInfo *dept;
  int x, y;
  hbasewinAttr *btn;
  TESTNULLVOID(parent);

  x = 125;
  y = 55;

  for (i = 0, k = 0; i < deptlist->len; i++)
  {
    node = list_at(deptlist, i);
    if (node && node->val)
    {
      dept = (DeptInfo *)node->val;
      btn = CreateButton(parent, x + 120 * k, y + j * 35, 100, 30, ID_DEPT_LINK + i, dept->deptname);
      btn->data = dept->id;
      k++;
      if (k % 7 == 0)
      {
        k = 0;
        j++;
      }
    }
  }

  list_destroy(deptlist);
}

void CreateDocInfoLabel(hbasewinAttr *win)
{
  int i = 0, j = 0, k = 0;
  hbasewinAttr *label;

  for (i = 0, j = 0; i < 10; i++)
  {
    label = CreateLabel(win, 90 + j * 160, 10 + k * 230 + 80, 150, 205, ID_FINDDOC_DOC + i, NULL);
    label->wintype = LABEL_FILE_TXT;

    CreateButton(win, 90 + j * 160 + 50, 10 + k * 230 + 80 + 205, 50, 20, ID_FINDDOC_DOCLINK + i, NULL);
    if (++j % 5 == 0)
    {
      j = 0;
      k++;
    }
  }
}

void clearDocInfo(hbasewinAttr *win)
{
  int i;
  int x, y;
  hbasewinAttr *ctrl;

  TESTNULLVOID(win);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  for (i = 0; i < 10; i++)
  {
    ctrl = findWinByID(win, ID_FINDDOC_DOC + i);
    if (ctrl->title)
    {
      free(ctrl->title);
      ctrl->title = NULL;
      fillRegionEx(x + ctrl->x, y + ctrl->y, ctrl->nWidth, ctrl->nHeight, 0xFFFF);
    }
    ctrl = findWinByID(win, ID_FINDDOC_DOCLINK + i);
    if (ctrl->title)
    {
      free(ctrl->title);
      ctrl->title = NULL;
      fillRegionEx(x + ctrl->x, y + ctrl->y, ctrl->nWidth, ctrl->nHeight, 0xFFFF);
    }
  }
}

void fillDocInfo(hbasewinAttr *win)
{
  int i, j = 0;
  hbasewinAttr *ctrl;
  list_t *docInfolist;
  DoctorInfo *docinfo;
  list_node_t *node;
  WinStyle *style;
  char title[64];

  TESTNULLVOID(win);
  TESTNULLVOID(win->style);
  style = win->style;

  clearDocInfo(win);

  docInfolist = ReadDoctorInfo(DOCTORINFOFILE);
  if (docInfolist)
  {
    for (i = 0; i < docInfolist->len && j < 10; i++)
    {
      node = list_at(docInfolist, i);
      if (node && node->val)
      {
        docinfo = (DoctorInfo *)node->val;
        if (docinfo->deptid == style->height) //style->height �������id
        {
          ctrl = findWinByID(win, ID_FINDDOC_DOC + j); //lable
          if (ctrl->title)
          {
            free(ctrl->title);
            ctrl->title = NULL;
          }
          ctrl->title = malloc(64);
          sprintf(ctrl->title, "%sdatabase\\doctor\\%d.txt", DATAPATH, docinfo->id);
          ctrl->onPaint(ctrl, NULL);

          ctrl = findWinByID(win, ID_FINDDOC_DOCLINK + j); //button
          if (ctrl->title)
          {
            free(ctrl->title);
            ctrl->title = NULL;
          }
          ctrl->title = malloc(6);
          strcpy(ctrl->title, "�Һ�");
          ctrl->data = docinfo->id;
          ctrl->onPaint(ctrl, NULL);
          j++;
        }
      }
    }
    list_destroy(docInfolist);
  }
}

hbasewinAttr *CreateFindDocpage(hbasewinAttr *parent, int winID, int userid)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, NULL);
  hbasewinAttr *ctrl;
  WinStyle *style;

  TESTNULL(page, NULL);
  Createhyperlink(page, 20, 10, 65, 25, ID_FINDDOC_RETURN, "[�� ҳ]");

  page->onPaint = OnPaint_FindDoc;
  //page->onDestroy = OnDestory_Dept;
  page->EventHandler = EventHandler_FindDoc;
  style = malloc(sizeof(WinStyle));
  getWinTheme(style, 1);
  page->style = style;
  style->type = userid;
  style->height = 901; //Ĭ���ڿ�

  CreateDocDept(page);
  CreateDocInfoLabel(page);
  return page;
}

void OnPaint_FindDoc(hbasewinAttr *win, void *val)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  TESTNULLVOID(win);
  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- ѡ��ҽ��", _h);

  printTextLineXY(x + 30, y + 60, "ѡ�����:", _h);
  freeFont(_h);

  repaintChildren(win, val);

  fillDocInfo(win);
}

void EventHandler_FindDoc(hbasewinAttr *win, int type, void *val)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(val);
  _g = (globaldef *)val;
  hitwin = checkmousewin(win, &_g->mouse);

  if (type == EVENT_MOUSE)
  {
    switch (hitwin->winID)
    {
    case ID_FINDDOCPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
      break;
    case ID_FINDDOC_RETURN:
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
      if (hitwin->winID >= ID_DEPT_LINK && hitwin->winID < ID_DEPT_LINK + 10)       //�������
      {                                                                             //�����̬���ɵ�button
        if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��link���ڲ�����ʾ��׼���
          _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;
        if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
        { //��갴��
          if (hitwin->onClick)
            hitwin->onClick(hitwin, NULL);
        }
        else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        { //����ͷ�

          WinStyle *style;
          if (hitwin->onLeave)
            hitwin->onLeave(hitwin, NULL);

          clearDocInfo(win);
          style = (WinStyle *)win->style;
          style->height = hitwin->data;
          fillDocInfo(win);
        }
      }
      else if (hitwin->winID >= ID_FINDDOC_DOCLINK && hitwin->winID < ID_FINDDOC_DOCLINK + 10)
      {//ת������Һ�ҳ��
        if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //��link���ڲ�����ʾ��׼���
          _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;
        if (_g->mouse.leftClickState == MOUSE_BUTTON_DOWN)
        { //��갴��
          if (hitwin->onClick)
            hitwin->onClick(hitwin, NULL);
        }
        else if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
        { //����ͷ�
          // hbasewinAttr *ctrl = findWinByID(win, hitwin->winID);
          if (hitwin->onLeave)
            hitwin->onLeave(hitwin, NULL);

          //TRACE(("%s(%d):deptid = %d\n", __FILE__, __LINE__, hitwin->data));
          //ת�� �Һ�ҳ��
          if (win->parent && win->parent->winID == ID_DESKTOP) //�ҵ�desktop
          {
            _g->data = hitwin->data;
            _g->activePageID = ID_REGISTERPAGE;
            win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
          }
        }
        else if (hitwin->winID >= ID_FINDDOC_DOC && hitwin->winID < ID_FINDDOC_DOC + 10)
        {                                                                              //ҽ����Ϣlabel �ؼ�
          if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��link���ڲ�����ʾ��׼���
            _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
        }

        break;
      }
    }
  }
}