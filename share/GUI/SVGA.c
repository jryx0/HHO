#include "macrodef.h"
#include "SVGA.h"
#include "hhosvga.h"

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void initSVGA64k(void) // 1
{
  union REGS regs;
  /*设置VESA VBE模式的功能号*/
  regs.x.ax = 0x4f02;
  regs.x.bx = 0x117;
  int86(0x10, &regs, &regs);

  /*ax != 0x004f意味着初始化失败，输出错误信息见上,下同*/
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
  /*获取当前VESA VBE模式的功能号*/
  regs.x.ax = 0x4f03;
  int86(0x10, &regs, &regs);

  /*显示错误信息*/
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

  /*上一次的页面号,用于减少切换次数,是使用次数很多的重要变量*/
  static unsigned char old_page = 0;
  /*标志数，用于判断是否是第一次换页*/
  static int flag = 0;

  /*窗口页面控制功能号*/
  regs.x.ax = 0x4f05;
  regs.x.bx = 0;

  /*如果是第一次换页*/
  if (flag == 0)
  {
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
    flag++;
  }

  /*如果和上次页面号不同，则进行换页*/
  else if (page != old_page)
  {
    old_page = page;
    regs.x.dx = page;
    int86(0x10, &regs, &regs);
  }
}

void putpixel64k(int x, int y, unsigned int color) // 3
{
  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*要切换的页面号*/
  unsigned char new_page;

  /*对应显存地址偏移量*/
  unsigned long int page;

  /*判断点是否在屏幕范围内，不在就退出*/
  if (x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
  {
    return;
  }

  /*计算显存地址偏移量和对应的页面号，做换页操作*/
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /*32k个点一换页，除以32k的替代算法*/
  selectpage(new_page);

  /*向显存写入颜色，对应点画出*/
  video_buffer[page] = color;
}

unsigned int getpixel64k(int x, int y) // 4
{
  /*显存指针常量，指向显存首地址，指针本身不允许修改*/
  unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;

  /*要切换的页面号*/
  unsigned char new_page;

  /*对应显存地址偏移量*/
  unsigned long int page;

  /*判断点是否在屏幕范围内，不在就退出*/
  if (x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
  {
    return 0;
  }

  /*计算显存地址偏移量和对应的页面号，做换页操作*/
  page = ((unsigned long int)y << 10) + x;
  new_page = page >> 15; /*32k个点一换页，除以32k的替代算法*/
  selectpage(new_page);

  /*返回颜色*/
  return video_buffer[page];
}

int Putbmp64k(int x, int y, const char *path) //4
{
  /*指向图片文件的文件指针*/
  FILE *fpbmp;

  /*行像素缓存指针*/
  COLORS24 *buffer;

  /*图片的宽度、高度、一行像素所占字节数（含补齐空字节）*/
  long int width, height, linebytes;

  /*循环变量*/
  int i, j;

  /*图片位深*/
  unsigned int bit;

  /*压缩类型数*/
  unsigned long int compression;

  /*打开文件*/
  if ((fpbmp = fopen(path, "rb")) == NULL)
  {
    return -1;
  }

  /*读取位深*/
  fseek(fpbmp, 28L, 0);
  fread(&bit, 2, 1, fpbmp);

  /*非24位图则退出*/
  if (bit != 24U)
  {
    fclose(fpbmp);
    return -1;
  }

  /*读取压缩类型*/
  fseek(fpbmp, 30L, 0);
  fread(&compression, 4, 1, fpbmp);

  /*采用压缩算法则退出*/
  if (compression != 0UL)
  {
    fclose(fpbmp);
    return -1;
  }

  /*读取宽度、高度*/
  fseek(fpbmp, 18L, 0);
  fread(&width, 4, 1, fpbmp);
  fread(&height, 4, 1, fpbmp);

  /*宽度超限则退出*/
  if (width > SCR_WIDTH)
  {
    fclose(fpbmp);
    return -1;
  }

  /*计算一行像素占字节数，包括补齐的空字节*/
  linebytes = (3 * width) % 4;

  if (!linebytes)
  {
    linebytes = 3 * width;
  }
  else
  {
    linebytes = 3 * width + 4 - linebytes;
  }

  /*开辟行像素数据动态储存空间*/
  if ((buffer = (COLORS24 *)malloc(linebytes)) == 0)
  {
    fclose(fpbmp);
    return -1;
  }

  /*行扫描形式读取图片数据并显示*/
  fseek(fpbmp, 54L, 0);
  // for (i = height - 1; i > -1; i--)
  // {
  //   fread(buffer, linebytes, 1, fpbmp); /*读取一行像素数据*/

  //   /*一行像素的数据处理和画出*/
  //   for (j = 0; j < width; j++)
  //   {
  //     /*0x117模式下，原图红绿蓝各8位分别近似为5位、6位、5位*/
  //     buffer[j].R >>= 3;
  //     buffer[j].G >>= 2;
  //     buffer[j].B >>= 3;
  //     putpixel64k(j + x, i + y,
  //                 ((((unsigned int)buffer[j].R) << 11) | (((unsigned int)buffer[j].G) << 5) | ((unsigned int)buffer[j].B))); /*计算最终颜色，红绿蓝从高位到低位排列*/
  //   }
  // }
  for (i = 1; i < height; i++)
  {
    fseek(fpbmp, -linebytes * i, SEEK_END);
    fread(buffer, linebytes, 1, fpbmp); /*读取一行像素数据*/

    /*一行像素的数据处理和画出*/
    for (j = 0; j < width; j++)
    {
      /*0x117模式下，原图红绿蓝各8位分别近似为5位、6位、5位*/
      buffer[j].R >>= 3;
      buffer[j].G >>= 2;
      buffer[j].B >>= 3;
      putpixel64k(j + x, i + y, ((((unsigned int)buffer[j].R) << 11) | (((unsigned int)buffer[j].G) << 5) | ((unsigned int)buffer[j].B))); /*计算最终颜色，红绿蓝从高位到低位排列*/
    }
  }

  free(buffer);
  fclose(fpbmp);
  return 0;
}