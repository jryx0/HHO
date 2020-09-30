#ifndef __HHOSVGA_H__
#define __HHOSVGA_H__
#include "macrodef.h"



/**********************************************************
Function：		GetSVGA
Description：	获取SVGA显示模式号bx。摘录常用的模式号如下：
模式号		分辨率		颜色数		颜色位定义
0x101		640*480		256				-
0x103		800*600		256				-
0x104		1024*768	16				-
0x105		1024*768	256				-
0x110		640*480		32K			1:5:5:5
0x111		640*480		64K			5:6:5
0x112		640*480		16.8M		8:8:8
0x113		800*600		32K			1:5:5:5
0x114		800*600		64K			5:6:5
0x115		800*600		16.8M		8:8:8
0x116		1024*768	32K			1:5:5:5
0x117		1024*768	64K			5:6:5
0x118		1024*768	16.8M		8:8:8
Input：			None
Output：		初始化失败时会屏幕输出错误号。
Return：		unsigned int graph_regs.x.bx	显示模式号
**********************************************************/
void initSVGA64k(void);
unsigned int getSVGA(void);

/**********************************************************
Function：		Selectpage
Description：	带判断功能的换页函数，解决读写显存时跨段寻址问题
Input：			register char page		需要换到的页面号
Output：		None
Return：		None
**********************************************************/
void selectpage(register char page);

/**********************************************************
Function：		Putbmp64k
Description：	24位非压缩bmp图定位显示函数。
                                只支持24位非压缩bmp图，宽度像素最大允许为1024！
                                其余bmp类型均不支持！
                                仅在0x117模式下使用！
                                为了简化，没有设置文件类型检测功能检测功能，请勿读入不合要求的文件！
Input：			int x		图片左上角的横坐标（屏幕参考系）
                                int y		图片左上角的纵坐标（屏幕参考系）
                                const char * path	bmp图片路径
Output：		屏幕上显示图片
Return：		0	显示成功
                        -1	显示失败
**********************************************************/

void line(int x1, int y1, int x2, int y2, int color);
void bar(int x1, int y1, int x2, int y2, long bkcolor);
void rectangle(int x1, int y1, int x2, int y2, long color);
void putpixel64k(int x, int y, unsigned int color);
void clearScreen(int color);
#endif