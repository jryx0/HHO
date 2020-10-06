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
 * ����ṹ
 * 
 */
typedef struct __font
{
  FILE *fpCurrentFont; //���ֵ����ֿ��ļ�ָ��
  FILE *fpASC;         //�ַ������ֿ��ļ�ָ��

  int fontcolor;
  int totalbytes; //���������ֽ���

  unsigned char currentFontSize; //�ֺ�
  unsigned char currentFontType; //����

  unsigned char byteperline; //������ÿ���ֽ���,��ʽ:(fontsize + 7)/8

  unsigned char xgap; //�ּ��
  unsigned char ygap; //�м��

  unsigned char ascScalex; //Ӣ�ķŴ���x
  unsigned char ascScaley; //Ӣ�ķŴ���y

  unsigned char ascy;    //Ӣ��λ��ƫ���� y����
  unsigned char ascSize; //Ӣ�Ŀ��

} hfont;

unsigned int RGB565(char, char, char);

/**
 * �����Ļ,ʹ��char�����Ļ,����64K��int,��������ɫ�� color << 8 | color
 * ���0xFFFF,0x0000,����ɫ�ٶȸ���
 * @param color ������ɫ
 */
void clearScreen(char color);
/**
 * ����һ����������(x1, y1), (x2,y2)��ʹx1 < x2, y1 < y2
 * ����ʹx1,x2,y1,y2����Ļ������  ���������ֽض�
 * @param *x1
 * @param *x2 
 * @param *x2
 * @param *y2
 */
void setStandardRegion(int *x1, int *y1, int *x2, int *y2); /**
 * ����һ����������,���ϵ�(x1, y1)����Ⱥ͸߶�
 * ����ʹ������������Ļ������ ���������ֽض�
 * @param *x1
 * @param *x2 
 * @param *x2
 * @param *y2
 */
void setStandardRegionEx(int *x, int *y, int *width, int *height);

/**
 * ������������㷨��������ʽ��,���ٵ���selectpage�Ĵ���
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������
 */
void fillRegion(int x1, int y1, int x2, int y2, int color);

/**
 * ������������㷨���߶ȿ����ʽ��,���ٵ���selectpage�Ĵ���
 * @param  x1 ����������
 * @param  y1 ����������
 * @param  x2 ����������
 * @param  y2 ����������
 */
void fillRegionEx(int x, int y, int width, int height, int color);

/**
 * �����ڴ����ƣ����������ڱ���С����ı������������ڴ����
 * Ҫ��width * heigth * 2 < 65536 
 * @param buffer �������滺��
 * @param x
 * @param y   ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void savebackgroundEx(unsigned int *buffer, int x, int y, int width, int height);
/**
 * ���������ڱ���С����Ļָ���������savebackgroundEx��Ϊ�����
 * Ҫ��width * heigth * 2 < 65536 
 * λ�ÿ��Բ�һ�����߶ȺͿ��Ҫһ�¡�
 * @param buffer ���汳������
 * @param x
 * @param y   ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void restorebackgroundEx(unsigned int *buffer, int x, int y, int width, int height);

/**
 * �����ڴ����ƣ��Ѵ����򱳾��������ļ��С�
 * @param fpBK �ļ�ָ�룬���ڱ��汳��
 * @param x
 * @param y   ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void savebackgroundFile(FILE *fpBK, int x, int y, int width, int height);

/**
 * �����ڴ����ƣ��ѱ������ļ������򱳾��ı����ָ�����Ļ�ϣ�
 * λ�ÿ��Բ�һ�����߶ȺͿ��Ҫһ�¡�
 * @param fpBK �ļ�ָ�룬���ڱ��汳��
 * @param x
 * @param y   ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void restorebackgroundFile(FILE *fpBK, int x, int y, int width, int height);

/**
 * ��ˮƽ��������ʽ
 * @param  x ������������
 * @param  y ������������
 * @param  width �߳���
 * @param  color ��ɫ 
 */
void linexEx(int x, int y, int width, int color);

/**
 * ��ˮƽ��������ʽ
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void linex(int x1, int y1, int x2, int y2, int color);

/**
 * ����ֱ��������ʽ
 * @param  x ������������
 * @param  y ������������
 * @param  height �߸߶�
 * @param  color ��ɫ 
 */
void lineyEx(int x, int y, int height, int color);

/**
 * ��ˮƽ��������ʽ
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void liney(int x1, int y1, int x2, int y2, int color);
/**
 * �������껭�� 
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void line(int x1, int y1, int x2, int y2, int color);

/**
 * ��������������� 
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void bar(int x1, int y1, int x2, int y2, int bkcolor);

/**
 * ��ˮƽ�ߣ��������ÿ�ȡ�����
 * @param  x ������������
 * @param  y ������������  
 * @param width ˮƽ�߳���
 * @param line_width �ߴ�ϸ
 * @param dot_style �������  
 */
void linex_styleEx(int x, int y, int width, int color, char line_width, char dot_style);
/**
 * ��ˮƽ�ߣ��������ÿ�ȡ�����
 * @param  x ������������
 * @param  y ������������  
 * @param height ˮƽ�߳���
 * @param line_width �ߴ�ϸ
 * @param dot_style �������  
 */
void liney_styleEx(int x, int y, int height, int color, char line_width, char dot_style);
/**
 * �������껭������ 
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void rectangle(int x1, int y1, int x2, int y2, int color, char line_width, char dot_style);
/**
 * �������򻭳�����  
 * @param  x ������������
 * @param  y ������������
 * @param width
 * @param height ��ȡ��߶�
 * @param  color ��ɫ
 */
void rectangleEx(int x, int y, int width, int height, int color, char line_width, char dot_style);

/**
 * ��ӡascii�� * 
 * @param fpACSII ascii�ֿ��ļ�ָ��
 * @param x
 * @param y
 * @param xsize x���ű���
 * @param ysize y���ű���
 * @param asc �ַ�
 * @param color ��ɫ
 */
void printASCII(FILE *fpACSII, int x, int y, int xsize, int ysize, char asc, int color);

/**
 * ��ӡascii��
 * @param x
 * @param y 
 * @param asc �ַ�
 * @param font ������Ϣ
 */
void printASC(int x, int y, char acs, hfont *font);

#endif