#include "macrodef.h"
#include "SVGAUTIL.H"

#include <dos.h>
#include <GRAPHICS.H>
#include <stdio.h>

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

//控制区域在屏幕范围内
void setStandardRegionEx(int *x, int *y, int *width, int *height)
{
  if (*x < 0)
    *x = 0;
  if (*x > getmaxx())
    *x = getmaxx();

  if (*y < 0)
    *y = 0;
  if (*y > getmaxy())
    *y = getmaxy();

  if (*x + *width >= getmaxx())
    *width = getmaxx() - *x;
  if (*y + *height > getmaxy())
    *height = getmaxy() - *y;
}

//把屏幕区域保存在文件中
void savebackgroundFile(int x, int y, int width, int height)
{
  FILE *fpBK = fopen("bk.data", "w");

  int j;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改
  setStandardRegionEx(&x, &y, &width, &height);

  for (j = 0; j < height; j++)
  {
    /* 计算显存地址偏移量和对应的页面号，做换页操作 */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
    selectpage(page);
    fwrite(&video_buffer[offest], 1, width * 2, fpBK);
  }
  fflush(fpBK);
  fclose(fpBK);
}

void restorebackgroundFile(int x, int y, int width, int height)
{
  FILE *fpBK = fopen("bk.data", "r");
  int j;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改

  if (fpBK == NULL)
    return;
  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < height; j++)
  {
    /* 计算显存地址偏移量和对应的页面号，做换页操作 */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
    selectpage(page);

    fread(&video_buffer[offest], 1, width * 2, fpBK);
  }
  fclose(fpBK);
}

//保存小面积的背景
//width * heigth * 2 < 65536
void savebackgroundEx(unsigned int *buffer, int x, int y, int width, int height)
{
  int j;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改

  if (buffer == NULL)
    return;
  setStandardRegionEx(&x, &y, &width, &height);

  for (j = 0; j < height; j++)
  {
    /* 计算显存地址偏移量和对应的页面号，做换页操作 */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
    selectpage(page);

    memcpy(buffer + j * width, &video_buffer[offest], width * 2); //width * 2, int 两位
  }
}

void restorebackgroundEx(unsigned int *buffer, int x, int y, int width, int height)
{
  int j;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改

  if (buffer == NULL)
    return;

  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < height; j++)
  {
    /* 计算显存地址偏移量和对应的页面号，做换页操作 */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
    selectpage(page);

    memcpy(&video_buffer[offest], buffer + j * width, width * 2); //width * 2, int 两位
  }
}

void putpixel64k(int x, int y, unsigned int color)  
{
  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*要切换的页面号*/
  unsigned char new_page;

  /*对应显存地址偏移量*/
  unsigned long int page;   

  /*判断点是否在屏幕范围内，不在就退出*/
  if (x < 0 || x > (getmaxx()) || y < 0 || y > (getmaxy()))
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

unsigned int getpixel64k(int x, int y)
{
  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*要切换的页面号*/
  unsigned char new_page;

  /*对应显存地址偏移量*/
  unsigned long int page;

  /*判断点是否在屏幕范围内，不在就退出*/
  if (x < 0 || x > (getmaxx() - 1) || y < 0 || y > getmaxy())
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