#ifndef __HGLOBAL_H__
#define __HGLOBAL_H__

#include "macrodef.h"
#include "mouse.h"
#include "pinyin.h"
#include "HBaseWin.h"

#include <stdio.h>

//--------------------------------------------------------------------

//全局变量结构
typedef struct _globaldef
{
  FILE *fphanzi_ss24; //宋体点阵字库文件指针
  FILE *fphanzi_sh24; //黑体点阵字库文件指针
  FILE *fphanzi_ss16; //宋体点阵字库文件指针
  FILE *fphanzi_sh16; //黑体点阵字库文件指针

  FILE *fpBK; //背景存放的文件

  unsigned char cursor_arrow[MOUSE_WIDTH][MOUSE_HEIGHT]; //保存鼠标图形地址
  unsigned char cursor_hand[MOUSE_WIDTH][MOUSE_HEIGHT];  //保存鼠标图形地址
  unsigned int cursorBK[MOUSE_WIDTH][MOUSE_HEIGHT];      //鼠标背景地址

  mousestatus mouse;
  pyInput *pingyin;

  hbasewinAttr *foucswin;
} globaldef;

globaldef *initGlobalSetting(void);
void destoryGlobalSettting(globaldef *_g);
globaldef *loadSvgaResouce(globaldef *_g);
void loadMouse(globaldef *_g);

#endif