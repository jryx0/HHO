#ifndef __HBASEWIN_H__
#define __HBASEWIN_H__
//#include "SVGAUTIL.H"
#include "list.h"
//#include <memory.h>

typedef enum winStatus { HHOSHOW, HHOHIDE, HHODELETE };

typedef enum winType {
  WIN,
  BUTTON,
  TEXTBOX,
  LIST,
  CHECKBOX,
  RADIOBOX,
  PANEL,
  DESKTOP
};

typedef void (*hhoevent)(struct winstruct *win, void *value);
typedef void (*hhoeventhandler)(struct winstruct *win, int type, void *value);

typedef struct {
  int standardShade[4];
  int capColour;
  int fontColour;
  int desktopColour;
  int capFontColour;

  //屏幕最大显示颜色 256 32k 64k颜色
  int maxcolors;
} win_colors;

typedef struct win_attr {
  //窗口类型：按钮、文本输入框、多选框
  int hhowintype;
  //窗口唯一ID
  int winID;

  //相对父窗口左上角坐标x,y
  int x;
  int y;
  //宽度
  int nWidth;
  //高度
  int nHeight;

  //窗口状态
  int status; // winStatus

  //窗口颜色
  // win_colors *colors;

  //显示标题
  char *title;

  //存储的数据，如文本输入框的字符串
  void *value;

  //背景
  int *bkarea;

  //响应鼠标事件函数指针单击,左键按下，左键按起，鼠标离开，鼠标进入
  hhoevent onClick, onLeftUp, onLeftDown, onLeave, onEnter;
  //响应键盘事件函数指针单击键盘
  hhoevent onKeyPress;
  //响应绘制事件，绘制自身窗口
  hhoevent onPaint;
  //响应激活事件，窗口激活
  hhoevent onActivate;

  //处理事件
  hhoeventhandler EventHandler;

  //子窗口列表
  list_t *children;
  //父窗口
  struct win_attr *parent;
  struct win_attr *desktop;
} hbasewinAttr;

hbasewinAttr *CreateWindowsEx(hbasewinAttr *parent, int x, int y, int nWidth,
                              int nHeight, int winID, const char *title);
void DestoryWindows(hbasewinAttr *win);

hbasewinAttr *addChild(hbasewinAttr *parent, hbasewinAttr *child);
hbasewinAttr *removeChild(hbasewinAttr *parent, hbasewinAttr *child);
hbasewinAttr *removeChildByID(hbasewinAttr *parent, int childid);

win_colors *getWinStandrandColors();
void OnPaint(hbasewinAttr *win, void *val);
void repaintChildren(hbasewinAttr *win);
int getAbsoluteX(hbasewinAttr *win);
int getAbsoluteY(hbasewinAttr *win);

int checkmouseInside(hbasewinAttr *win, int x, int y);
#endif