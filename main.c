#include "macrodef.h"
#include "hglobal.h"
#include "hhosvga.h"
#include "mouse.h"
#include "HBaseWin.h"
#include "wdesktop.h"
#include "hlabel.h"

#include <conio.h>
#include <stdlib.h>
#include <mem.h>

#define DEBUG

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
  CreateLabel(mainPage, 5, 90, 300, 150, 0, NULL);

  if (mainPage && mainPage->onPaint)
    mainPage->onPaint(mainPage, NULL);

#ifdef DEBUG
  //初始化屏幕背景
  clearScreen(0xFF);
  Putbmp64k(0, 0, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linexEx(0, 80, SCR_WIDTH, RGB565(100, 100, 100));
  //屏幕缓存测试
  hsvgatest();
  //字体测试
  // printHZKSS16(300 + 16, 150, "华中科技大学校医院华中科技大学校医院", 0x0);
  // printHZKSH16(300 + 16, 150 + 16, "啊华中科技大学校医院", 0x0);
  //int x, int y, char *s, int flag, int part, int color
  //printText(300 + 16, 150, "啊a华中科技大学校医院华中科技大学校医院内外妇儿精神头脑著", SIMKAI, 16, 0, 0x0);
  //printText(300 + 16, 150 + 50, "华Aa中科技大学校医院华中科技大学校医院青霉素", SIMSUN, 24, 0, 0x0);
  //printText(300 + 16, 150 + 100, "华中vcc科技大学校医院华中科技大学校医院青霉素", SIMKAI, 32, 0, 0x0);
  //printText(300 + 16, 150 + 140, "华中科hust技大学校医院华中科技大学校医院青霉素", SIMHEI, 48, 0, 0x0);

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
      {
        hfont *_hfont;
        hregion *region;

        unsigned char *c = "我";
        memset(_hfont, 0, sizeof(hfont));
        //chinese setting
        _hfont->currentFontSize = 16;
        _hfont->byteperline = (_hfont->currentFontSize + 7) / 8;
        _hfont->currentFontType = SIMSUN;
        _hfont->totalbytes = _hfont->byteperline * _hfont->currentFontSize;
        _hfont->fpCurrentFont = getFontFile(_hfont->currentFontType, _hfont->currentFontSize);
        _hfont->xgap = 0;
        _hfont->ygap = 3;
        _hfont->fontcolor = 0x0;

        //ACS setting
        _hfont->fpASC = getFontFile(ASCII, 16);
        _hfont->ascScaley = 1;
        _hfont->ascScalex = 1;
        _hfont->ascy = 0;
        _hfont->ascSize = 8;

        calcFontSetting(_hfont);
        // printHZWord(_global->mouse.x + 16, _global->mouse.y + 16, c, _hfont);
        // printASC(_global->mouse.x + 64, _global->mouse.y + 16, 'c', _hfont);

        region = malloc(sizeof(hregion));
        region->left_top.x = _global->mouse.x + 16;
        region->left_top.y = _global->mouse.y;
        region->right_bottom.x = region->left_top.x + 150;
        region->right_bottom.y = region->left_top.y + 150;
        clearRegion(region->left_top.x, region->left_top.y,
                    region->left_top.x + 150, region->left_top.y + 155, 0xff);

        rectangleEx(region->left_top.x, region->left_top.y, 150, 155, 0x0, 1, 1);
        //printTextEx(region, "武汉大学人民医院附属口腔医院\r\na\n中\ra\rf\rA\r中国科技大学附属人民医院科\ra\rB内外妇儿肠胃眼睛骨头等等科室\r技\rFa\r大\ra学,\r\n", _hfont);
        printText(region, "武汉大学人民医院附属口腔医院\r\na\n中\ra\rf\rA\r中国科技大学附属人民医院科\ra\rB内外妇儿肠胃眼睛骨头等等科室\r技\rFa\r大\ra学,\r\n", _hfont);

        free(c);
        free(region);
        fclose(_hfont->fpCurrentFont);
        fclose(_hfont->fpASC);
        free(_hfont);
      }
      SaveMouseBk(&_global->mouse);
    }

    if (_global->mouse.rightClickState == MOUSE_BUTTON_UP)
    {
      break;
    }
#endif

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