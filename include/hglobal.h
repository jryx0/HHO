#ifndef __HGLOBAL_H__
#define __HGLOBAL_H__

#include "hhosvga.h"
#include "mouse.h"
#include "pinyin.h"
#include "HBaseWin.h"

#include <stdio.h>

//////////////////////////////////////宏定义///////////////////////////////////
//显示模式宏定义
#define SVGA64K
#define SVGA1024x768
// #define SVGA32K
// #define SVGA800x600

//-------------------------全局文件路径--------------------------------
//数据文件夹定义
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

//字体文件
#define FONTSIZE16 32                         //一个汉字32字节
#define FILE_SIMSUN16 DATAPATH "font\\HZK16S" //宋体16点阵字库
#define FILE_SIMHEI16 DATAPATH "font\\HZK16H" //黑体16点阵字库

#define FONTSIZE24 72                         //一个汉字72字节
#define FILE_SIMSUN24 DATAPATH "font\\HZK24S" //宋体24点阵字库
#define FILE_SIMHEI24 DATAPATH "font\\HZK24H" //黑体24点阵字库

//mouse cursors
//鼠标大小
#define MOUSE_WIDHT 16
#define MOUSE_HEIGHT 16
#define FILE_CURSOR_ARROW DATAPATH "cursor\\arrows.cur"
#define FILE_CURSOR_HAND DATAPATH "cursor\\hand.cur"
//--------------------------------------------------------------------

//全局变量结构
typedef struct _globaldef
{
  FILE *fphanzi_ss24; //宋体点阵字库文件指针
  FILE *fphanzi_sh24; //黑体点阵字库文件指针
  FILE *fphanzi_ss16; //宋体点阵字库文件指针
  FILE *fphanzi_sh16; //黑体点阵字库文件指针

  FILE *fpBK; //背景存放的文件

  unsigned char cursor_arrow[MOUSE_WIDHT][MOUSE_HEIGHT]; //保存鼠标图形地址
  unsigned char cursor_hand[MOUSE_WIDHT][MOUSE_HEIGHT];  //保存鼠标图形地址
  unsigned int cursorBK[MOUSE_WIDHT][MOUSE_HEIGHT];      //鼠标背景地址

  mousestatus mouse;
  pyInput *pingyin;

  hbasewinAttr *foucswin;
} globaldef;

globaldef *initGlobalSetting(void);
void destoryGlobalSettting(globaldef *_g);
globaldef *loadSvgaResouce(globaldef *_g);
void loadMouse(globaldef *_g);

//处理borlandc在vscode中的兼容性
#ifndef __BORLANDC__
#define huge
#define far
#endif

#endif