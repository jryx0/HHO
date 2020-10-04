#include "pinyin.h"
#include "hglobal.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GRAPHICS.H>

/**
 * 打开拼音索引文件,读入数组中。
 * 为避免频发打开关闭字库文件，保存汉字库文件指针。 
 * @return pyInput 拼音结构指针
 */
pyInput *initPYHZIndex(void)
{
  FILE *fp1, *fp2;
  char ch;
  char py[16];
  int i = 0, j = 0;
  int k = 0;

  pyInput *idx;

  if ((fp1 = fopen(FILE_HANZI, "r")) == NULL)
  {
    printf("unable to open %s\r\n", FILE_HANZI);
    return NULL;
  }
  if ((fp2 = fopen(FILE_PYINDEX, "r")) == NULL)
  {
    printf("unable to open %s\r\n", FILE_PYINDEX);
    return NULL;
  }

  idx = malloc(sizeof(pyInput));
  idx->fpHanzi = fp1;

  while ((ch = fgetc(fp2)) != EOF)
  {
    if (ch != ';')
    { //读取也给拼音及索引
      py[i] = ch;
      if (ch == ',')
        k = i; //记录分隔位置
      i++;
    }
    else
    { //一个拼音已读取，逗号拆分拼音和索引
      py[i] = 0;
      i = 0;
      strncpy(idx->index[j].pinying, py, k);
      idx->index[j].pinying[k] = 0;             //拼音
      idx->index[j].postion = atoi(py + k + 1); //索引
      j++;
    }
  }

  fclose(fp2);
  return idx;
}

/**
 * 根据索引读取拼音对应的汉字起始地址，返回一个拼音对应的相同读音的汉字
 * 注意一个汉字两个字节。
 * @param hzIdx 拼音结构
 * @param py 拼音字符串，可以是不完整拼音
 * 
 * @return 候选汉字字符串。未找到返回NULL
 */
char *getCandidateHZbyPY(pyInput *hzIdx, const char *py)
{
  int len;
  int i;
  int offset = 0;
  char *hanzi = NULL; // 8个汉字

  if (hzIdx == NULL || hzIdx->fpHanzi == NULL)
    return NULL;

  len = strlen(py); //拼音长度
  for (i = 0; i < MAXNUMPY; i++)
  {
    if (strncmp(hzIdx->index[i].pinying, py, len) == 0)
    {                                   //比较拼音前n个字符是否相等（可以是不完整拼音）
      offset = hzIdx->index[i].postion; //汉字在字库中的偏移量（汉字的偏移）
      if (i < MAXNUMPY - 1)
        //计算相邻两个拼音之间由多少个汉字
        len = hzIdx->index[i + 1].postion - hzIdx->index[i].postion;
      else
        len = 20; //最后行

      //文件偏移
      fseek(hzIdx->fpHanzi, offset * 2, 0);

      hanzi = malloc(2 * len + 1);
      memset(hanzi, 0, len * 2 + 1);

      fread(hanzi, 2, len, hzIdx->fpHanzi);
      break;
    }
  }

  return hanzi;
}

/**
 * 检查是否是有效拼音，
 * 目前只检查是否是字母
 * 
 * @param py 拼音字符串
 * @return 1 合法拼音 0 非合法拼音
 */
int checkvalidatepy(char *py)
{
  int i = 0;
  if (py == NULL)
    return 0;

  for (i = 0; i < strlen(py); i++)
  {
    if (!isalpha(py[i]))
      return 0;
  }

  return 1;
}

/**
 * 释放拼音结构，关闭文件 
 */
void ClosePY(pyInput *hzIdx)
{
  if (hzIdx)
  {
    fclose(hzIdx->fpHanzi);
    free(hzIdx);
  }
}

void printtextxy(FILE *fpFont, int x, int y, unsigned char *str, int color, int fontsize)
{
  int i, j;
  char byte;

  int hanzibytes;
  unsigned char *fontdata;

  unsigned char qh, wh;
  unsigned long offset;

  if (fpFont == NULL || str == NULL)
  {
    fprintf(stderr, "无法打开汉字字库");
    return;
  }

  if (fontsize == 24)
    hanzibytes = 72; //24宽字体，每个汉字占72字节
  else
    hanzibytes = 32; //默认情况下，16宽字体，每个汉字占32字节
  fontdata = malloc(hanzibytes);

  while (*str)
  {
    if (*str >= 0xa0 && *(str + 1) > 0xa0)
    {
      qh = *str - 0xa1; //汉字区位码
      wh = *(str + 1) - 0xa1;
      offset = (94 * (qh) + (wh)) * 32; //计算该汉字在字库中偏移量

      //读取字库
      fseek(fpFont, offset, SEEK_SET);
      fread(fontdata, hanzibytes, 1, fpFont);

      //显示字体数据
      for (j = 0; j < hanzibytes; j++)
      {
        byte = *fontdata++;
        for (i = x; i < x + 8; i++)
        {
          if (byte & 0x80) //01101001
          {                //显示byte中为1的点
            if (j % 2)
              putpixel64k(i + 8, y + j / 2, 0);
            else
              putpixel64k(i, y + j / 2, 0);
          }
          byte <<= 1;
        }
      }
    }
    str += 2;
  }
  free(fontdata);
}

void printHZ(FILE *fpFont, int x, int y, unsigned char *str, int color, int fontsize)
{ //16
  unsigned char qh, wh, byte, i, j;
  unsigned long offset;
  unsigned char *buffer;
  char eng[2];

  if (fpFont == NULL || str == NULL)
  {
    fprintf(stderr, "无法打开汉字字库");
    return;
  }

    eng[1] = 0;
  buffer = malloc(fontsize * 2);

  while (*str)
  {
    if (*str >= 0xa0 && *(str + 1) >= 0xa0)
    {
      qh = str[0] - 0xa1; //汉字区位码
      wh = str[1] - 0xa1;
      offset = (94 * (qh) + (wh)) * 32L; //计算该汉字在字库中偏移量
      fseek(fpFont, offset, SEEK_SET);
      fread(buffer, fontsize * 2, 1, fpFont);

      for (j = 0; j < fontsize * 2; j++)
      {
        byte = *buffer++;
        for (i = x; i < x + 8; i++)
        {
          if (byte & 0x80)
          {
            if (j % 2)
              putpixel(i + 8, y + j / 2, (color));
            else
              putpixel(i, y + j / 2, (color));
          }
          byte <<= 1;
        }
      }

      x += fontsize;
      str += 2;
    }
    else
    { //英文
      eng[0] = str[0];
      settextstyle(1, HORIZ_DIR, 2);
      outtextxy(x, y, eng);
      str++;
      x += fontsize / 2;
    }
  }

  free(buffer);
}