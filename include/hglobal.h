#ifndef __HGLOBAL_H__
#define __HGLOBAL_H__
#include "macrodef.h"
#include "hhosvga.h"
#include "mouse.h"
#include "pinyin.h"
#include "HBaseWin.h"

//全局变量结构
typedef struct _globaldef
{  
  FILE *fphanzi_st24; //宋体点阵字库文件指针   
  FILE *fphanzi_ht24; //黑体点阵字库文件指针

  FILE *fpBK;

  unsigned char *cursor_arrow; //保存鼠标图形地址
  unsigned char *cursor_hand;  //保存鼠标图形地址
  unsigned int *cursorBK;      //鼠标背景地址

  mousestatus mouse;
  pyInput *pingyin;
  hbasewinAttr *foucswin;
} globaldef;

globaldef *initGlobalSetting();
void destoryGlobalSettting(globaldef *_g);
globaldef *loadSvgaResouce(globaldef *_g);
void loadMouse(globaldef *_g);
#endif