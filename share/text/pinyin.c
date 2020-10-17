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
  if (len == 0)
    return NULL;
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
