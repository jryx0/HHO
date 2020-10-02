#include "HBaseWin.h"
#include "hhogui.h"
#include "mouse.h"

#include <conio.h>
#include <stdlib.h>
#include <mem.h>
#include <alloc.h>

int main(void)
{
  // MOUSE mouse_new, mouse_old = {{0, 0}, 0}; //鼠标新旧结构体
  char kbchar = 0;
  int width = 64, height = 64;
  unsigned int far *buffer;

  initSVGA64k();

  clearScreen(0xFF);
  fillRegionEx(100, 200, 300, 160, 0x4AE6);
  fillRegionEx(150, 250, 300, 160, 0x4356);

  //memory size
  // buffer = (unsigned int far *)malloc(width * height * sizeof(unsigned int));
  // if (buffer == NULL)
  //   return;

  // rectangleEx(140, 240, width, height, 0x5555, 1, 1);
  // savebackgroundEx(buffer, 140, 240, width, height);
  // restorebackgroundEx(buffer, 400, 50, width, height);

  // free(buffer);

  rectangleEx(10, 10, 10000, 20000, 0x5555, 1, 1);

  // MouseInit();
  // MouseReset();
  while (1)
  {

    // MouseXYB(&mouse_new);
    // MousePutBk(mouse_old.position);
    // MouseStoreBk(mouse_new.position);
    // MouseDraw(mouse_new);
    //delay(30);
    //mouse_old = mouse_new;

    if (kbhit())
    {                   //如果有按键按下，则kbhit()函数返回真
      kbchar = getch(); //使用getch()函数获取按下的键值
      if (kbchar == 'c')
      {
        // setcolor(RealColor(15));
        // line(0, 0, 100, 10);
      }
      else if (kbchar == 'r')
      {
        // setfillstyle(SOLID_FILL, RealFillColor(9));
        // bar(10, 10, 80, 80);
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