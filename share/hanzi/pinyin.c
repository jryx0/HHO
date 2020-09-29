#include "pinyin.h"
#include "macrodef.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pyInput *initPYHZIndex(char *filename)
{
  FILE *fp1, *fp2;
  char ch;
  char py[16];
  int i = 0, j = 0;
  int k = 0;

  pyInput *idx;
  if (filename == NULL)
    return NULL;

  if ((fp1 = fopen(FILE_HANZI, "r")) == NULL)
  {
    printf("unable to open %s\r\n", filename);
    return NULL;
  }
  if ((fp2 = fopen(FILE_PYINDEX, "r")) == NULL)
  {
    printf("unable to open %s\r\n", filename);
    return NULL;
  }

  idx = malloc(sizeof(pyInput));
  idx->fpHanzi = fp1;

  while ((ch = fgetc(fp2)) != EOF)
  {
    if (ch != ';')
    {
      py[i] = ch;
      if (ch == ',')
        k = i;
      i++;
    }
    else
    {
      py[i] = 0;
      i = 0;
      strncpy(idx->index[j].pinying, py, k);
      idx->index[j].pinying[k] = 0;
      idx->index[j].postion = atoi(py + k + 1);
      j++;
    }
  }

  fclose(fp2);
  return idx;
}

char *getCandidateHZbyPY(pyInput *hzIdx, const char *py)
{
  int len;
  int i;
  int offset = 0;
  char *hanzi = NULL; // 8个汉字

  if (hzIdx == NULL || hzIdx->fpHanzi == NULL)
    return NULL;

  len = strlen(py);
  for (i = 0; i < MAXNUMPY; i++)
  {
    if (strncmp(hzIdx->index[i].pinying, py, len) == 0)
    {
      offset = hzIdx->index[i].postion;
      if (i < MAXNUMPY - 1)
        len = hzIdx->index[i + 1].postion - hzIdx->index[i].postion;
      else
        len = 20; //最后行

      fseek(hzIdx->fpHanzi, offset * 2, 0);
      hanzi = malloc(2 * len + 1);
      memset(hanzi, 0, len * 2 + 1);
      fread(hanzi, 2, len, hzIdx->fpHanzi);
      break;
    }
  }
  return hanzi;
}

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

void ClosePY(pyInput *hzIdx)
{
  fclose(hzIdx->fpHanzi);
  free(hzIdx);
}