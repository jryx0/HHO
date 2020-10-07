#include "macrodef.h"
#include "mouse.h"
#include "hhosvga.h"
#include "wdesktop.h"
#include "HBaseWin.h"
#include "wResource.h"

/**
 * ���ڻ��Ƴ���
 * 
 */
void OnPaint_Desktop(hbasewinAttr *win, void *value)
{
  if (win == NULL)
    return;

  clearScreen(COLOR_white);
  Putbmp64k(0, 0, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linexEx(0, 80, SCR_WIDTH, RGB565(100, 100, 100));

  repaintChildren(win);
}

/**
 * ɾ������
 */
void OnDestory_Desktop(hbasewinAttr *win, void *value)
{

  TESTNULLVOID(win);
  if (value == NULL)
  { //ɾ��ȫ������
    //�����Ӵ��������Ӵ���ɾ������ָ��
    destoryChildren(win);
    OnDestory(win, NULL);
    clearScreen(COLOR_white);
  }
  else
  {
    //ɾ��ָ������
    int *id = (int *)value;
    list_node_t *node = FindChildNodebyID(win, *id);

    if (node)
    {
      hbasewinAttr *child = NULL;

      if (node->val)
      {
        child = (hbasewinAttr *)node->val;
        if (child->onDestroy)
          child->onDestroy(child, NULL);
      }
      list_remove(win->children, node);

      //win ->onPaint(win, NULL);
    }
  }
}

/**
 * ����������
 * 
 */
hbasewinAttr *CreateDesktop()
{
  hbasewinAttr *desktop;

  desktop = CreateWindowsEx(NULL, 0, 0, SCR_WIDTH - 1, SCR_WIDTH - 1, 0, "desktop");
  desktop->onDestroy = OnDestory_Desktop;
  desktop->onPaint = OnPaint_Desktop;

  desktop->EventHandler = eventhandlerdesktop;
  return desktop;
}

void eventhandlerdesktop(hbasewinAttr *win, int type, void *value)
{
  // mousestatus *mouse;
  // if (win == NULL)
  //   return;

  // switch (type) {
  // case EVENT_MOUSE:
  //   mouse = (mousestatus *)value;

  //   if (mouse->leftClickState == MOUSE_BUTTON_DOWN && win->onLeftDown) {
  //     win->onLeftDown(win, NULL);
  //     if (win->onPaint) {
  //       HideMouse();
  //       win->onPaint(win, NULL);
  //       ShowMouse();
  //     }
  //   } else if (mouse->leftClickState == MOUSE_BUTTON_UP && win->onLeftUp) {
  //     win->onLeftUp(win, NULL);
  //     if (win->onPaint) {
  //       HideMouse();
  //       win->onPaint(win, NULL);
  //       ShowMouse();
  //     }
  //   }

  //   break;

  // default:
  //   break;
  // }
}
