#ifndef __SVGA_H__
#define __SVGA_H__

/*24λbmpͼ��ɫ�ṹ*/
typedef struct
{
  unsigned char B; /*��ɫ������BLUE��д*/
  unsigned char G; /*��ɫ������GREEN��д*/
  unsigned char R; /*��ɫ������RED��д*/
} COLORS24;

void initSVGA64k(void);
unsigned int getSVGA(void);
void selectpage(register char page);
void putpixel64k(int x, int y, unsigned int color);
unsigned int getpixel64k(int x, int y);
int Putbmp64k(int x, int y, const char *path);
/**
 * @brief ��ȡͼƬ�Ŀ�Ⱥ͸߶�
 * 
 * @return width ͼƬ���
 * @return height ͼƬ�߶�
 */
void getbmpWH(char *path, long *width, long *height);
#endif