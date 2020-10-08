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
extern char *sys_errlist[];
/**
 * label-显示文本文件中的内容
 * 
 */
hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title)
{
  hbasewinAttr *label;

  label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  label->onPaint = OnPaint_Label;
  label->onDestroy = OnDestoy_Lable;

  label->EventHandler = EventHandler_Label;

  return label;
}

/**
 * 删除自身
 * 
 * 
 */
void OnDestoy_Lable(hbasewinAttr *label, void *value)
{
  TESTNULLVOID(label);
  destoryChildren(label);

  OnDestory(label, value);

  free(label);
  label = NULL;
}

/**
 *  
 * 绘制label
 * 
 */
void OnPaint_Label(hbasewinAttr *label, void *value)
{
  int errNum = 0;
  FILE *fpFont;
  FILE *fptemp;
  hregion _region;
  char str[30];

  // int x, y, type = 3, len, linemax, line, i, maxline;
  // char *temp, *content;
  // if (label == NULL)
  //   return;
  // OnPaint(label, &type);
  OnPaint(label, NULL);

  TESTNULLVOID(label);
  TESTNULLVOID(label->title);

  if (value == NULL)
    fptemp = fopen(label->title, "r");
  else
    fptemp = fopen((char *)value, "r");

  if (fptemp == NULL)
  {     
    return;
  }

  fpFont = getFont(SIMSUN, 16, 0x00);

  _region.left_top.x = getAbsoluteX(label);
  _region.left_top.y = getAbsoluteY(label);
  _region.right_bottom.x = _region.left_top.x + label->nWidth;
  _region.right_bottom.y = _region.left_top.y + label->nHeight;

  printTextFile(&_region, fptemp, fpFont);
  freeFont(fpFont);
  fclose(fptemp);

  fpFont = NULL;
  fptemp = NULL;

  // x = getAbsoluteX(label);
  // y = getAbsoluteY(label);

  // linemax = label->nWidth / 16;
  // maxline = label->nHeight / 16;
  // if (linemax == 0 || maxline == 0)
  //   return;
  // if (fptemp = fopen(label->title, "rt") == NULL)
  // { //字符串小标签，超长不显示
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
  //   // 最后一个字的判定
  //   if (fptemp != EOF)
  //   {                           //超长
  //     *(content + i) = '…';     //单引号
  //     *(content + len * 2) = 0; //测试，确保有结尾
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
  // len = strlen(label->title) / 2; //一个汉字两个char，len为字数
  // line = len / linemax + ((len % linemax) == 0 ? 0 : 1);
  // temp = malloc(linemax * 2 + 1);
  // memset(temp, 0, linemax * 2 + 1);
  // for (i = 0; i < line - 1; i++)
  // {
  //   memcpy(temp, label->title + i * linemax * 2, linemax * 2);
  //   //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16); modified by 张皓
  //   printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/); //显示文本的函数改了，不需要打开汉字字库文件指针
  // }
  // memcpy(temp, label->title + i * linemax * 2, (len - linemax * (line - 1)) * 2);
  // //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16);
  // printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/);
}

void EventHandler_Label(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;

  TESTNULLVOID(win);
  TESTNULLVOID(value);
  _g = (globaldef *)value;
  switch (type)
  {
  case MOUSE_EVENT: //改变鼠标形状
    if (_g->mouse.currentCur != _g->cursor_hand)
      _g->mouse.currentCur = _g->cursor_hand;

    switch (_g->mouse.leftClickState)
    {
    case MOUSE_BUTTON_UP:
      //鼠标左键value
      //删除自身 跳转页面
      if (win->parent && win->parent->EventHandler)
        win->parent->EventHandler(win, MOUSE_BUTTON_UP, value);
      break;
    default:
      break;
    }

    break;

  default:
    break;
  }
}
