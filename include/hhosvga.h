#ifndef __HHOSVGA_H__
#define __HHOSVGA_H__

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
 * 由于内存限制，把大区域背景保存在文件中。
 * @param fpBK 文件指针，用于保存背景
 * @param x
 * @param y   左上坐标
 * @param width
 * @param height 宽度、高度
 */
void savebackgroundFile(FILE *fpBK, int x, int y, int width, int height);

/**
 * 水平线
 * @param x 
 * @param y 起始点
 * @param width 宽度 
 * 
 */
void linexEx(int x, int y, int width, int color);
void linex(int x1, int y1, int x2, int y2, int color);

/**
 * 垂直线
 * @param x 
 * @param y 起始点
 * @param height 高度 
 * 
 */
void lineyEx(int x, int y, int height, int color);
void liney(int x1, int y1, int x2, int y2, int color);

/**
 * 垂直线
 * @param x 
 * @param y 起始点
 * @param height 高度 
 * 
 */
void lineyEx(int x, int y, int height, int color);
void liney(int x1, int y1, int x2, int y2, int color);

/**
 * 画线
 * @param x1 
 * @param y1 起始点
 * @param x2
 * @param y2 结束点 
 * 
 */
void line(int x1, int y1, int x2, int y2, int color);

/**
 * 填充长方形区域
 * @param  x1 区域左坐标
 * @param  y1 区域上坐标
 * @param  x2 区域又坐标
 * @param  y2 区域下坐标
 */
void bar(int x1, int y1, int x2, int y2, int bkcolor);


/**
 * 画垂直线,可设置粗细和虚线线形
 * @param x 
 * @param y
 * @param width 水平线长度
 * @param line_width 线粗细
 * @param dot_style 间隔像素  
 */
void linex_styleEx(int x, int y, int width, int color, char line_width, char dot_style);

/**
 * 画垂直线,可设置粗细和虚线线形
 * @param height 水平线长度
 * @param line_width 线粗细
 * @param dot_style 间隔像素  
 */
void liney_styleEx(int x, int y, int height, int color, char line_width, char dot_style);

/**
 * 画长方形无填充
 * @param  x1
 * @param  y1
 * @param  x2
 * @param  y2
 */
void rectangle(int x1, int y1, int x2, int y2, int color, char line_width, char dot_style);
void rectangleEx(int x, int y, int width, int height, int color, char line_width, char dot_style);


#endif