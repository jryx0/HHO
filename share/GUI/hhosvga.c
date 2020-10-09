#include "macrodef.h"
#include "hhosvga.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mem.h>

unsigned int RGB565(int r, int g, int b)
{ // ����
  return (r << 11) + (g << 5) + (b << 0);
}

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
  pagesize = 1024 * 32;
  for (i = 0; i < pages; i++)
  {
    selectpage(i);
    memset(video_buffer, color, pagesize);
    memset(video_buffer + 0x4000, color, pagesize); // 16 << 10
  }
}
/**
 * �������,ʹ��char�����Ļ,����64K��int,��������ɫ�� color << 8 | color
 * ���0xFFFF,0x0000,����ɫ�ٶȸ���
 * @param color ������ɫ
 */
void clearRegion(int x1, int y1, int x2, int y2, char color)
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
    memset(video_buffer + offest, color, width * 2); //һ�����������ֽ� width* 2
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
 * ����һ����������(x1, y1), (x2,y2)��ʹx1 < x2, y1 < y2
 * ����ʹx1,x2,y1,y2����Ļ������  ���������ֽض�
 * @param *x1
 * @param *x2 
 * @param *x2
 * @param *y2
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

/**
 * ����һ����������,���ϵ�(x1, y1)����Ⱥ͸߶�
 * ����ʹ������������Ļ������ ���������ֽض�
 * @param *x1
 * @param *x2 
 * @param *x2
 * @param *y2
 */
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
 * ������������㷨��������ʽ��,���ٵ���selectpage�Ĵ���
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������
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

/**
 * ������������㷨���߶ȿ����ʽ��,���ٵ���selectpage�Ĵ���
 * @param  x ����������
 * @param  y ����������
 * @param  width ����������
 * @param  height ����������
 */
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
 * ���������ڱ���С����Ļָ���������savebackgroundEx��Ϊ�����
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

  fseek(fpBK, 0, SEEK_SET);
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

  fseek(fpBK, 0, SEEK_SET);
  setStandardRegionEx(&x, &y, &width, &height);
  for (j = 0; j < height; j++)
  {
    /* �����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ���� */
    offest = ((unsigned long int)(y + j) << 10) + x;
    page = offest >> 15; /* 32k����һ��ҳ������32k������㷨 */
    selectpage(page);

    fread(&video_buffer[offest], 1, width * 2, fpBK);
  }
}

/**
 * ��ˮƽ��������ʽ
 * @param  x ������������
 * @param  y ������������
 * @param  width �߳���
 * @param  color ��ɫ 
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
/**
 * ��ˮƽ��������ʽ
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void linex(int x1, int y1, int x2, int y2, int color)
{
  if (y1 == y2)
    linexEx(x1, y1, x2 - x1, color);
}

/**
 * ����ֱ��������ʽ
 * @param  x ������������
 * @param  y ������������
 * @param  height �߸߶�
 * @param  color ��ɫ 
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

/**
 * ��ˮƽ��������ʽ
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void liney(int x1, int y1, int x2, int y2, int color)
{
  if (x1 == x2)
    lineyEx(x1, y1, y2 - y1, color);
}

/**
 * �������껭�� 
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
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
 * ��������������� 
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void bar(int x1, int y1, int x2, int y2, int bkcolor)
{
  fillRegion(x1, y1, x2, y2, bkcolor);
}

/**
 * ��ˮƽ�ߣ��������ÿ�ȡ�����
 * @param  x ������������
 * @param  y ������������  
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
 * ��ˮƽ�ߣ��������ÿ�ȡ�����
 * @param  x ������������
 * @param  y ������������  
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
 * �������껭������ 
 * @param  x1 ������������
 * @param  y1 ������������
 * @param  x2 ������������
 * @param  y2 ������������ 
 * @param  color ��ɫ
 */
void rectangle(int x1, int y1, int x2, int y2, int color, char line_width, char dot_style)
{
  //setStandardRegion(&x1, &y1, &x2, &y2);
  linex_styleEx(x1, y1, x2 - x1, color, line_width, dot_style);
  liney_styleEx(x2, y1, y2 - y1, color, line_width, dot_style);
  linex_styleEx(x1, y2, x2 - x1, color, line_width, dot_style);
  liney_styleEx(x1, y1, y2 - y1, color, line_width, dot_style);
}
/**
 * �������򻭳�����  
 * @param  x ������������
 * @param  y ������������
 * @param width
 * @param height ��ȡ��߶�
 * @param  color ��ɫ
 */
void rectangleEx(int x, int y, int width, int height, int color, char line_width, char dot_style)
{
  //setStandardRegionEx(x, y, width, height);
  linex_styleEx(x, y, width, color, line_width, dot_style);
  liney_styleEx(x + width, y, height, color, line_width, dot_style);
  linex_styleEx(x, y + height, width, color, line_width, dot_style);
  liney_styleEx(x, y, height, color, line_width, dot_style);
}

/**
 * ��ʾһ��ascii��
 * @param x
 * @param y 
 * @param asc �ַ�
 * @param font ������Ϣ
 */
void printASC(int x, int y, char acs, hfont *font)
{
  int i, j, k, m;
  unsigned char buffer[16];

  if (!isprint(acs)) //�Ǵ�ӡ�ַ�������
    return;

  TESTNULLVOID(font);
  TESTNULLVOID(font->fpASC);

  fseek(font->fpASC, acs * 16L, SEEK_SET);
  fread(buffer, 16, 1, font->fpASC);

  for (i = 0; i < 16; i++)
    for (m = 1; m <= font->ascScaley; m++) //y��������
      for (j = 0; j < 8; j++)
        for (k = 1; k <= font->ascScalex; k++) //x��������
          if ((buffer[i] >> 7 - j) & 1)
            putpixel64k(x + j * font->ascScalex + k, y + m + i * font->ascScaley, font->fontcolor);
}

/**
 * ��ʾһ������
 * @param fpfont �ֿ��ļ�ָ��
 * @param x
 * @param y 
 * @param buffer �ֿ�����
 * @param fontsize ���ִ�С 16,24,32,48
 * @param color ��ɫ */
void printHZWord(int x, int y, unsigned char *buffer, hfont *font)
{
  unsigned char mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; //�������飬������ʾ���ֵ����е�����
  int i, j, pos;

  TESTNULL(font, );
  TESTNULL(font->fpCurrentFont, );

  for (i = 0; i < font->currentFontSize; i++)
  {
    pos = font->byteperline * i;                //16*16��������ÿһ���������ֽ�
    for (j = 0; j < font->currentFontSize; j++) //һ��һ�е�ɨ�裬��λ��Ϊ��1�ĵ���ʾ����
    {
      if ((mask[j % 8] & buffer[pos + j / 8]) != NULL) //j%8ֻ����0��8֮��ѭ����j/8��0��1֮��ѭ��
      {
        putpixel64k(x + j, y, font->fontcolor);
      }
    }
    y++;
  }
}

/**
 * @version v1 ����ı������汾1
 * @brief  ��һ��������������ض�(�س����з�Ҳ�����У�����)���ַ���ʾ��ʽ�������塢�ֺš��ּ�ࡢ�м��ȵ���_font�����ã�
 * �ַ�����������Ӣ�Ļ�ϡ� �Ǵ�ӡ�ַ��Կո����
 * 
 * @param region  Ҫ��ʾ������
 * @param text �ַ���
 * @param _font ��������
 * @return ��
 
void printTextLine(hregion *region, char *text, hfont *_font)
{

  int x0, y0;                //ÿ��������ʼ��
  unsigned char quma, weima; //���庺�ֵ������λ��
  unsigned long offset;
  unsigned char *buffer;
  int linenum = 0;
  //char isNewLine = FALSE;

  TESTNULLVOID(region);
  TESTNULLVOID(_font);

  buffer = (unsigned char *)malloc(_font->totalbytes);
  TESTNULL(buffer, );

  x0 = region->left_top.x;
  while (*text)
  {

    y0 = region->left_top.y;

    if (((unsigned char)text[0] >= 0xa0) &&
        ((unsigned char)text[1] >= 0xa0))
    {                                                            //��ӡ����                                                            //����
      quma = text[0] - 0xa1;                                     //�������
      weima = text[1] - 0xa1;                                    //���λ��
      offset = (94L * (quma) + (weima)) * _font->totalbytes;     //���Ҫ��ʾ�ĺ������ֿ��ļ��е�ƫ��
      fseek(_font->fpCurrentFont, offset, SEEK_SET);             //�ض�λ�ļ�ָ��
      fread(buffer, _font->totalbytes, 1, _font->fpCurrentFont); //�����ú��ֵľ����������,1ΪҪ���������

      printHZWord(x0, y0, buffer, _font); //�����������

      x0 += _font->currentFontSize + _font->xgap; //ƫ��һ�����ֿ��+�ּ��
      text += 2;                                  //��һ������
    }
    else
    {                                               //��ӡ�ַ�
      printASC(x0, y0 - _font->ascy, *text, _font); //��������ַ�,�Ǵ�ӡ�ַ��ÿո����
      x0 += _font->ascSize + _font->xgap;           //ƫ��һ���ַ����+�ּ��
      text++;                                       //��һ���ַ�
    }
  }
  free(buffer);
}
*/

/**
 * @version v1 ����ı������汾1
 * @brief  ��һ��������������ض�(�س����з�Ҳ�����У�����)���ַ���ʾ��ʽ�������塢�ֺš��ּ�ࡢ�м��ȵ���_font�����ã�
 * �ַ�����������Ӣ�Ļ�ϡ� �Ǵ�ӡ�ַ��Կո����
 * 
 * @param x
 * @param y  Ҫ��ʾ��ʼ����
 * @param text �ַ���
 * @param _font ��������
 * @return ��
 */
void printTextLineXY(int x, int y, char *text, hfont *_font)
{

  int x0, y0;                //ÿ��������ʼ��
  unsigned char quma, weima; //���庺�ֵ������λ��
  unsigned long offset;
  unsigned char *buffer;
  int linenum = 0;
  //char isNewLine = FALSE;

  TESTNULLVOID(_font);

  buffer = (unsigned char *)malloc(_font->totalbytes);
  TESTNULL(buffer, );

  x0 = x;
  while (*text)
  {
    y0 = y;

    if (((unsigned char)text[0] >= 0xa0) &&
        ((unsigned char)text[1] >= 0xa0))
    {                                                            //��ӡ����                                                            //����
      quma = text[0] - 0xa1;                                     //�������
      weima = text[1] - 0xa1;                                    //���λ��
      offset = (94L * (quma) + (weima)) * _font->totalbytes;     //���Ҫ��ʾ�ĺ������ֿ��ļ��е�ƫ��
      fseek(_font->fpCurrentFont, offset, SEEK_SET);             //�ض�λ�ļ�ָ��
      fread(buffer, _font->totalbytes, 1, _font->fpCurrentFont); //�����ú��ֵľ����������,1ΪҪ���������

      printHZWord(x0, y0, buffer, _font); //�����������

      x0 += _font->currentFontSize + _font->xgap; //ƫ��һ�����ֿ��+�ּ��
      text += 2;                                  //��һ������
    }
    else
    {                                               //��ӡ�ַ�
      printASC(x0, y0 - _font->ascy, *text, _font); //��������ַ�,�Ǵ�ӡ�ַ��ÿո����
      x0 += _font->ascSize + _font->xgap;           //ƫ��һ���ַ����+�ּ��
      text++;                                       //��һ���ַ�
    }
  }
  free(buffer);
}

/**
 * @version v2 ����ı������汾2 
 * @author 
 * @brief ��һ������������ַ������ַ���ʾ��ʽ�������塢�ֺš��ּ�ࡢ�м��ȵ���_font�����ã�
 * �ַ�����������Ӣ�Ļ�ϡ������п����񣬸�������߶ȺͿ�ȣ��Զ����С��ضϡ��Ǵ�ӡ�ַ��Կո������
 * �����س������л�س����б�ʾ���С�
 * 
 * @param region  Ҫ��ʾ������
 * @param text �ַ���
 * @param _font ��������
 *  @return ��
 */
void printTextEx(hregion *region, char *text, hfont *_font)
{
  int x0, y0;                //ÿ��������ʼ��
  unsigned char quma, weima; //���庺�ֵ������λ��
  unsigned long offset;
  unsigned char *buffer;
  int linenum = 0;
  char isNewLine = FALSE;

  char isFirstLine = TRUE; //�Ƿ��Ƕ������У����п������ֿ��

  TESTNULLVOID(region);
  TESTNULLVOID(_font);

  buffer = (unsigned char *)malloc(_font->totalbytes);
  TESTNULLVOID(buffer);

  //�жϸ߶ȺͿ���Ƿ��㹻
  // if ((region->left_top.x + _font->currentFontSize) > region->right_bottom.x ||
  //     (region->left_top.y + _font->currentFontSize) > region->right_bottom.y)
  // {
  //   return;
  // }

  x0 = region->left_top.x;
  while (*text)
  {
    if (isFirstLine)
    { //�������п������֡�
      x0 += _font->currentFontSize * 2;
      isFirstLine = FALSE;
    }

    y0 = region->left_top.y + linenum * _font->currentFontSize + _font->ygap; //����߶� y + ����*�ַ��߶� + �м��
    if (((unsigned char)text[0] >= 0xa0) &&
        ((unsigned char)text[1] >= 0xa0))
    {                                                            //��ӡ����
      quma = text[0] - 0xa1;                                     //�������
      weima = text[1] - 0xa1;                                    //���λ��
      offset = (94L * (quma) + (weima)) * _font->totalbytes;     //���Ҫ��ʾ�ĺ������ֿ��ļ��е�ƫ��
      fseek(_font->fpCurrentFont, offset, SEEK_SET);             //�ض�λ�ļ�ָ��
      fread(buffer, _font->totalbytes, 1, _font->fpCurrentFont); //�����ú��ֵľ����������,1ΪҪ���������

      printHZWord(x0, y0, buffer, _font); //�����������

      x0 += _font->currentFontSize + _font->xgap; //ƫ��һ�����ֿ��+�ּ��
      text += 2;                                  //��һ������
    }
    else
    { //��ӡ�ַ�
      if (*text == '\r' || *text == '\n')
      {                     //���д���
        isFirstLine = TRUE; //�лس����з�˵�����µ�һ��

        // if (*(text + 1) != 0) //�������һ���ַ�
        //   if (*(text + 1) == '\r' || (*(text + 1) == '\n'))
        //     text += 2; //����\r\n���
        //   else
        //     text++; //ֻ��һ��\r��\n
        if (*(text + 1) != 0) //�������һ���ַ�
          if (*(text + 1) == '\r' || (*(text + 1) == '\n'))
            text++; //����\r\n���

        text++; //ֻ��һ��\r��\n
        isNewLine = TRUE;
      }
      else
      {                                               //�ַ�
        printASC(x0, y0 - _font->ascy, *text, _font); //��������ַ�

        x0 += _font->ascSize + _font->xgap; //ƫ��һ���ַ����+�ּ��
        text++;                             //��һ���ַ�
      }
    }

    if ((unsigned char)text[0] < 0xa0)
    { //�ַ����ַ�����ж�
      if (x0 + _font->ascSize > region->right_bottom.x)
        isNewLine = TRUE;
    }
    else
    { //�����ú��ֿ���ж�
      if (x0 + _font->currentFontSize > region->right_bottom.x)
        isNewLine = TRUE;
    }

    if (isNewLine)
    {
      linenum++;
      isNewLine = FALSE;

      if ((region->left_top.y + (linenum + 1) * _font->currentFontSize + _font->ygap) > region->right_bottom.y)
      { //�ж��Ƿ񳬸߶ȣ��˳� �߶Ƚض�
        break;
      }

      x0 = region->left_top.x;
    }
  }
  free(buffer);
}

/**
 * @version v3 ����ı������汾3
 * @author 
 * @brief ��ȡ�ļ�����һ������������ַ������ַ���ʾ��ʽ�������塢�ֺš��ּ�ࡢ�м��ȵ���_font�����ã�
 * �ַ�����������Ӣ�Ļ�ϡ������п����񣬸�������߶ȺͿ�ȣ��Զ����С��ض�
 * @param region  Ҫ��ʾ������  
 * @param fp �ļ�ָ��
 * @param _font ��������
 * @return ��
 */
void printTextFile(hregion *region, FILE *fp, hfont *_font)
{
#define BUFFEESIZE 256
  unsigned char Line[BUFFEESIZE];
  unsigned char *c;
  int x0, y0;                //ÿ��������ʼ��
  unsigned char quma, weima; //���庺�ֵ������λ��
  unsigned long offset;
  unsigned char *buffer;
  int linenum = 0;
  char isNewLine = FALSE;
  char isOutofRange = FALSE;

  char isFirstLine = TRUE; //�Ƿ��Ƕ������У����п������ֿ��

  TESTNULLVOID(region);
  TESTNULLVOID(_font);
  TESTNULLVOID(fp);

  buffer = (unsigned char *)malloc(_font->totalbytes);
  TESTNULLVOID(buffer);

  //�жϸ߶ȺͿ���Ƿ��㹻
  // if ((region->left_top.x + _font->currentFontSize) > region->right_bottom.x ||
  //     (region->left_top.y + _font->currentFontSize) > region->right_bottom.y)
  // {
  //   return;
  // }

  x0 = region->left_top.x;
  while (fgets(Line, sizeof(Line), fp) && !isOutofRange)
  { //fgets ������0a
    c = Line;
    while (*c)
    {
      if (isFirstLine)
      { //�������п������֡�
        x0 += _font->currentFontSize * 2;
        isFirstLine = FALSE;
      }

      y0 = region->left_top.y + linenum * _font->currentFontSize + _font->ygap; //����߶� y + ����*�ַ��߶� + �м��
      if (((unsigned char)c[0] >= 0xa0) &&
          ((unsigned char)c[1] >= 0xa0))
      {                                                            //��ӡ����
        quma = c[0] - 0xa1;                                        //�������
        weima = c[1] - 0xa1;                                       //���λ��
        offset = (94L * (quma) + (weima)) * _font->totalbytes;     //���Ҫ��ʾ�ĺ������ֿ��ļ��е�ƫ��
        fseek(_font->fpCurrentFont, offset, SEEK_SET);             //�ض�λ�ļ�ָ��
        fread(buffer, _font->totalbytes, 1, _font->fpCurrentFont); //�����ú��ֵľ����������,1ΪҪ���������

        printHZWord(x0, y0, buffer, _font); //�����������

        x0 += _font->currentFontSize + _font->xgap; //ƫ��һ�����ֿ��+�ּ��
        c += 2;                                     //��һ������
      }
      else
      { //��ӡ�ַ�
        if (*c == '\r' || *c == '\n')
        {                     //���д���
          isFirstLine = TRUE; //�лس����з�˵�����µ�һ��
          c++;
          if (*(c + 1) != 0) //�������һ���ַ�
            if (*(c + 1) == '\r' || (*(c + 1) == '\n'))
              c++; //����\r\n���

          isNewLine = TRUE;
        }
        else
        {                                            //�ַ�
          printASC(x0, y0 - _font->ascy, *c, _font); //��������ַ�

          x0 += _font->ascSize + _font->xgap; //ƫ��һ���ַ����+�ּ��
          c++;                                //��һ���ַ�
        }
      }

      if ((unsigned char)c[0] < 0xa0)
      { //�ַ����ַ�����ж�
        if (x0 + _font->ascSize > region->right_bottom.x)
          isNewLine = TRUE;
      }
      else
      { //�����ú��ֿ���ж�
        if (x0 + _font->currentFontSize > region->right_bottom.x)
          isNewLine = TRUE;
      }

      if (isNewLine)
      {
        linenum++;
        isNewLine = FALSE;

        if ((region->left_top.y + (linenum + 1) * _font->currentFontSize + _font->ygap) > region->right_bottom.y)
        { //�ж��Ƿ񳬸߶ȣ��˳� �߶Ƚض�
          isOutofRange = TRUE;
          break;
        }

        x0 = region->left_top.x;
      }
    }

    //printf("%s", Line);
  }

  free(buffer);
}

/**
 * ��ȡ�����ֿ��ļ�
 * @param type ��marcodef�ж��� 
 * @param size �ֺ� 16��24��32��48
 * 
 * @return �ֿ��ļ�ָ��
 */
FILE *getFontFile(int type, int size)
{
  char fontfile[32];
  switch (type)
  {
  case SIMKAI: //����
    sprintf(fontfile, FONTPATH "HZK%dK", size);
    break;
  case SIMHEI: //����
    sprintf(fontfile, FONTPATH "HZK%dH", size);
    break;
  case ASCII:
    sprintf(fontfile, FONTPATH "ASC16");
    break;
  default: //����
    sprintf(fontfile, FONTPATH "HZK%dS", size);
    break;
  }

  return fopen(fontfile, "rb");
}

/**
 * ����������Ϣ
 * 
 * 
 */
void calcFontSetting(hfont *font)
{
  TESTNULLVOID(font);

  //���㺺��ÿ��������Ҫ���ٸ��ֽڱ���
  font->byteperline = (font->currentFontSize + 7) / 8;

  //���㺺��������Ҫ�����ֽڱ���
  font->totalbytes = font->byteperline * font->currentFontSize;
  switch (font->currentFontSize)
  {
  case 16:
    font->ascScalex = 1;
    font->ascScaley = 1;
    font->ascy = 0;
    font->ascSize = 8 * 1;
    break;
  case 24:
    font->ascScalex = 2;
    font->ascScaley = 2;
    font->ascy = 3;
    font->ascSize = 8 * 2;
    break;
  case 32:
    font->ascScalex = 3;
    font->ascScaley = 3;
    font->ascy = 8;
    font->ascSize = 8 * 3;
    break;
  case 48:
    font->ascScalex = 4;
    font->ascScaley = 4;
    font->ascy = 6;
    font->ascSize = 8 * 4;
    break;
  default:
    break;
  }
}
/**
 * ��ȡĬ��������Ϣ,�����߱��븺���ͷ�hfontָ�� freefont
 * @warning �����߱��븺���ͷ�hfontָ��
 * @param type ��marcodef�ж��� 
 * @param size �ֺ� 16��24��32��48 
 */
hfont *getFont(int type, int size, int color)
{
  hfont *_hfont;
  char fontfile[32];

  _hfont = malloc(sizeof(hfont));
  TESTNULL(_hfont, NULL);
  memset(_hfont, 0, sizeof(hfont));

  _hfont->fpCurrentFont = getFontFile(type, size);
  TESTNULL(_hfont->fpCurrentFont, NULL);
  _hfont->fpASC = getFontFile(ASCII, 16);
  TESTNULL(_hfont->fpASC, NULL);

  _hfont->currentFontSize = size;
  _hfont->fontcolor = color;

  _hfont->currentFontType = type;
  _hfont->xgap = 1;
  _hfont->ygap = 1;

  calcFontSetting(_hfont);

  return _hfont;
}

/**
 * @brief ��ȡһ���ַ�������Ļ����ʾ��Ҫ���ٸ����أ������лس����з���
 * ���Իس����з�Ϊ������
 * 
 * @param text �ı���
 * @param _f ������Ϣ
 */
int calcPrintTextLenght(char *text, hfont *_f)
{
  int totalPixel = 0;

  TESTNULL(text, 0);
  TESTNULL(_f, 0);

  while (*text)
  {
    if (*text > 0xa0) //����
      totalPixel += _f->currentFontSize + _f->xgap;
    else if (*text == '\r' || *text == '\n')
      break;
    else
      totalPixel += _f->ascSize + _f->xgap;
  }
  return totalPixel - _f->xgap;
}

/**
 * �ͷ�font��Ϣ
 * 
 */
void freeFont(hfont *_f)
{
  if (_f)
  {
    fclose(_f->fpCurrentFont);
    fclose(_f->fpASC);

    free(_f);
  }
}

void hsvgatest()
{
  unsigned int *buffer, i;
  int width = 100, height = 100;
  FILE *fpBak = fopen("scr.bak", "wb+");
  //memory size
  buffer = (unsigned int far *)malloc(width * height * sizeof(unsigned int));
  if (buffer == NULL)
    return;

  fillRegionEx(100, 200, 300, 160, 0x4AE6);
  fillRegionEx(150, 250, 300, 160, 0x4356);

  rectangleEx(140, 240, width, height, 0x5555, 1, 1);
  savebackgroundEx(buffer, 140, 240, width, height);
  restorebackgroundEx(buffer, 400, 50, width, height);

  savebackgroundFile(fpBak, 80, 150, 200, 100);
  restorebackgroundFile(fpBak, 0, 0, 200, 100);

  fclose(fpBak);
  free(buffer);
}

/**
 * ��ʾһ������
 * @param fpfont �ֿ��ļ�ָ��
 * @param x
 * @param y 
 * @param word ����
 * @param fontsize ���ִ�С 16,24,32,48
 * @param color ��ɫ * 
 * 
void printHZWord(int x, int y, unsigned char word[2], hfont *font)
{
  unsigned char quma, weima;                                               //���庺�ֵ������λ��
  unsigned long offset;                                                    //���庺�����ֿ��е�ƫ����
  unsigned char mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; //�������飬������ʾ���ֵ����е�����
  int i, j, pos;
  unsigned char *buffer;

  TESTNULL(font, );
  TESTNULL(font->fpCurrentFont, );

  buffer = (unsigned char *)malloc(font->totalbytes);

  quma = word[0] - 0xa1;                                   //�������
  weima = word[1] - 0xa1;                                  //���λ��
  offset = (94L * (quma) + (weima)) * font->totalbytes;    //���Ҫ��ʾ�ĺ������ֿ��ļ��е�ƫ��
  fseek(font->fpCurrentFont, offset, SEEK_SET);            //�ض�λ�ļ�ָ��
  fread(buffer, font->totalbytes, 1, font->fpCurrentFont); //�����ú��ֵľ����������,1ΪҪ���������

  for (i = 0; i < font->currentFontSize; i++)
  {
    pos = font->byteperline * i;                //16*16��������ÿһ���������ֽ�
    for (j = 0; j < font->currentFontSize; j++) //һ��һ�е�ɨ�裬��λ��Ϊ��1�ĵ���ʾ����
    {
      if ((mask[j % 8] & buffer[pos + j / 8]) != NULL) //j%8ֻ����0��8֮��ѭ����j/8��0��1֮��ѭ��
      {
        putpixel64k(x + j, y, font->fontcolor);
      }
    }
    y++;
  }

  free(buffer);
}*/

/**
//Slow verion
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