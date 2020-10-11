#ifndef __HBUTTON_H__
#define __HBUTTON_H__

#include "hhosvga.h"
#include "HBaseWin.h"

enum button_type
{
  b_standard = 0,  //��׼��ť
  b_list = 3,      //������ť
  b_img = 4,       //ͼƬ��ť
};

typedef struct _buttonstyle
{
  enum button_type;
  char *text;
  char *imgfile;
  char textalign;
  hfont *_font;
  int bkcolor1;
  int bkcolor2;
} buttonStyle;

/**
 * ��ť����
 * ��value �ֶδ洢buttonStyle���������û��ư�ť
 * 
 */
hbasewinAttr *CreateButton(hbasewinAttr *parent, int x, int y, int nWidth,
                           int nHeight, int winID, const char *title);
/**
 * ��ť���� 
 */
void OnPaint_button(hbasewinAttr *btn, void *value);

/**
 * ��׼��ť���� 
 */
void OnPaint_button_S(hbasewinAttr *btn, void *value);

/**
 * �����Ӱ�ť���� 
 */
void OnPaint_button_H(hbasewinAttr *btn, void *value);

/**
 * �ı���ť���� 
 */
void OnPaint_button_T(hbasewinAttr *btn, void *value);

/**
 * 
 */
void OnPaint_button_L(hbasewinAttr *btn, void *value);
void OnPaint_button_I(hbasewinAttr *btn, void *value);
#endif