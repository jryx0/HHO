#include "HBaseWin.h"
#include "hbutton.h"
#include <graphics.h>
#include <string.h>

hbasewinAttr *CreateButton(hbasewinAttr *parent, int x, int y, int nWidth,
                           int nHeight, int winID, const char *title)
{
  hbasewinAttr *button;

  button = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  button->onPaint = OnPaintButtonUp;
  button->onLeftUp = OnLeftUpButton;
  button->onLeftDown = OnLeftDownButton;
  return button;
}

void OnPaintButtonUp(hbasewinAttr *btn, void *value)
{
  int x, y, type = 1, len;
  if (btn == NULL)
    return;
  OnPaint(btn, &type);
  if (btn->title == NULL)
    return;
  len = strlen(btn->title) * 8;
  x = getAbsoluteX(btn);
  y = getAbsoluteY(btn);
  setcolor(RealDrawColor(WHITE));
  if (len < btn->nWidth)
    outtextxy((btn->nWidth - len) / 2 + x, (btn->nHeight - 8) / 2 + y, btn->title); //8个像素点一个字符
  else
    outtextxy(x, (btn->nHeight - 8) / 2 + y, btn->title);
}

void OnPaintButtonDown(hbasewinAttr *btn, void *value)
{
  int x, y, type = 2, len;
  if (btn == NULL)
    return;
  OnPaint(btn, &type);
  if (btn->title == NULL)
    return;
  len = strlen(btn->title) * 8;
  x = getAbsoluteX(btn);
  y = getAbsoluteY(btn);
  setcolor(RealDrawColor(WHITE));
  if (len < btn->nWidth)
    outtextxy((btn->nWidth - len) / 2 + x, (btn->nHeight - 8) / 2 + y, btn->title); //8个像素点一个字符
  else
    outtextxy(x, (btn->nHeight - 8) / 2 + y, btn->title);
}
void OnLeftUpButton(hbasewinAttr *btn, void *value)
{
  if (btn == NULL)
    return;
  btn->onPaint=OnPaintButtonUp;
}
void OnLeftDownButton(hbasewinAttr *btn, void *value)
{
  if (btn == NULL)
    return;
  btn->onPaint=OnPaintButtonDown;
}
