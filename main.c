#include "macrodef.h"
#include "hhosvga.h"
#include "mouse.h"
#include "hglobal.h"
#include "htextbox.h"
#include "HBaseWin.h"
#include "wdesktop.h"
#include "hlabel.h"
#include "hinput.h"

#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <mem.h>
#include <string.h>
#include <ctype.h>
#include <bios.h>

int main(void)
{
  char kbchar = 0;
  int key, modifiers;
  char candiateHZString[256] = {0};

  globaldef *_global;
  hbasewinAttr *desktop;
  //hbasewinAttr *winInput = NULL;
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
  //_global->pyWin = findWinByID(desktop, ID_PINYIN);
  if (desktop && desktop->onPaint) //ˢ��ҳ��
  {
    desktop->onPaint(desktop, NULL);
    //desktop->onPaint(desktop, &_global->theme);
  }

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
    else
      blink = 0;

    //�����¼�����
    if (_bios_keybrd(_KEYBRD_READY))
    {
      key = _bios_keybrd(_KEYBRD_READ);
      kbchar = key & 0xFF;
      /* Determine if shift keys are used */
      modifiers = _bios_keybrd(_KEYBRD_SHIFTSTATUS);

      //TRACE(("key = %x, %x, '%c', modifiers= %x\n", key, key & 0xFF, key & 0xFF, modifiers));

      if (0x6100 == key) //&& modifiers == 0x224)
      {                  //�˳�  ctrl + F4
        _global->isExit = TRUE;
      }
      else if (0x5f00 == key)
      { // ctrl + F2 �л����
        _global->theme++;
        if (_global->theme >= 4)
          _global->theme = 1;

        desktop->onTheme(desktop, &_global->theme);
        desktop->onPaint(desktop, NULL);
      }
      else if (0x5E00 == key) //&& modifiers == 0x224)
      {                       //�л����뷨  ctrl + F1
        _global->InputMode = !_global->InputMode;
        desktop->onPaint(desktop, _global);
        if (_global->InputMode == ENGLISH)
          closePY(desktop, _global);
      }
      else if (_global->foucsedTextBox)
      { //�м����textbox
        if (CHINESE == _global->InputMode && _global->foucsedTextBox->wintype != TEXTBOX_PASSWORD)
        { //����
          //TRACE(("key = %x, %x, '%c', modifiers= %x\n", key, key & 0xFF, key & 0xFF, modifiers));
          if ((kbchar >= 'a' && kbchar <= 'z') || (kbchar >= 'A' && kbchar <= 'Z'))
          { //ƴ�����Һ���
            char *candihz;
            if (_global->pyNum < 6)
              _global->pystring[_global->pyNum++] = tolower(kbchar);

            candihz = getCandidateHZbyPY(_global->pinyin, _global->pystring);
            if (candihz)
              strcpy(candiateHZString, candihz);
            else
              memset(candiateHZString, 0, 256);
            _global->hzstring = candiateHZString;

            if (desktop->onKeyPress)
              desktop->onKeyPress(desktop, _global);

            if (candihz)
              free(candihz);
            //_global->hzstring = NULL;
            //TRACE(("%s(%d): ����ģʽ:key = %x, %x, %c, modifiers= %x\n", __FILE__, __LINE__, key, key & 0xFF, key & 0xFF, modifiers));
          }
          else if (_global->pystring[0] != 0)
          { //ƴ��״̬��,����ƴ��,�����ַ���ֱ������
            if (8 == kbchar)
            { //backspce  ����backspace ɾ��ƴ�����²���
              char *candihz;
              if (_global->pyNum > 0)
                _global->pystring[--_global->pyNum] = 0;

              candihz = getCandidateHZbyPY(_global->pinyin, _global->pystring);
              if (candihz)
                strcpy(candiateHZString, candihz);
              else
                memset(candiateHZString, 0, 256);
              _global->hzstring = candiateHZString;
              if (desktop->onKeyPress)
                desktop->onKeyPress(desktop, _global);

              if (candihz)
                free(candihz);
              candihz = NULL;
              //_global->hzstring = NULL;
            }
            else if ((kbchar >= '0' && kbchar <= '9') || ' ' == kbchar)
            { //ѡ����
              char str[3];
              int index = kbchar - '1';

              if (kbchar == ' ')
                index = 0;
              if (kbchar == '0')
                index = 9;

              //TRACE(("ѡ����:%s\n", _global->hzstring));
              if (strlen(_global->hzstring) == 0)
              {
                if (_global->foucsedTextBox->onKeyPress)
                  _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, _global->pystring);
              }
              else
              {
                strncpy(str, _global->hzstring + index * 2, 2);
                str[2] = 0;
                if (_global->foucsedTextBox->onKeyPress)
                  _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
              }

              _global->hzstring = NULL;
              memset(_global->pystring, 0, 7);
              _global->pyNum = 0;
              if (desktop->onKeyPress)
                desktop->onKeyPress(desktop, _global);
            }
            else if ('=' == kbchar)
            { //��һҳ +
              int len = strlen(candiateHZString);
              int offest = _global->hzstring - candiateHZString;
              //TRACE(("��ҳ+1:%s\n", _global->hzstring));
              if (offest + 20 < len)
              {
                _global->hzstring += 20;
                if (desktop->onKeyPress)
                  desktop->onKeyPress(desktop, _global);
                //TRACE(("��ҳ+2:%s\n", _global->hzstring));
              }
            }
            else if ('-' == kbchar)
            { //��һҳ -
              //int len = strlen(candiateHZString);
              int offest = _global->hzstring - candiateHZString;
              //TRACE(("��ҳ-1:%s\n", _global->hzstring));
              if (offest - 20 > 0)
              {
                _global->hzstring -= 20;
                if (desktop->onKeyPress)
                  desktop->onKeyPress(desktop, _global);
                //TRACE(("��ҳ-2:%s\n", _global->hzstring));
              }
            }
            else
            { //
              if (kbchar >= 0x20 || 0x0D == kbchar)
              { //ƴ��״̬��,��ƴ��,�����ַ����ַ�ģʽ����
                char str[2];
                str[1] = 0;
                str[0] = kbchar;
                if (_global->foucsedTextBox->onKeyPress)
                  _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
              }
              else
              {
                if (_global->foucsedTextBox->onKey)
                  _global->foucsedTextBox->onKey(_global->foucsedTextBox, &key);
              }

              // hidePYInput(_global->pyWin);
              //TRACE(("%s(%d): �ַ�ģʽ:key = %x, %x, %c, modifiers= %x\n", __FILE__, __LINE__, key, key & 0xFF, key & 0xFF, modifiers));
            }
          }
          else
          { //ƴ��״̬��,��ƴ��,�ַ�ģʽ����
            if (kbchar >= 0x20 || 0x0D == kbchar)
            {
              char str[2];
              str[1] = 0;
              str[0] = kbchar;
              if (_global->foucsedTextBox->onKeyPress)
                _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
            }
            else
            {
              if (_global->foucsedTextBox->onKey)
                _global->foucsedTextBox->onKey(_global->foucsedTextBox, &key);
            }
          }
        }
        else
        { //�ַ�ģʽ/����ģʽ
          if (kbchar >= 0x20 || kbchar == 0x0D)
          {
            char str[2];
            str[1] = 0;
            str[0] = kbchar;
            if (_global->foucsedTextBox->onKeyPress)
              _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
          }
          else
          {
            if (_global->foucsedTextBox->onKey)
              _global->foucsedTextBox->onKey(_global->foucsedTextBox, &key);
          }
        }
        /*if (kbchar >= 0x20 || kbchar == 0x0D)
        { //�ɴ�ӡ�ַ��ͻس�
          if (_global->InputMode == ENGLISH ||
              _global->foucsedTextBox->wintype == TEXTBOX_PASSWORD)
          { //�ַ�/��������ģʽ, �ַ�ģʽ/����ģʽ/��ǰ��ƴ��
            char str[2];
            str[1] = 0;
            str[0] = kbchar;
            if (_global->foucsedTextBox->onKeyPress)
              _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
            // hidePYInput(_global->pyWin);
            //TRACE(("%s(%d): �ַ�ģʽ:key = %x, %x, %c, modifiers= %x\n", __FILE__, __LINE__, key, key & 0xFF, key & 0xFF, modifiers));
          }
          else
          { //��������
            //ȷ�����뷨����λ��Ϊ��ǰ���λ�ã�����ʾ
            // if(kbchar == 0x0D) //�ո�or�س�
            if ((kbchar >= '0' && kbchar <= '9') || kbchar == ' ')
            { //ѡ����
              char str[3];
              int index = kbchar - '1';

              if (kbchar == ' ')
                index = 0;
              if (kbchar == '0')
                index = 9;

              TRACE(("ѡ����:%s\n", _global->hzstring));
              strncpy(str, _global->hzstring + index * 2, 2);
              // str[0] = *(_global->hzstring + index * 2);
              // str[1] = *(_global->hzstring + index * 2);
              str[2] = 0;
              if (_global->foucsedTextBox->onKeyPress)
                _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);

              _global->hzstring = NULL;
              memset(_global->pystring, 0, 7);
              _global->pyNum = 0;
              if (desktop->onKeyPress)
                desktop->onKeyPress(desktop, _global);
            }
            else if (kbchar == '+')
            {

            }
            else if (isalpha(kbchar))
            {
              char *candihz;
              if (_global->pyNum < 7)
                _global->pystring[_global->pyNum++] = tolower(kbchar);

              candihz = getCandidateHZbyPY(_global->pinyin, _global->pystring);
              if (candihz)
                strcpy(candiateHZString, candihz);
              else
                memset(candiateHZString, 0, 256);
              _global->hzstring = candiateHZString;

              if (desktop->onKeyPress)
                desktop->onKeyPress(desktop, _global);

              if (candihz)
                free(candihz);
              //_global->hzstring = NULL;
              //TRACE(("%s(%d): ����ģʽ:key = %x, %x, %c, modifiers= %x\n", __FILE__, __LINE__, key, key & 0xFF, key & 0xFF, modifiers));
            }
          }
        }
        else
        {
          if (_global->InputMode == ENGLISH || _global->pystring[0] == 0 ||
              _global->foucsedTextBox->wintype == TEXTBOX_PASSWORD)
          { //�ַ�ģʽ/����ģʽ/��ǰ��ƴ�� ɾ��
            if (_global->foucsedTextBox->onKey)
              _global->foucsedTextBox->onKey(_global->foucsedTextBox, &key);
          }
          else
          {
            char *candihz;
            if (key == 0x0E08)
            { //����backspace
              if (_global->pyNum > 0)
                _global->pystring[--_global->pyNum] = 0;

              candihz = getCandidateHZbyPY(_global->pinyin, _global->pystring);
              _global->hzstring = candihz;
              if (desktop->onKeyPress)
                desktop->onKeyPress(desktop, _global);

              if (candihz)
                free(candihz);
              candihz = NULL;
              //_global->hzstring = NULL;
            }
          }
        }*/
      }
    }

    if (_global->isExit)
      break;

    SaveMouseBk(&_global->mouse); //���汳��
    MouseDraw(&_global->mouse);   //��ʾ���
    delay(30);
  }

  destoryGlobalSettting(_global);
  return 0;
}