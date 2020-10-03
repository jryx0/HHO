#ifndef __HHOSVGA_H_
#define __HHOSVGA_H_
#include <stdio.h>

/**
 * ������ʾҳ
 * @param page ҳ�� 
 */
void selectpage(register char page);

/**
 * �����Ļ,ʹ��char�����Ļ,����64K��int,��������ɫ�� color << 8 | color
 * ���0xFFFF,0x0000,����ɫ�ٶȸ���
 * @param color ������ɫ
 */
void clearScreen(char color);

/**
 * ������������Ļ��Χ��
 * @param x  
 * @param y ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void setStandardRegionEx(int *x, int *y, int *width, int *height);

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
 * �ָ���������savebackgroundEx��Ϊ�����
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
 * ���㺯����graphics.h �е�putpixel��1024*768*64k�³���,
 * �ڴ�ģʽ��,���㺯�� 
 * @param x 
 * @param y ������
 * @param color ��ɫ
 */
void putpixel64k(int x, int y, unsigned int color);

/**
 * ���㺯����graphics.h �е�getpixel��1024*768*64k�³���,
 * �ڴ�ģʽ��,���㺯�� 
 * @param x 
 * @param y ������
 * @return color ��ɫ
 */
unsigned int getpixel64k(int x, int y);

#endif