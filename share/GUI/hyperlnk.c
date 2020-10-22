#include "HBaseWin.h"
#include "hhosvga.h"
#include "hyperlnk.h"
#include "hglobal.h"
#define UNDERLINE 1
#define NONE 0

/**
 * 创建超链接控件
 * 
 */
hbasewinAttr *Createhyperlink(hbasewinAttr *parent, int x, int y, int nWidth,
                              int nHeight, int winID, const char *title)
{
  hbasewinAttr *hyperlink = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);

  hyperlink->onPaint = OnPaint_hyperlink;
  hyperlink->onClick = OnClick_hyperlink;
  hyperlink->onLeave = OnLeave_hyperlink;

  hyperlink->style = getlinkStyle();
  getWinTheme((WinStyle *)hyperlink->style, 1);
  hyperlink->wintype = HYPERLINK;
  return hyperlink;
}

/**
 * 设置超链接点击时的屏幕显示
 * 
 */
void OnClick_hyperlink(hbasewinAttr *link, void *val)
{
  WinStyle *lnkStyle = NULL;
  TESTNULLVOID(link);
  TESTNULLVOID(link->style);

  lnkStyle = (WinStyle *)link->style;
  lnkStyle->type = UNDERLINE;

  OnPaint_hyperlink(link, val);
}

/**
 * 设置超链接点击完时的屏幕显示
 * 
 */
void OnLeave_hyperlink(hbasewinAttr *btn, void *val)
{
  WinStyle *lnkStyle = NULL;
  TESTNULLVOID(btn);
  TESTNULLVOID(btn->style);
  lnkStyle = (WinStyle *)btn->style;
  lnkStyle->type = NONE;

  OnPaint_hyperlink(btn, val);
}
/**
 * 设置超链接屏幕输出显示
 * 
 */
void OnPaint_hyperlink(hbasewinAttr *link, void *val)
{
  int x = 0, y = 0;
  WinStyle *lnkStyle = NULL;
  hfont *_font;

  TESTNULLVOID(link);
  TESTNULLVOID(link->style);

  lnkStyle = (WinStyle *)link->style;

  if (link->title != NULL)
  {
    hregion region;
    int wid;
    _font = getFont(lnkStyle->fonttype, lnkStyle->fontsize, 0x0000);

    x = getAbsoluteX(link);
    y = getAbsoluteY(link);
    fillRegionEx(x, y, link->nWidth, link->nHeight, 0xffff);

    region.left_top.x = x;
    region.left_top.y = y;

    //printTextLineXY(x, y, link->title, _font);
    wid = calcPrintTextLenght(link->title, _font);
    if (wid < link->nWidth)
      link->nWidth = wid + 1;
    if (lnkStyle->fontsize < link->nHeight)
      link->nHeight = lnkStyle->fontsize + 3;

    region.right_bottom.x = x + link->nWidth;
    region.right_bottom.y = y + link->nHeight;

    if (lnkStyle->type == UNDERLINE)
    {
      linex_styleEx(x, y + lnkStyle->fontsize + 2, link->nWidth, lnkStyle->bkcolor, 2, 1);
      //rectangle(x, y, region.right_bottom.x, region.right_bottom.y, lnkStyle->bkcolor1, 1, 2);
      rectangleEx(x, y, link->nWidth, link->nHeight, lnkStyle->bkcolor1, 1, 2);
    }
    else
    {
      linex_styleEx(x, y + lnkStyle->fontsize + 2, link->nWidth, 0xFFFF, 2, 1);
      rectangleEx(x, y, link->nWidth, link->nHeight, 0xFFFF, 1, 2);
    }

    printTextEx(&region, link->title, _font);
    freeFont(_font);
  }
  (void)val;
}

/**
 * 构造缺省按钮参数
 * 默认标准按钮
 * 删除button时由 freeWin释放内存
 */
WinStyle *getlinkStyle(void)
{
  WinStyle *lnkstyle = malloc(sizeof(WinStyle));
  TESTNULL(lnkstyle, NULL);

  lnkstyle->bkcolor = 0xFFFF;
  lnkstyle->fontcolor = 0x0000;
  lnkstyle->fontsize = 24;
  lnkstyle->fonttype = SIMSUN;
  lnkstyle->type = NONE;

  return lnkstyle;
}