#ifndef __HGLOBAL_H__
#define __HGLOBAL_H__

#include "macrodef.h"
#include "mouse.h"
#include "pinyin.h"
#include "HBaseWin.h"

#include <stdio.h>

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

#endif