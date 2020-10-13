#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "htextbox.h"
#include "hglobal.h"

#define ACTIVE 1
#define INACTIVE 0

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
  TRACE(("TEXTBOX destory1\n"));
  TESTNULLVOID(tb);
  OnLeave_TextBox(tb, NULL);
  TRACE(("TEXTBOX destory2\n"));
}

/**
 * 点击时,设置当前textbox为活动控件
 * 
 */
void OnClick_TextBox(hbasewinAttr *tb, void *value)
{
  TESTNULLVOID(tb);
  *(int *)(tb->value) = ACTIVE; //set textbox active
}

/**
 * 点击时,设置当前textbox为活动控件
 * 
 */
void OnLeave_TextBox(hbasewinAttr *tb, void *value)
{
  TESTNULLVOID(tb);
  *(int *)(tb->value) = INACTIVE; //set textbox inactive
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
}

void OnActive_TextBox(hbasewinAttr *tb, void *value)
{
  hbasewinAttr *prevtb = (hbasewinAttr *)value;

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
}
