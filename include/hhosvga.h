#ifndef __HHOSVGA_H__
#define __HHOSVGA_H__

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
 * �����ڴ����ƣ��Ѵ����򱳾��������ļ��С�
 * @param fpBK �ļ�ָ�룬���ڱ��汳��
 * @param x
 * @param y   ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void savebackgroundFile(FILE *fpBK, int x, int y, int width, int height);

/**
 * ˮƽ��
 * @param x 
 * @param y ��ʼ��
 * @param width ��� 
 * 
 */
void linexEx(int x, int y, int width, int color);
void linex(int x1, int y1, int x2, int y2, int color);

/**
 * ��ֱ��
 * @param x 
 * @param y ��ʼ��
 * @param height �߶� 
 * 
 */
void lineyEx(int x, int y, int height, int color);
void liney(int x1, int y1, int x2, int y2, int color);

/**
 * ��ֱ��
 * @param x 
 * @param y ��ʼ��
 * @param height �߶� 
 * 
 */
void lineyEx(int x, int y, int height, int color);
void liney(int x1, int y1, int x2, int y2, int color);

/**
 * ����
 * @param x1 
 * @param y1 ��ʼ��
 * @param x2
 * @param y2 ������ 
 * 
 */
void line(int x1, int y1, int x2, int y2, int color);

/**
 * ��䳤��������
 * @param  x1 ����������
 * @param  y1 ����������
 * @param  x2 ����������
 * @param  y2 ����������
 */
void bar(int x1, int y1, int x2, int y2, int bkcolor);


/**
 * ����ֱ��,�����ô�ϸ����������
 * @param x 
 * @param y
 * @param width ˮƽ�߳���
 * @param line_width �ߴ�ϸ
 * @param dot_style �������  
 */
void linex_styleEx(int x, int y, int width, int color, char line_width, char dot_style);

/**
 * ����ֱ��,�����ô�ϸ����������
 * @param height ˮƽ�߳���
 * @param line_width �ߴ�ϸ
 * @param dot_style �������  
 */
void liney_styleEx(int x, int y, int height, int color, char line_width, char dot_style);

/**
 * �������������
 * @param  x1
 * @param  y1
 * @param  x2
 * @param  y2
 */
void rectangle(int x1, int y1, int x2, int y2, int color, char line_width, char dot_style);
void rectangleEx(int x, int y, int width, int height, int color, char line_width, char dot_style);


#endif