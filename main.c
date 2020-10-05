#include "macrodef.h"
#include "hhosvga.h"
#include "mouse.h"
#include "HBaseWin.h"

#include <conio.h>
#include <stdlib.h>
#include <mem.h>

int main(void)
{
  mousestatus mouse_new, mouse_old = {0, 0, 0}; //鼠标新旧结构体
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

  MouseInit();
  ResetMouse(&mouse_old);
  while (1)
  {
    UpdateMouseStatus(&mouse_new);
    RestoreMouseBk(&mouse_old);
    SaveMouseBk(&mouse_new);
    MouseDraw(mouse_new);
    delay(30);
    mouse_old = mouse_new;

    if (mouse_new.leftClickState == MOUSE_BUTTON_UP)
    {
      // setcolor(RealColor(15));
      RestoreMouseBk(&mouse_new);
      line(mouse_new.x, mouse_new.y, mouse_new.x - random(1024), mouse_new.y - random(768), random(65535));
      SaveMouseBk(&mouse_new);
    }

    if (mouse_new.rightClickState == MOUSE_BUTTON_UP)
    {
      break;
    }

    if (kbhit())
    {                   //如果有按键按下，则kbhit()函数返回真
      kbchar = getch(); //使用getch()函数获取按下的键值
      if (kbchar == 'c')
      {
        // setcolor(RealColor(15));
        RestoreMouseBk(&mouse_new);
        line(mouse_new.x, mouse_new.y, mouse_new.x + random(1024), mouse_new.y + random(768), random(65535));
        SaveMouseBk(&mouse_new);
      }
      else if (kbchar == 'r')
      {
        RestoreMouseBk(&mouse_new);
        bar(mouse_new.x, mouse_new.y, mouse_new.x + random(1024), mouse_new.y + random(768), random(65535));
        SaveMouseBk(&mouse_new);
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