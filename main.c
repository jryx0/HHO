
#include "macrodef.h"
#include "SVGAUTIL.H"
#include "hhogui.h"
#include "hbutton.h"
#include "HBaseWin.h"
#include "mouse.h"
#include "wResource.h"
#include "wdesktop.h"

#include <conio.h>
#include <graphics.h>
#include <stdio.h>
#include <mem.h>
#include <alloc.h>

int main(int argc, char *argv[])
{
  int screenMode = -1, type = 0;
  char kbchar;
  mousestatus mouse;
  hbasewinAttr *btn1, *btn2, *btn3;
  hbasewinAttr *desktop;
  hbasewinAttr *currentwin = NULL, *oldwin = NULL;

  int width = 32, height = 32;
  unsigned int far *buffer;
  unsigned char cur[16][16];

  if (argc > 1)
    screenMode = atoi(argv[1]);

  initSvga(screenMode);

  // clearScreen(WhitePixel());

  // setfillstyle(SOLID_FILL, RealFillColor(0x0555));
  // bar(100, 200, 500, 400);

  // setfillstyle(SOLID_FILL, RealFillColor(0xA109));
  // bar(350, 300, 700, 600);

  // setcolor(RealColor(0x56B5));
  // setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
  // rectangle(320, 370, 320 + width, 370 + height);

  //cur = malloc(16 * 16);
  ReadCursor(cur);
  DrawCursor(cur, 10, 10);

  // buffer = (unsigned int far *)farmalloc(width * height * sizeof(unsigned int));
  // if (buffer == NULL)
  //   return;
  // savebackgroundEx(buffer, 320, 370, width, height);
  // restorebackgroundEx(buffer, 40, 370, width, height);
  // free(buffer);

  // savebackgroundFile(320, 370, width, height);
  // restorebackgroundFile(20, 20, width, height);

  InitMouse();
  memset(&mouse, 0, sizeof(mouse));

  desktop = CreateDesktop(screenMode);

  // btn1 = CreateButton(desktop, 10, 10, 80, 25, ID_WIN_LOG_PANEL, "aaa");
  // btn2 = CreateButton(desktop, 100, 10, 80, 25, ID_WIN_LOGIN, "aa");

  desktop->onPaint(desktop, NULL);

  // ShowMouse();
  SetMouseCoord(0, 0);
  buffer = (unsigned int far *)farmalloc(16 * 16);
  MouseSavebk(buffer, mouse.x, mouse.y);

  while (1)
  {
    MousePutbk(buffer, mouse.x, mouse.y);
    updateMouseStatus(&mouse);
    MouseSavebk(buffer, mouse.x, mouse.y);
    DrawCursor(cur, mouse.x, mouse.y);
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

  return 0;
}