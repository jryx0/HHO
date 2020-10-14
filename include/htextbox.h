#ifndef __HTEXTBOX_H__
#define __HTEXTBOX_H__

#include "hhosvga.h"
#include "HBaseWin.h"

typedef struct _textinfo
{
  hregion r;
  int x;
  int y;
  int width;
  int height;
  int curx;
  int cury;
  int active;
} textInfo;

hbasewinAttr *CreateTextBox(hbasewinAttr *parent, int x, int y, int nWidth,
                            int nHeight, int winID, const char *title);

void OnPaint_TextBox(hbasewinAttr *link, void *value);
/**
 * 点击时,设置当前textbox为活动控件
 * 
 */
void OnClick_TextBox(hbasewinAttr *tb, void *value);

/**
 * 点击时,设置当前textbox为活动控件
 * 
 */
void OnLeave_TextBox(hbasewinAttr *tb, void *value);

/**
 * 点击时,设置当前textbox为活动控件
 * 
 */
void OnDestory_TextBox(hbasewinAttr *tb, void *value);
/**
 * textbox 激活时处理 
 * 
 */
void OnActive_TextBox(hbasewinAttr *tb, void *value);

/**
 * 绘制文本框光标
 * @param textbox 要绘制的textbox
 * @param blink 闪烁标志
 */
void DrawTextCursor(hbasewinAttr *textbox, unsigned int blink);

#endif