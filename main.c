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

  //初始化系统参数
  _global = initGlobalSetting();

  //初始化图形界面
  initSVGA64k();

  //初始化桌面
  desktop = CreateDesktop();
  _global->activePage = findWinByID(desktop, ID_HOMEPAGE); //设置缺省活动页面-homepage
  if (desktop && desktop->onPaint)                         //刷新页面
    desktop->onPaint(desktop, NULL);

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

    //键盘事件处理
    if (_bios_keybrd(_KEYBRD_READY))
    {
      key = _bios_keybrd(_KEYBRD_READ);
      kbchar = key & 0xFF;
      /* Determine if shift keys are used */
      modifiers = _bios_keybrd(_KEYBRD_SHIFTSTATUS);

      if (key == 0x6100 && modifiers == 0x224)
      { //退出 左 ctrl + F4
        _global->isExit = TRUE;
      }
      else if (key == 0x5E00 && modifiers == 0x224)
      { //切换输入法 左 ctrl + F1
        _global->InputMode = !_global->InputMode;
      }
      else if (_global->foucsedTextBox)
      { //有激活的textbox
        if (kbchar >= 0x20 || kbchar == 0x0D)
        { //可打印字符和回车
          if (_global->InputMode == ENGLISH ||
              _global->foucsedTextBox->wintype == TEXTBOX_PASSWORD)
          { //字符/密码输入模式
            char str[2];
            str[1] = 0;
            str[0] = kbchar;
            _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
          }
          else
          { //中文输入
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
    {                   //如果有按键按下，则kbhit()函数返回真
      kbchar = getch(); //使用getch()函数获取按下的键值
      //kbchar = _bios_keybrd(_KEYBRD_READ);
      if (_global->foucsedTextBox && _global->foucsedTextBox->onKeyPress)
      {
        char str[2];
        str[1] = 0;
        str[0] = kbchar;
        _global->foucsedTextBox->onKeyPress(_global->foucsedTextBox, str);
      }

      if (kbchar == 'c')
      { //换鼠标
      }
      else if (kbchar == 'r')
      {
        if (desktop)
          desktop->onPaint(desktop, NULL);
      }
      else if (kbchar == ' ' || kbchar == 27)
      { //当按下ESC或空格退出时循环,ESC键的键值时27
        break;
      }
      TRACE(("press key:%c,%u\n", kbchar, kbchar));
    }
    */

    if (_global->isExit)
      break;

    SaveMouseBk(&_global->mouse); //保存背景
    MouseDraw(&_global->mouse);   //显示鼠标
    delay(30);
  }

  destoryGlobalSettting(_global);
  return 0;
}