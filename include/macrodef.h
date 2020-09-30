#ifndef __MACRODEF_H__
#define __MACRODEF_H__

#define SVGA64K1024x768

#ifdef SVGA64K1024x768
#define SCR_WIDTH 1024
#define SCR_HEIGHT 768
#define TOTALPAGES 924672
#else if define(SVGA64K800x600)
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define TOTALPIXEL 480000
#endif // SVGA64K1024x768

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
struct WORDREGS {
  unsigned int ax, bx, cx, dx, si, di, cflag, flags;
};

struct BYTEREGS {
  unsigned char al, ah, bl, bh, cl, ch, dl, dh;
};

union REGS {
  struct WORDREGS x;
  struct BYTEREGS h;
};
#endif //_REG_DEFS

#else
#define DATAPATH "E:\\HHO\\data\\"
#endif //__BORLANDC__

#endif