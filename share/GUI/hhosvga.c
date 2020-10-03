#include "macrodef.h"
#include "SVGAUTIL.H"

#include <dos.h>
#include <GRAPHICS.H>
#include <stdio.h>

void selectpage(register char page) // 3
{
  union REGS regs;

  /*��һ�ε�ҳ���,���ڼ����л�����,��ʹ�ô����ܶ����Ҫ����*/
  static unsigned char old_page = 0;
  /*��־���������ж��Ƿ��ǵ�һ�λ�ҳ*/
  static int flag = 0;

  /*����ҳ����ƹ��ܺ�*/
  regs.x.ax = 0x4f05;
  regs.x.bx = 0;

  /*����ǵ�һ�λ�ҳ*/
  if (flag == 0)
  {
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
    flag++;
  }

  /*������ϴ�ҳ��Ų�ͬ������л�ҳ*/
  else if (page != old_page)
  {
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
  int i, j = 16;
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

//������������Ļ��Χ��
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

//����Ļ���򱣴����ļ���
void savebackgroundFile(int x, int y, int width, int height)
{
  FILE *fpBK = fopen("bk.data", "w");

  int j;

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
    fwrite(&video_buffer[offest], 1, width * 2, fpBK);
  }
  fflush(fpBK);
  fclose(fpBK);
}

void restorebackgroundFile(int x, int y, int width, int height)
{
  FILE *fpBK = fopen("bk.data", "r");
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

//����С����ı���
//width * heigth * 2 < 65536
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

void putpixel64k(int x, int y, unsigned int color)  
{
  /*�Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*Ҫ�л���ҳ���*/
  unsigned char new_page;

  /*��Ӧ�Դ��ַƫ����*/
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

unsigned int getpixel64k(int x, int y)
{
  /*�Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*Ҫ�л���ҳ���*/
  unsigned char new_page;

  /*��Ӧ�Դ��ַƫ����*/
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

  /*������ɫ*/
  return video_buffer[page];
}