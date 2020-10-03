#ifndef __HGLOBAL_H__
#define __HGLOBAL_H__
#include "macrodef.h"
#include "hhosvga.h"
#include "mouse.h"
#include "pinyin.h"
#include "HBaseWin.h"

//ȫ�ֱ����ṹ
typedef struct _globaldef
{  
  FILE *fphanzi_st24; //��������ֿ��ļ�ָ��   
  FILE *fphanzi_ht24; //��������ֿ��ļ�ָ��

  FILE *fpBK;

  unsigned char *cursor_arrow; //�������ͼ�ε�ַ
  unsigned char *cursor_hand;  //�������ͼ�ε�ַ
  unsigned int *cursorBK;      //��걳����ַ

  mousestatus mouse;
  pyInput *pingyin;
  hbasewinAttr *foucswin;
} globaldef;

globaldef *initGlobalSetting();
void destoryGlobalSettting(globaldef *_g);
globaldef *loadSvgaResouce(globaldef *_g);
void loadMouse(globaldef *_g);
#endif