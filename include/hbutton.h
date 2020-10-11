#ifndef __HBUTTON_H__
#define __HBUTTON_H__

#include "hhosvga.h"
#include "HBaseWin.h"

enum button_type
{
  b_standard = 0,  //标准按钮
  b_list = 3,      //下拉按钮
  b_img = 4,       //图片按钮
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
 * 按钮创建
 * 在value 字段存储buttonStyle，根据设置绘制按钮
 * 
 */
hbasewinAttr *CreateButton(hbasewinAttr *parent, int x, int y, int nWidth,
                           int nHeight, int winID, const char *title);
/**
 * 按钮绘制 
 */
void OnPaint_button(hbasewinAttr *btn, void *value);

/**
 * 标准按钮绘制 
 */
void OnPaint_button_S(hbasewinAttr *btn, void *value);

/**
 * 超链接按钮绘制 
 */
void OnPaint_button_H(hbasewinAttr *btn, void *value);

/**
 * 文本按钮绘制 
 */
void OnPaint_button_T(hbasewinAttr *btn, void *value);

/**
 * 
 */
void OnPaint_button_L(hbasewinAttr *btn, void *value);
void OnPaint_button_I(hbasewinAttr *btn, void *value);
#endif