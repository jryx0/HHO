#include "macrodef.h"
#include "HBaseWin.h"
#include "hlabel.h"
#include "pinyin.h"
#include "hglobal.h"
#include <memory.h>


/**
 * label-��ʾ�ı��ļ��е�����
 * 
 */ 
hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title)
{
  hbasewinAttr *label;
   
  label = CreateWindowsEx(parent, x, y, nWidth, nHeight, winID, title);
  label->onPaint = OnPaintLabel;

  return label;
}

/**
 * 10.05�޸�
 * ����label
 * 
 */
void OnPaintLabel(hbasewinAttr *label, void *value) //ȫ���������������Ϊ����ʱ��ֱ�Ӻ������һ��
{
   



  // FILE *fpFont;
  // FILE *fptemp;
  // int x, y, type = 3, len, linemax, line, i, maxline;
  // char *temp, *content;
  // if (label == NULL)
  //   return;
  // OnPaint(label, &type);

  // if (label->title == NULL)
  //   return;
  // //fpFont = fopen(FILE_SIMSUN16 DATAPATH, "rt");

  // x = getAbsoluteX(label);
  // y = getAbsoluteY(label);

  // linemax = label->nWidth / 16;
  // maxline = label->nHeight / 16;
  // if (linemax == 0 || maxline == 0)
  //   return;
  // if (fptemp = fopen(label->title, "rt") == NULL)
  // { //�ַ���С��ǩ����������ʾ
  //   len = sizeof(label->title) / 2;
  //   content = malloc(len * 2 + 1);
  //   memset(content, 0, len * 2 + 1);
  //   memcpy(content, label->title, len * 2);
  //   line = len / linemax + 1;
  //   if (line > maxline)
  //     line = maxline;
  // }
  // else
  // {
  //   len = (label->nWidth / 16) * (label->nHeight / 16);
  //   content = malloc(len * 2 + 1);
  //   memset(content, 0, len * 2 + 1);
  //   for (i = 0; (fptemp != EOF) && (i < (len - 1) * 2); i++)
  //     fread(content + i, 1, 1, fptemp);
  //   // ���һ���ֵ��ж�
  //   if (fptemp != EOF)
  //   {                           //����
  //     *(content + i) = '��';     //������
  //     *(content + len * 2) = 0; //���ԣ�ȷ���н�β
  //   }
  //   fclose(fptemp);
  // }
  // temp = malloc(linemax * 2 + 1);
  // for (i = 0; i < line - 1; i++)
  // {
  //   memcpy(temp, content + i * linemax * 2, linemax * 2);
  //   printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/);
  // }
  // memcpy(temp, content + i * linemax * 2, (len - linemax * i) * 2);
  // printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/);
  // free(temp);
  // free(content);
  // len = strlen(label->title) / 2; //һ����������char��lenΪ����
  // line = len / linemax + ((len % linemax) == 0 ? 0 : 1);
  // temp = malloc(linemax * 2 + 1);
  // memset(temp, 0, linemax * 2 + 1);
  // for (i = 0; i < line - 1; i++)
  // {
  //   memcpy(temp, label->title + i * linemax * 2, linemax * 2);
  //   //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16); modified by ���
  //   printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/); //��ʾ�ı��ĺ������ˣ�����Ҫ�򿪺����ֿ��ļ�ָ��
  // }
  // memcpy(temp, label->title + i * linemax * 2, (len - linemax * (line - 1)) * 2);
  // //printtextxy(fpFont, x, y + 16 * i, temp, 0 /*color*/, 16);
  // printText(x, y + 16 * i, temp, SIMSUN, 16, 0, 0x0 /*color*/);
}