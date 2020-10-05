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

  //字体测试
  // printHZKSS16(300 + 16, 150, "华中科技大学校医院华中科技大学校医院", 0x0);
  // printHZKSH16(300 + 16, 150 + 16, "啊华中科技大学校医院", 0x0);
  //int x, int y, char *s, int flag, int part, int color
  printText(300 + 16, 150, "啊a华中科技大学校医院华中科技大学校医院内外妇儿精神头脑著", SIMKAI, 16, 0, 0x0);
  printText(300 + 16, 150 + 50, "华Aa中科技大学校医院华中科技大学校医院青霉素", SIMSUN, 24, 0, 0x0);
  printText(300 + 16, 150 + 100, "华中vcc科技大学校医院华中科技大学校医院青霉素", SIMKAI, 32, 0, 0x0);
  printText(300 + 16, 150 + 140, "华中科hust技大学校医院华中科技大学校医院青霉素", SIMHEI, 48, 0, 0x0);
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
      }
      else if (kbchar == 'r')
      {
        RestoreMouseBk(&_global->mouse);
        bar(_global->mouse.x, _global->mouse.y, _global->mouse.x + random(1024), _global->mouse.y + random(768), random(65535));
        SaveMouseBk(&_global->mouse);
      }
      else if (kbchar == 'a')
      {
        //printHZKSS24(_global->mouse.x + 24, _global->mouse.y, "啊华中科技大学校医院青霉素眼科内外妇儿精神感冒", 0x0);
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