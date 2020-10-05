#include "hglobal.h"
#include "hhosvga.h"
#include "SVGA.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

/**
 * �����Ļ,ʹ��char�����Ļ,����64K��int,��������ɫ�� color << 8 | color
 * ���0xFFFF,0x0000,����ɫ�ٶȸ���
 * @param color ������ɫ
 */
void clearScreen(char color)
{
  int i;
  unsigned int pages = 0, pagesize = 0;

  /*�Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  pages = 24; //1024x768 = 24, 800*600 = 14
  pagesize = 1024 * 32 * 2;
  for (i = 0; i <= pages; i++)
  {
    selectpage(i);
    memset(video_buffer, color, pagesize - 1);
    //memset(video_buffer + 0x4000, color, pagesize); // 16 << 10
  }
}

void swap(int *a, int *b)
{
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

/**
 * ����һ����������ʹx1 < x2, y1 < y2
 * ����ʹx1,x2,y1,y2����Ļ������ 
 * 
 */
void setStandardRegion(int *x1, int *y1, int *x2, int *y2)
{
  if (*x1 < 0)
    *x1 = 0;

  if (*x1 > SCR_WIDTH)
    *x1 = SCR_WIDTH;

  if (*x2 < 0)
    *x2 = 0;

  if (*x2 > SCR_WIDTH)
    *x2 = SCR_WIDTH;

  if (*y1 < 0)
    *y1 = 0;

  if (*y1 > SCR_HEIGHT)
    *y1 = SCR_HEIGHT;

  if (*y2 < 0)
    *y2 = 0;

  if (*y2 > SCR_HEIGHT)
    *y2 = SCR_HEIGHT;

  if (*x1 > *x2)
    swap(x1, x2);

  if (*y1 > *y2)
    swap(y1, y2);
}
void setStandardRegionEx(int *x, int *y, int *width, int *height)
{
  if (*x < 0)
    *x = 0;
  if (*x > SCR_WIDTH)
    *x = SCR_WIDTH;

  if (*y < 0)
    *y = 0;
  if (*y > SCR_HEIGHT)
    *y = SCR_HEIGHT;

  if (*x + *width > SCR_WIDTH)
    *width = SCR_WIDTH - *x - 2;
  if (*y + *height > SCR_HEIGHT)
    *height = SCR_HEIGHT - *y - 2;
}
/**
 * ������������㷨,���ٵ���selectpage�Ĵ���
 * @param  x1 ����������
 * @param  y1 ����������
 * @param  x2 ����������
 * @param  y2 ����������
 */
void fillRegion(int x1, int y1, int x2, int y2, int color)
{
  int i, j, width, height;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�

  setStandardRegion(&x1, &y1, &x2, &y2);
  width = x2 - x1;
  height = y2 - y1;

  for (j = 0; j < height; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y1 + j) << 10) + x1;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);
    for (i = 0; i < width; i++)
    {
      video_buffer[offest + i] = color;
    }
  }
}
void fillRegionEx(int x, int y, int width, int height, int color)
{
  int i, j;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�
  setStandardRegionEx(&x, &y, &width, &height);

  for (j = 0; j < height; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);
    for (i = 0; i < width; i++)
    {
      video_buffer[offest + i] = color;
    }
  }
}

/**
 * �����ڴ����ƣ����������ڱ���С����ı������������ڴ����
 * Ҫ��width * heigth * 2 < 65536 
 * @param buffer �������滺��
 * @param x
 * @param y   ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void savebackgroundEx(unsigned int *buffer, int x, int y, int width, int height)
{
  int j;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�

  if (buffer == NULL)
    return;
  setStandardRegionEx(&x, &y, &width, &height);

  for (j = 0; j < height; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);

    memcpy(buffer + j * width, &video_buffer[offest], width * 2); //width * 2, int ��λ
  }
}

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
void restorebackgroundEx(unsigned int *buffer, int x, int y, int width, int height)
{
  int j;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�

  if (buffer == NULL)
    return;

  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < height; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);

    memcpy(&video_buffer[offest], buffer + j * width, width * 2); //width * 2, int ��λ
  }
}
/**
 * �����ڴ����ƣ��Ѵ����򱳾��������ļ��С�
 * @param fpBK �ļ�ָ�룬���ڱ��汳��
 * @param x
 * @param y   ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void savebackgroundFile(FILE *fpBK, int x, int y, int width, int height)
{
  int j;

  unsigned char page;       // Ҫ�л���ҳ���
  unsigned long int offest; // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  setStandardRegionEx(&x, &y, &width, &height);

  for (j = 0; j < height; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);
    fwrite(&video_buffer[offest], 1, width * 2, fpBK);
  }
  fflush(fpBK);
  fclose(fpBK);
}

/**
 * �����ڴ����ƣ��ѱ������ļ������򱳾��ı����ָ�����Ļ�ϣ�
 * λ�ÿ��Բ�һ�����߶ȺͿ��Ҫһ�¡�
 * @param fpBK �ļ�ָ�룬���ڱ��汳��
 * @param x
 * @param y   ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void restorebackgroundFile(FILE *fpBK, int x, int y, int width, int height)
{
  int j;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�

  if (fpBK == NULL)
    return;
  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < height; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);

    fread(&video_buffer[offest], 1, width * 2, fpBK);
  }
  fclose(fpBK);
}

/**
 * ˮƽ��
 * @param x 
 * @param y ��ʼ��
 * @param width ��� 
 * 
 */
void linexEx(int x, int y, int width, int color)
{
  int i, height = 0;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�

  setStandardRegionEx(&x, &y, &width, &height);
  /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
  offest = ((unsigned long int)(y) << 10) + x;
  page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
  selectpage(page);
  for (i = 0; i < width; i++)
  {
    video_buffer[offest + i] = color;
  }
}
void linex(int x1, int y1, int x2, int y2, int color)
{
  //setStandardRegion(&x1, &y1, &x2, &y2);
  linexEx(x1, y1, x2 - x1, color);
}

/**
 * ��ֱ��
 * @param x 
 * @param y ��ʼ��
 * @param height �߶� 
 * 
 */
void lineyEx(int x, int y, int height, int color)
{
  int j, width = 0;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�
  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < height; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);

    video_buffer[offest] = color;
  }
}
void liney(int x1, int y1, int x2, int y2, int color)
{
  //setStandardRegion(&x1, &y1, &x2, &y2);
  lineyEx(x1, y1, y2 - y1, color);
}

/**
 * ����
 * @param x1 
 * @param y1 ��ʼ��
 * @param x2
 * @param y2 ������ 
 * 
 */
void line(int x1, int y1, int x2, int y2, int color)
{
  int x, y, dx, dy, s1, s2, p, temp, interchange, i;

  if (x1 == x2)
  {
    liney(x1, y1, x2, y2, color);
    return;
  }
  else if (y1 == y2)
  {
    linex(x1, y1, x2, y2, color);
    return;
  }

  x = x1;
  y = y1;
  dx = abs(x2 - x1);
  dy = abs(y2 - y1);

  if (x2 > x1)
    s1 = 1;
  else
    s1 = -1;

  if (y2 > y1)
    s2 = 1;
  else
    s2 = -1;

  if (dy > dx)
  {
    temp = dx;
    dx = dy;
    dy = temp;
    interchange = 1;
  }
  else
    interchange = 0;

  p = 2 * dy - dx;
  for (i = 1; i <= dx; i++)
  {
    putpixel64k(x, y, color);
    if (p >= 0)
    {
      if (interchange == 0)
        y = y + s2;
      else
        x = x + s1;
      p = p - 2 * dx;
    }
    if (interchange == 0)
      x = x + s1;
    else
      y = y + s2;
    p = p + 2 * dy;
  }
  return;
}

/**
 * ��䳤��������
 * @param  x1 ����������
 * @param  y1 ����������
 * @param  x2 ����������
 * @param  y2 ����������
 */
void bar(int x1, int y1, int x2, int y2, int bkcolor)
{
  fillRegion(x1, y1, x2, y2, bkcolor);
}

/**
 * ����ֱ��,�����ô�ϸ����������
 * @param x 
 * @param y
 * @param width ˮƽ�߳���
 * @param line_width �ߴ�ϸ
 * @param dot_style �������  
 */
void linex_styleEx(int x, int y, int width, int color, char line_width, char dot_style)
{
  int i, j, height = 0;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�
  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < line_width; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);
    for (i = 0; i < width; i++)
    {
      video_buffer[offest + i] = color;

      if ((dot_style > 1) && (i % dot_style == 0))
        i += 2;
    }
  }
}

/**
 * ����ֱ��,�����ô�ϸ����������
 * @param height ˮƽ�߳���
 * @param line_width �ߴ�ϸ
 * @param dot_style �������  
 */
void liney_styleEx(int x, int y, int height, int color, char line_width, char dot_style)
{
  int i, j, width = 0;

  unsigned char page;                                                     // Ҫ�л���ҳ���
  unsigned long int offest;                                               // ��Ӧ�Դ��ַƫ����
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L; // �Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�
  setStandardRegionEx(&x, &y, &width, &height);
  for (i = 0; i < line_width; i++)
    for (j = 0; j < height; j++)
    {
      /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
      offest = ((unsigned long int)(y + j) << 10) + x + i;
      page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
      selectpage(page);

      video_buffer[offest] = color;

      if ((dot_style > 1) && (j % dot_style == 0))
        j += 2;
    }
}

/**
 * �������������
 * @param  x1
 * @param  y1
 * @param  x2
 * @param  y2
 */
void rectangle(int x1, int y1, int x2, int y2, int color, char line_width, char dot_style)
{
  //setStandardRegion(&x1, &y1, &x2, &y2);
  linex_styleEx(x1, y1, x2 - x1, color, line_width, dot_style);
  liney_styleEx(x2, y1, y2 - y1, color, line_width, dot_style);
  linex_styleEx(x1, y2, x2 - x1, color, line_width, dot_style);
  liney_styleEx(x1, y1, y2 - y1, color, line_width, dot_style);
}
void rectangleEx(int x, int y, int width, int height, int color, char line_width, char dot_style)
{
  //setStandardRegionEx(x, y, width, height);
  linex_styleEx(x, y, width, color, line_width, dot_style);
  liney_styleEx(x + width, y, height, color, line_width, dot_style);
  linex_styleEx(x, y + height, width, color, line_width, dot_style);
  liney_styleEx(x, y, height, color, line_width, dot_style);
}
/**
 *Slow verion 
 
void linexx(int x1, int y1, int x2, int y2, int color) // 1
{
  int i;
  if (x1 >= SCR_WIDTH)
    x1 = SCR_WIDTH - 1;

  if (x2 >= SCR_WIDTH)
    x2 = SCR_WIDTH - 1;

  if (y1 >= SCR_HEIGHT)
    y1 = SCR_HEIGHT - 1;

  if (y2 >= SCR_HEIGHT)
    y2 = SCR_HEIGHT - 1;

  if (x1 < 0)
    x1 = 0;

  if (x2 < 0)
    x2 = 0;

  if (y1 < 0)
    y1 = 0;

  if (y2 < 0)
    y2 = 0;

  if (y1 != y2 || x1 >= x2)
    return (0);

  for (i = x1; i <= x2; i++)
    putpixel64k(i, y1, color);
  return 0;
}

void linexy(int x1, int y1, int x2, int y2, int color) // 2
{
  int i;
  if (x1 >= SCR_WIDTH)
    x1 = SCR_WIDTH - 1;

  if (x2 >= SCR_WIDTH)
    x2 = SCR_WIDTH - 1;

  if (y1 >= SCR_HEIGHT)
    y1 = SCR_HEIGHT - 1;

  if (y2 >= SCR_HEIGHT)
    y2 = SCR_HEIGHT - 1;

  if (x1 < 0)
    x1 = 0;

  if (x2 < 0)
    x2 = 0;

  if (y1 < 0)
    y1 = 0;

  if (y2 < 0)
    y2 = 0;

  if (x1 != x2 || y1 >= y2)
    return (0);
  for (i = y1; i <= y2; i++)
    putpixel64k(x1, i, color);
  return 0;
}
*/