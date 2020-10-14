#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "hglobal.h"
#include "homepage.h"
#include "wResource.h"
#include <string.h>

void Homepage_MouseHandler(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;
  hbasewinAttr *hitwin;

  TESTNULLVOID(win);
  TESTNULLVOID(value);

  _g = (globaldef *)value;
  //确定是否是子窗口事件，非子窗口事件返回本身
  hitwin = checkmousewin(win, &_g->mouse);

  switch (hitwin->winID)
  {                 //鼠标在那个子窗口或本身
  case ID_HOMEPAGE: //本身
    if (type == EVENT_MOUSE)
    {                                                                              //homepage 处理鼠标移动理鼠标移动
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow) //在homepage窗口部分显示标准鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;

      if (_g->mouse.rightClickState == MOUSE_BUTTON_UP)
      {
        hbasewinAttr *child = CreateLabel(hitwin, random(800), random(600), 300, 150, ID_LABEL_3, NULL);
        if (child)
          child->onPaint(child, NULL);
      }
    }
    break;
  case ID_LABEL_1: //label1
    if (type == EVENT_MOUSE)
    {                                                                             //label1 处理鼠标移动理鼠标移动
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //改变显示 鼠标左键释放
        if (hitwin && hitwin->onPaint)
          hitwin->onPaint(hitwin, "c:\\hho\\data\\news\\1.txt");
      }
    }
    break;

  case ID_LABEL_2: //label2
    if (type == EVENT_MOUSE)
    {                                                                             //label1 处理鼠标移动理鼠标移动
      if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand) //在label1窗口部分显示手型鼠标
        _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_hand;

      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      { //改变显示
        if (hitwin && hitwin->onPaint)
          hitwin->onPaint(hitwin, "main.c");
      }
    }
    break;
  case ID_LABEL_3: //label3
    if (type == EVENT_MOUSE)
    { //不改变鼠标形状
      if (_g->mouse.leftClickState == MOUSE_BUTTON_UP)
      {
        list_node_t *n;
        //删除label
        int x = getAbsoluteX(hitwin);
        int y = getAbsoluteY(hitwin);
        fillRegionEx(x, y, hitwin->nWidth + 1, hitwin->nHeight + 1, 0xFFFF); //清除子窗口区域

        TRACE(("%s(%d): 删除label%u\n", __FILE__, __LINE__, hitwin->winID));
        hitwin->onDestroy(hitwin, NULL);        
      }
    }
    break;
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
  hbasewinAttr *label;
  TESTNULL(page, NULL);

  CreateLabel(page, 15, 90, 300, 150, ID_LABEL_1, "readme");
  CreateLabel(page, 15 + 350, 90, 300, 150, ID_LABEL_2, "c:\\hho\\data\\news\\1.txt");
  label = CreateLabel(page, 15 + 700, 90, 300, 150, ID_LABEL_3, NULL);
  label->value = malloc(10);
  strcpy((char *)label->value, "test");

  page->onPaint = OnPaint_homepage;
  page->EventHandler = EventHandler_homepage;

  return page;
}

void OnPaint_homepage(hbasewinAttr *win, void *value)
{
  TESTNULLVOID(win);

  //测试用
  //rectangleEx(win->x, win->y, win->nWidth, win->nHeight, 0xF801, 1, 3);

  repaintChildren(win);

  //disable unused warnning
  (void)value;
}
