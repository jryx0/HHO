#ifndef __MACRODEF_H__
#define __MACRODEF_H__

//////////////////////////////////////宏定义///////////////////////////////////
//显示模式宏定义
#define SVGA64K
#define SVGA1024x768

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768

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

//处理borlandc在vscode中的兼容性
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