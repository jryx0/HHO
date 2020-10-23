#include "HBaseWin.h"
#include "hhosvga.h"
#include "hchkbox.h"
#include <string.h>

hbasewinAttr *CreateCheckBox(hbasewinAttr *parent, int x, int y, int nWidth,
                             int nHeight, int winID, const char *title)
{
  hbasewinAttr *checkbox;
  // if (nHeight < 16 || nWidth < 32)
  //   return NULL;
  checkbox = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  checkbox->onPaint = OnPaintCheckBox;
  checkbox->onClick = OnClickCheckBox;

  return checkbox;
}

void OnPaintCheckBoxNone(hbasewinAttr *checkbox, void *value)
{
  int x0, y0, x1, y1, x2, y2;//, type = 3;
  if (checkbox == NULL)
    return;
  //OnPaint(checkbox, &type);
  x0 = getAbsoluteX(checkbox);
  y0 = getAbsoluteY(checkbox);
  // x1 = x0 + 4;
  // y1 = y0 + (checkbox->nHeight - 9) / 2;
  // x2 = x1 + 32;
  // y2 = y1 + 32;
  x1 = x0 + checkbox->nWidth;
  y1 = y0 + checkbox->nHeight;
  fillRegion(x0, y0, x1, y1, 0xFFFF);
  rectangle(x0 - 1, y0 - 1, x1 + 1, y1 + 1, 0x0000, 1, 1);
  (void) value;
}

void OnPaintCheckBoxRight(hbasewinAttr *checkbox, void *value)
{
  int x0, y0, x1, y1, x2, y2;//, type = 3;
  if (checkbox == NULL)
    return;
  //OnPaint(checkbox, &type);
  x0 = getAbsoluteX(checkbox);
  y0 = getAbsoluteY(checkbox);
  x1 = x0 + 4;
  y1 = y0 + (checkbox->nHeight - 9) / 2;
  x2 = x1 + 9;
  y2 = y1 + 9;
  fillRegion(x1, y1, x2, y2, 0xFFFF);
  rectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, 0x0000, 1, 1);
  line(x1, y1 + 5, x1 + 5, y2, 0);
  line(x1 + 5, y2, x2, y1, 0);
  (void) value;
}

void OnPaintCheckBoxCross(hbasewinAttr *checkbox, void *value)
{
  int x0, y0, x1, y1, x2, y2;//, type = 3;
  if (checkbox == NULL)
    return;
  //OnPaint(checkbox, &type);
  x0 = getAbsoluteX(checkbox);
  y0 = getAbsoluteY(checkbox);
  x1 = x0 + 4;
  y1 = y0 + (checkbox->nHeight - 9) / 2;
  x2 = x1 + 9;
  y2 = y1 + 9;
  fillRegion(x1, y1, x2, y2, 0xFFFF);
  rectangle(x1 - 1, y1 - 1, x2 + 1, y2 + 1, 0x0000, 1, 1);
  line(x1, y1, x2, y2, 0);
  line(x1, y2, x2, y1, 0);
  (void) value;
}

void OnClickCheckBox(hbasewinAttr *checkbox, void *value)
{
  TESTNULLVOID(checkbox);

  if (++checkbox->data >= 3)
    checkbox->data = 0;
    (void)value;
}

void OnPaintCheckBox(hbasewinAttr *checkbox, void *value)
{

  switch (checkbox->data)
  {
  case 1:
    OnPaintCheckBoxRight(checkbox, value);
    break;
  case 2:
    OnPaintCheckBoxCross(checkbox, value);
    break;
  default:
    OnPaintCheckBoxNone(checkbox, value);
    break;
  }
}