#ifndef __HHOSVGA_H__
#define __HHOSVGA_H__
#include "macrodef.h"



/**********************************************************
Function��		GetSVGA
Description��	��ȡSVGA��ʾģʽ��bx��ժ¼���õ�ģʽ�����£�
ģʽ��		�ֱ���		��ɫ��		��ɫλ����
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
Input��			None
Output��		��ʼ��ʧ��ʱ����Ļ�������š�
Return��		unsigned int graph_regs.x.bx	��ʾģʽ��
**********************************************************/
void initSVGA64k(void);
unsigned int getSVGA(void);

/**********************************************************
Function��		Selectpage
Description��	���жϹ��ܵĻ�ҳ�����������д�Դ�ʱ���Ѱַ����
Input��			register char page		��Ҫ������ҳ���
Output��		None
Return��		None
**********************************************************/
void selectpage(register char page);

/**********************************************************
Function��		Putbmp64k
Description��	24λ��ѹ��bmpͼ��λ��ʾ������
                                ֻ֧��24λ��ѹ��bmpͼ����������������Ϊ1024��
                                ����bmp���;���֧�֣�
                                ����0x117ģʽ��ʹ�ã�
                                Ϊ�˼򻯣�û�������ļ����ͼ�⹦�ܼ�⹦�ܣ�������벻��Ҫ����ļ���
Input��			int x		ͼƬ���Ͻǵĺ����꣨��Ļ�ο�ϵ��
                                int y		ͼƬ���Ͻǵ������꣨��Ļ�ο�ϵ��
                                const char * path	bmpͼƬ·��
Output��		��Ļ����ʾͼƬ
Return��		0	��ʾ�ɹ�
                        -1	��ʾʧ��
**********************************************************/

void line(int x1, int y1, int x2, int y2, int color);
void bar(int x1, int y1, int x2, int y2, long bkcolor);
void rectangle(int x1, int y1, int x2, int y2, long color);
void putpixel64k(int x, int y, unsigned int color);
void clearScreen(int color);
#endif