#ifndef __HGLOBAL_H__
#define __HGLOBAL_H__

#include "macrodef.h"
#include "mouse.h"
#include "pinyin.h"
#include "HBaseWin.h"

#include <stdio.h>

#define CHINESE 1
#define ENGLISH 0

//--------------------------------------------------------------------

//ȫ�ֱ����ṹ
//Dos ���ļ�����<15
typedef struct _globaldef
{
  //FILE *fpBK;  //������ŵ��ļ�
  //FILE *fpLog; //��־�ļ�ָ��

  unsigned char cursor_arrow[MOUSE_WIDTH][MOUSE_HEIGHT]; //�������ͼ�ε�ַ-��ͷ
  unsigned char cursor_hand[MOUSE_WIDTH][MOUSE_HEIGHT];  //�������ͼ�ε�ַ-��
  mousestatus mouse;

  pyInput *pinyin;
  hbasewinAttr *foucsedTextBox;

  hbasewinAttr *activePage;
  int activePageID;
  unsigned int userid;     //�û�ID
  unsigned char usertype;  //�û�����
  unsigned char isExit;    //�Ƿ��˳�����
  unsigned char InputMode; //����ģʽ
} globaldef;

globaldef *initGlobalSetting(void);
void destoryGlobalSettting(globaldef *_g);
globaldef *loadSvgaResouce(globaldef *_g);
void loadMouse(globaldef *_g);

/**
 * ��ȡ�����ֿ��ļ�
 * @param type ��marcodef�ж��� 
 * @param size �ֺ� 16��24��32��48
 * 
 * @return �ֿ��ļ�ָ��
 */
FILE *getFontFile(int type, int size);

/**
 * ɾ���ַ�����ָ��λ�õ��ַ�
 */
int DelPosChar(char *str, int pos);

#endif