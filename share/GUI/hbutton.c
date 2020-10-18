#include "HBaseWin.h"
#include "hbutton.h"
#include "hglobal.h"

hbasewinAttr *CreateButton(hbasewinAttr *parent, int x, int y, int nWidth,
                           int nHeight, int winID, const char *title)
{
  WinStyle *btnStyle = NULL;
  hbasewinAttr *button = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);

  button->onPaint = OnPaint_button;
  button->onClick = OnClick_button;
  button->onLeave = OnLeave_button;

  btnStyle = getbtnStyle();
  getWinTheme(btnStyle, 1);
  button->value = btnStyle;

  return button;
}

void OnClick_button(hbasewinAttr *btn, void *value)
{
  WinStyle *btnStyle = NULL;
  TESTNULLVOID(btn);
  TESTNULLVOID(btn->value);
  btnStyle = (WinStyle *)btn->value;
  btnStyle->bkcolor = btnStyle->bkcolor1; //颜色变深

  OnPaint_button(btn, value);
}

void OnLeave_button(hbasewinAttr *btn, void *value)
{
  WinStyle *btnStyle = NULL;
  TESTNULLVOID(btn);
  TESTNULLVOID(btn->value);
  btnStyle = (WinStyle *)btn->value;
  btnStyle->bkcolor = btnStyle->bkcolor2; //颜色变深

  OnPaint_button(btn, value);
}

void OnPaint_button(hbasewinAttr *btn, void *value)
{
  int x = 0, y = 0;
  WinStyle *btnStyle = NULL;
  hfont *_font;

  TESTNULLVOID(btn);
  TESTNULLVOID(btn->value);

  btnStyle = (WinStyle *)btn->value;
  y = getAbsoluteY(btn);
  x = getAbsoluteX(btn);
  fillRegionEx(x, y, btn->nWidth, btn->nHeight, btnStyle->bkcolor);
  //rectangleEx(btn->x, btn->y, btn->nWidth, btn->nHeight, 0x05FF, 1, 1);

  if (btn->title != NULL)
  {
    _font = getFont(btnStyle->fonttype, btnStyle->fontsize, 0xFFFF);

    if (btnStyle->textalign == TEXT_CENTER)
    {
      int len = calcPrintTextLenght(btn->title, _font);
      y += (btn->nHeight - _font->currentFontSize) / 2;
      x += (btn->nWidth - len) / 2;
    }
    printTextLineXY(x, y, btn->title, _font);
    freeFont(_font);
  }
  (void)value;
}

/**
 * 构造缺省按钮参数
 * 默认标准按钮
 * 删除button时由 freeWin释放内存
 */
WinStyle *getbtnStyle(void)
{
  WinStyle *btnStyle = malloc(sizeof(WinStyle));
  TESTNULL(btnStyle, NULL);

  btnStyle->bkcolor = 0x03DF;
  btnStyle->bkcolor1 = 0x003F;
  btnStyle->bkcolor2 = 0x03DF;

  btnStyle->type = STANDARD;
  btnStyle->textalign = TEXT_CENTER;

  btnStyle->fontcolor = 0xFFFF;
  btnStyle->fontsize = 24;
  btnStyle->fonttype = SIMSUN;

  return btnStyle;
}
