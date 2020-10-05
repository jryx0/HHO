#include "hglobal.h"
#include "SVGA.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void initSVGA64k(void) // 1
{
  union REGS regs;
  /*设置VESA VBE模式的功能号*/
  regs.x.ax = 0x4f02;
  regs.x.bx = 0x117;
  int86(0x10, &regs, &regs);

  /*ax != 0x004f意味着初始化失败，输出错误信息见上,下同*/
  if (regs.x.ax != 0x004f)
  {
    printf("Error in setting SVGA mode!\nError code:0x%x\n", regs.x.ax);
    delay(5000);
    exit(1);
  }
}

unsigned int getSVGA(void) // 2
{

  union REGS regs;
  /*获取当前VESA VBE模式的功能号*/
  regs.x.ax = 0x4f03;
  int86(0x10, &regs, &regs);

  /*显示错误信息*/
  if (regs.x.ax != 0x004f)
  {
    printf("Error in getting SVGA mode!\nError code:0x%x\n", regs.x.ax);
    delay(5000);
    exit(1);
  }

  return regs.x.bx;
}

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

void putpixel64k(int x, int y, unsigned int color) // 3
{
  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*要切换的页面号*/
  unsigned char new_page;

  /*对应显存地址偏移量*/
  unsigned long int page;

  /*判断点是否在屏幕范围内，不在就退出*/
  if (x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
  {
    return;
  }

  /*计算显存地址偏移量和对应的页面号，做换页操作*/
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /*32k个点一换页，除以32k的替代算法*/
  selectpage(new_page);

  /*向显存写入颜色，对应点画出*/
  video_buffer[page] = color;
}

unsigned int getpixel64k(int x, int y) // 4
{
  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*要切换的页面号*/
  unsigned char new_page;

  /*对应显存地址偏移量*/
  unsigned long int page;

  /*判断点是否在屏幕范围内，不在就退出*/
  if (x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
  {
    return 0;
  }

  /*计算显存地址偏移量和对应的页面号，做换页操作*/
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /*32k个点一换页，除以32k的替代算法*/
  selectpage(new_page);

  /*返回颜色*/
  return video_buffer[page];
}