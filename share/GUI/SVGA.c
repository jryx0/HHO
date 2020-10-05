#include "hglobal.h"
#include "SVGA.h"

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