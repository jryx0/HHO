#include "pinyin.h"
#include "hglobal.h"
#include "svga.h"
#include "hhosvga.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

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
    fprintf(stderr, "unable to open %s\r\n", FILE_HANZI);
    return NULL;
  }
  if ((fp2 = fopen(FILE_PYINDEX, "r")) == NULL)
  {
    fprintf(stderr, "unable to open %s\r\n", FILE_PYINDEX);
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
    hzIdx = NULL;
  }
}

/**
 * 打印一行汉字
 * 
 * @param x
 * @param y 坐标
 * @param text 文本串
 * @param fonttype 字体-宏定义 宋体(SIMSUN)黑体(SIMHEI)楷体(SIMKAI)
 * @param fontsize 字号 16、24、32、48
 * @param gap 字和字之间的间距，像素为单位
 * @param color 颜色
 
void printText(int x, int y, char *text, int fonttype, int fontsize, int gap, int color)
{
  FILE *fpfont = NULL, *fpascii = NULL;                                    //定义汉字库文件指针
  unsigned char quma, weima;                                               //定义汉字的区码和位码
  unsigned long offset;                                                    //定义汉字在字库中的偏移量
  unsigned char mask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; //功能数组，用于显示汉字点阵中的亮点
  int i, j, pos;

  fpascii = fopen(FONTPATH "ASC16", "rb");

  switch (fontsize) //不同的flag对应不同的汉字库，实现了汉字的大小可根据需要改变
  {
  case 16:
  {
    char mat[32]; //16*16的汉字需要32个字节的数组来存储
    int y0 = y;
    int x0 = x;
    fpfont = getFontFile(fonttype, 16); //fopen("data\\font\\HZK16S", "rb"); //使用相对路径
    if (fpfont == NULL)
    {
      fprintf(stderr, "无法打开HZ16S字库");
      return;
    }

    while (*text != NULL)
    {
      y = y0;
      if (((unsigned char)text[0] >= 0xa0) &&
          ((unsigned char)text[1] >= 0xa0))
      {

        quma = text[0] - 0xa1;                  //求出区码
        weima = text[1] - 0xa1;                 //求出位码
        offset = (94 * (quma) + (weima)) * 32L; //求出要显示的汉字在字库文件中的偏移
        fseek(fpfont, offset, SEEK_SET);        //重定位文件指针
        fread(mat, 32, 1, fpfont);              //读出该汉字的具体点阵数据,1为要读入的项数

        for (i = 0; i < 16; i++)
        {
          pos = 2 * i;             //16*16矩阵中有每一行有两外字节
          for (j = 0; j < 16; j++) //一行一行地扫描，将位上为了1的点显示出来
          {
            if ((mask[j % 8] & mat[pos + j / 8]) != NULL) //j%8只能在0—8之间循环，j/8在0，1之间循环
            {
              putpixel64k(x + j, y, color);
            }
          }
          y++;
        }
        x += 16 + gap; //汉字宽16偏移gap个像素
        text += 2;     //汉字2个字节
      }
      else
      {
        //printASCII(fpascii, x, y, 1, 1, text[0], color);
        x += 8 * 1 + gap; //8*放大倍数，偏移gap个像素
        text++;
      }
    }
    break;
  }

  case 24:
  {
    char mat[72]; //24*24矩阵要72个字节来存储
    int y0 = y;
    int x0 = x;
    fpfont = getFontFile(fonttype, 24); //fopen("data\\font\\Hzk24k", "rb");
    if (fpfont == NULL)
    {
      fprintf(stderr, "无法打开HZk24k字库");
      return;
    }
    while (*text != NULL)
    {
      y = y0;
      if (((unsigned char)text[0] >= 0xa0) &&
          ((unsigned char)text[1] >= 0xa0))
      {
        quma = text[0] - 0xa1;  //求出区码
        weima = text[1] - 0xa1; //求出位码
        offset = (94 * (quma) + (weima)) * 72L;
        fseek(fpfont, offset, SEEK_SET);
        fread(mat, 72, 1, fpfont);
        for (i = 0; i < 24; i++)
        {
          pos = 3 * i;             //矩阵中每一行有三个字节
          for (j = 0; j < 24; j++) // 每一行有24位
          {
            if ((mask[j % 8] & mat[pos + j / 8]) != NULL)
              putpixel64k(x + j, y, color);
          }
          y++;
        }
        x += 24 + gap; //汉字宽24偏移gap个像素
        text += 2;
      }
      else
      {
        //printASCII(fpascii, x, y - 3, 2, 2, text[0], color);
        x += 8 * 2 + gap; //8*放大倍数，偏移gap个像素
        text++;
      }
    }
    break;
  }

  case 32:
  {
    char mat[128]; //32*32的汉字需要128个字节的数组来存储
    int y0 = y;
    int x0 = x;
    fpfont = getFontFile(fonttype, 32); //fopen("data\\font\\HZK32S", "rb");
    if (fpfont == NULL)
    {
      fprintf(stderr, "无法打开HZK32S字库");
      return;
    }
    while (*text != NULL)
    {

      y = y0;
      if (((unsigned char)text[0] >= 0xa0) &&
          ((unsigned char)text[1] >= 0xa0))
      {
        quma = text[0] - 0xa1;  //求出区码
        weima = text[1] - 0xa1; //求出位码
        offset = (94 * (quma) + (weima)) * 128L;
        fseek(fpfont, offset, SEEK_SET);
        fread(mat, 128, 1, fpfont);
        for (i = 0; i < 32; i++)
        {
          pos = 4 * i; //32*32矩阵中有每一行有两外字节
          for (j = 0; j < 32; j++)
          {
            if ((mask[j % 8] & mat[pos + j / 8]) != NULL)
            {
              putpixel64k(x + j, y, color);
            }
          }
          y++;
        }
        x += 32 + gap; //汉字宽32偏移gap个像素
        text += 2;     //汉字2个字节
      }
      else
      {
        //printASCII(fpascii, x, y - 8, 3, 3, text[0], color);
        x += 8 * 3 + gap; //8*放大倍数，偏移gap个像素
        text++;
      }
    }
    break;
  }

  case 48:
  {
    char mat[288]; //48*48的汉字需要288个字节的数组来存储
    int y0 = y;
    int x0 = x;
    fpfont = getFontFile(fonttype, 48); //fopen("data\\font\\Hzk48k", "rb");
    if (fpfont == NULL)
    {
      fprintf(stderr, "无法打开Hzk48k字库");
      return;
    }
    while (*text != NULL)
    {
      y = y0;
      if (((unsigned char)text[0] >= 0xa0) &&
          ((unsigned char)text[1] >= 0xa0))
      {
        quma = text[0] - 0xa1;                   //求出区码
        weima = text[1] - 0xa1;                  //求出位码
        offset = (94 * (quma) + (weima)) * 288L; //求出要显示的汉字在字库文件中的偏移
        fseek(fpfont, offset, SEEK_SET);         //重定位文件指针
        fread(mat, 288, 1, fpfont);              //读出该汉字的具体点阵数据,1为要读入的项数

        for (i = 0; i < 48; i++)
        {
          pos = 6 * i;
          for (j = 0; j < 48; j++) //一行一行地扫描，将位上为了1的点显示出来
          {
            if ((mask[j % 8] & mat[pos + j / 8]) != NULL) //j%8只能在0—8之间循环，j/8在0，1之间循环
            {
              putpixel64k(x + j, y, color);
            }
          }
          y++;
        }
        x += 48 + gap; //汉字宽48偏移gap个像素
        text += 2;     //汉字2个字节
      }
      else
      {
        //printASCII(fpascii, x, y - 4, 4, 4, text[0], color);
        x += 8 * 4 + gap; //8*放大倍数，偏移gap个像素
        text++;
      }
    }
    break;
  }

  default:
    break;
  }
  fclose(fpascii);
  fclose(fpfont);
}
*/