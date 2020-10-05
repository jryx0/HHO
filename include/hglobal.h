#ifndef __HGLOBAL_H__
#define __HGLOBAL_H__

#include "mouse.h"
#include "pinyin.h"
#include "HBaseWin.h"

#include <stdio.h>

//////////////////////////////////////�궨��///////////////////////////////////
//��ʾģʽ�궨��
#define SVGA64K
#define SVGA1024x768

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768

// #define SVGA32K
// #define SVGA800x600

//-------------------------ȫ���ļ�·��--------------------------------
//�����ļ��ж���
#if !defined(__BORLANDC__)
#define DATAPATH "data\\"
#else
#define DATAPATH "C:\\HHO\\data\\"
#endif

#define USERINFOFILE DATAPATH "userinfo.txt"
#define DOCTORINFOFILE DATAPATH "doctorinfo.txt"

//pinyin files
#define FILE_PYINDEX DATAPATH "pinyin\\index.txt"
#define FILE_HANZI DATAPATH "pinyin\\hanzi.txt"

//�����ļ�
#define FONTSIZE16 32                         //һ������32�ֽ�
#define FILE_SIMSUN16 DATAPATH "font\\HZK16S" //����16�����ֿ�
#define FILE_SIMHEI16 DATAPATH "font\\HZK16H" //����16�����ֿ�

#define FONTSIZE24 72                         //һ������72�ֽ�
#define FILE_SIMSUN24 DATAPATH "font\\HZK24S" //����24�����ֿ�
#define FILE_SIMHEI24 DATAPATH "font\\HZK24H" //����24�����ֿ�

//mouse cursors
//����С
#define MOUSE_WIDTH 16
#define MOUSE_HEIGHT 16
#define FILE_CURSOR_ARROW DATAPATH "cursor\\arrows.cur"
#define FILE_CURSOR_HAND DATAPATH "cursor\\hand.cur"

#define MOUSE_ENTERED 1
#define MOUSE_LEFT 2
#define MOUSE_OUTSIDE 4
#define MOUSE_INSIDE 8
#define MOUSE_BUTTON_DOWN 16
#define MOUSE_BUTTON_UP 32
#define MOUSE_BUTTON_STILL_DOWN 64
#define MOUSE_BUTTON_STILL_UP 128
//--------------------------------------------------------------------

//ȫ�ֱ����ṹ
typedef struct _globaldef
{
  FILE *fphanzi_ss24; //��������ֿ��ļ�ָ��
  FILE *fphanzi_sh24; //��������ֿ��ļ�ָ��
  FILE *fphanzi_ss16; //��������ֿ��ļ�ָ��
  FILE *fphanzi_sh16; //��������ֿ��ļ�ָ��

  FILE *fpBK; //������ŵ��ļ�

  unsigned char cursor_arrow[MOUSE_WIDTH][MOUSE_HEIGHT]; //�������ͼ�ε�ַ
  unsigned char cursor_hand[MOUSE_WIDTH][MOUSE_HEIGHT];  //�������ͼ�ε�ַ
  unsigned int cursorBK[MOUSE_WIDTH][MOUSE_HEIGHT];      //��걳����ַ

  mousestatus mouse;
  pyInput *pingyin;

  hbasewinAttr *foucswin;
} globaldef;

globaldef *initGlobalSetting(void);
void destoryGlobalSettting(globaldef *_g);
globaldef *loadSvgaResouce(globaldef *_g);
void loadMouse(globaldef *_g);

//����borlandc��vscode�еļ�����
#ifndef __BORLANDC__
#define huge
#define far

#ifndef _REG_DEFS
#define _REG_DEFS
struct WORDREGS
{
  unsigned int ax, bx, cx, dx, si, di, cflag, flags;
};

struct BYTEREGS
{
  unsigned char al, ah, bl, bh, cl, ch, dl, dh;
};

union REGS
{
  struct WORDREGS x;
  struct BYTEREGS h;
};
#endif //_REG_DEFS
#endif

#endif