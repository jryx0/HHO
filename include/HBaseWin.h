#ifndef __HBASEWIN_H__
#define __HBASEWIN_H__
//#include "SVGAUTIL.H"
#include "list.h"
#include "mouse.h"
//#include <memory.h>

typedef enum winStatus
{
  HHOSHOW = 0,
  HHOHIDE = 10,
  HHODELETE = 20
};

typedef enum winType
{
  WIN,
  BUTTON,
  TEXTBOX,
  TEXTBOX_PASSWORD,
  TEXTBOX_NUMBER,
  HYPERLINK,
  LIST,
  CHECKBOX,
  RADIOBOX,
  PANEL,
  DESKTOP,
  DESKTOP_INPUT,
};

typedef void (*hhoevent)(struct winstruct *win, void *value);
typedef void (*hhoeventhandler)(struct winstruct *win, int type, void *value);

typedef struct _style
{
  char type;
  char textalign;

  char fontsize;
  char fonttype;
  int fontcolor;

  int bkcolor;
  int bkcolor1;
  int bkcolor2;

  int height;
} WinStyle;

typedef struct winstruct
{
  //窗口类型
  enum winType wintype;
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
  //int status; // winStatus

  //显示标题
  char *title;

  //存储的窗口参数:颜色/字体/可扩充保存数据
  void far *value;

  //响应鼠标事件函数指针单击,左键按下，左键按起，鼠标离开，鼠标进入
  hhoevent onClick, onLeave; //, onLeftUp, onLeftDown, onEnter;
  //响应键盘事件函数指针单击键盘
  hhoevent onKeyPress, onKey;
  //响应绘制事件，绘制自身窗口
  hhoevent onPaint;
  //响应激活事件，窗口激活
  hhoevent onActivate;

  //窗口颜色样式改变
  hhoevent onTheme;

  //窗口删除
  hhoevent onDestroy;

  //处理事件
  hhoeventhandler EventHandler;

  //子窗口列表
  list_t *children;
  //父窗口
  struct winstruct *parent;
  //struct winstruct *desktop;
} hbasewinAttr;

/**
 * @brief 初始化窗口基本信息。
 * @author 
 * @param parent 父窗口指针可为空
 * @param x      相对父窗口 左上
 * @param y      相对父窗口 右上
 * @param nWidth  窗口宽度
 * @param nHeight 窗口高度
 * @param winID  窗口ID，全局唯一性
 * @param title  窗口标题
 * @return NULL 失败
 */
hbasewinAttr *CreateWindowsEx(hbasewinAttr *parent, int x, int y, int nWidth,
                              int nHeight, int winID, const char *title);
/**
 * @brief hhoevent函数删除窗口，并释放资源
 * @param win 待删除窗口 * 
 */
void OnDestory(hbasewinAttr *win, void *val);

/**
 * @brief hhoevent绘制窗口
 * @param win 要绘制的窗口
 * @param val 传入的参数
 */
void OnPaint(hbasewinAttr *win, void *val);

/**
 * @brief 遍历所有子窗口，调用OnDestory窗口函数指针，删除子窗口 * 
 * @param win 要删除的窗口  
 * @return 无 
 */
void destoryChildren(hbasewinAttr *win);
/**
 * 清除窗口所在的区域
 * 
 */
void clearWinRegion(hbasewinAttr *win, int color);
/**
 * 释放窗口
 * @param win 要删除的窗口  
 */
void freeWin(hbasewinAttr *win);

/**
 * 添加子窗口
 * @param parent 父窗口
 * @param child 子窗口
 * 
 * 如父窗口为空则不添加 
 */
hbasewinAttr *addChild(hbasewinAttr *parent, hbasewinAttr *child);

/**
 * 获取窗口绝对坐标 x
 * @param win 窗口
 * 通过累加父窗口x坐标，获取绝对坐标
 */
int getAbsoluteX(hbasewinAttr *win);

/**
 * 获取窗口绝对坐标 x
 * @param win 窗口
 * 通过累加父窗口x坐标，获取绝对坐标
 */
int getAbsoluteY(hbasewinAttr *win);

/**
 * @brief 遍历所有子窗口，调用onPaint窗口函数指针绘制窗口 
 * @param win 要删除的窗口  
 * @return 无 
 */
void repaintChildren(hbasewinAttr *win, void *val);

/**
 * 检查坐标 坐标x，y是否在窗口中
 * 
 * @param win 要检查的窗口
 * @param x 绝对坐标点x
 * @param y 绝对坐标点y
 */
int checkpointInside(hbasewinAttr *win, int x, int y);

/**
 * 检查鼠标点击在窗口中
 * 
 * @param win 窗口
 * @param mouse 鼠标状态
 * 
 * @return 返回被点击的子窗口，若无子窗寇被点击，返回窗口
 */
hbasewinAttr *checkmousewin(hbasewinAttr *win, mousestatus *mouse);
/**
 * 在win中查找winID的窗口
 * 
 */
hbasewinAttr *findWinByID(hbasewinAttr *win, int winID);

/**
 * 遍历win的所有子窗口,theme
 * @param win 窗口
 * @param val 参数
 * @param event 事件
 * @return 无
 */
void themeChildren(hbasewinAttr *win, void *val);
void OnTheme(hbasewinAttr *win, void *val);

#endif