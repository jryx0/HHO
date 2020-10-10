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
  LIST,
  CHECKBOX,
  RADIOBOX,
  PANEL,
  DESKTOP
};

typedef void (*hhoevent)(struct winstruct *win, void *value);
typedef void (*hhoeventhandler)(struct winstruct *win, int type, void *value);

typedef struct
{
  int standardShade[4];
  int capColour;
  int fontColour;
  int desktopColour;
  int capFontColour;

  //��Ļ�����ʾ��ɫ 256 32k 64k��ɫ
  int maxcolors;
} win_colors;

typedef struct winstruct
{
  //�������ͣ���ť���ı�����򡢶�ѡ��
  int hhowintype;
  //����ΨһID
  int winID;

  //��Ը��������Ͻ�����x,y
  int x;
  int y;
  //���
  int nWidth;
  //�߶�
  int nHeight;

  //����״̬
  int status; // winStatus

  //������ɫ
  // win_colors *colors;

  //��ʾ����
  char *title;

  //�洢�����ݣ����ı��������ַ���
  void *value;

  //����
  int *bkarea;

  //��Ӧ����¼�����ָ�뵥��,������£������������뿪��������
  hhoevent onClick, onLeftUp, onLeftDown, onLeave, onEnter;
  //��Ӧ�����¼�����ָ�뵥������
  hhoevent onKeyPress;
  //��Ӧ�����¼�������������
  hhoevent onPaint;
  //��Ӧ�����¼������ڼ���
  hhoevent onActivate;

  //����ɾ��
  hhoevent onDestroy;

  //�����¼�
  hhoeventhandler EventHandler;

  //�Ӵ����б�
  list_t *children;
  //������
  struct winstruct *parent;
  struct winstruct *desktop;
} hbasewinAttr;

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
                              int nHeight, int winID, const char *title);
/**
 * @brief hhoevent����ɾ�����ڣ����ͷ���Դ
 * @param win ��ɾ������ * 
 */
void OnDestory(hbasewinAttr *win, void *val);

/**
 * @brief hhoevent���ƴ���
 * @param win Ҫ���ƵĴ���
 * @param val ����Ĳ���
 */
void OnPaint(hbasewinAttr *win, void *val);

/**
 * @brief ���������Ӵ��ڣ�����OnDestory���ں���ָ�룬ɾ���Ӵ��� * 
 * @param win Ҫɾ���Ĵ���  
 * @return �� 
 */
void destoryChildren(hbasewinAttr *win);
/**
 * ����������ڵ�����
 * 
 */
void clearWinRegion(hbasewinAttr *win, int color);
/**
 * �ͷŴ���
 * @param win Ҫɾ���Ĵ���  
 */
void freeWin(hbasewinAttr *win);

/**
 * ����Ӵ���
 * @param parent ������
 * @param child �Ӵ���
 * 
 * �縸����Ϊ������� 
 */
hbasewinAttr *addChild(hbasewinAttr *parent, hbasewinAttr *child);

/**
 * ��ȡ���ھ������� x
 * @param win ����
 * ͨ���ۼӸ�����x���꣬��ȡ��������
 */
int getAbsoluteX(hbasewinAttr *win);

/**
 * ��ȡ���ھ������� x
 * @param win ����
 * ͨ���ۼӸ�����x���꣬��ȡ��������
 */
int getAbsoluteY(hbasewinAttr *win);

/**
 * @brief ���������Ӵ��ڣ�����onPaint���ں���ָ����ƴ��� 
 * @param win Ҫɾ���Ĵ���  
 * @return �� 
 */
void repaintChildren(hbasewinAttr *win);

/**
 * ������� ����x��y�Ƿ��ڴ�����
 * 
 * @param win Ҫ���Ĵ���
 * @param x ���������x
 * @param y ���������y
 */
int checkpointInside(hbasewinAttr *win, int x, int y);

/**
 * ���������ڴ�����
 * 
 * @param win ����
 * @param mouse ���״̬
 * 
 * @return ���ر�������Ӵ��ڣ������Ӵ��ܱ���������ش���
 */
hbasewinAttr *checkmousewin(hbasewinAttr *win, mousestatus *mouse);

/**
 * ���ݴ���ID�����Ӵ���
 *  win
 */
// hbasewinAttr *FindChildWinbyID(hbasewinAttr *win, int id);

/**
 * ���ݴ���ID�����Ӵ���
 *  win
 */
// list_node_t *FindChildNodebyID(hbasewinAttr *win, int id);
#endif