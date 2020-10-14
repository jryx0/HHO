#include "macrodef.h"
#include "hhosvga.h"
#include "mouse.h"
#include "hglobal.h"
#include "htextbox.h"
#include "HBaseWin.h"
#include "wdesktop.h"
#include "hlabel.h"

#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <mem.h>

int main(void)
{
  char kbchar = 0;
  globaldef *_global;
  hbasewinAttr *desktop;
  hbasewinAttr *child = NULL;
  int blink = 0;
  //int size;

  //��ʼ��ϵͳ����
  _global = initGlobalSetting();

  //��ʼ��ͼ�ν���
  initSVGA64k();

  //��ʼ������
  desktop = CreateDesktop();
  _global->activePage = findWinByID(desktop, ID_HOMEPAGE); //����ȱʡ�ҳ��-homepage
  if (desktop && desktop->onPaint)                         //ˢ��ҳ��
    desktop->onPaint(desktop, NULL);

  //��ʼ�����
  ResetMouse(&_global->mouse);

  while (1) //��ѭ��
  {
    //��괦��
    RestoreMouseBk(&_global->mouse); //�������
    UpdateMouseStatus(&_global->mouse);
    child = checkmousewin(desktop, &_global->mouse);
    if (child)
      if (child->winID < ID_DESKTOP_MAX)
      { //�����desktop�� desktop��ҳ�桢�ؼ���Լ���ؼ�IDֵ<ID_DESKTOP_MAX��������ؼ���desktop����
        //�磺�˵����л�ҳ�桢��¼�ȵ�
        if (desktop->EventHandler)
          desktop->EventHandler(child, EVENT_MOUSE, _global);
      }
      else
      { //�����ҳ������,��ҳ�洦���¼�
        if (child->EventHandler)
          child->EventHandler(child, EVENT_MOUSE, _global);
      }

    SaveMouseBk(&_global->mouse); //���汳��
    MouseDraw(&_global->mouse);   //��ʾ���

    if (_global->foucsedTextBox)
      DrawTextCursor(_global->foucsedTextBox, blink++);

    delay(30);

    //���̴���
    if (kbhit())
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
      if (kbchar == 'c')
      { //�����
        RestoreMouseBk(&_global->mouse);
        clearScreen(0xffff);
        SaveMouseBk(&_global->mouse);

        if (desktop)
          _global->mouse.currentCur = (unsigned char(*)[MOUSE_WIDTH])_global->cursor_arrow;
      }
      else if (kbchar == 'r')
      {
        if (desktop)
          desktop->onPaint(desktop, NULL);
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