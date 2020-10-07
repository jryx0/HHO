#include "macrodef.h"
#include "SVGA.h"
#include "hhosvga.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void initSVGA64k(void) // 1
{
  union REGS regs;
  /*����VESA VBEģʽ�Ĺ��ܺ�*/
  regs.x.ax = 0x4f02;
  regs.x.bx = 0x117;
  int86(0x10, &regs, &regs);

  /*ax != 0x004f��ζ�ų�ʼ��ʧ�ܣ����������Ϣ����,��ͬ*/
  if (regs.x.ax != 0x004f)
  {
    printf("Error in setting SVGA mode!\nError code:0x%x\n", regs.x.ax);
    delay(5000);
    exit(1);
  }
}

unsigned int getSVGA(void) // 2
{

  union REGS regs;
  /*��ȡ��ǰVESA VBEģʽ�Ĺ��ܺ�*/
  regs.x.ax = 0x4f03;
  int86(0x10, &regs, &regs);

  /*��ʾ������Ϣ*/
  if (regs.x.ax != 0x004f)
  {
    printf("Error in getting SVGA mode!\nError code:0x%x\n", regs.x.ax);
    delay(5000);
    exit(1);
  }

  return regs.x.bx;
}

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

void putpixel64k(int x, int y, unsigned int color) // 3
{
  /*�Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*Ҫ�л���ҳ���*/
  unsigned char new_page;

  /*��Ӧ�Դ��ַƫ����*/
  unsigned long int page;

  /*�жϵ��Ƿ�����Ļ��Χ�ڣ����ھ��˳�*/
  if (x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
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

unsigned int getpixel64k(int x, int y) // 4
{
  /*�Դ�ָ�볣����ָ���Դ��׵�ַ��ָ�뱾�������޸�*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*Ҫ�л���ҳ���*/
  unsigned char new_page;

  /*��Ӧ�Դ��ַƫ����*/
  unsigned long int page;

  /*�жϵ��Ƿ�����Ļ��Χ�ڣ����ھ��˳�*/
  if (x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
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

int Putbmp64k(int x, int y, const char *path) //4
{
  /*ָ��ͼƬ�ļ����ļ�ָ��*/
  FILE *fpbmp;

  /*�����ػ���ָ��*/
  COLORS24 *buffer;

  /*ͼƬ�Ŀ�ȡ��߶ȡ�һ��������ռ�ֽ�������������ֽڣ�*/
  long int width, height, linebytes;

  /*ѭ������*/
  int i, j;

  /*ͼƬλ��*/
  unsigned int bit;

  /*ѹ��������*/
  unsigned long int compression;

  /*���ļ�*/
  if ((fpbmp = fopen(path, "rb")) == NULL)
  {
    return -1;
  }

  /*��ȡλ��*/
  fseek(fpbmp, 28L, 0);
  fread(&bit, 2, 1, fpbmp);

  /*��24λͼ���˳�*/
  if (bit != 24U)
  {
    fclose(fpbmp);
    return -1;
  }

  /*��ȡѹ������*/
  fseek(fpbmp, 30L, 0);
  fread(&compression, 4, 1, fpbmp);

  /*����ѹ���㷨���˳�*/
  if (compression != 0UL)
  {
    fclose(fpbmp);
    return -1;
  }

  /*��ȡ��ȡ��߶�*/
  fseek(fpbmp, 18L, 0);
  fread(&width, 4, 1, fpbmp);
  fread(&height, 4, 1, fpbmp);

  /*��ȳ������˳�*/
  if (width > SCR_WIDTH)
  {
    fclose(fpbmp);
    return -1;
  }

  /*����һ������ռ�ֽ�������������Ŀ��ֽ�*/
  linebytes = (3 * width) % 4;

  if (!linebytes)
  {
    linebytes = 3 * width;
  }
  else
  {
    linebytes = 3 * width + 4 - linebytes;
  }

  /*�������������ݶ�̬����ռ�*/
  if ((buffer = (COLORS24 *)malloc(linebytes)) == 0)
  {
    fclose(fpbmp);
    return -1;
  }

  /*��ɨ����ʽ��ȡͼƬ���ݲ���ʾ*/
  fseek(fpbmp, 54L, 0);
  // for (i = height - 1; i > -1; i--)
  // {
  //   fread(buffer, linebytes, 1, fpbmp); /*��ȡһ����������*/

  //   /*һ�����ص����ݴ���ͻ���*/
  //   for (j = 0; j < width; j++)
  //   {
  //     /*0x117ģʽ�£�ԭͼ��������8λ�ֱ����Ϊ5λ��6λ��5λ*/
  //     buffer[j].R >>= 3;
  //     buffer[j].G >>= 2;
  //     buffer[j].B >>= 3;
  //     putpixel64k(j + x, i + y,
  //                 ((((unsigned int)buffer[j].R) << 11) | (((unsigned int)buffer[j].G) << 5) | ((unsigned int)buffer[j].B))); /*����������ɫ���������Ӹ�λ����λ����*/
  //   }
  // }
  for (i = 1; i < height; i++)
  {
    fseek(fpbmp, -linebytes * i, SEEK_END);
    fread(buffer, linebytes, 1, fpbmp); /*��ȡһ����������*/

    /*һ�����ص����ݴ���ͻ���*/
    for (j = 0; j < width; j++)
    {
      /*0x117ģʽ�£�ԭͼ��������8λ�ֱ����Ϊ5λ��6λ��5λ*/
      buffer[j].R >>= 3;
      buffer[j].G >>= 2;
      buffer[j].B >>= 3;
      putpixel64k(j + x, i + y, ((((unsigned int)buffer[j].R) << 11) | (((unsigned int)buffer[j].G) << 5) | ((unsigned int)buffer[j].B))); /*����������ɫ���������Ӹ�λ����λ����*/
    }
  }

  free(buffer);
  fclose(fpbmp);
  return 0;
}