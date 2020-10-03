#ifndef __HHOSVGA_H_
#define __HHOSVGA_H_
#include <stdio.h>

/**
 * 控制显示页
 * @param page 页号 
 */
void selectpage(register char page);

/**
 * 清除屏幕,使用char填充屏幕,由于64K是int,故填充的颜色是 color << 8 | color
 * 填充0xFFFF,0x0000,等颜色速度更快
 * @param color 填充的颜色
 */
void clearScreen(char color);

/**
 * 控制区域在屏幕范围内
 * @param x  
 * @param y 左上坐标
 * @param width
 * @param height 宽度、高度
 */
void setStandardRegionEx(int *x, int *y, int *width, int *height);

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
 * 恢复背景，和savebackgroundEx互为逆操作
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
 * 画点函数，graphics.h 中的putpixel在1024*768*64k下出错,
 * 在此模式下,画点函数 
 * @param x 
 * @param y 点坐标
 * @param color 颜色
 */
void putpixel64k(int x, int y, unsigned int color);

/**
 * 画点函数，graphics.h 中的getpixel在1024*768*64k下出错,
 * 在此模式下,画点函数 
 * @param x 
 * @param y 点坐标
 * @return color 颜色
 */
unsigned int getpixel64k(int x, int y);

#endif