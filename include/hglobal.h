#ifndef __HGLOBAL_H__
#define __HGLOBAL_H__

#include "macrodef.h"
#include "mouse.h"
#include "pinyin.h"
#include "HBaseWin.h"

#include <stdio.h>

//--------------------------------------------------------------------
 
//全局变量结构
//Dos 打开文件数量<15
typedef struct _globaldef
{
  FILE *fpBK;  //背景存放的文件
  FILE *fpLog; //日志文件指针

  unsigned char cursor_arrow[MOUSE_WIDTH][MOUSE_HEIGHT]; //保存鼠标图形地址-箭头
  unsigned char cursor_hand[MOUSE_WIDTH][MOUSE_HEIGHT];  //保存鼠标图形地址-手
  mousestatus mouse;

  pyInput *pingyin;
  hbasewinAttr *foucswin;
} globaldef;

globaldef *initGlobalSetting(void);
void destoryGlobalSettting(globaldef *_g);
globaldef *loadSvgaResouce(globaldef *_g);
void loadMouse(globaldef *_g);

/**
 * 获取汉字字库文件
 * @param type 在marcodef中定义 
 * @param size 字号 16、24、32、48
 * 
 * @return 字库文件指针
 */
FILE *getFontFile(int type, int size);

#endif