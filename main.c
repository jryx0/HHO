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

  //初始化系统参数
  _global = initGlobalSetting();

  //初始化图形界面
  initSVGA64k();

  //初始化屏幕背景
  clearScreen(0xFF);

#ifdef DEBUG
  //屏幕缓存测试
  hsvgatest();

  //printHZ(_global->fphanzi_sh16, 700, 50, "华中科技大学校医院", 0x5F40, 16);
  // printtextxy(_global->fphanzi_sh16, 700, 50, "华中科技大学校医院", 0x5F40, 16);
  // printtextxy(_global->fphanzi_ss16, 700, 70, "华中科技大学校医院", 0x5F40, 16);
  // printtextxy(_global->fphanzi_sh24, 700, 90, "华中科技大学校医院", 0x5F40, 16);
  // printtextxy(_global->fphanzi_ss24, 700, 110, "华中科技大学校医院", 0x5F40, 16);
#endif

  //初始化鼠标
  ResetMouse(&_global->mouse);

  while (1)
  { //循环
    UpdateMouseStatus(&_global->mouse);
    // CreateMouseEvent();

    // UpdateKeyboard();
    // CreateKeyboardEvent();

    // EventHandler();
    delay(30);

//////////////////// 测试代码/////////////////////////////////////////
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
    {                   //如果有按键按下，则kbhit()函数返回真
      kbchar = getch(); //使用getch()函数获取按下的键值
      if (kbchar == 'c')
      {
        // setcolor(RealColor(15));
        RestoreMouseBk(&_global->mouse);
        line(_global->mouse.x, _global->mouse.y, _global->mouse.x + random(1024), _global->mouse.y + random(768), random(65535));
        SaveMouseBk(&_global->mouse);
      }
      else if (kbchar == 'r')
      {
        RestoreMouseBk(&_global->mouse);
        bar(_global->mouse.x, _global->mouse.y, _global->mouse.x + random(1024), _global->mouse.y + random(768), random(65535));
        SaveMouseBk(&_global->mouse);
      }
      else if (kbchar == 'a')
      {
      }
      else if (kbchar == ' ' || kbchar == 27)
      {

        break;
      } //当按下ESC或空格退出时循环，ESC键的键值时27.
    }
#endif
  }

  destoryGlobalSettting(_global);
  return 0;
}