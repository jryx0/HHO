#include "macrodef.h"
#include "SVGAUTIL.H"

#include <dos.h>

void selectpage(register char page) // 3
{
  union REGS regs;

  /*上一次的页面号,用于减少切换次数,是使用次数很多的重要变量*/
  static unsigned char old_page = 0;
  /*标志数，用于判断是否是第一次换页*/
  static int flag = 0;

  /*窗口页面控制功能号*/
  regs.x.ax = 0x4f05;
  regs.x.bx = 0;

  /*如果是第一次换页*/
  if (flag == 0)
  {
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
    flag++;
  }

  /*如果和上次页面号不同，则进行换页*/
  else if (page != old_page)
  {
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
  }
}

/**
 * 清除屏幕,使用char填充屏幕,由于64K是int,故填充的颜色是 color << 8 | color
 * 填充0xFFFF,0x0000,等颜色速度更快
 * @param color 填充的颜色
 */
void clearScreen(char color)
{
  int i, j = 16;
  unsigned int pages = 0, pagesize = 0;

  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  pages = 24; //1024x768 = 24, 800*600 = 14
  pagesize = 1024 * 32 * 2;
  for (i = 0; i <= pages; i++)
  {
    selectpage(i);
    memset(video_buffer, color, pagesize - 1);
    //memset(video_buffer + 0x4000, color, pagesize); // 16 << 10
  }
}