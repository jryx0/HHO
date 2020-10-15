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
    //�������
    RestoreMouseBk(&_global->mouse);

    //����¼�����
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

    //��괦��
    if (_global->foucsedTextBox)
      DrawTextCursor(_global->foucsedTextBox, blink++);

    //�����¼�����
    if (_bios_keybrd(_KEYBRD_READY))
    {
      key = _bios_keybrd(_KEYBRD_READ);
      kbchar = key & 0xFF;
      /* Determine if shift keys are used */
      modifiers = _bios_keybrd(_KEYBRD_SHIFTSTATUS);

      if (key == 0x6100 && modifiers == 0x224)
      { //�˳� �� ctrl + F4
        _global->isExit = TRUE;
      }
      else if (key == 0x5E00 && modifiers == 0x224)
      { //�л����뷨 �� ctrl + F1
        _global->InputMode = !_global->InputMode;
      }
      else if (_global->foucsedTextBox)
      { //�м����textbox
        if (kbchar >= 0x20 || kbchar == 0x0D)
        { //�ɴ�ӡ�ַ��ͻس�
          if (_global->InputMode == ENGLISH ||
              _global->foucsedTextBox->wintype == TEXTBOX_PASSWORD)
          { //�ַ�/��������ģʽ
            char str[2];
            str[1] = 0;
            str[0] = kbchar;
            _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
          }
          else
          { //��������
          }
        }
        else
        {
          if(_global->foucsedTextBox->onKey)
            _global->foucsedTextBox->onKey(_global->foucsedTextBox, &key);         
        }
      }
      TRACE(("key = %x, %x, %c, modifiers= %x\n", key, key & 0xFF, key & 0xFF, modifiers));
    }

    /*
    if (kbhit())
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
      //kbchar = _bios_keybrd(_KEYBRD_READ);
      if (_global->foucsedTextBox && _global->foucsedTextBox->onKeyPress)
      {
        char str[2];
        str[1] = 0;
        str[0] = kbchar;
        _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
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
    */

    if (_global->isExit)
      break;

    SaveMouseBk(&_global->mouse); //���汳��
    MouseDraw(&_global->mouse);   //��ʾ���
    delay(30);
  }

  destoryGlobalSettting(_global);
  return 0;
}