#include "hglobal.h"
#include "SVGAUTIL.H"
#include "hhosvga.h"

#include <dos.h>
#include <GRAPHICS.H>
#include <stdio.h>
#include <memory.h>

/**
 * ������ʾҳ
 * @param page ҳ�� 
 */
void selectpage(register char page)
{
  union REGS regs;
  static unsigned char old_page = 0;
  static int flag = 0;

  /*����ҳ����ƹ��ܺ�*/
  regs.x.ax = 0x4f05;
  regs.x.bx = 0;

  if (flag == 0)
  { /*����ǵ�һ�λ�ҳ*/
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
    flag++;
  }
  else if (page != old_page)
  { /*������ϴ�ҳ��Ų�ͬ������л�ҳ*/
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
  }
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

  /*�Դ�ָ�볣��*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  pages = 24; //1024x768 = 24, 800*600 = 14
  pagesize = 1024 * 32 * 2;
  for (i = 0; i <= pages; i++)
  {
    selectpage(i);
    memset(video_buffer, color, pagesize - 1);
    //memset(video_buffer + 0x4000, color, pagesize); // 16 << 10
  }

  // selectpage(0);
  // memset(video_buffer, 0xff, 1024 * 32 * 2 - 1);
  // selectpage(1);
  // memset(video_buffer, 0xff, 1024 * 32 * 2 - 1);
}

/**
 * ������������Ļ��Χ��
 * @param x  
 * @param y ��������
 * @param width
 * @param height ��ȡ��߶�
 */
void setStandardRegionEx(int *x, int *y, int *width, int *height)
{
  if (*x < 0)
    *x = 0;
  if (*x > getmaxx())
    *x = getmaxx();

  if (*y < 0)
    *y = 0;
  if (*y > getmaxy())
    *y = getmaxy();

  if (*x + *width >= getmaxx())
    *width = getmaxx() - *x;
  if (*y + *height > getmaxy())
    *height = getmaxy() - *y;
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
 * ���㺯����graphics.h �е�putpixel��1024*768*64k�³���,
 * �ڴ�ģʽ��,���㺯�� 
 * @param x 
 * @param y ������
 * @param color ��ɫ
 */
void putpixel64k(int x, int y, unsigned int color)
{
  /*�Դ�ָ�볣����ָ���Դ��׵�ַ */
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;
  unsigned char new_page;
  unsigned long int page;

  /*�жϵ��Ƿ�����Ļ��Χ�ڣ����ھ��˳�*/
  if (x < 0 || x > (getmaxx()) || y < 0 || y > (getmaxy()))
  {
    return;
  }

  /*�����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ����*/
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /*32k����һ��ҳ������32k������㷨*/
  selectpage(new_page);

  /*���Դ�д����ɫ����Ӧ�㻭��*/
  video_buffer[page] = color;
}

/**
 * ���㺯����graphics.h �е�getpixel��1024*768*64k�³���,
 * �ڴ�ģʽ��,���㺯�� 
 * @param x 
 * @param y ������
 * @return color ��ɫ
 */
unsigned int getpixel64k(int x, int y)
{
  /*�Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;
  unsigned char new_page;
  unsigned long int page;

  /*�жϵ��Ƿ�����Ļ��Χ�ڣ����ھ��˳�*/
  if (x < 0 || x > (getmaxx() - 1) || y < 0 || y > getmaxy())
  {
    return 0;
  }

  /*�����Դ��ַƫ�����Ͷ�Ӧ��ҳ��ţ�����ҳ����*/
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /*32k����һ��ҳ������32k������㷨*/
  selectpage(new_page);

  return video_buffer[page];
}