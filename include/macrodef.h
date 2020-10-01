#ifndef __MACRODEF_H__
#define __MACRODEF_H__

// #define SVGA64K
// #define SVGA1024x768
// #define SVGA32K
// #define SVGA800x600

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