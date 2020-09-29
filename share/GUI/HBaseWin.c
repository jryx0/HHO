#include "HBaseWin.h"
#include "wResource.h"
#include "SVGAUTIL.H"
#include "list.h"

#include <memory.h>
#include <graphics.h>
#include <string.h>

hbasewinAttr *CreateWindowsEx(hbasewinAttr *parent, int x, int y, int nWidth,
                              int nHeight, int winID, const char *title)
{
  hbasewinAttr *HHOwin = malloc(sizeof(hbasewinAttr));
  memset(HHOwin, 0, sizeof(hbasewinAttr));

  HHOwin->parent = parent;
  HHOwin->hhowintype = WIN;
  HHOwin->winID = winID;
  HHOwin->x = x;
  HHOwin->y = y;
  HHOwin->nWidth = nWidth;
  HHOwin->nHeight = nHeight;
  if (title != NULL)
  {
    HHOwin->title = malloc(strlen(title) + 1);
    memset(HHOwin->title, 0, strlen(title) + 1);
    strcpy(HHOwin->title, title);
  }
  addChild(parent, HHOwin);
  HHOwin->onPaint = OnPaint;
  return HHOwin;
}

void DestoryWindows(hbasewinAttr *win)
{
}

hbasewinAttr *addChild(hbasewinAttr *parent, hbasewinAttr *child)
{
  if (parent == NULL)
    return parent;
  if (child == NULL)
    return child;
  if (parent->children == NULL)
    parent->children = list_new();
  list_rpush(parent->children, list_node_new(child));
  return parent;
}

/**
 *
 *   x1,y1                          x2,y1
 *     +---------------------------+
 *     | x1+1,y1+1    x2-1,y1+1    |
 *     |      +--------+           |
 *     |      |        |           |
 *     |      +--------+           |
 *     |  x1+1,y2-1   x2-1,y2-1    |
 *     +---------------------------+
 *   x1,y2                         x2,y2
 */
void OnPaint(hbasewinAttr *win, void *val)
{
  int x1, x2, y1, y2;
  int type;

  if (win == NULL)
    return;
  if (val == NULL)
    type = 0;
  else
    type = *(int *)val;
  x1 = getAbsoluteX(win);
  y1 = getAbsoluteY(win);
  x2 = x1 + win->nWidth;
  y2 = y1 + win->nHeight;

  switch (type)
  {
  case 1: //͹��
    //��Ȧ
    setcolor(RealDrawColor(Shade0));
    line(x1, y1, x2, y1);
    line(x1, y1, x1, y2);
    setcolor(RealDrawColor(Shade3));
    line(x2, y1, x2, y2);
    line(x1, y2, x2, y2);
    //��Ȧ
    setcolor(RealDrawColor(Shade1));
    line(x1 + 1, y1 + 1, x2 - 1, y1 + 1);
    line(x1 + 1, y1 + 1, x1 + 1, y2 - 1);
    setcolor(RealDrawColor(Shade2));
    line(x2 - 1, y1 + 1, x2 - 1, y2 - 1);
    line(x1 + 1, y2 - 1, x2 - 1, y2 - 1);
    //�������
    setfillstyle(SOLID_FILL, RealFillColor(Shade1));
    bar(x1 + 2, y1 + 2, x2 - 2, y2 - 2);
    break;
  case 2: //����
    //��Ȧ
    setcolor(RealDrawColor(Shade3));
    line(x1, y1, x2, y1);
    line(x1, y1, x1, y2);
    setcolor(RealDrawColor(Shade0));
    line(x2, y1, x2, y2);
    line(x1, y2, x2, y2);
    //��Ȧ
    setcolor(RealDrawColor(Shade2));
    line(x1 + 1, y1 + 1, x2 - 1, y1 + 1);
    line(x1 + 1, y1 + 1, x1 + 1, y2 - 1);
    setcolor(RealDrawColor(Shade1));
    line(x2 - 1, y1 + 1, x2 - 1, y2 - 1);
    line(x1 + 1, y2 - 1, x2 - 1, y2 - 1);
    //�������
    setfillstyle(SOLID_FILL, RealFillColor(Shade1));
    bar(x1 + 2, y1 + 2, x2 - 2, y2 - 2);
    break;
  default: //ƽ��
    setfillstyle(SOLID_FILL, RealFillColor(Shade1));
    bar(x1, y1, x2, y2);
    setcolor(RealDrawColor(Shade3));
    rectangle(x1, y1, x2, y2);
    break;
  }
}

int getAbsoluteX(hbasewinAttr *win)
{
  if (win == NULL)
    return 0;
  // if (win->parent==NULL)
  //   return win->x;
  return win->x + getAbsoluteX(win->parent);
}

int getAbsoluteY(hbasewinAttr *win)
{
  if (win == NULL)
    return 0;
  // if (win->parent==NULL)
  //   return win->y;
  return win->y + getAbsoluteY(win->parent);
}

void repaintChildren(hbasewinAttr *win)
{
  list_iterator_t *it;
  list_node_t *node;
  hbasewinAttr *childwin;

  if (win == NULL || win->children == NULL)
    return;

  it = list_iterator_new(win->children, LIST_HEAD);
  while ((node = list_iterator_next(it)))
  {
    if (node->val == NULL)
      continue;

    childwin = (hbasewinAttr *)(node->val);
    if (childwin->onPaint != NULL)
      childwin->onPaint(childwin, NULL);
  }
  list_iterator_destroy(it);
}

int checkpointInside(hbasewinAttr *win, int x, int y) {
  int x1, y1, x2, y2;
  if (win == NULL)
    return 0;

  x1 = getAbsoluteX(win);
  y1 = getAbsoluteY(win);
  x2 = x1 + win->nWidth;
  y2 = y1 + win->nHeight;

  return ((x1 < x && x < x2) && (y1 < y && y < y2));
}