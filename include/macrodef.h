#ifndef __MACRODEF_H__
#define __MACRODEF_H__

#include <stdio.h>

//显示模式宏定义
#define SVGA64K
#define SVGA1024x768
// #define SVGA32K
// #define SVGA800x600

//全局文件路径
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


//鼠标大小
#define MOUSE_WIDHT  16
#define MOUSE_HEIGHT 16

//处理borlandc在vscode中的兼容性
#ifndef __BORLANDC__
#define huge
#define far
#endif

#endif