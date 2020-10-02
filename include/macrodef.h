#ifndef __MACRODEF_H__
#define __MACRODEF_H__

#define SVGA64K
#define SVGA1024x768
// #define SVGA32K
// #define SVGA800x600

#ifdef SVGA1024x768
#define MAXX 1023
#define MAXY 767
#elif defined(SVGA800x600)
#define MAXX 799
#define MAXY 599
#else
#define MAXX 0
#define MAXY 0
#endif

#define USERINFOFILE DATAPATH "userinfo.txt"
#define DOCTORINFOFILE DATAPATH "doctorinfo.txt"

#define FILE_PYINDEX DATAPATH "index.txt"
#define FILE_HANZI DATAPATH "hanzi.txt"

#ifndef __BORLANDC__
#define DATAPATH "data\\"
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

#else
#define DATAPATH "E:\\HHO\\data\\"
#endif //__BORLANDC__

#endif