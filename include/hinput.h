#ifndef __HINPUT_H__
#define __HINPUT_H__

#include "HBaseWin.h"

typedef struct _wininputinfo
{
  char pinyin[6];
  char *hz;
  char page;
  char status; //状态是否隐藏
  //unsigned int *pyBK;
} winInputInfo;

hbasewinAttr *CreatePYInput(hbasewinAttr *parent, int startx, int starty, int winID, char *title);

void OnPaint_PY(hbasewinAttr *py, void *val);
/**
 * 处理控制键: 回车/空格
 */
void OnKey_PY(hbasewinAttr *py, void *val);
/**
 * 处理按键
 */
void OnkeyPress_PY(hbasewinAttr *py, void *val);

/**
 * 离开
 */
void OnLeave_PY(hbasewinAttr *py, void *val);

void showPYInput(hbasewinAttr *py, int newx, int newy);

void hidePYInput(hbasewinAttr *py);
#endif