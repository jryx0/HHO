//#include "macrodef.h"
#include "hhosvga.h"
#include "HBaseWin.h"
#include "htextbox.h"
#include "hglobal.h"
#include <memory.h>
#include <string.h>

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
  tb->onKeyPress = OnKeyPress_Textbox;
  tb->onKey = OnKey_Textbox;
  tb->wintype = TEXTBOX;

  ti = malloc(sizeof(textInfo));

  ti->r.left_top.x = getAbsoluteX(tb) + 6;
  ti->r.left_top.y = getAbsoluteY(tb) + 6;
  ti->r.right_bottom.x = ti->r.left_top.x + tb->nWidth - 6;
  ti->r.right_bottom.y = ti->r.left_top.y + tb->nHeight;
  ti->active = INACTIVE;
  if (title)
  {
    ti->textMaxlen = strlen(title);
    ti->curTextindex = ti->textMaxlen;
  }
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
      if (tb->wintype == TEXTBOX_PASSWORD)
      { //��ӡ****
        char *password = malloc(strlen(tb->title) + 1);
        memset(password, 0, strlen(tb->title) + 1);
        memset(password, '*', strlen(tb->title));

        printTextEx4(&ti->r, password, _font, &ti->curx, &ti->cury);

        free(password);
      }
      else
      { //������ӡ
        printTextEx4(&ti->r, tb->title, _font, &ti->curx, &ti->cury);
      }

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

  if (ti->active == INACTIVE && color == 0xFFFF) //�Ǽ���״̬
    return;

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
  {
    ti->curTextindex = len;
  }

  switch (*(int *)key)
  {
  case 0x0e08:
    /*ɾ��*/
    if (ti->curTextindex > 0)
    {
      ch = textbox->title[ti->curTextindex - 1];
      if (ch > 0xa0) //����
        ti->curTextindex--;

      ti->curTextindex--;
    }

    break;
  case 0x4800:
    /* �� */

    break;
  case 0x5000:
    /* �� */
    break;
    
  case 0x4b00:
    /* �� */

    ch = textbox->title[ti->curTextindex - 1];
    if (ch > 0xa0) //����
      ti->curTextindex--;

    ti->curTextindex--;
    break;
  case 0x4d00:
    /* �� */
    break;
  default:
    break;
  }
}

void OnKeyPress_Textbox(hbasewinAttr *textbox, void *str)
{
  textInfo *ti = NULL;
  int newLen = 0;
  int len = 0;
  TESTNULLVOID(textbox);
  TESTNULLVOID(str);
  TESTNULLVOID(textbox->value);

  //���ع��
  DrawTextCursor(textbox, 1);

  ti = (textInfo *)textbox->value;
  if (ti->active == INACTIVE)
    return;
  len = strlen(str);
  newLen = strlen(textbox->title) + len + 1;
  if (newLen + 32 > 1024) //�������ֵ1024���ַ�
  {
    TRACE(("%s(%c):textbox(%d)�ַ�����(%d)�������ֵ1024!\n",
           __FILE__, __LINE__, textbox->winID, newLen));
    return;
  }

  if (newLen > ti->textMaxlen)
  { //��չ�ַ�����С
    textbox->title = realloc(textbox->title, newLen + 32);
    ti->textMaxlen = newLen + 32;
  }

  strcpy(textbox->title + strlen(textbox->title), str);

  ti->curTextindex += len;
  textbox->onPaint(textbox, NULL);
}