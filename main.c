#include "macrodef.h"
#include "hglobal.h"
#include "hhosvga.h"
#include "mouse.h"
#include "HBaseWin.h"

#include <conio.h>
#include <stdlib.h>
#include <mem.h>

#define DEBUG

int main(void)
{
  char kbchar = 0;
  globaldef *_global;

  //��ʼ��ϵͳ����
  _global = initGlobalSetting();

  //��ʼ��ͼ�ν���
  initSVGA64k();

  //��ʼ����Ļ����
  clearScreen(0xFF);

#ifdef DEBUG
  //��Ļ�������
  hsvgatest();

  //printHZ(_global->fphanzi_sh16, 700, 50, "���пƼ���ѧУҽԺ", 0x5F40, 16);
  // printtextxy(_global->fphanzi_sh16, 700, 50, "���пƼ���ѧУҽԺ", 0x5F40, 16);
  // printtextxy(_global->fphanzi_ss16, 700, 70, "���пƼ���ѧУҽԺ", 0x5F40, 16);
  // printtextxy(_global->fphanzi_sh24, 700, 90, "���пƼ���ѧУҽԺ", 0x5F40, 16);
  // printtextxy(_global->fphanzi_ss24, 700, 110, "���пƼ���ѧУҽԺ", 0x5F40, 16);
#endif

  //��ʼ�����
  ResetMouse(&_global->mouse);

  while (1)
  { //ѭ��
    UpdateMouseStatus(&_global->mouse);
    // CreateMouseEvent();

    // UpdateKeyboard();
    // CreateKeyboardEvent();

    // EventHandler();
    delay(30);

//////////////////// ���Դ���/////////////////////////////////////////
#ifdef DEBUG
    if (_global->mouse.leftClickState == MOUSE_BUTTON_UP)
    {
      // setcolor(RealColor(15));
      RestoreMouseBk(&_global->mouse);
      line(_global->mouse.x, _global->mouse.y, _global->mouse.x - random(1024), _global->mouse.y - random(768), random(65535));
      SaveMouseBk(&_global->mouse);
    }

    if (_global->mouse.rightClickState == MOUSE_BUTTON_UP)
    {
      break;
    }

    if (kbhit())
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
      if (kbchar == 'c')
      {
        // setcolor(RealColor(15));
        RestoreMouseBk(&_global->mouse);
        line(_global->mouse.x, _global->mouse.y, _global->mouse.x + random(1024), _global->mouse.y + random(768), random(65535));
        SaveMouseBk(&_global->mouse);
      }
      else if (kbchar == 'r')
      {
        RestoreMouseBk(&_global->mouse);
        bar(_global->mouse.x, _global->mouse.y, _global->mouse.x + random(1024), _global->mouse.y + random(768), random(65535));
        SaveMouseBk(&_global->mouse);
      }
      else if (kbchar == 'a')
      {
      }
      else if (kbchar == ' ' || kbchar == 27)
      {

        break;
      } //������ESC��ո��˳�ʱѭ����ESC���ļ�ֵʱ27.
    }
#endif
  }

  destoryGlobalSettting(_global);
  return 0;
}