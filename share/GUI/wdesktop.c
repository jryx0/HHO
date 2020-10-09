#include "macrodef.h"
#include "mouse.h"
#include "hhosvga.h"
#include "wdesktop.h"
#include "HBaseWin.h"
#include "wResource.h"
#include "hglobal.h"

#include "testpage.h"
#include "homepage.h"

/**
 * ���ڻ��Ƴ���
 * 
 */
void OnPaint_Desktop(hbasewinAttr *win, void *val)
{
  hfont *_font;

  if (win == NULL)
    return;

  clearScreen(COLOR_white);
  //draw header
  Putbmp64k(0, 0, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linex_styleEx(0, HEADER_HEIGHT, SCR_WIDTH, 0x2104, 2, 1);

  repaintChildren(win);

  
  //draw footer
  linex_styleEx(0, SCR_HEIGHT - FOOTER_HEIGHT, SCR_WIDTH, 0x2104, 2, 1);
  _font = getFont(SIMSUN, 16, 0x00);
  _font->fontcolor = 0x4228;
  printTextLineXY(100, 500, win ->title, _font);
  printTextLineXY(215, SCR_HEIGHT - FOOTER_HEIGHT + 15, "��ַ������ʡ�人�к�ɽ������·1037��      ��ѯ�绰:027-87543469", _font);
  freeFont(_font);

  (void)val;
}

/**
 * ɾ������
 */
void OnDestory_Desktop(hbasewinAttr *win, void *value)
{

  // TESTNULLVOID(win);
  // if (value == NULL)
  // { //ɾ��ȫ������
  //   //�����Ӵ��������Ӵ���ɾ������ָ��
  //   destoryChildren(win);
  //   OnDestory(win, NULL);
  //   clearScreen(COLOR_white);
  // }
  // else
  // {
  //   //ɾ��ָ������
  //   int *id = (int *)value;
  //   list_node_t *node = FindChildNodebyID(win, *id);

  //   if (node)
  //   {
  //     hbasewinAttr *child = NULL;

  //     if (node->val)
  //     {
  //       int x1, y1, x2, y2;
  //       child = (hbasewinAttr *)node->val;
  //       x1 = getAbsoluteX(child);
  //       y1 = getAbsoluteY(child);
  //       x2 = x1 + child->nWidth;
  //       y2 = y1 + child->nHeight;

  //       clearRegion(x1, y1, x2, y2, COLOR_white);

  //       if (child->onDestroy)
  //         child->onDestroy(child, NULL);
  //     }
  //     list_remove(win->children, node);

  //     //win ->onPaint(win, NULL);
  //   }
  // }
}

/**
 * ����������
 * 
 */
hbasewinAttr *CreateDesktop(void)
{
  hbasewinAttr *desktop;

  desktop = CreateWindowsEx(NULL, 0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1, ID_DESKTOP, "desktop");

  //������ҳ
  //CreateHomepage(desktop, ID_HOMEPAGE);
  //����testҳ
  CreateTestPage(desktop, ID_TESTPAGE, "���пƼ���ѧ�Զ����˹�����");

  desktop->onPaint = OnPaint_Desktop;
  desktop->EventHandler = eventhandlerdesktop;

  return desktop;
}

void eventhandlerdesktop(hbasewinAttr *win, int type, void *value)
{
  globaldef *_g;

  TESTNULLVOID(win);
  TESTNULLVOID(value);
  _g = (globaldef *)value;
  switch (type)
  {
  case EVENT_MOUSE:
    if (_g->mouse.currentCur != (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow)
      _g->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_g->cursor_arrow;
    break;

  default:
    break;
  }
}
