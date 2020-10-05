
#include "hglobal.h"
#include "hhosvga.h"
#include "svga.h"
#include "HBaseWin.h"
#include "mouse.h"

#include <conio.h>
#include <stdlib.h>
#include <mem.h>

#define MOUSEWIDTH 20
#define MOUSEHIGHT 30

int main(void)
{
  mousestatus mouse_new, mouse_old; //����¾ɽṹ��
  char kbchar = 0;
  mousestatus _mouse;
  globaldef *_global;

  //��ʼ��ϵͳ����
  _global = initGlobalSetting();

  //��ʼ��ͼ�ν���
  initSVGA64k();

  //����ͼ����Դ
  loadSvgaResouce(_global);

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

  mouse_old.x = 0;
  mouse_old.y = 0;

  resetMouse();

  while (1)
  {
    getMouseStatus(&mouse_new.leftClickState, &mouse_new.x, &mouse_new.y);
    restoreMouseBk(_global->cursorBK, mouse_old.x, mouse_old.y, MOUSE_WIDTH, MOUSE_HEIGHT);
    saveMouseBK(_global->cursorBK, mouse_new.x, mouse_new.y, MOUSEWIDTH, MOUSEHIGHT);
    drawMousecursor(_global->cursor_arrow, mouse_new.x, mouse_new.y);
    delay(30);
    mouse_old = mouse_new;

    if (kbhit())
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
      if (kbchar == 'c')
      {
        // setcolor(RealColor(15));
        restoreMouseBk(_global->cursorBK, mouse_new.x, mouse_new.y, MOUSE_WIDTH, MOUSE_HEIGHT);
        line(mouse_new.x, mouse_new.y, mouse_new.x + random(1024), mouse_new.y + random(768), random(65535));
        saveMouseBK(_global->cursorBK, mouse_new.x, mouse_new.y, MOUSEWIDTH, MOUSEHIGHT);
      }
      else if (kbchar == 'r')
      {
        restoreMouseBk(_global->cursorBK, mouse_new.x, mouse_new.y, MOUSE_WIDTH, MOUSE_HEIGHT);
        bar(mouse_new.x, mouse_new.y, mouse_new.x + random(1024), mouse_new.y + random(768), random(65535));
        saveMouseBK(_global->cursorBK, mouse_new.x, mouse_new.y, MOUSEWIDTH, MOUSEHIGHT);
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