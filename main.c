#include "macrodef.h"
#include "HBaseWin.h"
#include "SVGAUTIL.H"
#include "hbutton.h"
#include "mouse.h"
#include "wResource.h"
#include "wdesktop.h"

#include <conio.h>
#include <graphics.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int screenMode = -1, type = 0;
  char kbchar;
  mousestatus mouse;
  hbasewinAttr *btn1, *btn2, *btn3;
  hbasewinAttr *desktop;
  hbasewinAttr *currentwin = NULL, *oldwin = NULL;

  if (argc > 1)
    screenMode = atoi(argv[1]);

  initSvga(screenMode);
  InitMouse();

  desktop = CreateDesktop(screenMode);

  btn1 = CreateButton(desktop, 10, 10, 80, 25, ID_WIN_LOG_PANEL, "aaa");
  btn2 = CreateButton(desktop, 100, 10, 80, 25, ID_WIN_LOGIN, "aa");

  //desktop->onPaint(desktop, NULL);

  clearScreen(RealColor(0xA815));

  ShowMouse();

  while (1)
  {
    updateMouseStatus(&mouse);

    currentwin = checkmousewin(desktop, &mouse);
    if (currentwin)
      desktop->EventHandler(currentwin, EVENT_MOUSE, &mouse);

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