#ifndef __HBUTTON_H__
#define __HBUTTON_H__

#include "hhosvga.h"
#include "HBaseWin.h"

//////////////////////////////按钮状态宏定义//////////////////////////////////////////////
#define BUTTON_UP 0 //正常状态
#define BUTTON_DOWN 1
#define BUTTON_HOVE 2 //在button上面

//文字对齐
#define TEXT_CENTER 0

//IMG button 使用
#define TEXT_LEFT 1
#define TEXT_DOWN 2
#define TEXT_RIGHT 3

//////////////////////////////按钮样式宏定义//////////////////////////////////////////////////

#define STANDARD 1   //标准按钮
#define LISTBUTTON 2 //下拉按钮
#define IMGBUTTON 3  //图片按钮



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

void OnClick_button(hbasewinAttr *btn, void *value);
void OnLeave_button(hbasewinAttr *btn, void *value);
/**
 * 构造缺省按钮参数
 * 默认标准按钮
 */
WinStyle *getbtnStyle(void);
#endif