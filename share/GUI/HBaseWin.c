
//#include "macrodef.h"
#include "mouse.h"
#include "hhosvga.h"
#include "wResource.h"
#include "HBaseWin.h"
#include "list.h"
#include "hglobal.h"

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
  HHOwin->wintype = WIN;
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

  HHOwin->onTheme = OnTheme;

  return HHOwin;
}

/**
 * @brief hhoevent����ɾ�����ڣ����ͷ���Դ
 * @param win ��ɾ������  
 */
void OnDestory(hbasewinAttr *win, void *val)
{
  list_node_t *node = NULL;

  TESTNULLVOID(win);
  destoryChildren(win);

  if (win->children)
    list_destroy(win->children);

  if (win->parent && win->parent->children)
  {
    node = list_find(win->parent->children, win);
    if (node)
      list_remove(win->parent->children, node);
  }
  else
  {
    freeWin(win);
    win = NULL;
  }
  (void)val;
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

  //�����ã�ÿ��������䱳���������
  fillRegion(x1, y1, x2, y2, COLOR_ligtgray);
  rectangle(x1, y1, x2, y2, COLOR_darkgray, 1, 2);
  (void)val;
}

/**
 * @brief �ݹ��Ӵ��ڣ���OnDestoryʱ���ã�ɾ���Ӵ��ڵ������Ӵ���
 * win�����ɵ��ú���ɾ����
 *  
 * @param win Ҫɾ���Ĵ���  
 * @return �� 
 */
void destoryChildren(hbasewinAttr *win)
{
  list_iterator_t *it;
  list_node_t *node;
  hbasewinAttr *childwin = NULL;

  TESTNULLVOID(win);
  TESTNULLVOID(win->children);

  it = list_iterator_new(win->children, LIST_HEAD);
  while ((node = list_iterator_next(it)))
  {
    if (node->val != NULL)
    {
      childwin = (hbasewinAttr *)(node->val);
      if (childwin->children != NULL)
      {
        destoryChildren(childwin);
        list_destroy(childwin->children);
      }
      childwin->onDestroy(childwin, NULL);
    }
  }
  list_iterator_destroy(it);
}

/**
 * ����������ڵ�����
 * 
 */
void clearWinRegion(hbasewinAttr *win, int color)
{
  TESTNULLVOID(win);
  fillRegionEx(win->x, win->y, win->nWidth, win->nHeight, color);
}

/**
 * �Ƚ����������Ƿ���ͬһ��
 * 
 */
int matchWin(void *w1, void *w2)
{
  if (w1 && w2)
    return ((hbasewinAttr *)w1)->winID == ((hbasewinAttr *)w2)->winID;
  return 0;
}

/**
 * �ͷŴ���
 * 
 */
void freeWin(hbasewinAttr *win)
{
  //hbasewinAttr *win = (hbasewinAttr *)val;
  if (win->title)
    free(win->title);

  if (win->style)
	free(win->style);

  free(win);
  win = NULL;
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
  list_node_t *node = NULL;
  TESTNULL(parent, NULL);
  TESTNULL(child, NULL);

  if (parent->children == NULL)
  {
    parent->children = list_new();
    parent->children->match = matchWin;
    parent->children->free = freeWin;
  }

  node = list_node_new(child);
  if (node)
    list_rpush(parent->children, node);
  else
  {
    TRACE(("%s(%d): addChild ����list_node_tʧ��, winID = %d��", __FILE__, __LINE__, child->winID));
  }

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
void repaintChildren(hbasewinAttr *win, void *val)
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
      childwin->onPaint(childwin, val);
  }
  list_iterator_destroy(it);
}

/**
 * �ҵ��������ڣ����д��ڵĸ�����
 */
hbasewinAttr *getRootWin(hbasewinAttr *win)
{
  TESTNULL(win, win);
  if (win->parent == NULL)
    return win;

  return getRootWin(win->parent);
}

/**
 * ��win�в���winID���Ӵ���
 * 
 */
hbasewinAttr *findWinByID(hbasewinAttr *win, int winID)
{
  hbasewinAttr *child = NULL;
  TESTNULL(win, NULL);

  if (win->winID == winID)
    return win;

  if (win->children)
  {
    list_iterator_t *it = list_iterator_new(win->children, LIST_HEAD);
    list_node_t *node;
    while ((node = list_iterator_next(it)))
      if (node->val)
      {
        child = (hbasewinAttr *)node->val;
        if (child->winID == winID)
          break;

        if (child->children)
        {
          if (child = findWinByID(child, winID))
            break;
        }
        else
          child = NULL;
      }
    list_iterator_destroy(it);
  }

  return child;
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

  // if (win->status == HHOHIDE || win->status == HHODELETE)
  //   return 0;

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

  TESTNULL(mouse, win);
  TESTNULL(win, NULL);

  it = list_iterator_new(win->children, LIST_HEAD);
  while (node = list_iterator_next(it))
  {
    if (node->val == NULL)
      continue;

    temp = (hbasewinAttr *)(node->val);
    if (checkpointInside(temp, mouse->x, mouse->y))
    {

      list_iterator_destroy(it);
      return temp; //�ҵ������֮����
    }
  }
  list_iterator_destroy(it);

  return win; //û�е���Ӵ��ڣ���������
}

void OnTheme(hbasewinAttr *win, void *val)
{
  WinStyle *_winStyle = NULL;

  TESTNULLVOID(win);
  TESTNULLVOID(val);

  if (win->style)
  {
	_winStyle = (WinStyle *)win->style;
    getWinTheme(_winStyle, *(int *)val);
  }
  themeChildren(win, val);
}
/**
 * ����win�������Ӵ���,ִ��theme�¼�
 * @param win ����
 * @param val ����
 * @param event �¼�
 * @return ��
 */
void themeChildren(hbasewinAttr *win, void *val)
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

    if (childwin->onTheme != NULL)
      childwin->onTheme(childwin, val);
  }
  list_iterator_destroy(it);
}
