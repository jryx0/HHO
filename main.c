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

  //��ʼ��ϵͳ����
  _global = initGlobalSetting();

  //��ʼ��ͼ�ν���
  initSVGA64k();

  mainPage = CreateDesktop();
  CreateLabel(mainPage, 15, 90, 300, 150, 0, "readme");
  CreateLabel(mainPage, 15 + 350, 90, 300, 150, 1, "c:\\hho\\data\\news\\1.txt");

  if (mainPage && mainPage->onPaint)
    mainPage->onPaint(mainPage, NULL);

#ifdef DEBUG ////////////////////SVGA���Դ���/////////////////////////////////////////
  //��ʼ����Ļ����
  clearScreen(0xFF);
  Putbmp64k(0, 0, "c:\\hho\\data\\bmp\\hhologo.bmp");
  linexEx(0, 80, SCR_WIDTH, RGB565(100, 100, 100));
  //��Ļ�������
  hsvgatest();

#endif

  //��ʼ�����
  ResetMouse(&_global->mouse);

  while (1)
  { //ѭ��
    UpdateMouseStatus(&_global->mouse);
    //CreateMouseEvent(mainPage);

    // UpdateKeyboard();
    // CreateKeyboardEvent();

    // EventHandler();
    delay(30);

    if (kbhit())
    {                   //����а������£���kbhit()����������
      kbchar = getch(); //ʹ��getch()������ȡ���µļ�ֵ
      if (kbchar == 'c')
      { //����ɾ������
        if (mainPage && mainPage->onDestroy)
        { //ɾ��ȫ�����ڣ���������
          mainPage->onDestroy(mainPage, NULL);
          free(mainPage);
          mainPage = NULL;
        }
      }
      else if (kbchar == 'd')
      {
        if (mainPage && mainPage->onDestroy)
        { //ɾ��һ������
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
      { //���ڴ�������
        if (!mainPage)
        { //mainpage Ϊ�մ���
          mainPage = CreateDesktop();
          CreateLabel(mainPage, 15, 90, 300, 150, 0, "readme");
          CreateLabel(mainPage, 15 + 350, 90, 300, 150, 1, "c:\\hho\\data\\news\\1.txt");

          if (mainPage && mainPage->onPaint)
            mainPage->onPaint(mainPage, NULL);
        }
      }
      else if (kbchar == ' ' || kbchar == 27)
      { //������ESC��ո��˳�ʱѭ����ESC���ļ�ֵʱ27

        break;
      }
    }
#ifdef DEBUG //////////////////// ��� ���Դ���/////////////////////////////////////////
    if (_global->mouse.leftClickState == MOUSE_BUTTON_UP)
    {
      RestoreMouseBk(&_global->mouse);
      line(_global->mouse.x, _global->mouse.y, _global->mouse.x - random(1024), _global->mouse.y - random(768), random(65535));
      SaveMouseBk(&_global->mouse);

      //�������
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

        //printTextEx(region, "�人��ѧ����ҽԺ������ǻҽԺ\r\na\n��\ra\rf\rA\r�й��Ƽ���ѧ��������ҽԺ��\ra\rB���⸾����θ�۾���ͷ�ȵȿ���\r��\rFa\r��\raѧ,\r\n", _hfont);
        fptext = fopen("c:\\hho\\readme", "r");
        printTextFile(region, fptext, _hfont);

        //region->left_top.y -= 32;
        //printTextLine(region, "\n���п�hust����ѧУҽԺ���пƼ���ѧУҽԺ��ù��", _hfont);

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