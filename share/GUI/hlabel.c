#include "macrodef.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "pinyin.h"
#include "hglobal.h"
#include <memory.h>
//#include <graphics.h>

/**
 * 1.写一个函数，专门读文件
 * 2.要考虑字体大小
 * 3.考虑到内存的原因，文件在显示的读取是不是更好一些？虽然速度慢一点 
 * 
 */
hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title)
{ //title指向文件名
  hbasewinAttr *label;
  FILE *fptemp;
  char *temp;
  int i, len;

  if (title == NULL)
  {
    label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
    return;
  }

  if (fptemp = fopen(title, "r") == NULL)
    label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  else
  {
    len = (nWidth / 16) * (nHeight / 16);
    temp = malloc(len * 2 + 1);
    memset(temp, 0, len * 2 + 1);
    for (i = 0; (fptemp != EOF) && (i < (len - 1) * 2); i++)
      fread(temp + i, 1, 1, fptemp);
    //最后一个字的判定
    if (fptemp != EOF)
    {                        //超长
      *(temp + i) = '…';     //单引号
      *(temp + len * 2) = 0; //测试，确保有结尾
    }
    else
    {
      fread(temp + i, 1, 2, fptemp);
    }
    label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, temp);
    fclose(fptemp);
  }
  label->onPaint = OnPaintLabel;

  return label;
}

/**
 * 10.05修改 by 张皓
 * 
 * 
 */
void OnPaintLabel(hbasewinAttr *label, void *value)
{
  //FILE *fpFont; by：张皓不需要
  int x, y, type = 3, len, linemax, maxline, i;
  char *temp = NULL;
  if (label == NULL)
    return;
  OnPaint(label, &type);

  if (label->title == NULL)
    return;
  //fpFont = fopen(FILE_SIMSUN16 DATAPATH, "rt");

  len = strlen(label->title) / 2; //一个汉字两个char，len为字数
  x = getAbsoluteX(label);
  y = getAbsoluteY(label);

  linemax = label->nWidth / 16;
  maxline = len / linemax + ((len % linemax) == 0 ? 0 : 1);
  temp = malloc(linemax * 2 + 1);
  memset(temp, 0, linemax * 2 + 1);
  for (i = 0; i < maxline - 1; i++)
  {
    memcpy(temp, label->title + i * linemax * 2, linemax * 2);
    //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16); modified by 张皓
    printText(x, y + 16 * i, temp, SIMSUN, 24, 0, 0x0 /*color*/); //显示文本的函数改了，不需要打开汉字字库文件指针
  }
  memcpy(temp, label->title + i * linemax * 2, (len - linemax * (maxline - 1)) * 2);
  //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16);
  printText(x, y + 16 * i, temp, SIMSUN, 24, 0, 0x0 /*color*/);
}