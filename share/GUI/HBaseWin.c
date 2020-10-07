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
 * @brief ��ʼ�����ڻ�����Ϣ��
 * @author 
 * @param parent ������ָ���Ϊ��
 * @param x      ��Ը����� ����
 * @param y      ��Ը����� ����
 * @param nWidth  ���ڿ��
 * @param nHeight ���ڸ߶�
 * @param winID  ����ID��ȫ��Ψһ��
 * @param title  ���ڱ���
 * @return NULL ʧ��
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

  //����Ӵ���
  addChild(parent, HHOwin);
  //���ô��ڻ��ƺ���ָ��
  HHOwin->onPaint = OnPaint;
  //���ô����������ָ��
  HHOwin->onDestroy = OnDestory;

  return HHOwin;
}

/**
 * @brief hhoevent����ɾ�����ڣ����ͷ���Դ
 * @param win ��ɾ������ * 
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
 * @brief hhoevent���ƴ���
 * @param win Ҫ���ƵĴ���
 * @param val ����Ĳ���
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
 * @brief ���������Ӵ��ڣ�����OnDestory���ں���ָ�룬ɾ���Ӵ��� * 
 * @param win Ҫɾ���Ĵ���  
 * @return �� 
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
 * ����Ӵ���
 * @param parent ������
 * @param child �Ӵ���
 * 
 * �縸����Ϊ������� 
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
 * ��ȡ���ھ������� x
 * @param win ����
 * ͨ���ۼӸ�����x���꣬��ȡ��������
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
 * ��ȡ���ھ������� x
 * @param win ����
 * ͨ���ۼӸ�����x���꣬��ȡ��������
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
 * @brief ���������Ӵ��ڣ�����onPaint���ں���ָ����ƴ��� 
 * @param win Ҫɾ���Ĵ���  
 * @return �� 
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
 * ���ݴ���ID�����Ӵ���
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
 * ���ݴ���ID�����Ӵ���
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
 * ������� ����x��y�Ƿ��ڴ�����
 * 
 * @param win Ҫ���Ĵ���
 * @param x ���������x
 * @param y ���������y
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
 * ���������ڴ�����
 * 
 * @param win ����
 * @param mouse ���״̬
 * 
 * @return ���ر�������Ӵ��ڣ������Ӵ��ܱ���������ش���
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