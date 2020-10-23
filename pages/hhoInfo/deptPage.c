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
  y = 35;

  for (i = 0; i < deptlist->len; i++)
  {
    node = list_at(deptlist, i);
    if (node && node->val)
    {
      dept = (DeptInfo *)node->val;
      //Createhyperlink(parent, x, y + 30 * i, 65, 25, ID_DEPT_LINK + i, dept->deptname);
      btn = CreateButton(parent, x, y + 70 * i, 125, 45, ID_DEPT_LINK + i, dept->deptname);
      btn->data = ID_DEPT_LINK + i;
    }
  }

  list_destroy(deptlist);
}

hbasewinAttr *CreateDeptpage(hbasewinAttr *parent, int winID, char *title)
{
  hbasewinAttr *page = CreateWindowsEx(parent, PAGE_X, PAGE_Y, PAGE_W, PAGE_H, winID, title);
  hbasewinAttr *ctrl;

  Createhyperlink(page, 20, 10, 65, 25, ID_DEPT_RETURN, "[�� ҳ]");
  // ctrl = CreateLabel(page, 20, 25, PAGE_W - 40, PAGE_H, ID_DEPT_TEXT, title);
  // ctrl->wintype = LABEL_FILE_TXT; //����label ���ļ��ж�ȡ�ı�,����ʾ

  page->onPaint = OnPaint_Dept;
  page->onDestroy = OnDestory_Dept;
  page->EventHandler = EventHandler_deptpage;

  CreateDept(page);

  return page;
}

void OnDestory_Dept(hbasewinAttr *win, void *val)
{
}

void OnPaint_Dept(hbasewinAttr *win, void *val)
{
  int x, y;

  DeptInfo *dept;
  list_node_t *node;

  TESTNULLVOID(win);

  x = getAbsoluteX(win);
  y = getAbsoluteY(win);

  repaintChildren(win, val);

  rectangleEx(x, y + 35, 300, PAGE_H, 0x0000, 1, 2);
  rectangleEx(x + 305, y + 35, PAGE_W - 310, PAGE_H, 0x0000, 1, 2);
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
          if (hitwin->onLeave)
            hitwin->onLeave(hitwin, NULL);
          TRACE(("deptpage:mouse in button id = %d\n", hitwin->winID));
          //ת��homepage
        }
      }
      break;
    }
  }
}