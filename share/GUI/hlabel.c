#include "HBaseWin.h"
#include "hlabel.h"
#include "pinyin.h"
#include "hglobal.h"
#include <memory.h>
#include <graphics.h>

hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title)
{
  hbasewinAttr *label;

  label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  label->onPaint = OnPaintLabel;

  return label;
}

void OnPaintLabel(hbasewinAttr *label, void *value)
{
  FILE *fpFont;
  int x, y, type = 3, len, linemax, maxline, i, flag = 0;
  char *temp = NULL;
  if (label == NULL)
    return;
  OnPaint(label, &type);

  if (label->title == NULL)
    return;
  fpFont = fopen(FILE_SIMSUN16 DATAPATH, "rt");
  len = strlen(label->title) / 2; //一个汉字两个char，len为字数
  x = getAbsoluteX(label);
  y = getAbsoluteY(label);
  linemax = label->nWidth / 16;
  maxline = label->nHeight / 16;
  temp = malloc(linemax * 2 + 1);
  memset(temp, 0, linemax * 2 + 1);
  if (len <= (linemax * maxline))
    maxline = len / linemax + ((len % linemax) == 0 ? 0 : 1);
  else
    flag = 1;
  // setcolor(RealDrawColor(WHITE));
  for (i = 0; i < maxline - 1; i++)
  {
    memcpy(temp, label->title + i * linemax * 2, linemax * 2);
    printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16);
  }
  if (flag == 1)
  {
    memcpy(temp, label->title + (maxline - 1) * linemax * 2, (linemax - 1) * 2);
    temp[2 * (linemax - 1)] = 0;
    printtextxy(fpFont, x, y + 16 * (maxline - 1), temp, 0 /*color*/, 16);
  }
  else
  {
    memcpy(temp, label->title + (maxline - 1) * linemax * 2, 2 * (len - linemax * (maxline - 1)));
    temp[2 * (len - linemax * (maxline - 1))] = 0;
    printtextxy(fpFont, x, y + 16 * (maxline - 1), temp, 0 /*color*/, 16);
  }

  //   outtextxy((label->nWidth - len) / 2 + x, (label->nHeight - 16) / 2 + y, label->title); //16个像素点一个字符
  //   outtextxy(x, (label->nHeight - 16) / 2 + y, label->title);
}

void LabelRead()
{
}