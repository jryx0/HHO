#include "HBaseWin.h"
#include "hhogui.h"
#include "mouse.h"

#include <conio.h>
#include <stdlib.h>
#include <graphics.h>

int main(int argc, char *argv[])
{
  MOUSE mouse_new, mouse_old = {{0, 0}, 0}; //����¾ɽṹ��
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
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
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
      } //������ESC��ո��˳�ʱѭ����ESC���ļ�ֵʱ27.
    }
  }

  return 0;
}