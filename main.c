#include "HBaseWin.h"
#include "hhogui.h"
#include "mouse.h"

#include <conio.h>
#include <stdlib.h>
#include <mem.h>
#include <alloc.h>

int main(void)
{
  MOUSE mouse_new, mouse_old = {{0, 0}, 0}; //����¾ɽṹ��
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
  MouseReset();
  while (1)
  {
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
        MousePutBk(mouse_new.position);
        line(mouse_new.position.x, mouse_new.position.y, mouse_new.position.x + random(1024), mouse_new.position.y + random(768), random(65535));
        MouseStoreBk(mouse_new.position);
      }
      else if (kbchar == 'r')
      {
        MousePutBk(mouse_new.position);
        bar(mouse_new.position.x, mouse_new.position.y, mouse_new.position.x + random(1024), mouse_new.position.y + random(768), random(65535));
        MouseStoreBk(mouse_new.position);
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