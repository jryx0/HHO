#include "macrodef.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "pinyin.h"
#include "hglobal.h"
#include <memory.h>
//#include <graphics.h>

/**
 * 1.дһ��������ר�Ŷ��ļ�
 * 2.Ҫ���������С
 * 3.���ǵ��ڴ��ԭ���ļ�����ʾ�Ķ�ȡ�ǲ��Ǹ���һЩ����Ȼ�ٶ���һ�� 
 * 
 */
hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title)
{ //titleָ���ļ���
  hbasewinAttr *label;
  FILE *fptemp;
  char *temp;
  int i, len;

  if (title == NULL)
  {
    label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
    return;
  }

  if (fptemp = fopen(title, "r") == NULL)
    label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  else
  {
    len = (nWidth / 16) * (nHeight / 16);
    temp = malloc(len * 2 + 1);
    memset(temp, 0, len * 2 + 1);
    for (i = 0; (fptemp != EOF) && (i < (len - 1) * 2); i++)
      fread(temp + i, 1, 1, fptemp);
    //���һ���ֵ��ж�
    if (fptemp != EOF)
    {                        //����
      *(temp + i) = '��';     //������
      *(temp + len * 2) = 0; //���ԣ�ȷ���н�β
    }
    else
    {
      fread(temp + i, 1, 2, fptemp);
    }
    label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, temp);
    fclose(fptemp);
  }
  label->onPaint = OnPaintLabel;

  return label;
}

/**
 * 10.05�޸� by ���
 * 
 * 
 */
void OnPaintLabel(hbasewinAttr *label, void *value)
{
  //FILE *fpFont; by����𩲻��Ҫ
  int x, y, type = 3, len, linemax, maxline, i;
  char *temp = NULL;
  if (label == NULL)
    return;
  OnPaint(label, &type);

  if (label->title == NULL)
    return;
  //fpFont = fopen(FILE_SIMSUN16 DATAPATH, "rt");

  len = strlen(label->title) / 2; //һ����������char��lenΪ����
  x = getAbsoluteX(label);
  y = getAbsoluteY(label);

  linemax = label->nWidth / 16;
  maxline = len / linemax + ((len % linemax) == 0 ? 0 : 1);
  temp = malloc(linemax * 2 + 1);
  memset(temp, 0, linemax * 2 + 1);
  for (i = 0; i < maxline - 1; i++)
  {
    memcpy(temp, label->title + i * linemax * 2, linemax * 2);
    //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16); modified by ���
    printText(x, y + 16 * i, temp, SIMSUN, 24, 0, 0x0 /*color*/); //��ʾ�ı��ĺ������ˣ�����Ҫ�򿪺����ֿ��ļ�ָ��
  }
  memcpy(temp, label->title + i * linemax * 2, (len - linemax * (maxline - 1)) * 2);
  //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16);
  printText(x, y + 16 * i, temp, SIMSUN, 24, 0, 0x0 /*color*/);
}