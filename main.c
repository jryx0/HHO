#include "macrodef.h"
#include "hhosvga.h"
#include "hglobal.h"
#include "mouse.h"

#include "HBaseWin.h"
#include "wdesktop.h"
#include "hlabel.h"

#include <conio.h>
#include <stdlib.h>
#include <mem.h>

//#define DEBUG

int main(void)
{
  char kbchar = 0;
  globaldef *_global;
  hbasewinAttr *mainPage;

  //初始化系统参数
  _global = initGlobalSetting();

  //初始化图形界面
  initSVGA64k();

  mainPage = CreateDesktop();
  CreateLabel(mainPage, 15, 90, 300, 150, 0, "readme");
  CreateLabel(mainPage, 15 + 350, 90, 300, 150, 1, "c:\\hho\\data\\news\\1.txt");

  if (mainPage && mainPage->onPaint)
    mainPage->onPaint(mainPage, NULL);

#ifdef DEBUG ////////////////////SVGA测试代码/////////////////////////////////////////
  //初始化屏幕背景
  clearScreen(0xFF);
  Putbmp64k(0, 0, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linexEx(0, 80, SCR_WIDTH, RGB565(100, 100, 100));
  //屏幕缓存测试
  hsvgatest();

#endif

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
      { //窗口删除测试
        if (mainPage && mainPage->onDestroy)
        { //删除全部窗口，包括自身
          mainPage->onDestroy(mainPage, NULL);
          free(mainPage);
          mainPage = NULL;
        }
      }
      else if (kbchar == 'd')
      {
        if (mainPage && mainPage->onDestroy)
        { //删除一个窗口
          int winid = 1;
          mainPage->onDestroy(mainPage, &winid);
          if(mainPage ->onPaint)
            mainPage ->onPaint(mainPage, NULL);
        }
      }
      else if (kbchar == 'r')
      {
        RestoreMouseBk(&_global->mouse);
        bar(_global->mouse.x, _global->mouse.y, _global->mouse.x + random(1024), _global->mouse.y + random(768), random(65535));
        SaveMouseBk(&_global->mouse);
      }
      else if (kbchar == 'a')
      { //窗口创建测试
        if (!mainPage)
        { //mainpage 为空创建
          mainPage = CreateDesktop();
          CreateLabel(mainPage, 15, 90, 300, 150, 0, "readme");
          CreateLabel(mainPage, 15 + 350, 90, 300, 150, 1, "c:\\hho\\data\\news\\1.txt");

          if (mainPage && mainPage->onPaint)
            mainPage->onPaint(mainPage, NULL);
        }
      }
      else if (kbchar == ' ' || kbchar == 27)
      { //当按下ESC或空格退出时循环，ESC键的键值时27

        break;
      }
    }
#ifdef DEBUG //////////////////// 鼠标 测试代码/////////////////////////////////////////
    if (_global->mouse.leftClickState == MOUSE_BUTTON_UP)
    {
      RestoreMouseBk(&_global->mouse);
      line(_global->mouse.x, _global->mouse.y, _global->mouse.x - random(1024), _global->mouse.y - random(768), random(65535));
      SaveMouseBk(&_global->mouse);

      //字体测试
      {
        hfont *_hfont;
        hregion *region;
        FILE *fptext;

        _hfont = getFont(SIMKAI, 16, 0x55);

        region = malloc(sizeof(hregion));
        region->left_top.x = _global->mouse.x + 30;
        region->left_top.y = _global->mouse.y;
        region->right_bottom.x = region->left_top.x + 150;
        region->right_bottom.y = region->left_top.y + 150;
        clearRegion(region->left_top.x, region->left_top.y,
                    region->left_top.x + 150, region->left_top.y + 155, 0xff);

        rectangleEx(region->left_top.x, region->left_top.y, 150, 155, 0x0, 1, 1);

        //printTextEx(region, "武汉大学人民医院附属口腔医院\r\na\n中\ra\rf\rA\r中国科技大学附属人民医院科\ra\rB内外妇儿肠胃眼睛骨头等等科室\r技\rFa\r大\ra学,\r\n", _hfont);
        fptext = fopen("c:\\hho\\readme", "r");
        printTextFile(region, fptext, _hfont);

        //region->left_top.y -= 32;
        //printTextLine(region, "\n华中科hust技大学校医院华中科技大学校医院青霉素", _hfont);

        fclose(fptext);
        free(region);
        //free(_hfont);
        freeFont(_hfont);
      }
    }

    if (_global->mouse.rightClickState == MOUSE_BUTTON_UP)
    {
      break;
    }
#endif
  }

  destoryGlobalSettting(_global);
  return 0;
}