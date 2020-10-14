#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "htextbox.h"
#include "hglobal.h"

#define ACTIVE 1
#define INACTIVE 0
#define FREQ 40

hbasewinAttr *CreateTextBox(hbasewinAttr *parent, int x, int y, int nWidth,
                            int nHeight, int winID, const char *title)
{
  hbasewinAttr *tb = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  TESTNULL(tb, NULL);
  tb->onPaint = OnPaint_TextBox;
  tb->onClick = OnClick_TextBox;
  tb->onLeave = OnLeave_TextBox;
  tb->onDestroy = OnDestory_TextBox;
  tb->onActivate = OnActive_TextBox;

  tb->value = malloc(2); //int 控制活动状态时的绘制
  tb->wintype = TEXTBOX;

  return tb;
}

void OnDestory_TextBox(hbasewinAttr *tb, void *value)
{
  //TRACE(("TEXTBOX destory1\n"));
  TESTNULLVOID(tb);
  OnLeave_TextBox(tb, NULL);
  // TRACE(("TEXTBOX destory2\n"));
  (void)value;
}

/**
 * 点击时,设置当前textbox为活动控件
 * 
 */
void OnClick_TextBox(hbasewinAttr *tb, void *value)
{
  TESTNULLVOID(tb);
  *(int *)(tb->value) = ACTIVE; //set textbox active
  (void)value;
}

/**
 * 点击时,设置当前textbox为活动控件
 * 
 */
void OnLeave_TextBox(hbasewinAttr *tb, void *value)
{
  TESTNULLVOID(tb);
  *(int *)(tb->value) = INACTIVE; //set textbox inactive
  DrawTextCursor(tb, 1);
  (void)value;
}

void OnPaint_TextBox(hbasewinAttr *tb, void *value)
{
  globaldef *_g;
  int x = 0, y = 0;

  TESTNULLVOID(tb);

  x = getAbsoluteX(tb);
  y = getAbsoluteY(tb);

  if (*(int *)(tb->value) == ACTIVE)
  {
    rectangle(x, y, x + tb->nWidth, y + tb->nHeight, 0x03DF, 2, 1);
  }
  else
  {
    rectangle(x, y, x + tb->nWidth, y + tb->nHeight, 0xFFFF, 2, 1);
    rectangle(x, y, x + tb->nWidth, y + tb->nHeight, 0x6BAF, 1, 1);
  }
  (void)value;
}

/**
 * 激活窗口，单tb=NULL，只设置原textbox为未激活
 * 
 * @param tb 需要激活的窗口
 * @param value globaldef,需要foucsedTextBox
 * 
 */
void OnActive_TextBox(hbasewinAttr *tb, void *value)
{

  globaldef *_g = (globaldef *)value;
  hbasewinAttr *prevtb = NULL;

  if (_g)
    prevtb = _g->foucsedTextBox;

  //prevtb为空，说明无激活窗口
  if (prevtb && prevtb->onLeave) //上一个textbox 离开
  {
    prevtb->onLeave(prevtb, NULL);
    prevtb->onPaint(prevtb, NULL);
  }

  if (tb && tb->onClick) //现有textbox
  {
    tb->onClick(tb, NULL);
    tb->onPaint(tb, NULL);
  }

  _g->foucsedTextBox = tb;
  //TRACE(("tb=%u,_g->foucsedTextBox=%u\n", tb, _g->foucsedTextBox));
}

/**
 * 绘制文本框光标
 * @param textbox 要绘制的窗口
 * @param blink 闪烁标志
 */
void DrawTextCursor(hbasewinAttr *textbox, unsigned int blink)
{
  static unsigned int color = 0xFFFF;
  TESTNULLVOID(textbox);

  if (blink % FREQ < FREQ / 2 + 3)
  {
    if (color == 0xFFFF) //减少闪烁频次
      return;
    color = 0xFFFF; //显示
  }
  else
  {
    if (color == 0x0000) //减少闪烁频次
      return;
    color = 0x0000; //隐藏
  }

  liney_styleEx(getAbsoluteX(textbox) + 10, getAbsoluteY(textbox) + 6,
                23, color, 2, 1);
}