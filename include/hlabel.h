#ifndef __HLABEL_H__
#define __HLABEL_H__

#include "HBaseWin.h"

#define LABEL_TITLE 0  //��ʾtitle
#define LABEL_FILE_TXT 1   //��ʾ�ļ���Ϊtile��txt�ļ� 
#define LABEL_FILE_IMG 2   //��ʾ�ļ���Ϊtitle��ͼƬ�ļ�

hbasewinAttr *CreateLabel(hbasewinAttr *parent, int x, int y, int nWidth,
                          int nHeight, int winID, const char *title);
void OnPaint_Label(hbasewinAttr *label, void *value);

#endif