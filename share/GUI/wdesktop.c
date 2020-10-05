#include "wdesktop.h"
#include "HBaseWin.h"
#include "hhosvga.h"
#include "mouse.h"
#include "wResource.h"

void OnPaint_Desktop(hbasewinAttr *win, void *value)
{

  if (win == NULL)
    return;

  // setfillstyle(SOLID_FILL, RealFillColor(desktopColor));
  // bar(win->x, win->y, win->nWidth, win->nHeight);

  // ///////color test///////////
  // setfillstyle(SOLID_FILL, RealFillColor(desktopColor));
  // bar(maxx - 20, maxy - 100, maxx - 5, maxy);
  // rectangle(maxx - 20, maxy - 100, maxx - 5, maxy);

  // setfillstyle(SOLID_FILL, RealFillColor(CapColor));
  // bar(maxx - 20 - 20, maxy - 100, maxx - 5 - 20, maxy);

  // setfillstyle(SOLID_FILL, RealFillColor(Shade0));
  // bar(maxx - 20 - 40, maxy - 100, maxx - 5 - 40, maxy);

  // setfillstyle(SOLID_FILL, RealFillColor(Shade1));
  // bar(maxx - 20 - 60, maxy - 100, maxx - 5 - 60, maxy);

  // setfillstyle(SOLID_FILL, RealFillColor(Shade2));
  // bar(maxx - 20 - 80, maxy - 100, maxx - 5 - 80, maxy);

  // setfillstyle(SOLID_FILL, RealFillColor(Shade3));
  // bar(maxx - 20 - 100, maxy - 100, maxx - 5 - 100, maxy);
  ////////////////////////////
  clearScreen(0xFF);
  Putbmp64k(0, 0, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linexEx(0, 80, SCR_WIDTH, RGB565(100, 100, 100));

  repaintChildren(win);
}

hbasewinAttr *CreateDesktop()
{
  hbasewinAttr *desktop;
  int maxx = getmaxx() - 1;
  int maxy = getmaxy() - 1;

  desktop = CreateWindowsEx(NULL, 0, 0, maxx, maxy, 0, NULL);

  //setviewport(0, 0, maxx, maxy, 1);
  // switch (screenmode) {
  // // 1024*768 256
  // case 1:
  //   desktop = CreateWindowsEx(NULL, 0, 0, 1023, 767, 0, NULL);
  //   SetMouseArea(0, 0, 1023, 767);
  //   break;
  // // 800*600 32k
  // case 2:
  //   desktop = CreateWindowsEx(NULL, 0, 0, 799, 599, 0, NULL);
  //   SetMouseArea(0, 0, 799, 599);
  //   break;
  // // 800*600 64k
  // case 3:
  //   desktop = CreateWindowsEx(NULL, 0, 0, 799, 599, 0, NULL);
  //   SetMouseArea(0, 0, 799, 599);
  //   break;
  // // 1024*768 256
  // default:
  //   desktop = CreateWindowsEx(NULL, 0, 0, 1023, 767, 0, NULL);
  //   SetMouseArea(0, 0, 1023, 767);
  //   break;
  // }

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
hbasewinAttr *checkmousewin(hbasewinAttr *win, mousestatus *mouse)
{
  list_iterator_t *it;
  list_node_t *node;
  hbasewinAttr *temp;

  if (win == NULL || mouse == NULL)
    return win;
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