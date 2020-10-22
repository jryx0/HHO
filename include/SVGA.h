#ifndef __SVGA_H__
#define __SVGA_H__

/*24位bmp图颜色结构*/
typedef struct
{
  unsigned char B; /*蓝色分量，BLUE缩写*/
  unsigned char G; /*绿色分量，GREEN缩写*/
  unsigned char R; /*红色分量，RED缩写*/
} COLORS24;

void initSVGA64k(void);
unsigned int getSVGA(void);
void selectpage(register char page);
void putpixel64k(int x, int y, unsigned int color);
unsigned int getpixel64k(int x, int y);
int Putbmp64k(int x, int y, const char *path);
/**
 * @brief 获取图片的宽度和高度
 * 
 * @return width 图片宽度
 * @return height 图片高度
 */
void getbmpWH(char *path, long *width, long *height);
#endif