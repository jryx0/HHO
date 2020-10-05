#include "inc\\svga.h"
#include "inc\\hanzi.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void printHZ12(int x, int y, char *s, int color, int dx, int dy, int gap, int mode) // 12*12点阵汉字的显示，gap表示间隔，mode为0则横向输出，1则纵向输出
{
  char *s1;
  unsigned long offset;
  FILE *fp;
  char buffer[24]; //buffer用来存储一个汉字
  int m, n, i, j, k;
  unsigned char qh, wh;

  if ((fp = fopen("hzk12.hzk", "rb")) == NULL)
  {
    printf("Can't open hzk12.c,Please add it");
    exit(1);
  }
  s1 = s;
  while (*s)
  {
    qh = *(s)-0xa0; //汉字区位码
    wh = *(s + 1) - 0xa0;
    offset = (94 * (qh - 1) + (wh - 1)) * 24L; //计算该汉字在字库中偏移量
    fseek(fp, offset, SEEK_SET);
    fread(buffer, 24, 1, fp); //取出汉字32字节的点阵字模存入buffer中（一个汉字）

    for (i = 0; i < 12; i++) //将32位字节的点阵按位在屏幕上打印出来(1:打印,0:不打印),显示汉字
    {
      for (n = 0; n < dx; n++)
      {
        for (j = 0; j < 2; j++)
        {
          for (k = 0; k < 8; k++)
          {
            for (m = 0; m < dy; m++)
            {
              if (((buffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
              {
                PaintPoint(x + 8 * j * dy + k * dy + m, y + i * dx + n, color);
              }
            }
          }
        }
      }
    }
    s += 2; //一个汉字内码占用两个字节
    if (mode == 0)
    {
      x += gap; //字间间隔
    }
    if (mode == 1)
    {
      y += gap;
    }
  }
  s = s1;
  fclose(fp);
}
void printHZ16(int x, int y, char *s, long color, int dx, int dy) // 16*16点阵汉字的显示
{
  char *s1;
  unsigned long offset;
  FILE *fp;
  char buffer[32]; //buffer用来存储一个汉字
  int m, n, i, j, k;
  unsigned char qh, wh;

  if ((fp = fopen("output\\lib\\HZK16", "rb")) == NULL)
  {
    printf("Can't open hzk16.c,Please add it");
    getch();
    exit(1);
  }
  s1 = s;
  while (*s)
  {
    qh = *(s)-0xa0; //汉字区位码
    wh = *(s + 1) - 0xa0;
    offset = (94 * (qh - 1) + (wh - 1)) * 32L; //计算该汉字在字库中偏移量
    fseek(fp, offset, SEEK_SET);
    fread(buffer, 32, 1, fp); //取出汉字32字节的点阵字模存入buffer中（一个汉字）

    for (i = 0; i < 16; i++) //将32位字节的点阵按位在屏幕上打印出来(1:打印,0:不打印),显示汉字
    {
      for (n = 0; n < dx; n++)
      {
        for (j = 0; j < 2; j++)
        {
          for (k = 0; k < 8; k++)
          {
            for (m = 0; m < dy; m++)
            {
              if (((buffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
              {
                PaintPoint(x + 8 * j * dy + k * dy + m, y + i * dx + n, color);
              }
            }
          }
        }
      }
    }
    s += 2;  //一个汉字内码占用两个字节
    x += 16; //字间间隔
  }
  s = s1;
  fclose(fp);
}