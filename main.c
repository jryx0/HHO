#include "macrodef.h"
#include "hhosvga.h"
#include "mouse.h"
#include "hglobal.h"

#include "HBaseWin.h"
#include "wdesktop.h"
#include "hlabel.h"

#include <conio.h>
#include <stdlib.h>
#include <mem.h>

int main(void)
{
  char kbchar = 0;
  globaldef *_global;
  hbasewinAttr *desktop;

  //��ʼ��ϵͳ����
  _global = initGlobalSetting();

  //��ʼ��ͼ�ν���
  initSVGA64k();

  desktop = CreateDesktop();

  if (desktop && desktop->onPaint)
    desktop->onPaint(desktop, NULL);

  //��ʼ�����
  ResetMouse(&_global->mouse);

  while (1)
  { //ѭ��
    UpdateMouseStatus(&_global->mouse);
    //CreateMouseEvent(mainPage);

    // UpdateKeyboard();
    // CreateKeyboardEvent();

    // EventHandler();
    delay(30);

    if (kbhit())
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
      if (kbchar == 'c')
      { //�����
        if (desktop)
          _global->mouse.currentCur = _global->cursor_arrow;
      }
      else if (kbchar == ' ' || kbchar == 27)
      { //������ESC��ո��˳�ʱѭ����ESC���ļ�ֵʱ27

        break;
      }
    }
  }

  destoryGlobalSettting(_global);
  return 0;
}