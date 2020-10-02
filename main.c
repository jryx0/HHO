#include "HBaseWin.h"
#include "hhogui.h"
#include "mouse.h"

#include <conio.h>
#include <stdlib.h>
#include <graphics.h>

int main(int argc, char *argv[])
{
  MOUSE mouse_new, mouse_old = {{0, 0}, 0}; //鼠标新旧结构体
  char kbchar;

  initSVGA64k();
  clearScreen(0x0555);

  MouseInit();
  MouseReset();
  while (1)
  {
    //fillRegionEx(random(SCR_WIDTH), random(SCR_HEIGHT), random(SCR_WIDTH), random(SCR_HEIGHT), random(65535));
    MouseXYB(&mouse_new);
    MousePutBk(mouse_old.position);
    MouseStoreBk(mouse_new.position);
    MouseDraw(mouse_new);
    delay(30);
    mouse_old = mouse_new;
    
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