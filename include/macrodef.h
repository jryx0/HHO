#ifndef __MACRODEF_H__
#define __MACRODEF_H__

#include <stdio.h>

//��ʾģʽ�궨��
#define SVGA64K
#define SVGA1024x768
// #define SVGA32K
// #define SVGA800x600

//ȫ���ļ�·��
#define USERINFOFILE DATAPATH "userinfo.txt"
#define DOCTORINFOFILE DATAPATH "doctorinfo.txt"
#define FILE_PYINDEX DATAPATH "index.txt"
#define FILE_HANZI DATAPATH "hanzi.txt"
#define FILE_CURSOR_ARROW DATAPATH "arrows.cur"
#define FILE_CURSOR_HAND DATAPATH "hand.cur"

#if !defined(__BORLANDC__)
#define DATAPATH "data\\"
#else
#define DATAPATH "C:\\HHO\\data\\"
#endif


//����С
#define MOUSE_WIDHT  16
#define MOUSE_HEIGHT 16

//����borlandc��vscode�еļ�����
#ifndef __BORLANDC__
#define huge
#define far
#endif

#endif