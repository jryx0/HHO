#ifndef __HTEXTBOX_H__
#define __HTEXTBOX_H__

#include "hhosvga.h"
#include "HBaseWin.h"

typedef struct _textinfo
{
  hregion r;

  //��ǰ�����ʼ����
  int curx;
  int cury;

  //��ǰ�����������λ��
  int curTextindex;  

  //�Ƿ񼤻�״̬
  int active;

  //��ǰ������󻺴�ռ�(����չ)
  int textMaxlen;
} textInfo;

hbasewinAttr *CreateTextBox(hbasewinAttr *parent, int x, int y, int nWidth,
                            int nHeight, int winID, const char *title);

void OnPaint_TextBox(hbasewinAttr *link, void *value);
/**
 * ���ʱ,���õ�ǰtextboxΪ��ؼ�
 * 
 */
void OnClick_TextBox(hbasewinAttr *tb, void *value);

/**
 * ���ʱ,���õ�ǰtextboxΪ��ؼ�
 * 
 */
void OnLeave_TextBox(hbasewinAttr *tb, void *value);

/**
 * ���ʱ,���õ�ǰtextboxΪ��ؼ�
 * 
 */
void OnDestory_TextBox(hbasewinAttr *tb, void *value);
/**
 * textbox ����ʱ���� 
 * 
 */
void OnActive_TextBox(hbasewinAttr *tb, void *value);

/**
 * �����ı�����
 * @param textbox Ҫ���Ƶ�textbox
 * @param blink ��˸��־
 */
void DrawTextCursor(hbasewinAttr *textbox, unsigned int blink);

void OnKeyPress_Textbox(hbasewinAttr *textbox, void *str);
void OnKey_Textbox(hbasewinAttr *textbox, void *key);
#endif