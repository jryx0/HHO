//#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "htextbox.h"
#include "hglobal.h"
#include "hyperlnk.h"
#include <memory.h>
#include <string.h>

#define ACTIVE 1
#define INACTIVE 0
#define FREQ 30

#define DEFAULT_FONTNAME SIMSUN
#define DEFAULT_FONTSIZE 24
#define DEFAULT_FONTCOLOR 0x0000

hbasewinAttr *CreateTextBox(hbasewinAttr *parent, int x, int y, int nWidth,
                            int nHeight, int winID, const char *title)
{
  textInfo *ti;
  hbasewinAttr *tb = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  TESTNULL(tb, NULL);
  tb->onPaint = OnPaint_TextBox;
  tb->onClick = OnClick_TextBox;
  tb->onLeave = OnLeave_TextBox;
  tb->onDestroy = OnDestory_TextBox;
  tb->onActivate = OnActive_TextBox;
  tb->onKeyPress = OnKeyPress_Textbox;
  tb->onKey = OnKey_Textbox;
  tb->wintype = TEXTBOX;

  ti = malloc(sizeof(textInfo));

  ti->r.left_top.x = getAbsoluteX(tb) + 6;
  ti->r.left_top.y = getAbsoluteY(tb) + 6;
  ti->r.right_bottom.x = ti->r.left_top.x + tb->nWidth - 6 - 2;
  ti->r.right_bottom.y = ti->r.left_top.y + tb->nHeight - 6 - 2;
  ti->active = INACTIVE;
  ti->fontsize = DEFAULT_FONTSIZE;
  if (title)
  {
    ti->textMaxlen = strlen(title);
    ti->curTextindex = ti->textMaxlen;
  }
  else
    ti->curTextindex = 0;

  tb->value = ti; // malloc(2); //int 控制活动状态时的绘制
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
  textInfo *ti;
  TESTNULLVOID(tb);
  ti = tb->value;

  if (ti)
    ti->active = ACTIVE;

  // *(int *)(tb->value) = ACTIVE; //set textbox active
  (void)value;
}

/**
 * 点击时,设置当前textbox为活动控件
 * 
 */
void OnLeave_TextBox(hbasewinAttr *tb, void *value)
{
  textInfo *ti;
  TESTNULLVOID(tb);
  ti = tb->value;

  if (ti)
    ti->active = INACTIVE;
  //*(int *)(tb->value) = INACTIVE; //set textbox inactive
  DrawTextCursor(tb, FREQ - 1);
  (void)value;
}

void OnPaint_TextBox(hbasewinAttr *tb, void *value)
{
  textInfo *ti;
  int x = 0, y = 0;

  TESTNULLVOID(tb);

  ti = tb->value;

  x = getAbsoluteX(tb);
  y = getAbsoluteY(tb);

  if (ti)
  {
    hfont *_font = getFont(DEFAULT_FONTNAME, DEFAULT_FONTSIZE, DEFAULT_FONTCOLOR);
    if (ti->active == ACTIVE)
    // if (*(int *)(tb->value) == ACTIVE)
    {
      rectangle(x, y, x + tb->nWidth, y + tb->nHeight, 0x03DF, 2, 1);
    }
    else
    {
      rectangle(x, y, x + tb->nWidth, y + tb->nHeight, 0xFFFF, 2, 1);
      rectangle(x, y, x + tb->nWidth, y + tb->nHeight, 0x6BAF, 1, 1);
    }

    fillRegion(ti->r.left_top.x, ti->r.left_top.y, ti->r.right_bottom.x, ti->r.right_bottom.y, 0xFFFF);

    if (tb->title)
    {
      //printTextEx(&(ti->r), tb->title, _font);
      if (tb->wintype == TEXTBOX_PASSWORD)
      { //打印****
        char *password = malloc(strlen(tb->title) + 1);
        memset(password, 0, strlen(tb->title) + 1);
        memset(password, '*', strlen(tb->title));

        printTextEx4(&ti->r, password, _font, &ti->curx, &ti->cury);
        ti->maxCol = printTextEx5(&ti->r, password, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);

        free(password);
      }
      else
      { //正常打印
        printTextEx4(&ti->r, tb->title, _font, &ti->curx, &ti->cury);
        ti->maxCol = printTextEx5(&ti->r, tb->title, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);
      }

      // ti->curx = ti->x + calcPrintTextLenght(tb->title, _font);
    }

    freeFont(_font);
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

  if (tb->wintype == TEXTBOX || tb->wintype == TEXTBOX_NUMBER || tb->wintype == TEXTBOX_PASSWORD)
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
  textInfo *ti;
  static unsigned int color = 0xFFFF;
  TESTNULLVOID(textbox);

  ti = (textInfo *)textbox->value;
  TESTNULLVOID(ti);

  if (ti->active == INACTIVE && color == 0xFFFF) //非激活状态
    return;

  if (blink % FREQ > FREQ / 2 + 3)
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

  if (textbox->title)
    liney_styleEx(ti->curx, ti->cury,
                  23, color, 2, 1);
  // liney_styleEx(getAbsoluteX(textbox) + 10 + strlen(textbox->title) * 12, getAbsoluteY(textbox) + 6,
  //               23, color, 2, 1);
  else
    liney_styleEx(getAbsoluteX(textbox) + 10, getAbsoluteY(textbox) + 6,
                  23, color, 2, 1);
}

void OnKey_Textbox(hbasewinAttr *textbox, void *key)
{
  int len;
  textInfo *ti;
  unsigned char ch;
  TESTNULLVOID(textbox);
  TESTNULLVOID(key);
  TESTNULLVOID(textbox->value);

  len = strlen(textbox->title);
  ti = (textInfo *)textbox->value;
  if (ti->curTextindex > len)
    ti->curTextindex = len;

  if (ti->curTextindex < 0)
    ti->curTextindex = 0;

  //隐藏光标
  DrawTextCursor(textbox, FREQ - 1);

  switch (*(int *)key)
  {
  case 0x5300:
  { //del删除
    // if (0 < ti->curTextindex && ti->curTextindex < len - 1)
    // {
    //   //hfont *_font = getFont(DEFAULT_FONTNAME, DEFAULT_FONTSIZE, DEFAULT_FONTCOLOR);
    //   ch = textbox->title[ti->curTextindex + 1];
    //   DelPosChar(textbox->title, ti->curTextindex);
    //   if (ch > 0xa0) //汉字
    //   {
    //     DelPosChar(textbox->title, ti->curTextindex);
    //     ti->curTextindex--;
    //   }
    //   ti->curTextindex--;

    //   //printTextEx5(&ti->r, textbox->title, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);
    //   OnPaint_TextBox(textbox, NULL);
    //   //freeFont(_font);
    // }

    if (0 <= ti->curTextindex && ti->curTextindex < len)
    {
      hfont *_font = getFont(DEFAULT_FONTNAME, DEFAULT_FONTSIZE, DEFAULT_FONTCOLOR);
      ch = textbox->title[ti->curTextindex];
      if (ch > 0xa0) //汉字
      {
        DelPosChar(textbox->title, ti->curTextindex + 1);
        //ti->curTextindex++;
      }
      DelPosChar(textbox->title, ti->curTextindex + 1);
      //ti->curTextindex++;
      printTextEx5(&ti->r, textbox->title, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);
      OnPaint_TextBox(textbox, NULL);
      freeFont(_font);
    }
  }
  break;
  case 0x0e08:
    /* <---删除*/
    if (0 < ti->curTextindex && ti->curTextindex <= len)
    {
      //hfont *_font = getFont(DEFAULT_FONTNAME, DEFAULT_FONTSIZE, DEFAULT_FONTCOLOR);
      ch = textbox->title[ti->curTextindex - 1];
      if (ch > 0xa0) //汉字
      {
        DelPosChar(textbox->title, ti->curTextindex);
        ti->curTextindex--;
      }
      DelPosChar(textbox->title, ti->curTextindex);
      ti->curTextindex--;
      //printTextEx5(&ti->r, textbox->title, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);
      OnPaint_TextBox(textbox, NULL);
      //freeFont(_font);
    }
    break;
  case 0x4800:
    /* 上 */
    //if (ti->curTextindex - ti->maxCol >= 0)
    {
      hfont *_font = getFont(DEFAULT_FONTNAME, DEFAULT_FONTSIZE, DEFAULT_FONTCOLOR);
      ti->curTextindex = ti->curTextindex - ti->maxCol - 1;
      if (ti->curTextindex <= 0)
        ti->curTextindex = 0;

      ch = textbox->title[ti->curTextindex];
      // if(ti->curTextindex > 0)
      //   if(ch > 0xa0 && textbox->title[ti->curTextindex-1] <= 0xa0)
      // // if (ch > 0xa0)
      //   ti->curTextindex++;
      ti->curTextindex++;
      printTextEx5(&ti->r, textbox->title, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);
      freeFont(_font);
    }
    break;
  case 0x5000:
    /* 下 */
    //  if (ti->curTextindex + ti->maxCol <= len)
    {
      hfont *_font = getFont(DEFAULT_FONTNAME, DEFAULT_FONTSIZE, DEFAULT_FONTCOLOR);
      ti->curTextindex = ti->curTextindex + ti->maxCol - 1;
      if (ti->curTextindex >= len)
        ti->curTextindex = len - 1;
      ch = textbox->title[ti->curTextindex];
      // if (ch > 0xa0)
      //   ti->curTextindex++;
      ti->curTextindex++;

      printTextEx5(&ti->r, textbox->title, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);
      freeFont(_font);
    }
    break;

  case 0x4b00:
    /*左*/
    if (0 < ti->curTextindex && ti->curTextindex <= len)
    {
      hfont *_font = getFont(DEFAULT_FONTNAME, DEFAULT_FONTSIZE, DEFAULT_FONTCOLOR);
      ch = textbox->title[ti->curTextindex - 1];
      if (ch > 0xa0) //汉字
        ti->curTextindex--;
      ti->curTextindex--;
      printTextEx5(&ti->r, textbox->title, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);

      freeFont(_font);
    }
    break;
  case 0x4d00:
    /* 右 */
    if (0 <= ti->curTextindex && ti->curTextindex <= len)
    {
      hfont *_font = getFont(DEFAULT_FONTNAME, DEFAULT_FONTSIZE, DEFAULT_FONTCOLOR);
      if (ti->curTextindex < len)
      {
        ch = textbox->title[ti->curTextindex];
        if (ch > 0xa0) //汉字
          ti->curTextindex++;
        ti->curTextindex++;
      }
      printTextEx5(&ti->r, textbox->title, _font, &ti->curTextindex, &ti->curx, &ti->cury, FALSE);

      freeFont(_font);
    }
    break;
  default:
    break;
  }
}

void OnKeyPress_Textbox(hbasewinAttr *textbox, void *str)
{
  textInfo *ti = NULL;
  int titlelen = 0;
  int newLen = 0;
  int len = 0;
  int i;
  TESTNULLVOID(textbox);
  TESTNULLVOID(str);
  TESTNULLVOID(textbox->value);

  //隐藏光标
  DrawTextCursor(textbox, FREQ - 1);

  ti = (textInfo *)textbox->value;
  if (ti->active == INACTIVE)
    return;

  titlelen = strlen(textbox->title);
  len = strlen(str);
  //newLen = strlen(textbox->title) + len + 1;
  newLen = titlelen + len;
  if (newLen + 32 > 1024) //最大允许值1024个字符
  {
    TRACE(("%s(%c):textbox(%d)字符长度(%d)超过最大值1024!\n",
           __FILE__, __LINE__, textbox->winID, newLen));
    return;
  }

  if (newLen > ti->textMaxlen)
  { //扩展字符串大小
    textbox->title = realloc(textbox->title, newLen + 33);
    ti->textMaxlen = newLen + 32;
  }

  //插入字符串
  for (i = 1; i <= (titlelen - ti->curTextindex); i++)
  {
    textbox->title[newLen - i] = textbox->title[titlelen - i];
  }
  for (i = 0; i < len; i++)
  {
    textbox->title[ti->curTextindex + i] = ((char *)str)[i];
  }
  textbox->title[newLen] = 0;
  //strcpy(textbox->title + strlen(textbox->title), str);

  ti->curTextindex += len;
  textbox->onPaint(textbox, NULL);
  TRACE(("%s(%c):插入字符%s\n", __FILE__, __LINE__, str));
}
