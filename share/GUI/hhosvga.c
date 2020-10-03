#include "macrodef.h"
#include "SVGAUTIL.H"

#include <dos.h>

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