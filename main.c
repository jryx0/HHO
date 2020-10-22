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

  //初始化系统参数
  _global = initGlobalSetting();

  //初始化图形界面
  initSVGA64k();

  //初始化桌面
  desktop = CreateDesktop();

  _global->activePage = findWinByID(desktop, ID_HOMEPAGE); //设置缺省活动页面-homepage
  //_global->pyWin = findWinByID(desktop, ID_PINYIN);
  if (desktop && desktop->onPaint) //刷新页面
  {
    desktop->onPaint(desktop, NULL);
    //desktop->onPaint(desktop, &_global->theme);
  }

  //初始化鼠标
  ResetMouse(&_global->mouse);

  while (1) //主循环
  {
    //隐藏鼠标
    RestoreMouseBk(&_global->mouse);

    //鼠标事件处理
    UpdateMouseStatus(&_global->mouse);
    child = checkmousewin(desktop, &_global->mouse);
    if (child)
      if (child->winID < ID_DESKTOP_MAX)
      { //点击到desktop， desktop有页面、控件。约定控件ID值<ID_DESKTOP_MAX。
        //点击ID<ID_DESKTOP_MAX的控件由desktop处理
        //如：菜单，切换页面、登录等等
        if (desktop->EventHandler)
          desktop->EventHandler(child, EVENT_MOUSE, _global);
      }
      else
      { //点击到页面区域,由页面处理事件
        if (child->EventHandler)
          child->EventHandler(child, EVENT_MOUSE, _global);
      }

    //光标处理
    if (_global->foucsedTextBox)
      DrawTextCursor(_global->foucsedTextBox, blink++);
    else
      blink = 0;

    //键盘事件处理
    if (_bios_keybrd(_KEYBRD_READY))
    {
      key = _bios_keybrd(_KEYBRD_READ);
      kbchar = key & 0xFF;
      /* Determine if shift keys are used */
      modifiers = _bios_keybrd(_KEYBRD_SHIFTSTATUS);

      //TRACE(("key = %x, %x, '%c', modifiers= %x\n", key, key & 0xFF, key & 0xFF, modifiers));

      if (0x6100 == key) //&& modifiers == 0x224)
      {                  //退出  ctrl + F4
        _global->isExit = TRUE;
      }
      else if (0x5f00 == key)
      { // ctrl + F2 切换风格
        _global->theme++;
        if (_global->theme >= 4)
          _global->theme = 1;

        desktop->onTheme(desktop, &_global->theme);
        desktop->onPaint(desktop, NULL);
      }
      else if (0x5E00 == key) //&& modifiers == 0x224)
      {                       //切换输入法  ctrl + F1
        _global->InputMode = !_global->InputMode;
        desktop->onPaint(desktop, _global);
        if (_global->InputMode == ENGLISH)
          closePY(desktop, _global);
      }
      else if (_global->foucsedTextBox)
      { //有激活的textbox
        if (CHINESE == _global->InputMode && _global->foucsedTextBox->wintype != TEXTBOX_PASSWORD)
        { //中文
          //TRACE(("key = %x, %x, '%c', modifiers= %x\n", key, key & 0xFF, key & 0xFF, modifiers));
          if ((kbchar >= 'a' && kbchar <= 'z') || (kbchar >= 'A' && kbchar <= 'Z'))
          { //拼音查找汉字
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
            //TRACE(("%s(%d): 汉字模式:key = %x, %x, %c, modifiers= %x\n", __FILE__, __LINE__, key, key & 0xFF, key & 0xFF, modifiers));
          }
          else if (_global->pystring[0] != 0)
          { //拼音状态下,若无拼音,其他字符可直接输入
            if (8 == kbchar)
            { //backspce  中文backspace 删除拼音重新查找
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
            { //选择汉字
              char str[3];
              int index = kbchar - '1';

              if (kbchar == ' ')
                index = 0;
              if (kbchar == '0')
                index = 9;

              //TRACE(("选择汉字:%s\n", _global->hzstring));
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
            { //下一页 +
              int len = strlen(candiateHZString);
              int offest = _global->hzstring - candiateHZString;
              //TRACE(("翻页+1:%s\n", _global->hzstring));
              if (offest + 20 < len)
              {
                _global->hzstring += 20;
                if (desktop->onKeyPress)
                  desktop->onKeyPress(desktop, _global);
                //TRACE(("翻页+2:%s\n", _global->hzstring));
              }
            }
            else if ('-' == kbchar)
            { //上一页 -
              //int len = strlen(candiateHZString);
              int offest = _global->hzstring - candiateHZString;
              //TRACE(("翻页-1:%s\n", _global->hzstring));
              if (offest - 20 > 0)
              {
                _global->hzstring -= 20;
                if (desktop->onKeyPress)
                  desktop->onKeyPress(desktop, _global);
                //TRACE(("翻页-2:%s\n", _global->hzstring));
              }
            }
            else
            { //
              if (kbchar >= 0x20 || 0x0D == kbchar)
              { //拼音状态下,有拼音,其他字符由字符模式处理
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
              //TRACE(("%s(%d): 字符模式:key = %x, %x, %c, modifiers= %x\n", __FILE__, __LINE__, key, key & 0xFF, key & 0xFF, modifiers));
            }
          }
          else
          { //拼音状态下,无拼音,字符模式处理
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
        { //字符模式/密码模式
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
        { //可打印字符和回车
          if (_global->InputMode == ENGLISH ||
              _global->foucsedTextBox->wintype == TEXTBOX_PASSWORD)
          { //字符/密码输入模式, 字符模式/密码模式/当前无拼音
            char str[2];
            str[1] = 0;
            str[0] = kbchar;
            if (_global->foucsedTextBox->onKeyPress)
              _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
            // hidePYInput(_global->pyWin);
            //TRACE(("%s(%d): 字符模式:key = %x, %x, %c, modifiers= %x\n", __FILE__, __LINE__, key, key & 0xFF, key & 0xFF, modifiers));
          }
          else
          { //中文输入
            //确定输入法窗口位置为当前光标位置，并显示
            // if(kbchar == 0x0D) //空格or回车
            if ((kbchar >= '0' && kbchar <= '9') || kbchar == ' ')
            { //选择汉字
              char str[3];
              int index = kbchar - '1';

              if (kbchar == ' ')
                index = 0;
              if (kbchar == '0')
                index = 9;

              TRACE(("选择汉字:%s\n", _global->hzstring));
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
              //TRACE(("%s(%d): 汉字模式:key = %x, %x, %c, modifiers= %x\n", __FILE__, __LINE__, key, key & 0xFF, key & 0xFF, modifiers));
            }
          }
        }
        else
        {
          if (_global->InputMode == ENGLISH || _global->pystring[0] == 0 ||
              _global->foucsedTextBox->wintype == TEXTBOX_PASSWORD)
          { //字符模式/密码模式/当前无拼音 删除
            if (_global->foucsedTextBox->onKey)
              _global->foucsedTextBox->onKey(_global->foucsedTextBox, &key);
          }
          else
          {
            char *candihz;
            if (key == 0x0E08)
            { //中文backspace
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

    SaveMouseBk(&_global->mouse); //保存背景
    MouseDraw(&_global->mouse);   //显示鼠标
    delay(30);
  }

  destoryGlobalSettting(_global);
  return 0;
}