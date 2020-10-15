#include "HBaseWin.h"
#include "hhosvga.h"
#include "hyperlnk.h"

#define UNDERLINE 1
#define NONE 0

/**
 * ���������ӿؼ�
 * 
 */
hbasewinAttr *Createhyperlink(hbasewinAttr *parent, int x, int y, int nWidth,
                              int nHeight, int winID, const char *title)
{
  hbasewinAttr *hyperlink = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);

  hyperlink->onPaint = OnPaint_hyperlink;
  hyperlink->onClick = OnClick_hyperlink;
  hyperlink->onLeave = OnLeave_hyperlink;

  hyperlink->value = getlinkStyle();
  hyperlink->wintype = HYPERLINK;
  return hyperlink;
}

/**
 * ���ó����ӵ��ʱ����Ļ��ʾ
 * 
 */
void OnClick_hyperlink(hbasewinAttr *link, void *value)
{
  WinStyle *lnkStyle = NULL;
  TESTNULLVOID(link);
  TESTNULLVOID(link->value);

  lnkStyle = (WinStyle *)link->value;
  lnkStyle->type = UNDERLINE;

  OnPaint_hyperlink(link, value);
}

/**
 * ���ó����ӵ����ʱ����Ļ��ʾ
 * 
 */
void OnLeave_hyperlink(hbasewinAttr *btn, void *value)
{
  WinStyle *lnkStyle = NULL;
  TESTNULLVOID(btn);
  TESTNULLVOID(btn->value);
  lnkStyle = (WinStyle *)btn->value;
  lnkStyle->type = NONE;

  OnPaint_hyperlink(btn, value);
}
/**
 * ���ó�������Ļ�����ʾ
 * 
 */
void OnPaint_hyperlink(hbasewinAttr *link, void *value)
{
  int x = 0, y = 0;
  WinStyle *lnkStyle = NULL;
  hfont *_font;

  TESTNULLVOID(link);
  TESTNULLVOID(link->value);

  lnkStyle = (WinStyle *)link->value;

  if (link->title != NULL)
  {
    hregion region;
    _font = getFont(lnkStyle->fonttype, lnkStyle->fontsize, lnkStyle->fontcolor);
    y = getAbsoluteY(link);
    x = getAbsoluteX(link);

    region.left_top.x = x;
    region.left_top.y = y;
    region.right_bottom.x = x + link->nWidth;
    region.right_bottom.y = y + link->nHeight;
    //
    printTextEx(&region, link->title, _font);
    freeFont(_font);

    if (lnkStyle->type == UNDERLINE)
    {
      linex_styleEx(x, y + lnkStyle->fontsize + 2, link->nWidth, lnkStyle->fontcolor, 2, 1);
      rectangle(x, y, region.right_bottom.x, region.right_bottom.y, lnkStyle->fontcolor, 1, 2);
    }
    else
    {
      linex_styleEx(x, y + lnkStyle->fontsize + 2, link->nWidth, lnkStyle->bkcolor, 2, 1);
      rectangle(x, y, region.right_bottom.x, region.right_bottom.y, lnkStyle->bkcolor, 1, 2);
    }
  }
}

/**
 * ����ȱʡ��ť����
 * Ĭ�ϱ�׼��ť
 * ɾ��buttonʱ�� freeWin�ͷ��ڴ�
 */
WinStyle *getlinkStyle(void)
{
  WinStyle *lnkStyle = malloc(sizeof(WinStyle));
  TESTNULL(lnkStyle, NULL);

  lnkStyle->bkcolor = 0xFFFF;
  lnkStyle->fontcolor = 0x0000;
  lnkStyle->fontsize = 24;
  lnkStyle->fonttype = SIMSUN;
  lnkStyle->type = NONE;

  return lnkStyle;
}