
#include "hglobal.h"
#include "SVGAUTIL.H"
#include "HBaseWin.h"
#include "mouse.h"

#include <conio.h>
#include <graphics.h>
#include <stdio.h>
#include <dos.h>

int main(int argc, char *argv[])
{
  int screenMode = -1;
  char kbchar;
  mousestatus mouse;
  globaldef *_global;

  //初始化系统参数
  _global = initGlobalSetting();

  //初始化图形界面
  if (argc > 1)
    screenMode = atoi(argv[1]);
  initSvga(screenMode);

  clearScreen(0xff);

  //加载图形资源
  loadSvgaResouce(_global);
  loadMouse(_global);

  setcolor(RealDrawColor(0x00));
  
  printHZ(_global->fphanzi_ss16, 100, 200, "你好ａa1１", 0x00, 16);
  printHZ(_global->fphanzi_sh16, 100, 250, "我", 0x00, 16);

  while (1)
  {

    MousePutbk((unsigned int *)_global->cursorBK,
               _global->mouse.x, _global->mouse.y, MOUSE_WIDHT, MOUSE_HEIGHT);
    updateMouseStatus(&(_global->mouse));

    MouseSavebk((unsigned int *)_global->cursorBK,
                _global->mouse.x, _global->mouse.y, MOUSE_WIDHT, MOUSE_HEIGHT);
    DrawCursor((unsigned char *)_global->cursor_hand,
               _global->mouse.x, _global->mouse.y, MOUSE_WIDHT, MOUSE_HEIGHT);

    delay(30);
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

  destoryGlobalSettting(_global);
  return 0;
}
