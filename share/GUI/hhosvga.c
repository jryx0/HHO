#include "hglobal.h"
#include "SVGAUTIL.H"
#include "hhosvga.h"

#include <dos.h>
#include <GRAPHICS.H>
#include <stdio.h>
#include <memory.h>

/**
 * 控制显示页
 * @param page 页号 
 */
void selectpage(register char page)
{
  union REGS regs;
  static unsigned char old_page = 0;
  static int flag = 0;

  /*窗口页面控制功能号*/
  regs.x.ax = 0x4f05;
  regs.x.bx = 0;

  if (flag == 0)
  { /*如果是第一次换页*/
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
    flag++;
  }
  else if (page != old_page)
  { /*如果和上次页面号不同，则进行换页*/
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
  int i;
  unsigned int pages = 0, pagesize = 0;

  /*显存指针常量*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  pages = 24; //1024x768 = 24, 800*600 = 14
  pagesize = 1024 * 32 * 2;
  for (i = 0; i <= pages; i++)
  {
    selectpage(i);
    memset(video_buffer, color, pagesize - 1);
    //memset(video_buffer + 0x4000, color, pagesize); // 16 << 10
  }

  // selectpage(0);
  // memset(video_buffer, 0xff, 1024 * 32 * 2 - 1);
  // selectpage(1);
  // memset(video_buffer, 0xff, 1024 * 32 * 2 - 1);
}

/**
 * 控制区域在屏幕范围内
 * @param x  
 * @param y 左上坐标
 * @param width
 * @param height 宽度、高度
 */
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

/**
 * 由于内存限制，仅仅适用于保存小面积的背景，放在在内存堆中
 * 要求width * heigth * 2 < 65536 
 * @param buffer 背景保存缓存
 * @param x
 * @param y   左上坐标
 * @param width
 * @param height 宽度、高度
 */
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

/**
 * 恢复背景，和savebackgroundEx互为逆操作
 * 要求width * heigth * 2 < 65536 
 * 位置可以不一样，高度和宽度要一致。
 * @param buffer 保存背景缓存
 * @param x
 * @param y   左上坐标
 * @param width
 * @param height 宽度、高度
 */
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

/**
 * 由于内存限制，把大区域背景保存在文件中。
 * @param fpBK 文件指针，用于保存背景
 * @param x
 * @param y   左上坐标
 * @param width
 * @param height 宽度、高度
 */
void savebackgroundFile(FILE *fpBK, int x, int y, int width, int height)
{
  int j;

  unsigned char page;       // 要切换的页面号
  unsigned long int offest; // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

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

/**
 * 由于内存限制，把保存在文件大区域背景的背景恢复到屏幕上，
 * 位置可以不一样，高度和宽度要一致。
 * @param fpBK 文件指针，用于保存背景
 * @param x
 * @param y   左上坐标
 * @param width
 * @param height 宽度、高度
 */
void restorebackgroundFile(FILE *fpBK, int x, int y, int width, int height)
{
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

/**
 * 画点函数，graphics.h 中的putpixel在1024*768*64k下出错,
 * 在此模式下,画点函数 
 * @param x 
 * @param y 点坐标
 * @param color 颜色
 */
void putpixel64k(int x, int y, unsigned int color)
{
  /*显存指针常量，指向显存首地址 */
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;
  unsigned char new_page;
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

/**
 * 画点函数，graphics.h 中的getpixel在1024*768*64k下出错,
 * 在此模式下,画点函数 
 * @param x 
 * @param y 点坐标
 * @return color 颜色
 */
unsigned int getpixel64k(int x, int y)
{
  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;
  unsigned char new_page;
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

  return video_buffer[page];
}