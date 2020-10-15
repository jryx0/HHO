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
#include <stdio.h>
#include <mem.h>
#include <bios.h>

int main(void)
{
  char kbchar = 0;
  int key, modifiers;
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
      { //�����desktop�� desktop��ҳ�桢�ؼ���Լ���ؼ�IDֵ<ID_DESKTOP_MAX��
        //���ID<ID_DESKTOP_MAX�Ŀؼ���desktop����
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

    if (_global->isExit) //��Ļ��ť�˳�
      break;

    delay(30);

    // //���̴���
    // if (_bios_keybrd(_KEYBRD_READY))
    // {
    //   key = _bios_keybrd(_KEYBRD_READ);
    //   /* Determine if shift keys are used */
    //   modifiers = _bios_keybrd(_KEYBRD_SHIFTSTATUS);
    //   TRACE(("key = %x, %x, %c, modifiers= %x\n", key, key & 0xFF, key & 0xFF, modifiers));

    //   if(key && 0xFF)
    //   {

    //   }



    // }

    
    if (kbhit())
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
      //kbchar = _bios_keybrd(_KEYBRD_READ);
      if (_global->foucsedTextBox && _global->foucsedTextBox->onKeyPress)
      {

        char str[2];
        str[1] = 0;
        str[0] = kbchar;
        DrawTextCursor(_global->foucsedTextBox, 1); //���ع��
        //_global->foucsedTextBox->onPaint(NULL, NULL);
        _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
        _global->foucsedTextBox->onPaint(_global->foucsedTextBox, NULL);
      }

      if (kbchar == 'c')
      { //�����
      }
      else if (kbchar == 'r')
      {
        if (desktop)
          desktop->onPaint(desktop, NULL);
      }
      else if (kbchar == ' ' || kbchar == 27)
      { //������ESC��ո��˳�ʱѭ��,ESC���ļ�ֵʱ27
        break;
      }
      TRACE(("press key:%c,%u\n", kbchar, kbchar));
    }
  }

  destoryGlobalSettting(_global);
  return 0;
}