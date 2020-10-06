#ifndef __HHOSVGA_H__
#define __HHOSVGA_H__

#include "SVGA.h"
#include <stdio.h>

typedef struct _point
{
  int x;
  int y;
} hpoint;

typedef struct _region
{
  hpoint left_top;
  hpoint right_bottom;
} hregion;

/**
 * 字体结构
 * 
 */
typedef struct __font
{
  FILE *fpCurrentFont; //汉字点阵字库文件指针
  FILE *fpASC;         //字符点阵字库文件指针

  int fontcolor;
  int totalbytes; //单个字总字节数

  unsigned char currentFontSize; //字号
  unsigned char currentFontType; //字体

  unsigned char byteperline; //单个字每行字节数,公式:(fontsize + 7)/8

  unsigned char xgap; //字间隔
  unsigned char ygap; //行间隔

  unsigned char ascScalex; //英文放大倍数x
  unsigned char ascScaley; //英文放大倍数y

  unsigned char ascy;    //英文位置偏移量 y方向
  unsigned char ascSize; //英文宽度

} hfont;

unsigned int RGB565(char, char, char);

/**
 * 清除屏幕,使用char填充屏幕,由于64K是int,故填充的颜色是 color << 8 | color
 * 填充0xFFFF,0x0000,等颜色速度更快
 * @param color 填充的颜色
 */
void clearScreen(char color);
/**
 * 设置一个矩形区域(x1, y1), (x2,y2)，使x1 < x2, y1 < y2
 * 并且使x1,x2,y1,y2在屏幕区域内  ，超出部分截断
 * @param *x1
 * @param *x2 
 * @param *x2
 * @param *y2
 */
void setStandardRegion(int *x1, int *y1, int *x2, int *y2); /**
 * 设置一个矩形区域,左上点(x1, y1)、宽度和高度
 * 并且使矩形区域在屏幕区域内 ，超出部分截断
 * @param *x1
 * @param *x2 
 * @param *x2
 * @param *y2
 */
void setStandardRegionEx(int *x, int *y, int *width, int *height);

/**
 * 快速填充区域算法（坐标形式）,减少调用selectpage的次数
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标
 */
void fillRegion(int x1, int y1, int x2, int y2, int color);

/**
 * 快速填充区域算法（高度宽度形式）,减少调用selectpage的次数
 * @param  x1 区域左坐标
 * @param  y1 区域上坐标
 * @param  x2 区域又坐标
 * @param  y2 区域下坐标
 */
void fillRegionEx(int x, int y, int width, int height, int color);

/**
 * 由于内存限制，仅仅适用于保存小面积的背景，放在在内存堆中
 * 要求width * heigth * 2 < 65536 
 * @param buffer 背景保存缓存
 * @param x
 * @param y   左上坐标
 * @param width
 * @param height 宽度、高度
 */
void savebackgroundEx(unsigned int *buffer, int x, int y, int width, int height);
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
void restorebackgroundEx(unsigned int *buffer, int x, int y, int width, int height);

/**
 * 由于内存限制，把大区域背景保存在文件中。
 * @param fpBK 文件指针，用于保存背景
 * @param x
 * @param y   左上坐标
 * @param width
 * @param height 宽度、高度
 */
void savebackgroundFile(FILE *fpBK, int x, int y, int width, int height);

/**
 * 由于内存限制，把保存在文件大区域背景的背景恢复到屏幕上，
 * 位置可以不一样，高度和宽度要一致。
 * @param fpBK 文件指针，用于保存背景
 * @param x
 * @param y   左上坐标
 * @param width
 * @param height 宽度、高度
 */
void restorebackgroundFile(FILE *fpBK, int x, int y, int width, int height);

/**
 * 画水平线坐标形式
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标
 * @param  width 线长度
 * @param  color 颜色 
 */
void linexEx(int x, int y, int width, int color);

/**
 * 画水平线坐标形式
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void linex(int x1, int y1, int x2, int y2, int color);

/**
 * 画垂直线坐标形式
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标
 * @param  height 线高度
 * @param  color 颜色 
 */
void lineyEx(int x, int y, int height, int color);

/**
 * 画水平线坐标形式
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void liney(int x1, int y1, int x2, int y2, int color);
/**
 * 根据坐标画线 
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void line(int x1, int y1, int x2, int y2, int color);

/**
 * 根据坐标填充区域 
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void bar(int x1, int y1, int x2, int y2, int bkcolor);

/**
 * 画水平线，可以设置宽度、线型
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标  
 * @param width 水平线长度
 * @param line_width 线粗细
 * @param dot_style 间隔像素  
 */
void linex_styleEx(int x, int y, int width, int color, char line_width, char dot_style);
/**
 * 画水平线，可以设置宽度、线型
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标  
 * @param height 水平线长度
 * @param line_width 线粗细
 * @param dot_style 间隔像素  
 */
void liney_styleEx(int x, int y, int height, int color, char line_width, char dot_style);
/**
 * 根据坐标画长方形 
 * @param  x1 区域左上坐标
 * @param  y1 区域右上坐标
 * @param  x2 区域左下坐标
 * @param  y2 区域右下坐标 
 * @param  color 颜色
 */
void rectangle(int x1, int y1, int x2, int y2, int color, char line_width, char dot_style);
/**
 * 根据区域画长方形  
 * @param  x 区域左上坐标
 * @param  y 区域右上坐标
 * @param width
 * @param height 宽度、高度
 * @param  color 颜色
 */
void rectangleEx(int x, int y, int width, int height, int color, char line_width, char dot_style);

/**
 * 打印ascii码 * 
 * @param fpACSII ascii字库文件指针
 * @param x
 * @param y
 * @param xsize x缩放比例
 * @param ysize y缩放比例
 * @param asc 字符
 * @param color 颜色
 */
void printASCII(FILE *fpACSII, int x, int y, int xsize, int ysize, char asc, int color);

/**
 * 打印ascii码
 * @param x
 * @param y 
 * @param asc 字符
 * @param font 字体信息
 */
void printASC(int x, int y, char acs, hfont *font);

#endif