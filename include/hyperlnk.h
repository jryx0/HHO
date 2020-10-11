#ifndef __HYPERLNK_H__
#define __HYPERLNK_H__

#include "hhosvga.h"
#include "HBaseWin.h"

#define UNDERLINE 1
#define NONE 0

/**
 * 创建超链接控件
 * 
 */
hbasewinAttr *Createhyperlink(hbasewinAttr *parent, int x, int y, int nWidth,
                               int nHeight, int winID, const char *title);

/**
 * 设置超链接点击时的屏幕显示
 * 
 */
void OnClick_hyperlink(hbasewinAttr *link, void *value);

/**
 * 设置超链接点击完时的屏幕显示
 * 
 */
void OnLeave_hyperlink(hbasewinAttr *btn, void *value);
/**
 * 设置超链接屏幕输出显示
 * 
 */
void OnPaint_hyperlink(hbasewinAttr *link, void *value);

/**
 * 构造缺省按钮参数
 * 默认标准按钮
 * 删除button时由 freeWin释放内存
 */
WinStyle *getlinkStyle(void);

#endif