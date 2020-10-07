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

  //��ʼ��ϵͳ����
  _global = initGlobalSetting();

  //��ʼ��ͼ�ν���
  initSVGA64k();

  mainPage = CreateDesktop();
  CreateLabel(mainPage, 5, 90, 300, 150, 0, NULL);

  if (mainPage && mainPage->onPaint)
    mainPage->onPaint(mainPage, NULL);

#ifdef DEBUG
  //��ʼ����Ļ����
  clearScreen(0xFF);
  Putbmp64k(0, 0, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linexEx(0, 80, SCR_WIDTH, RGB565(100, 100, 100));
  //��Ļ�������
  hsvgatest();
  //�������
  // printHZKSS16(300 + 16, 150, "���пƼ���ѧУҽԺ���пƼ���ѧУҽԺ", 0x0);
  // printHZKSH16(300 + 16, 150 + 16, "�����пƼ���ѧУҽԺ", 0x0);
  //int x, int y, char *s, int flag, int part, int color
  //printText(300 + 16, 150, "��a���пƼ���ѧУҽԺ���пƼ���ѧУҽԺ���⸾������ͷ����", SIMKAI, 16, 0, 0x0);
  //printText(300 + 16, 150 + 50, "��Aa�пƼ���ѧУҽԺ���пƼ���ѧУҽԺ��ù��", SIMSUN, 24, 0, 0x0);
  //printText(300 + 16, 150 + 100, "����vcc�Ƽ���ѧУҽԺ���пƼ���ѧУҽԺ��ù��", SIMKAI, 32, 0, 0x0);
  //printText(300 + 16, 150 + 140, "���п�hust����ѧУҽԺ���пƼ���ѧУҽԺ��ù��", SIMHEI, 48, 0, 0x0);

#endif

  //��ʼ�����
  ResetMouse(&_global->mouse);

  while (1)
  { //ѭ��
    UpdateMouseStatus(&_global->mouse);
    // CreateMouseEvent();

    // UpdateKeyboard();
    // CreateKeyboardEvent();

    // EventHandler();
    delay(30);

//////////////////// ���Դ���/////////////////////////////////////////
#ifdef DEBUG
    if (_global->mouse.leftClickState == MOUSE_BUTTON_UP)
    {
      // setcolor(RealColor(15));
      RestoreMouseBk(&_global->mouse);
      line(_global->mouse.x, _global->mouse.y, _global->mouse.x - random(1024), _global->mouse.y - random(768), random(65535));
      {
        hfont *_hfont;
        hregion *region;

        unsigned char *c = "��";
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
        //printTextEx(region, "�人��ѧ����ҽԺ������ǻҽԺ\r\na\n��\ra\rf\rA\r�й��Ƽ���ѧ��������ҽԺ��\ra\rB���⸾����θ�۾���ͷ�ȵȿ���\r��\rFa\r��\raѧ,\r\n", _hfont);
        printText(region, "�人��ѧ����ҽԺ������ǻҽԺ\r\na\n��\ra\rf\rA\r�й��Ƽ���ѧ��������ҽԺ��\ra\rB���⸾����θ�۾���ͷ�ȵȿ���\r��\rFa\r��\raѧ,\r\n", _hfont);

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
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
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
      { //������ESC��ո��˳�ʱѭ����ESC���ļ�ֵʱ27

        break;
      }
    }
  }

  destoryGlobalSettting(_global);
  return 0;
}