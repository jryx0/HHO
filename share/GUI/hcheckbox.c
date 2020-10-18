#include "HBaseWin.h"
#include "hhosvga.h"
#include "hcheckbox.h"
#include <string.h>

hbasewinAttr *CreateCheckBox(hbasewinAttr *parent, int x, int y, int nWidth,
                             int nHeight, int winID, const char *title)
{
  hbasewinAttr *checkbox;
  if (nHeight < 16 || nWidth < 32)
    return NULL;
  checkbox = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  checkbox->onPaint = OnPaintCheckBox;
}

void OnPaintCheckBoxNone(hbasewinAttr *checkbox, void *value)
{
  int x0, y0, x1, y1, x2, y2, type = 3;
  if (checkbox == NULL)
    return;
  OnPaint(checkbox, &type);
  x0 = getAbsoluteX(checkbox);
  y0 = getAbsoluteX(checkbox);
  x1 = x0 + 4;
  y1 = (checkbox->nHeight - 9) / 2;
  x2 = x1 + 9;
  y2 = y1 + 9;
  fillRegion(x1, y1, x2, y2, 0xFFFF);
  rectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, 0);
}

void OnPaintCheckBoxRight(hbasewinAttr *checkbox, void *value)
{
  int x0, y0, x1, y1, x2, y2, type = 3;
  if (checkbox == NULL)
    return;
  OnPaint(checkbox, &type);
  x0 = getAbsoluteX(checkbox);
  y0 = getAbsoluteX(checkbox);
  x1 = x0 + 4;
  y1 = (checkbox->nHeight - 9) / 2;
  x2 = x1 + 9;
  y2 = y1 + 9;
  fillRegion(x1, y1, x2, y2, 0xFFFF);
  rectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, 0);
  line(x1, y1 + 5, x1 + 5, y2, 0);
  line(x1 + 5, y2, x2, y1, 0);
}

void OnPaintCheckBoxCross(hbasewinAttr *checkbox, void *value)
{
  int x0, y0, x1, y1, x2, y2, type = 3;
  if (checkbox == NULL)
    return;
  OnPaint(checkbox, &type);
  x0 = getAbsoluteX(checkbox);
  y0 = getAbsoluteX(checkbox);
  x1 = x0 + 4;
  y1 = (checkbox->nHeight - 9) / 2;
  x2 = x1 + 9;
  y2 = y1 + 9;
  fillRegion(x1, y1, x2, y2, 0xFFFF);
  rectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, 0);
  line(x1, y1, x2, y2, 0);
  line(x1, y2, x2, y1, 0);
}

void OnPaintCheckBox(hbasewinAttr *checkbox, void *value)
{
  int judge;
  judge = *(checkbox->value);
  switch (judge)
  {
  case 1:
    OnPaintCheckBoxRight(hbasewinAttr * checkbox, void *value);
    break;
  case 2:
    OnPaintCheckBoxCross(hbasewinAttr * checkbox, void *value);
    break;
  default:
    OnPaintCheckBoxNone(hbasewinAttr * checkbox, void *value);
    break;
  }
}