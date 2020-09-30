#include "HBaseWin.h"
#include "hbutton.h"
#include "hhosvga.h"
#include "mouse.h"
#include "wResource.h"
#include "wdesktop.h"

#include <conio.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int screenMode, type = 0;
  char kbchar;
  mousestatus mouse;
  hbasewinAttr *btn1, *btn2, *btn3;
  hbasewinAttr *desktop;
  hbasewinAttr *currentwin = NULL, *oldwin = NULL;

  initSVGA64k();
  line(100, 20, 100, 200, 64170);
  line(101, 20, 101, 200, 64170);

  /*initSvga(screenMode);
  InitMouse();

  desktop = CreateDesktop(screenMode);

  btn1 = CreateButton(desktop, 10, 10, 80, 25, ID_WIN_LOG_PANEL, "aaa");
  btn2 = CreateButton(desktop, 100, 10, 80, 25, ID_WIN_LOGIN, "aa");

  desktop->onPaint(desktop, NULL);

  ShowMouse();*/
  //bar(0, 0, SCR_WIDTH - 1, SCR_HEIGHT - 1,  65535);
  clearScreen(0x0555);

  while (1) {

    // line(random(SCR_WIDTH), random(SCR_HEIGHT), random(SCR_WIDTH),
    //      random(SCR_HEIGHT), random(65535));

    /*updateMouseStatus(&mouse);
       currentwin = checkmousewin(desktop, &mouse);
       if (currentwin)
         desktop->EventHandler(currentwin, EVENT_MOUSE, &mouse);


         if (oldwin != currentwin && oldwin)
           oldwin->onPaint(oldwin, NULL);

         oldwin = currentwin;*/

    if (kbhit()) {      //如果有按键按下，则kbhit()函数返回真
      kbchar = getch(); //使用getch()函数获取按下的键值
      if (kbchar == 'c') {
        // setcolor(RealColor(15));
        // line(0, 0, 100, 10);
      } else if (kbchar == 'r') {
        // setfillstyle(SOLID_FILL, RealFillColor(9));
        // bar(10, 10, 80, 80);
      } else if (kbchar == 'a') {
      } else if (kbchar == ' ' || kbchar == 27) {
        break;
      } //当按下ESC或空格退出时循环，ESC键的键值时27.
    }
  }

  return 0;
}