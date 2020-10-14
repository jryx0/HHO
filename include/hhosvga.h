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

  unsigned char firstline;
  //unsigned char underline;

} hfont;

unsigned int RGB565(int r, int g, int b);
/**
 * 清除屏幕,使用char填充屏幕,由于64K是int,故填充的颜色是 color << 8 | color
 * 填充0xFFFF,0x0000,等颜色速度更快
 * @param color 填充的颜色
 */
void clearScreen(char color);

/**
 * 清除区域,使用char填充屏幕,由于64K是int,故填充的颜色是 color << 8 | color
 * 填充0xFFFF,0x0000,等颜色速度更快
 * @param color 填充的颜色
 */
void clearRegion(int x1, int y1, int x2, int y2, char color);

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
 * 显示一个ascii码
 * @param x
 * @param y 
 * @param asc 字符
 * @param font 字体信息
 */
void printASC(int x, int y, char acs, hfont *font);

/**
 * 显示一个汉字
 * @param fpfont 字库文件指针
 * @param x
 * @param y 
 * @param buffer 字库数据
 * @param fontsize 汉字大小 16,24,32,48
 * @param color 颜色 */
void printHZWord(int x, int y, unsigned char *buffer, hfont *font);

/**
 * @version v1 输出文本函数版本1
 * @brief  在一个行内输出，不截断(回车换行符也不换行，忽略)。字符显示格式包括字体、字号、字间距、行间距等等在_font中设置，
 * 字符串可以是中英文混合。 非打印字符以空格替代
 * 
 * @param x
 * @param y  要显示起始坐标
 * @param text 字符串
 * @param _font 字体设置
 * @return 无
 */
void printTextLineXY(int x, int y, char *text, hfont *_font);

/**
 * @version v2 输出文本函数版本2 
 * @author 
 * @brief 在一个区域内输出字符串，字符显示格式包括字体、字号、字间距、行间距等等在_font中设置，
 * 字符串可以是中英文混合。能首行空两格，根据区域高度和宽度，自动换行、截断。非打印字符以空格替代，
 * 单个回车、换行或回车换行表示新行。
 * 
 * @param region  要显示的区域
 * @param text 字符串
 * @param _font 字体设置
 *  @return 无
 */
void printTextEx(hregion *region, char *text, hfont *_font);

/**
 * @version v3 输出文本函数版本3
 * @author 
 * @brief 读取文件，在一个区域内输出字符串，字符显示格式包括字体、字号、字间距、行间距等等在_font中设置，
 * 字符串可以是中英文混合。能首行空两格，根据区域高度和宽度，自动换行、截断
 * @param region  要显示的区域  
 * @param fp 文件指针
 * @param _font 字体设置
 * @return 无
 */
void printTextFile(hregion *region, FILE *fp, hfont *_font);

/**
 * 获取默认字体信息
 * 调用完后必须hfont指针，调用freeFont
 * @param type 在marcodef中定义 
 * @param size 字号 16、24、32、48 
 * @param color 颜色
 */
hfont *getFont(int type, int size, int color);
/**
 * 计算字体信息
 * 
 * 
 */
void calcFontSetting(hfont *font);

/**
 * 获取汉字字库文件
 * @param type 在marcodef中定义 
 * @param size 字号 16、24、32、48
 * 
 * @return 字库文件指针
 */
FILE *getFontFile(int type, int size);
/**
 * @brief 获取一个字符串在屏幕上显示需要多少个像素，若含有回车换行符，
 * 则以回车换行符为结束。
 * 
 * @param text 文本串
 * @param _f 字体信息
 */
int calcPrintTextLenght(unsigned char *text, hfont *_f);

void printTextEx4(hregion *region, char *text, hfont *_font, int *x, int *y);
/**
 * 释放font信息
 * 
 */
void freeFont(hfont *_f);
#endif