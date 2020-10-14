//#include "macrodef.h"
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
  textInfo *ti;
  hbasewinAttr *tb = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  TESTNULL(tb, NULL);
  tb->onPaint = OnPaint_TextBox;
  tb->onClick = OnClick_TextBox;
  tb->onLeave = OnLeave_TextBox;
  tb->onDestroy = OnDestory_TextBox;
  tb->onActivate = OnActive_TextBox;
  tb->wintype = TEXTBOX;

  ti = malloc(sizeof(textInfo));
  ti->x = getAbsoluteX(tb) + 6;
  ti->y = getAbsoluteY(tb) + 6;
  ti->width = nWidth - 10;
  ti->height = nHeight;

  ti->r.left_top.x = getAbsoluteX(tb) + 6;
  ti->r.left_top.y = getAbsoluteY(tb) + 6;
  ti->r.right_bottom.x = ti->r.left_top.x + ti->width;
  ti->r.right_bottom.y = ti->r.left_top.y + ti->height;

  ti->active = INACTIVE;

  tb->value = ti; // malloc(2); //int ���ƻ״̬ʱ�Ļ���
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
 * ���ʱ,���õ�ǰtextboxΪ��ؼ�
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
 * ���ʱ,���õ�ǰtextboxΪ��ؼ�
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
  DrawTextCursor(tb, 1);
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
    hfont *_font = getFont(SIMSUN, 24, 0x0000);
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
    if (tb->title)
    {
      //printTextEx(&(ti->r), tb->title, _font);
      printTextEx4(&ti->r, tb->title, _font, &ti->curx, &ti->cury);
      // ti->curx = ti->x + calcPrintTextLenght(tb->title, _font);
    }

    freeFont(_font);
  }
  (void)value;
}

/**
 * ����ڣ���tb=NULL��ֻ����ԭtextboxΪδ����
 * 
 * @param tb ��Ҫ����Ĵ���
 * @param value globaldef,��ҪfoucsedTextBox
 * 
 */
void OnActive_TextBox(hbasewinAttr *tb, void *value)
{

  globaldef *_g = (globaldef *)value;
  hbasewinAttr *prevtb = NULL;

  if (_g)
    prevtb = _g->foucsedTextBox;

  //prevtbΪ�գ�˵���޼����
  if (prevtb && prevtb->onLeave) //��һ��textbox �뿪
  {
    prevtb->onLeave(prevtb, NULL);
    prevtb->onPaint(prevtb, NULL);
  }

  if (tb && tb->onClick) //����textbox
  {
    tb->onClick(tb, NULL);
    tb->onPaint(tb, NULL);
  }

  _g->foucsedTextBox = tb;
  //TRACE(("tb=%u,_g->foucsedTextBox=%u\n", tb, _g->foucsedTextBox));
}

/**
 * �����ı�����
 * @param textbox Ҫ���ƵĴ���
 * @param blink ��˸��־
 */
void DrawTextCursor(hbasewinAttr *textbox, unsigned int blink)
{
  textInfo *ti;
  static unsigned int color = 0xFFFF;
  TESTNULLVOID(textbox);

  ti = (textInfo *)textbox->value;
  TESTNULLVOID(ti);

  if (blink % FREQ < FREQ / 2 + 3)
  {
    if (color == 0xFFFF) //������˸Ƶ��
      return;
    color = 0xFFFF; //��ʾ
  }
  else
  {
    if (color == 0x0000) //������˸Ƶ��
      return;
    color = 0x0000; //����
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