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

  //�������
  // printHZKSS16(300 + 16, 150, "���пƼ���ѧУҽԺ���пƼ���ѧУҽԺ", 0x0);
  // printHZKSH16(300 + 16, 150 + 16, "�����пƼ���ѧУҽԺ", 0x0);
  //int x, int y, char *s, int flag, int part, int color
  printText(300 + 16, 150, "��a���пƼ���ѧУҽԺ���пƼ���ѧУҽԺ���⸾������ͷ����", SIMKAI, 16, 0, 0x0);
  printText(300 + 16, 150 + 50, "��Aa�пƼ���ѧУҽԺ���пƼ���ѧУҽԺ��ù��", SIMSUN, 24, 0, 0x0);
  printText(300 + 16, 150 + 100, "����vcc�Ƽ���ѧУҽԺ���пƼ���ѧУҽԺ��ù��", SIMKAI, 32, 0, 0x0);
  printText(300 + 16, 150 + 140, "���п�hust����ѧУҽԺ���пƼ���ѧУҽԺ��ù��", SIMHEI, 48, 0, 0x0);
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
      }
      else if (kbchar == 'r')
      {
        RestoreMouseBk(&_global->mouse);
        bar(_global->mouse.x, _global->mouse.y, _global->mouse.x + random(1024), _global->mouse.y + random(768), random(65535));
        SaveMouseBk(&_global->mouse);
      }
      else if (kbchar == 'a')
      {
        //printHZKSS24(_global->mouse.x + 24, _global->mouse.y, "�����пƼ���ѧУҽԺ��ù���ۿ����⸾�������ð", 0x0);
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