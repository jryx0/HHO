
#include "macrodef.h"
#include "SVGAUTIL.H"
#include "HBaseWin.h"
#include "mouse.h"
#include "hglobal.h"

#include <conio.h>
#include <graphics.h>
#include <stdio.h>
#include <mem.h>
#include <alloc.h>

int main(int argc, char *argv[])
{
  int screenMode = -1;
  char kbchar;
  mousestatus mouse;
  globaldef *_global;

  //初始化系统参数
  _global = initGlobalSetting();
  if (_global == NULL)
  {
    printf("初始化失败！\r\n");
  }

  //初始化图形界面
  if (argc > 1)
    screenMode = atoi(argv[1]);
  initSvga(screenMode);

  clearScreen(0xff);

  //加载图形资源
  loadSvgaResouce(_global);
  loadMouse(_global);

  while (1)
  {

    MousePutbk(_global->cursorBK, _global->mouse.x, _global->mouse.y, MOUSE_WIDHT, MOUSE_HEIGHT);
    updateMouseStatus(&(_global->mouse));
    MouseSavebk(_global->cursorBK, _global->mouse.x, _global->mouse.y, MOUSE_WIDHT, MOUSE_HEIGHT);
    DrawCursor(_global->cursor_arrow, _global->mouse.x, _global->mouse.y, MOUSE_WIDHT, MOUSE_HEIGHT);


    delay(10);
    // currentwin = checkmousewin(desktop, &mouse);
    // if (currentwin)
    //   desktop->EventHandler(currentwin, EVENT_MOUSE, &mouse);

    /*
      if (oldwin != currentwin && oldwin)
        oldwin->onPaint(oldwin, NULL);

      oldwin = currentwin;*/

    if (kbhit())
    {                   //如果有按键按下，则kbhit()函数返回真
      kbchar = getch(); //使用getch()函数获取按下的键值
      if (kbchar == 'c')
      {
        setcolor(RealColor(15));
        line(0, 0, 100, 10);
      }
      else if (kbchar == 'r')
      {
        setfillstyle(SOLID_FILL, RealFillColor(9));
        bar(10, 10, 80, 80);
      }
      else if (kbchar == 'a')
      {
      }
      else if (kbchar == ' ' || kbchar == 27)
      {
        break;
      } //当按下ESC或空格退出时循环，ESC键的键值时27.
    }
  }

  return 0;
}
