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
  { //循环
    UpdateMouseStatus(&_global->mouse);
    //CreateMouseEvent(mainPage);

    // UpdateKeyboard();
    // CreateKeyboardEvent();

    // EventHandler();
    delay(30);

    if (kbhit())
    {                   //如果有按键按下，则kbhit()函数返回真
      kbchar = getch(); //使用getch()函数获取按下的键值
      if (kbchar == 'c')
      { //换鼠标
        if (desktop)
          _global->mouse.currentCur = _global->cursor_arrow;
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