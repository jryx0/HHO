#include "macrodef.h"
#include "wResource.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hyperlnk.h"
#include "hglobal.h"
#include "hnews.h"
#include "hlabel.h"
#include "hbutton.h"
#include "hdept.h"
#include "data.h"
#include "list.h"

///��̬����hyperlink
///��Ӧ��̬����hyperlink�¼�

void CreateDept(hbasewinAttr *parent)
{
  list_t *deptlist = ReadDeptInfo(DEPTINFOFILE);
  int i;
  list_node_t *node;
  DeptInfo *dept;
  int x, y;
  hbasewinAttr *btn;
  TESTNULLVOID(parent);

  x = 15;
  y = 55;

  for (i = 0; i < deptlist->len; i++)
  {
    node = list_at(deptlist, i);
    if (node && node->val)
    {
      dept = (DeptInfo *)node->val;
      //Createhyperlink(parent, x, y + 30 * i, 65, 25, ID_DEPT_LINK + i, dept->deptname);
      btn = CreateButton(parent, x, y + 50 * i, 125, 45, ID_DEPT_LINK + i, dept->deptname);
      btn->data = dept->id;
    }
  }

  list_destroy(deptlist);
}

hbasewinAttr *CreateDeptpage(hbasewinAttr *parent, int winID, char *title)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, title);
  hbasewinAttr *ctrl;
  WinStyle *style;

  Createhyperlink(page, 20, 10, 65, 25, ID_DEPT_RETURN, "[�� ҳ]");
  Createhyperlink(page, 170, PAGE_H - 30, 200, 25, ID_DEPT_DOCLINK, NULL);

  ctrl = CreateLabel(page, 160, 35, PAGE_W - 220, PAGE_H - 50, ID_DEPT_TEXT, NULL);
  ctrl->wintype = LABEL_FILE_TXT; //����label ���ļ��ж�ȡ�ı�,����ʾ

  page->onPaint = OnPaint_Dept;
  //page->onDestroy = OnDestory_Dept;
  page->EventHandler = EventHandler_deptpage;
  page->style = malloc(sizeof(WinStyle));
  getWinTheme((WinStyle *)page->style, 1);

  CreateDept(page);

  return page;
}

// void OnDestory_Dept(hbasewinAttr *win, void *val)
// {
// }

void OnPaint_Dept(hbasewinAttr *win, void *val)
{
  int x, y;
  WinStyle *style;
  hfont *_h;

  // DeptInfo *dept;
  // list_node_t *node;

  TESTNULLVOID(win);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  style = (WinStyle *)win->style;
  _h = getFont(style->fonttype, style->fontsize, 0x0000);
  printTextLineXY(x + style->fontsize * 6, y + 12, "- ���Ҽ��", _h);
  freeFont(_h);

  repaintChildren(win, val);

  // rectangleEx(x + 15, y + 35, 140, PAGE_H - 10, 0x0000, 1, 2);
  // rectangleEx(x + 160, y + 25, PAGE_W - 250, PAGE_H - 100, 0x0000, 1, 2);
}

void EventHandler_deptpage(hbasewinAttr *win, int type, void *val)
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
    case ID_DEPTPAGE:
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //��homepage���ڲ�����ʾ��׼���
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
      break;
    case ID_DEPT_RETURN:
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
    case ID_DEPT_DOCLINK:
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

        //ת��login
        if (_g->isLogin)
        {
        }
        else if (win->parent && win->parent->winID == ID_DESKTOP) //�ҵ�desktop
        {
          _g->activePageID = ID_LOGINPAGE;
          win->parent->EventHandler(win->parent, EVENT_PAGE_CHANGE, _g);
        }
      }
      break;
    default:
      if (hitwin->winID >= ID_DEPT_LINK && hitwin->winID < ID_DEPT_LINK + 10)       //+ win->data)
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
          hbasewinAttr *ctrl = findWinByID(win, ID_DEPT_TEXT);
          if (hitwin->onLeave)
            hitwin->onLeave(hitwin, NULL);
          //TRACE(("deptpage:mouse in button id = %d\n", hitwin->winID));
          //label����ʾ ����
          if (ctrl)
          {
            ctrl->wintype = LABEL_FILE_TXT;
            if (ctrl->title != NULL)
              free(ctrl->title);

            ctrl->title = malloc(64);
            sprintf(ctrl->title, "%sdatabase\\dept\\%d.txt", DATAPATH, hitwin->data);
            ctrl->onPaint(ctrl, NULL);
          }
          //ת��ҽ���Һ�
          ctrl = findWinByID(win, ID_DEPT_DOCLINK);
          if (ctrl)
          {
            if (ctrl->title)
              free(ctrl->title);
            ctrl->title = malloc(32);
            sprintf(ctrl->title, "[ѡ��%sҽ���Һ�]     ", hitwin->title);
            ctrl->onPaint(ctrl, NULL);
            ctrl->data = hitwin->data; // ���ұ�� �ڿ� = 901
          }
        }
      }
      break;
    }
  }
}