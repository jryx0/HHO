#include "macrodef.h"
#include "hhosvga.h"
#include "hglobal.h"
#include "pinyin.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "mouse.h"

#include <memory.h>
#include <errno.h>
#include <stdlib.h>

/**
 * label-��ʾ�ı��ļ��е�����
 * 
 */
hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title)
{
  hbasewinAttr *label;

  label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  label->onPaint = OnPaint_Label;

  label->style = malloc(sizeof(WinStyle));
  memset(label->style, 0, sizeof(WinStyle));
  getWinTheme(label->style, 1);

  return label;
}

void OnPaint_Label_Title(hbasewinAttr *label, void *value)
{
  hfont *fpfont;
  hregion _region;
  WinStyle *lbstyle;

  TESTNULLVOID(label);
  TESTNULLVOID(label->title)
  lbstyle = label->style;

  if (lbstyle)
    fpfont = getFont(lbstyle->fonttype, lbstyle->fontsize, lbstyle->fontcolor);
  else
    fpfont = getFont(SIMSUN, 16, 0x00);

  _region.left_top.x = getAbsoluteX(label);
  _region.left_top.y = getAbsoluteY(label);
  _region.right_bottom.x = _region.left_top.x + label->nWidth;
  _region.right_bottom.y = _region.left_top.y + label->nHeight;
  fillRegion(_region.left_top.x, _region.left_top.y, _region.right_bottom.x, _region.right_bottom.y, 0xFFFF);
  printTextEx(&_region, label->title, fpfont);

  freeFont(fpfont);
  (void)value;
}

void OnPaint_Label_File(hbasewinAttr *label, void *value)
{
  hfont *fpfont;
  hregion _region;
  WinStyle *style;
  FILE *fpFile;
  
  TESTNULLVOID(label);
  TESTNULLVOID(label->title)
  TESTNULLVOID(label->style);
  style = label->style;

  if (style)
    fpfont = getFont(style->fonttype, style->fontsize, style->fontcolor);
  else
    fpfont = getFont(SIMSUN, 16, 0x00);

  _region.left_top.x = getAbsoluteX(label);
  _region.left_top.y = getAbsoluteY(label);
  _region.right_bottom.x = _region.left_top.x + label->nWidth;
  _region.right_bottom.y = _region.left_top.y + label->nHeight;

  fillRegion(_region.left_top.x, _region.left_top.y, _region.right_bottom.x, _region.right_bottom.y, 0xFFFF);

  
  fpFile = fopen(label->title, "r");
  if (fpFile)
    printTextFileV6(&_region, fpFile, fpfont);

  freeFont(fpfont);
  fclose(fpFile);
  (void)value;
}

void OnPaint_Label_Img(hbasewinAttr *label, void *value)
{
  TESTNULLVOID(label);
  TESTNULLVOID(label->title);

  fillRegionEx(getAbsoluteX(label), getAbsoluteY(label), label->x, label->y, 0xFFFF);
  //Putbmp64k(getAbsoluteX(label), getAbsoluteY(label), label->title);
  Putbmp565(getAbsoluteX(label), getAbsoluteY(label), label->title);
  (void)value;
}

/**
 *  
 * ����label
 * 
 */
void OnPaint_Label(hbasewinAttr *label, void *value)
{
  switch (label->wintype)
  {
  case LABEL_TITLE:
    OnPaint_Label_Title(label, value);
    break;
  case LABEL_FILE_TXT:
    OnPaint_Label_File(label, value);
    break;
  case LABEL_FILE_IMG:
    OnPaint_Label_Img(label, value);
    break;

  default:
    break;
  }

  // x = getAbsoluteX(label);
  // y = getAbsoluteY(label);

  // linemax = label->nWidth / 16;
  // maxline = label->nHeight / 16;
  // if (linemax == 0 || maxline == 0)
  //   return;
  // if (fptemp = fopen(label->title, "rt") == NULL)
  // { //�ַ���С��ǩ����������ʾ
  //   len = sizeof(label->title) / 2;
  //   content = malloc(len * 2 + 1);
  //   memset(content, 0, len * 2 + 1);
  //   memcpy(content, label->title, len * 2);
  //   line = len / linemax + 1;
  //   if (line > maxline)
  //     line = maxline;
  // }
  // else
  // {
  //   len = (label->nWidth / 16) * (label->nHeight / 16);
  //   content = malloc(len * 2 + 1);
  //   memset(content, 0, len * 2 + 1);
  //   for (i = 0; (fptemp != EOF) && (i < (len - 1) * 2); i++)
  //     fread(content + i, 1, 1, fptemp);
  //   // ���һ���ֵ��ж�
  //   if (fptemp != EOF)
  //   {                           //����
  //     *(content + i) = '��';     //������
  //     *(content + len * 2) = 0; //���ԣ�ȷ���н�β
  //   }
  //   fclose(fptemp);
  // }
  // temp = malloc(linemax * 2 + 1);
  // for (i = 0; i < line - 1; i++)
  // {
  //   memcpy(temp, content + i * linemax * 2, linemax * 2);
  //   printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/);
  // }
  // memcpy(temp, content + i * linemax * 2, (len - linemax * i) * 2);
  // printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/);
  // free(temp);
  // free(content);
  // len = strlen(label->title) / 2; //һ����������char��lenΪ����
  // line = len / linemax + ((len % linemax) == 0 ? 0 : 1);
  // temp = malloc(linemax * 2 + 1);
  // memset(temp, 0, linemax * 2 + 1);
  // for (i = 0; i < line - 1; i++)
  // {
  //   memcpy(temp, label->title + i * linemax * 2, linemax * 2);
  //   //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16); modified by ���
  //   printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/); //��ʾ�ı��ĺ������ˣ�����Ҫ�򿪺����ֿ��ļ�ָ��
  // }
  // memcpy(temp, label->title + i * linemax * 2, (len - linemax * (line - 1)) * 2);
  // //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16);
  // printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/);

  (void)value;
}
