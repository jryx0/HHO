#include "macrodef.h"
#include "hhosvga.h"
#include "mouse.h"
#include "hglobal.h"

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
  int size;

  //初始化系统参数
  _global = initGlobalSetting();

  //初始化图形界面
  initSVGA64k();

  desktop = CreateDesktop();

  if (desktop && desktop->onPaint)
    desktop->onPaint(desktop, NULL);

  //初始化鼠标
  ResetMouse(&_global->mouse);

  while (1)
  {                                  //循环
    RestoreMouseBk(&_global->mouse); //隐藏鼠标

    UpdateMouseStatus(&_global->mouse);
    child = checkmousewin(desktop, &_global->mouse);
    if (child)
      if (child->EventHandler)
        child->EventHandler(child, EVENT_MOUSE, _global);
      else if (desktop->EventHandler)
        desktop->EventHandler(desktop, EVENT_MOUSE, _global);

    SaveMouseBk(&_global->mouse); //保存背景
    MouseDraw(&_global->mouse);   //显示鼠标

    // UpdateKeyboard();
    // CreateKeyboardEvent();

    // EventHandler();
    delay(30);

    if (kbhit())
    {                   //如果有按键按下，则kbhit()函数返回真
      kbchar = getch(); //使用getch()函数获取按下的键值
      if (kbchar == 'c')
      { //换鼠标
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
      else if (kbchar == 'd')
      {
        if (desktop)
        {
          if (desktop && desktop->onDestroy)
          {
            char t[20];
            hfont *_hfont = getFont(SIMKAI, 16, 0x0000);
            int winID = ID_HOMEPAGE;

            RestoreMouseBk(&_global->mouse);
            desktop->onDestroy(desktop, &winID);
            SaveMouseBk(&_global->mouse);

            // sprintf(t, "len = %d", desktop->children->len);
            // printTextLineXY(100, 100, t, _hfont);
          }
        }
      }
      else if (kbchar == ' ' || kbchar == 27)
      { //当按下ESC或空格退出时循环，ESC键的键值时27

        break;
      }
    }
  }

  destoryGlobalSettting(_global);
  return 0;
}