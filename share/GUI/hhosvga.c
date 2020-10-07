#include "macrodef.h"
#include "hhosvga.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mem.h>

unsigned int RGB565(int r, int g, int b)
{ // 连接
  return (r << 11) + (g << 5) + (b << 0);
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
/**
 * 清除区域,使用char填充屏幕,由于64K是int,故填充的颜色是 color << 8 | color
 * 填充0xFFFF,0x0000,等颜色速度更快
 * @param color 填充的颜色
 */
void clearRegion(int x1, int y1, int x2, int y2, char color)
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
    memset(video_buffer + offest, color, width * 2); //一个像素两个字节 width* 2
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
 * 设置一个矩形区域(x1, y1), (x2,y2)，使x1 < x2, y1 < y2
 * 并且使x1,x2,y1,y2在屏幕区域内  ，超出部分截断
 * @param *x1
 * @param *x2 
 * @param *x2
 * @param *y2
 */
void setStandardRegion(int *x1, int *y1, int *x2, int *y2)
{
  if (*x1 < 0)
    *x1 = 0;

  if (*x1 > SCR_WIDTH)
    *x1 = SCR_WIDTH;

  if (*x2 < 0)
    *x2 = 0;

  if (*x2 > SCR_WIDTH)
    *x2 = SCR_WIDTH;

  if (*y1 < 0)
    *y1 = 0;

  if (*y1 > SCR_HEIGHT)
    *y1 = SCR_HEIGHT;

  if (*y2 < 0)
    *y2 = 0;

  if (*y2 > SCR_HEIGHT)
    *y2 = SCR_HEIGHT;

  if (*x1 > *x2)
    swap(x1, x2);

  if (*y1 > *y2)
    swap(y1, y2);
}

/**
 * 设置一个矩形区域,左上点(x1, y1)、宽度和高度
 * 并且使矩形区域在屏幕区域内 ，超出部分截断
 * @param *x1
 * @param *x2 
 * @param *x2
 * @param *y2
 */
void setStandardRegionEx(int *x, int *y, int *width, int *height)
{
  if (*x < 0)
    *x = 0;
  if (*x > SCR_WIDTH)
    *x = SCR_WIDTH;

  if (*y < 0)
    *y = 0;
  if (*y > SCR_HEIGHT)
    *y = SCR_HEIGHT;

  if (*x + *width > SCR_WIDTH)
    *width = SCR_WIDTH - *x - 2;
  if (*y + *height > SCR_HEIGHT)
    *height = SCR_HEIGHT - *y - 2;
}

/**
 * 快速填充区域算法（坐标形式）,减少调用selectpage的次数
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标
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

/**
 * 快速填充区域算法（高度宽度形式）,减少调用selectpage的次数
 * @param  x1 区域左坐标
 * @param  y1 区域上坐标
 * @param  x2 区域又坐标
 * @param  y2 区域下坐标
 */
void fillRegionEx(int x, int y, int width, int height, int color)
{
  int i, j;

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
    for (i = 0; i < width; i++)
    {
      video_buffer[offest + i] = color;
    }
  }
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
 * 仅仅适用于保存小面积的恢复背景，和savebackgroundEx互为逆操作
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

  fseek(fpBK, 0, SEEK_SET);
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

  fseek(fpBK, 0, SEEK_SET);
  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < height; j++)
  {
    /* 计算显存地址偏移量和对应的页面号，做换页操作 */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
    selectpage(page);

    fread(&video_buffer[offest], 1, width * 2, fpBK);
  }
}

/**
 * 画水平线坐标形式
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标
 * @param  width 线长度
 * @param  color 颜色 
 */
void linexEx(int x, int y, int width, int color)
{
  int i, height = 0;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改

  setStandardRegionEx(&x, &y, &width, &height);
  /* 计算显存地址偏移量和对应的页面号，做换页操作 */
  offest = ((unsigned long int)(y) << 10) + x;
  page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
  selectpage(page);
  for (i = 0; i < width; i++)
  {
    video_buffer[offest + i] = color;
  }
}
/**
 * 画水平线坐标形式
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void linex(int x1, int y1, int x2, int y2, int color)
{
  if (y1 == y2)
    linexEx(x1, y1, x2 - x1, color);
}

/**
 * 画垂直线坐标形式
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标
 * @param  height 线高度
 * @param  color 颜色 
 */
void lineyEx(int x, int y, int height, int color)
{
  int j, width = 0;

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

    video_buffer[offest] = color;
  }
}

/**
 * 画水平线坐标形式
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void liney(int x1, int y1, int x2, int y2, int color)
{
  if (x1 == x2)
    lineyEx(x1, y1, y2 - y1, color);
}

/**
 * 根据坐标画线 
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void line(int x1, int y1, int x2, int y2, int color)
{
  int x, y, dx, dy, s1, s2, p, temp, interchange, i;

  if (x1 == x2)
  {
    liney(x1, y1, x2, y2, color);
    return;
  }
  else if (y1 == y2)
  {
    linex(x1, y1, x2, y2, color);
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

  if (dy > dx)
  {
    temp = dx;
    dx = dy;
    dy = temp;
    interchange = 1;
  }
  else
    interchange = 0;

  p = 2 * dy - dx;
  for (i = 1; i <= dx; i++)
  {
    putpixel64k(x, y, color);
    if (p >= 0)
    {
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

/**
 * 根据坐标填充区域 
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void bar(int x1, int y1, int x2, int y2, int bkcolor)
{
  fillRegion(x1, y1, x2, y2, bkcolor);
}

/**
 * 画水平线，可以设置宽度、线型
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标  
 * @param width 水平线长度
 * @param line_width 线粗细
 * @param dot_style 间隔像素  
 */
void linex_styleEx(int x, int y, int width, int color, char line_width, char dot_style)
{
  int i, j, height = 0;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改
  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < line_width; j++)
  {
    /* 计算显存地址偏移量和对应的页面号，做换页操作 */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
    selectpage(page);
    for (i = 0; i < width; i++)
    {
      video_buffer[offest + i] = color;

      if ((dot_style > 1) && (i % dot_style == 0))
        i += 2;
    }
  }
}

/**
 * 画水平线，可以设置宽度、线型
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标  
 * @param height 水平线长度
 * @param line_width 线粗细
 * @param dot_style 间隔像素  
 */
void liney_styleEx(int x, int y, int height, int color, char line_width, char dot_style)
{
  int i, j, width = 0;

  unsigned char page;                                                     // 要切换的页面号
  unsigned long int offest;                                               // 对应显存地址偏移量
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // 显存指针常量，指向显存首地址，指针本身不允许修改
  setStandardRegionEx(&x, &y, &width, &height);
  for (i = 0; i < line_width; i++)
    for (j = 0; j < height; j++)
    {
      /* 计算显存地址偏移量和对应的页面号，做换页操作 */
      offest = ((unsigned long int)(y + j) << 10) + x + i;
      page = offest >> 15; /* 32k个点一换页，除以32k的替代算法 */
      selectpage(page);

      video_buffer[offest] = color;

      if ((dot_style > 1) && (j % dot_style == 0))
        j += 2;
    }
}

/**
 * 根据坐标画长方形 
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void rectangle(int x1, int y1, int x2, int y2, int color, char line_width, char dot_style)
{
  //setStandardRegion(&x1, &y1, &x2, &y2);
  linex_styleEx(x1, y1, x2 - x1, color, line_width, dot_style);
  liney_styleEx(x2, y1, y2 - y1, color, line_width, dot_style);
  linex_styleEx(x1, y2, x2 - x1, color, line_width, dot_style);
  liney_styleEx(x1, y1, y2 - y1, color, line_width, dot_style);
}
/**
 * 根据区域画长方形  
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标
 * @param width
 * @param height 宽度、高度
 * @param  color 颜色
 */
void rectangleEx(int x, int y, int width, int height, int color, char line_width, char dot_style)
{
  //setStandardRegionEx(x, y, width, height);
  linex_styleEx(x, y, width, color, line_width, dot_style);
  liney_styleEx(x + width, y, height, color, line_width, dot_style);
  linex_styleEx(x, y + height, width, color, line_width, dot_style);
  liney_styleEx(x, y, height, color, line_width, dot_style);
}

/**
 * 显示一个ascii码
 * @param x
 * @param y 
 * @param asc 字符
 * @param font 字体信息
 */
void printASC(int x, int y, char acs, hfont *font)
{
  int i, j, k, m;
  unsigned char buffer[16];

  if (!isprint(acs)) //非打印字符，返回
    return;

  TESTNULL(font, );
  TESTNULL(font->fpASC, );

  fseek(font->fpASC, acs * 16L, SEEK_SET);
  fread(buffer, 16, 1, font->fpASC);

  for (i = 0; i < 16; i++)
    for (m = 1; m <= font->ascScaley; m++) //y方向缩放
      for (j = 0; j < 8; j++)
        for (k = 1; k <= font->ascScalex; k++) //x方向缩放
          if ((buffer[i] >> 7 - j) & 1)
            putpixel64k(x + j * font->ascScalex + k, y + m + i * font->ascScaley, font->fontcolor);
}

void calcFontSetting(hfont *font)
{
  TESTNULL(font, );

  //计算汉字每行像素需要多少个字节保存
  font->byteperline = (font->currentFontSize + 7) / 8;

  //计算汉字像素需要多少字节保存
  font->totalbytes = font->byteperline * font->currentFontSize;
  switch (font->currentFontSize)
  {
  case 16:
    font->ascScalex = 1;
    font->ascScaley = 1;
    font->ascy = 0;
    font->ascSize = 8 * 1;
    break;
  case 24:
    font->ascScalex = 2;
    font->ascScaley = 2;
    font->ascy = 3;
    font->ascSize = 8 * 2;
    break;
  case 32:
    font->ascScalex = 3;
    font->ascScaley = 3;
    font->ascy = 8;
    font->ascSize = 8 * 3;
    break;
  case 48:
    font->ascScalex = 4;
    font->ascScaley = 4;
    font->ascy = 6;
    font->ascSize = 8 * 4;
    break;
  default:
    break;
  }
}

/**
 * 显示一个汉字
 * @param fpfont 字库文件指针
 * @param x
 * @param y 
 * @param buffer 字库数据
 * @param fontsize 汉字大小 16,24,32,48
 * @param color 颜色 */
void printHZWord(int x, int y, unsigned char *buffer, hfont *font)
{
  unsigned char mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; //功能数组，用于显示汉字点阵中的亮点
  int i, j, pos;

  TESTNULL(font, );
  TESTNULL(font->fpCurrentFont, );

  for (i = 0; i < font->currentFontSize; i++)
  {
    pos = font->byteperline * i;                //16*16矩阵中有每一行有两外字节
    for (j = 0; j < font->currentFontSize; j++) //一行一行地扫描，将位上为了1的点显示出来
    {
      if ((mask[j % 8] & buffer[pos + j / 8]) != NULL) //j%8只能在0—8之间循环，j/8在0，1之间循环
      {
        putpixel64k(x + j, y, font->fontcolor);
      }
    }
    y++;
  }
}

/**
 * 在一个区域内输出字符串，字符显示格式包括字体、字号、字间距、行间距等等在_font中设置，
 * 字符串可以是中英文混合。
 * 
 * @param region  要显示的区域
 * @param text 字符串
 * @param _font 字体设置
 * 
 */
void printTextEx(hregion *region, char *text, hfont *_font)
{

  int x0, y0;                //每个汉字起始点
  unsigned char quma, weima; //定义汉字的区码和位码
  unsigned long offset;
  unsigned char *buffer;
  int linenum = 0;
  char isNewLine = FALSE;

  TESTNULL(region, );
  TESTNULL(_font, );

  buffer = (unsigned char *)malloc(_font->totalbytes);
  TESTNULL(buffer, );

  //判断高度和宽度是否足够
  if ((region->left_top.x + _font->currentFontSize) > region->right_bottom.x ||
      (region->left_top.y + _font->currentFontSize) > region->right_bottom.y)
  {
    return;
  }

  x0 = region->left_top.x;
  while (*text)
  {

    if ((x0 + _font->currentFontSize) < region->right_bottom.x)                 //计算x0 + 字符宽度 是否大于右边界
    {                                                                           //在同一行内输出
      y0 = region->left_top.y + linenum * _font->currentFontSize + _font->ygap; //计算高度 y + 行数*字符高度 + 行间距

      if (((unsigned char)text[0] >= 0xa0) &&
          ((unsigned char)text[1] >= 0xa0))
      {                                                            //打印中文                                                            //汉字
        quma = text[0] - 0xa1;                                     //求出区码
        weima = text[1] - 0xa1;                                    //求出位码
        offset = (94L * (quma) + (weima)) * _font->totalbytes;     //求出要显示的汉字在字库文件中的偏移
        fseek(_font->fpCurrentFont, offset, SEEK_SET);             //重定位文件指针
        fread(buffer, _font->totalbytes, 1, _font->fpCurrentFont); //读出该汉字的具体点阵数据,1为要读入的项数

        printHZWord(x0, y0, buffer, _font); //输出单个汉字

        x0 += _font->currentFontSize + _font->xgap; //偏移一个汉字宽度+字间距
        text += 2;                                  //下一个汉字
      }
      else
      { //打印字符

        if (*text == '\r' || *text == '\n')
        {                              //换行
          x0 = region->right_bottom.x; //强行设置x0换行
          if (*(text + 1) != 0)        //不是最后一个字符
            if (*(text + 1) == '\r' || (*(text + 1) == '\n'))
              text += 2; //处理\r\n情况
            else
              text++; //只有一个\r或\n
        }
        else
        {                                               //字符
          printASC(x0, y0 - _font->ascy, *text, _font); //输出单个字符
          x0 += _font->ascSize + _font->xgap;           //偏移一个字符宽度+字间距
          text++;                                       //下一个字符
        }
      }
    }
    else
    { //换行

      if ((unsigned char)text[0] < 0xa0)
      { //最后一个是字符
        if ((x0 + _font->ascSize) < region->right_bottom.x)
        {
          printASC(x0, y0 - _font->ascy, *text, _font); //输出单个字符
          x0 += _font->ascSize + _font->xgap;           //偏移一个字符 +　字间距
          text++;                                       //下一个字符
        }
      }

      linenum++;

      if ((region->left_top.y + (linenum + 1) * _font->currentFontSize + _font->ygap) > region->right_bottom.y)
      { //判断是否超高度，退出 高度截断
        int clearwidth = _font->currentFontSize;
        int clearheight = _font->currentFontSize;

        if (*(text - 1) < 0xa0)
          clearwidth = _font->ascSize;

        clearRegion(x0 - clearwidth, y0, x0, y0 + clearheight, 0xff);
        printASC(x0 - clearwidth, y0 - _font->ascy, '.', _font);
        break;
      }

      x0 = region->left_top.x;
    }
  }

  free(buffer);
}

void print(hregion *region, char *text, hfont *_font)
{
  int x0, y0;                //每个汉字起始点
  unsigned char quma, weima; //定义汉字的区码和位码
  unsigned long offset;
  unsigned char *buffer;
  int linenum = 0;
  char isNewLine = FALSE;
  int lasetwordstartx = 0;

  TESTNULL(region, );
  TESTNULL(_font, );

  buffer = (unsigned char *)malloc(_font->totalbytes);
  TESTNULL(buffer, );

  //判断高度和宽度是否足够
  if ((region->left_top.x + _font->currentFontSize) > region->right_bottom.x ||
      (region->left_top.y + _font->currentFontSize) > region->right_bottom.y)
  {
    return;
  }

  x0 = region->left_top.x;
  while (*text)
  {
    y0 = region->left_top.y + linenum * _font->currentFontSize + _font->ygap; //计算高度 y + 行数*字符高度 + 行间距
    if (((unsigned char)text[0] >= 0xa0) &&
        ((unsigned char)text[1] >= 0xa0))
    {                                                            //打印中文
      quma = text[0] - 0xa1;                                     //求出区码
      weima = text[1] - 0xa1;                                    //求出位码
      offset = (94L * (quma) + (weima)) * _font->totalbytes;     //求出要显示的汉字在字库文件中的偏移
      fseek(_font->fpCurrentFont, offset, SEEK_SET);             //重定位文件指针
      fread(buffer, _font->totalbytes, 1, _font->fpCurrentFont); //读出该汉字的具体点阵数据,1为要读入的项数

      printHZWord(x0, y0, buffer, _font); //输出单个汉字
      lasetwordstartx = x0;

      x0 += _font->currentFontSize + _font->xgap; //偏移一个汉字宽度+字间距
      text += 2;                                  //下一个汉字
    }
    else
    { //打印字符
      if (*text == '\r' || *text == '\n')
      {                       //换行处理
        if (*(text + 1) != 0) //不是最后一个字符
          if (*(text + 1) == '\r' || (*(text + 1) == '\n'))
            text += 2; //处理\r\n情况
          else
            text++; //只有一个\r或\n
        isNewLine = TRUE;
      }
      else
      {                                               //字符
        printASC(x0, y0 - _font->ascy, *text, _font); //输出单个字符
        lasetwordstartx = x0;
        x0 += _font->ascSize + _font->xgap; //偏移一个字符宽度+字间距
        text++;                             //下一个字符
      }
    }

    if (text[0] < 0xa0)
    {
      if (x0 + _font->ascSize > region->right_bottom.x)
        isNewLine = TRUE;
    }
    else
    {
      if (x0 + _font->currentFontSize > region->right_bottom.x)
        isNewLine = TRUE;
    }

    if (isNewLine)
    {
      linenum++;
      isNewLine = FALSE;

      if ((region->left_top.y + (linenum + 1) * _font->currentFontSize + _font->ygap) > region->right_bottom.y)
      { //判断是否超高度，退出 高度截断         
        break;
      }

      x0 = region->left_top.x;
    }
  }
  free(buffer);
}
void hsvgatest()
{
  unsigned int *buffer, i;
  int width = 100, height = 100;
  FILE *fpBak = fopen("scr.bak", "wb+");
  //memory size
  buffer = (unsigned int far *)malloc(width * height * sizeof(unsigned int));
  if (buffer == NULL)
    return;

  fillRegionEx(100, 200, 300, 160, 0x4AE6);
  fillRegionEx(150, 250, 300, 160, 0x4356);

  rectangleEx(140, 240, width, height, 0x5555, 1, 1);
  savebackgroundEx(buffer, 140, 240, width, height);
  restorebackgroundEx(buffer, 400, 50, width, height);

  savebackgroundFile(fpBak, 80, 150, 200, 100);
  restorebackgroundFile(fpBak, 0, 0, 200, 100);

  fclose(fpBak);
  free(buffer);
}

/**
 * 显示一个汉字
 * @param fpfont 字库文件指针
 * @param x
 * @param y 
 * @param word 汉字
 * @param fontsize 汉字大小 16,24,32,48
 * @param color 颜色 * 
 * 
void printHZWord(int x, int y, unsigned char word[2], hfont *font)
{
  unsigned char quma, weima;                                               //定义汉字的区码和位码
  unsigned long offset;                                                    //定义汉字在字库中的偏移量
  unsigned char mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; //功能数组，用于显示汉字点阵中的亮点
  int i, j, pos;
  unsigned char *buffer;

  TESTNULL(font, );
  TESTNULL(font->fpCurrentFont, );

  buffer = (unsigned char *)malloc(font->totalbytes);

  quma = word[0] - 0xa1;                                   //求出区码
  weima = word[1] - 0xa1;                                  //求出位码
  offset = (94L * (quma) + (weima)) * font->totalbytes;    //求出要显示的汉字在字库文件中的偏移
  fseek(font->fpCurrentFont, offset, SEEK_SET);            //重定位文件指针
  fread(buffer, font->totalbytes, 1, font->fpCurrentFont); //读出该汉字的具体点阵数据,1为要读入的项数

  for (i = 0; i < font->currentFontSize; i++)
  {
    pos = font->byteperline * i;                //16*16矩阵中有每一行有两外字节
    for (j = 0; j < font->currentFontSize; j++) //一行一行地扫描，将位上为了1的点显示出来
    {
      if ((mask[j % 8] & buffer[pos + j / 8]) != NULL) //j%8只能在0—8之间循环，j/8在0，1之间循环
      {
        putpixel64k(x + j, y, font->fontcolor);
      }
    }
    y++;
  }

  free(buffer);
}*/

/**
//Slow verion
void linexx(int x1, int y1, int x2, int y2, int color) // 1
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

void linexy(int x1, int y1, int x2, int y2, int color) // 2
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
*/