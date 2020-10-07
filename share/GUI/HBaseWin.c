#include "macrodef.h"
#include "mouse.h"
#include "hhosvga.h"
#include "wResource.h"
#include "HBaseWin.h"
#include "list.h"

#include <memory.h>
#include <string.h>

#pragma warn - pia

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
                              int nHeight, int winID, const char *title)
{
  hbasewinAttr *HHOwin = malloc(sizeof(hbasewinAttr));
  TESTNULL(HHOwin, NULL);

  memset(HHOwin, 0, sizeof(hbasewinAttr));

  HHOwin->parent = parent;
  HHOwin->hhowintype = WIN;
  HHOwin->winID = winID;
  HHOwin->x = x;
  HHOwin->y = y;
  HHOwin->nWidth = nWidth;
  HHOwin->nHeight = nHeight;
  if (title != NULL)
  {
    HHOwin->title = malloc(strlen(title) + 1);
    memset(HHOwin->title, 0, strlen(title) + 1);
    strcpy(HHOwin->title, title);
  }

  //添加子窗口
  addChild(parent, HHOwin);
  //设置窗口绘制函数指针
  HHOwin->onPaint = OnPaint;
  //设置窗口清除函数指针
  HHOwin->onDestroy = OnDestory;

  return HHOwin;
}

/**
 * @brief hhoevent函数删除窗口，并释放资源
 * @param win 待删除窗口 * 
 */
void OnDestory(hbasewinAttr *win, void *val)
{

  TESTNULLVOID(win);

  if (win->title)
    free(win->title);

  if (win->value)
    free(win->value);

  if (win->bkarea)
    free(win->bkarea);

  //free(win);
}

/**
 * @brief hhoevent绘制窗口
 * @param win 要绘制的窗口
 * @param val 传入的参数
 */
void OnPaint(hbasewinAttr *win, void *val)
{
  int x1, x2, y1, y2;
  int type;

  TESTNULLVOID(win);

  x1 = getAbsoluteX(win);
  y1 = getAbsoluteY(win);
  x2 = x1 + win->nWidth;
  y2 = y1 + win->nHeight;

  fillRegion(x1, y1, x2, y2, COLOR_ligtgray); //);
  rectangle(x1, y1, x2, y2, COLOR_darkgray, 1, 2);
}

/**
 * @brief 遍历所有子窗口，调用OnDestory窗口函数指针，删除子窗口 * 
 * @param win 要删除的窗口  
 * @return 无 
 */
void destoryChildren(hbasewinAttr *win)
{
  list_iterator_t *it;
  list_node_t *node;
  hbasewinAttr *childwin;

  TESTNULLVOID(win);
  TESTNULLVOID(win->children);

  it = list_iterator_new(win->children, LIST_HEAD);
  while ((node = list_iterator_next(it)))
  {
    if (node->val != NULL)
    {
      childwin = (hbasewinAttr *)(node->val);
      if (childwin->onPaint != NULL)
        childwin->onDestroy(childwin, NULL);
    }
    list_remove(win->children, node);
  }
  list_iterator_destroy(it);
}

/**
 * 添加子窗口
 * @param parent 父窗口
 * @param child 子窗口
 * 
 * 如父窗口为空则不添加 
 */

hbasewinAttr *addChild(hbasewinAttr *parent, hbasewinAttr *child)
{

  TESTNULLVOID(parent);
  TESTNULLVOID(child);

  if (parent->children == NULL)
  {
    parent->children = list_new();
    //parent->children->match = compareWin;
  }
  list_rpush(parent->children, list_node_new(child));
  return parent;
}

/**
 * 获取窗口绝对坐标 x
 * @param win 窗口
 * 通过累加父窗口x坐标，获取绝对坐标
 */
int getAbsoluteX(hbasewinAttr *win)
{
  if (win == NULL)
    return 0;
  // if (win->parent==NULL)
  //   return win->x;
  return win->x + getAbsoluteX(win->parent);
}
/**
 * 获取窗口绝对坐标 x
 * @param win 窗口
 * 通过累加父窗口x坐标，获取绝对坐标
 */
int getAbsoluteY(hbasewinAttr *win)
{
  if (win == NULL)
    return 0;
  // if (win->parent==NULL)
  //   return win->y;
  return win->y + getAbsoluteY(win->parent);
}

/**
 * @brief 遍历所有子窗口，调用onPaint窗口函数指针绘制窗口 
 * @param win 要删除的窗口  
 * @return 无 
 */
void repaintChildren(hbasewinAttr *win)
{
  list_iterator_t *it;
  list_node_t *node;
  hbasewinAttr *childwin;

  if (win == NULL || win->children == NULL)
    return;

  it = list_iterator_new(win->children, LIST_HEAD);
  while ((node = list_iterator_next(it)))
  {
    if (node->val == NULL)
      continue;

    childwin = (hbasewinAttr *)(node->val);
    if (childwin->onPaint != NULL)
      childwin->onPaint(childwin, NULL);
  }
  list_iterator_destroy(it);
}

int compareWin(hbasewinAttr *w1, hbasewinAttr *w2)
{
  if (w1 && w2)
    return w1->winID == w1->winID;

  return 0;
}

/**
 * 根据窗口ID查找子窗口
 *  win
 */
hbasewinAttr *FindChildWinbyID(hbasewinAttr *win, int id)
{
  list_iterator_t *it;
  list_node_t *node = NULL;
  hbasewinAttr *childwin = NULL;

  if (win == NULL || win->children == NULL)
    return;

  it = list_iterator_new(win->children, LIST_HEAD);
  while ((node = list_iterator_next(it)))
  {
    if (node->val == NULL)
      continue;

    childwin = (hbasewinAttr *)(node->val);
    if (childwin->winID == id)
      break;
  }
  list_iterator_destroy(it);

  return childwin;
}

/**
 * 根据窗口ID查找子窗口
 *  win
 */
list_node_t *FindChildNodebyID(hbasewinAttr *win, int id)
{
  list_iterator_t *it;
  list_node_t *node = NULL;
  hbasewinAttr *childwin = NULL;

  if (win == NULL || win->children == NULL)
    return;

  it = list_iterator_new(win->children, LIST_HEAD);
  while ((node = list_iterator_next(it)))
  {
    if (node->val == NULL)
      continue;

    childwin = (hbasewinAttr *)(node->val);
    if (childwin->winID == id)
      break;
  }
  list_iterator_destroy(it);

  return node;
}

/**
 * 检查坐标 坐标x，y是否在窗口中
 * 
 * @param win 要检查的窗口
 * @param x 绝对坐标点x
 * @param y 绝对坐标点y
 */
int checkpointInside(hbasewinAttr *win, int x, int y)
{
  int x1, y1, x2, y2;
  if (win == NULL)
    return 0;

  x1 = getAbsoluteX(win);
  y1 = getAbsoluteY(win);
  x2 = x1 + win->nWidth;
  y2 = y1 + win->nHeight;

  return ((x1 < x && x < x2) && (y1 < y && y < y2));
}

/**
 * 检查鼠标点击在窗口中
 * 
 * @param win 窗口
 * @param mouse 鼠标状态
 * 
 * @return 返回被点击的子窗口，若无子窗寇被点击，返回窗口
 */
hbasewinAttr *checkmousewin(hbasewinAttr *win, mousestatus *mouse)
{
  list_iterator_t *it;
  list_node_t *node;
  hbasewinAttr *temp;

  TESTNULLVOID(win);
  TESTNULLVOID(mouse);

  it = list_iterator_new(win->children, LIST_HEAD);
  while (node = list_iterator_next(it))
  {
    if (node->val == NULL)
      continue;

    temp = (hbasewinAttr *)(node->val);
    if (checkpointInside(temp, mouse->x, mouse->y))
    {
      list_iterator_destroy(it);
      return temp;
    }
  }
  list_iterator_destroy(it);
  return win;
}