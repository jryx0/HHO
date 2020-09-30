#include "hhosvga.h"
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
  if (regs.x.ax != 0x004f) {
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
  if (regs.x.ax != 0x004f) {
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
  if (flag == 0) {
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
    flag++;
  }

  /*如果和上次页面号不同，则进行换页*/
  else if (page != old_page) {
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
  }
}

int Linex(int x1, int y1, int x2, int y2, int color) // 1
{
  int i;
  if (x1 >= SCR_WIDTH)
    x1 = SCR_WIDTH - 1;

  if (x2 >= SCR_WIDTH)
    x2 = SCR_WIDTH - 1;

  if (y1 >= SCR_HEIGHT)
    y1 = SCR_HEIGHT - 1;

  if (y2 >= SCR_HEIGHT)
    y2 = SCR_HEIGHT - 1;

  if (x1 < 0)
    x1 = 0;

  if (x2 < 0)
    x2 = 0;

  if (y1 < 0)
    y1 = 0;

  if (y2 < 0)
    y2 = 0;

  if (y1 != y2 || x1 >= x2)
    return (0);

  for (i = x1; i <= x2; i++)
    putpixel64k(i, y1, color);
  return 0;
}

int Liney(int x1, int y1, int x2, int y2, int color) // 2
{
  int i;
  if (x1 >= SCR_WIDTH)
    x1 = SCR_WIDTH - 1;

  if (x2 >= SCR_WIDTH)
    x2 = SCR_WIDTH - 1;

  if (y1 >= SCR_HEIGHT)
    y1 = SCR_HEIGHT - 1;

  if (y2 >= SCR_HEIGHT)
    y2 = SCR_HEIGHT - 1;

  if (x1 < 0)
    x1 = 0;

  if (x2 < 0)
    x2 = 0;

  if (y1 < 0)
    y1 = 0;

  if (y2 < 0)
    y2 = 0;

  if (x1 != x2 || y1 >= y2)
    return (0);
  for (i = y1; i <= y2; i++)
    putpixel64k(x1, i, color);
  return 0;
}
void line(int x1, int y1, int x2, int y2, int color) {

  int x, y, dx, dy, s1, s2, p, temp, interchange, i;
  if (x1 == x2) {
    Liney(x1, y1, x2, y2, color);
    return;
  } else if (y1 == y2) {
    Linex(x1, y1, x2, y2, color);
    return;
  }

  x = x1;
  y = y1;
  dx = abs(x2 - x1);
  dy = abs(y2 - y1);

  if (x2 > x1)
    s1 = 1;
  else
    s1 = -1;

  if (y2 > y1)
    s2 = 1;
  else
    s2 = -1;

  if (dy > dx) {
    temp = dx;
    dx = dy;
    dy = temp;
    interchange = 1;

  } else
    interchange = 0;

  p = 2 * dy - dx;
  for (i = 1; i <= dx; i++) {
    putpixel64k(x, y, color);
    if (p >= 0) {
      if (interchange == 0)
        y = y + s2;
      else
        x = x + s1;
      p = p - 2 * dx;
    }
    if (interchange == 0)
      x = x + s1;
    else
      y = y + s2;
    p = p + 2 * dy;
  }
  return;
}

void bar(int x1, int y1, int x2, int y2, long bkcolor) {
  int i;
  for (i = x1; i <= x2; i++) {
    Liney(i, y1, i, y2, bkcolor);
  }

  return 0;
}

void clearScreen(int color) {
  int i, j;

  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  // unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  // for (i = 0; i < 24; i++) {
  //   selectpage(i);
  //   memset(video_buffer, color, 32 * 1024);
  // }
  for (j = 0; j < SCR_HEIGHT; j++)
    for (i = 0; i < SCR_WIDTH; i++)
      putpixel64k(i, j, color);
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
  if (x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1)) {
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
  if (x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1)) {
    return 0;
  }

  /*计算显存地址偏移量和对应的页面号，做换页操作*/
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /*32k个点一换页，除以32k的替代算法*/
  selectpage(new_page);

  /*返回颜色*/
  return video_buffer[page];
}
