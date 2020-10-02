#include "macrodef.h"
#include "hhogui.h"
#include "SVGA.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void clearScreen(int color)
{
  int i, j = 16;
  unsigned int pages = 0, pagesize = 0;

  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  pages = 24;

  pagesize = 1024 * 32 * 2;
  for (i = 0; i <= pages; i++)
  {
    selectpage(i);
    memset(video_buffer, color, pagesize - 1);
    //memset(video_buffer + 0x4000, color, pagesize); // 16 << 10
  }
}

/**
 * 快速填充区域算法
 * 
 */
void fillRegion(int x1, int y1, int x2, int y2, int color)
{
  int i, j, width, height;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改

  setStandardRegion(&x1, &y1, &x2, &y2);
  width = x2 - x1;
  height = y2 - y1;

  for (j = 0; j < height; j++)
  {
    /* 计算显存地址偏移量和对应的页面号，做换页操作 */
    offest = ((unsigned long int)(y1 + j) << 10) + x1;
    page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
    selectpage(page);
    for (i = 0; i < width; i++)
    {
      video_buffer[offest + i] = color;
    }
  }
}

void fillRegionEx(int x, int y, int width, int height, int color)
{
  int i, j;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改

  for (j = 0; j < height; j++)
  {
    /* 计算显存地址偏移量和对应的页面号，做换页操作 */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
    selectpage(page);
    for (i = 0; i < width; i++)
    {
      video_buffer[offest + i] = color;
    }
  }
}

void swap(int *a, int *b)
{
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

/**
 * 设置一个矩形区域，使x1 < x2, y1 < y2
 * 并且使x1,x2,y1,y2在屏幕区域内 
 * 
 */
void setStandardRegion(int *x1, int *y1, int *x2, int *y2)
{
  if (x1 < 0)
    x1 = 0;

  if (x1 > SCR_WIDTH)
    x1 = SCR_WIDTH;

  if (x2 < 0)
    x2 = 0;

  if (x2 > SCR_WIDTH)
    x2 = SCR_WIDTH;

  if (y1 < 0)
    y1 = 0;

  if (y1 > SCR_HEIGHT)
    y1 = SCR_HEIGHT;

  if (y2 < 0)
    y2 = 0;

  if (y2 > SCR_HEIGHT)
    y2 = SCR_HEIGHT;

  if (x1 > x2)
    swap(x1, x2);

  if (y1 > y2)
    swap(y1, y2);
}

// int Linex(int x1, int y1, int x2, int y2, int color) // 1
// {
//   int i;
//   if (x1 >= SCR_WIDTH)
//     x1 = SCR_WIDTH - 1;

//   if (x2 >= SCR_WIDTH)
//     x2 = SCR_WIDTH - 1;

//   if (y1 >= SCR_HEIGHT)
//     y1 = SCR_HEIGHT - 1;

//   if (y2 >= SCR_HEIGHT)
//     y2 = SCR_HEIGHT - 1;

//   if (x1 < 0)
//     x1 = 0;

//   if (x2 < 0)
//     x2 = 0;

//   if (y1 < 0)
//     y1 = 0;

//   if (y2 < 0)
//     y2 = 0;

//   if (y1 != y2 || x1 >= x2)
//     return (0);

//   for (i = x1; i <= x2; i++)
//     putpixel64k(i, y1, color);
//   return 0;
// }

// int Liney(int x1, int y1, int x2, int y2, int color) // 2
// {
//   int i;
//   if (x1 >= SCR_WIDTH)
//     x1 = SCR_WIDTH - 1;

//   if (x2 >= SCR_WIDTH)
//     x2 = SCR_WIDTH - 1;

//   if (y1 >= SCR_HEIGHT)
//     y1 = SCR_HEIGHT - 1;

//   if (y2 >= SCR_HEIGHT)
//     y2 = SCR_HEIGHT - 1;

//   if (x1 < 0)
//     x1 = 0;

//   if (x2 < 0)
//     x2 = 0;

//   if (y1 < 0)
//     y1 = 0;

//   if (y2 < 0)
//     y2 = 0;

//   if (x1 != x2 || y1 >= y2)
//     return (0);
//   for (i = y1; i <= y2; i++)
//     putpixel64k(x1, i, color);
//   return 0;
// }
// void line(int x1, int y1, int x2, int y2, int color)
// {

//   int x, y, dx, dy, s1, s2, p, temp, interchange, i;
//   if (x1 == x2)
//   {
//     Liney(x1, y1, x2, y2, color);
//     return;
//   }
//   else if (y1 == y2)
//   {
//     Linex(x1, y1, x2, y2, color);
//     return;
//   }

//   x = x1;
//   y = y1;
//   dx = abs(x2 - x1);
//   dy = abs(y2 - y1);

//   if (x2 > x1)
//     s1 = 1;
//   else
//     s1 = -1;

//   if (y2 > y1)
//     s2 = 1;
//   else
//     s2 = -1;

//   if (dy > dx)
//   {
//     temp = dx;
//     dx = dy;
//     dy = temp;
//     interchange = 1;
//   }
//   else
//     interchange = 0;

//   p = 2 * dy - dx;
//   for (i = 1; i <= dx; i++)
//   {
//     putpixel64k(x, y, color);
//     if (p >= 0)
//     {
//       if (interchange == 0)
//         y = y + s2;
//       else
//         x = x + s1;
//       p = p - 2 * dx;
//     }
//     if (interchange == 0)
//       x = x + s1;
//     else
//       y = y + s2;
//     p = p + 2 * dy;
//   }
//   return;
// }

// void bar(int x1, int y1, int x2, int y2, long bkcolor)
// {
//   int i;
//   for (i = x1; i <= x2; i++)
//   {
//     Liney(i, y1, i, y2, bkcolor);
//   }

//   return 0;
// }
