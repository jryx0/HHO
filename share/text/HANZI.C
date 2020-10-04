#include "HBaseWin.h"
#include "hanzi.h"
#include <graphics.h>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

int drawTextSting(int x, int y, char *str, int color, int gap)
{
  int totalwidth = 0;

  if (str == NULL)
    return 0;

  return 0;
}

void printHZ12(int x, int y, char *s, int color, int dx, int dy, int gap,
               int mode)
// 12 *  12点阵汉字的显示，gap表示间隔，mode为0则横向输出，1则纵向输出
{
  char *s1;
  unsigned long offset;
  FILE *fp;
  char buffer[24]; // buffer用来存储一个汉字
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

    //将12位字节的点阵按位在屏幕上打印出来(1:打印,0:不打印),显示汉字
    for (i = 0; i < 12; i++)
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
                putpixel(x + 8 * j * dy + k * dy + m, y + i * dx + n, color);
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

char *drawHZWord(FILE *fp, int x, int y, char hanzi[2], int color, int Scalex,
                 int Scaley, int fontsize)
{
  unsigned char qh, wh;
  unsigned long offset;
  unsigned char *buffer;

  if (fp == NULL)
    return;

  buffer = malloc(fontsize * 2);
  qh = hanzi[0] - 0xa1; //汉字区位码
  wh = hanzi[1] - 0xa1;
  offset = (94 * (qh) + (wh)) * 32L; //计算该汉字在字库中偏移量
  fseek(fp, offset, SEEK_SET);
  fread(buffer, fontsize * 2, 1, fp);
  drawHZBuffer2(x, y, buffer, color, fontsize);
  free(buffer);
}

char *drawHZWord16(FILE *fp, int x, int y, char hanzi[2], int color)
{
  return drawHZWord(fp, x, y, hanzi, color, 1, 1, 16);
}
char *drawHZWord12(FILE *fp, int x, int y, char hanzi[2], int color)
{
  return drawHZWord(fp, x, y, hanzi, color, 1, 1, 12);
}

void drawHZBuffer(int x, int y, char *hanzibuffer, int color, int size)
{
  int i, j, k;
  if (hanzibuffer == NULL)
    return;

  for (i = 0; i < size; i++)
    for (j = 0; j < 2; j++)
      for (k = 0; k < 8; k++)
        if (((hanzibuffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
        {
          putpixel(x + 8 * j + k, y + i, color);
        }

  return;
}

void drawHZBuffer2(int x, int y, char *hanzibuffer, int color, int size)
{
  int i, j, k;
  char byte;

  if (hanzibuffer == NULL)
    return;

  for (j = 0; j < size * 2; j++)
  {
    byte = *hanzibuffer++;
    for (i = x; i < x + 8; i++)
    {
      if (byte & 0x80)
      {
        if (j % 2)
        { // putpixel(i + 8, y + j / 2, RealDrawColor(color));
          putpixel(i + 8, y + j / 2, (color));
        }
        else
        { // putpixel(i, y + j / 2, RealDrawColor(color));
          putpixel(i, y + j / 2, (color));
        }
      }
      else
      {
        //背景
      }
      byte <<= 1;
    }
  }
  return;
}

void printHZ16(int x, int y, char *s, long color, int Scalex, int Scaley)
// 16*16点阵汉字的显示
{
  char *s1;
  unsigned long offset;
  FILE *fp;
  char buffer[32]; // buffer用来存储一个汉字
  int m, n, i, j, k;
  unsigned char qh, wh;

  if ((fp = fopen("HZK16", "rb")) == NULL)
  {
    printf("Can't open hzk16.c,Please add it");
    getch();
    exit(1);
  }
  s1 = s;
  while (*s)
  {
    qh = *(s)-0xa1; //汉字区位码
    wh = *(s + 1) - 0xa1;
    offset = (94 * (qh) + (wh)) * 32L; //计算该汉字在字库中偏移量
    fseek(fp, offset, SEEK_SET);
    fread(buffer, 32, 1, fp);
    //取出汉字32字节的点阵字模存入buffer中（一个汉字）

    for (i = 0; i < 16; i++)
    //将16位字节的点阵按位在屏幕上打印出来(1:打印,0:不打印),显示汉字
    {
      for (n = 0; n < Scalex; n++)
      {
        for (j = 0; j < 2; j++)
        {
          for (k = 0; k < 8; k++)
          {
            for (m = 0; m < Scaley; m++)
            {
              if (((buffer[i * 2 + j] >> (7 - k)) & 0x1) != NULL)
              {
                putpixel(x + 8 * j * Scaley + k * Scaley + m,
                         y + i * Scaley + n, color);
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
